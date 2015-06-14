#include "FlappyPtero.hpp"

namespace ptm
{
namespace applications
{
namespace flappy_ptero
{

Ptero::Ptero(uint32_t x, uint32_t y)
: gui::Component(x,y,10,7), _current_frame(0),_reverse(false)
{

}

Ptero::~Ptero()
{

}

void Ptero::paintOn(gui::Canvas *canvas)
{
  paintFrame(_current_frame,canvas);
}

void Ptero::nextAnimFrame()
{
  if(_reverse)
  {
    if(_current_frame>0)setAnimFrame(_current_frame-1);
    else
    {
      _reverse=false;
      setAnimFrame(_current_frame+1);
    }
  }
  else
  {
    if(_current_frame<5)setAnimFrame(_current_frame+1);
    else
    {
      _reverse=true;
      setAnimFrame(_current_frame-1);
    }
  }
}

void Ptero::setAnimFrame(uint8_t i)
{
  i%=6;
  _current_frame=i;
}

void Ptero::paintFrame(uint8_t frame,gui::Canvas *canvas)
{
  frame%=6;
  if(canvas)
  {
    for(uint32_t i=0;i<getWidth();++i)
    {
      for(uint32_t j=0;j<getHeight();++j)
      {
        if(_ptero[i][j+frame*getHeight()])canvas->drawPixel(i,j);
      }
    }
  }
}

GameBackground::GameBackground(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
: Component(x,y,width,height), _bgx(0), _bgy(0)
{

}

GameBackground::~GameBackground()
{

}

void GameBackground::paintOn(gui::Canvas *canvas)
{
  if(canvas)
  {
    for(uint32_t i = 0; i<getWidth(); ++i)
    {
      for(uint32_t j=0;j<getHeight();++j)
      {
        canvas->setBgColor(utilities::colors::RGBA(_bg[(_bgx+i)%168][(_bgy+j)%48]==0 ? 0x00000000 : 0xFFFFFFFF));
        canvas->drawBgPixel(i,j);
      }
    }
  }
}

GameContext::GameContext(FlappyPteroGame *game)
 : ApplicationContext(), _game(game), _game_panel(0,0,84,48), _game_bg(0,0,84,48), _bg_timer(&_timer_manager, 120,
                std::bind(&GameContext::onBgTimer, this), true), _ptero(15,20),
                _ptero_anim_timer(&_timer_manager, 80,
                                std::bind(&GameContext::onPteroAnim, this), true)
{

}
GameContext::~GameContext()
{

}
void GameContext::onUpdate()
{
  _ptero.nextAnimFrame();
}
void GameContext::onStart()
{
  _game_bg._bgx=0;
  _game_bg._bgy=0;
  _game_panel.addChild(&_game_bg);
  _game_panel.addChild(&_ptero);
  if(_game)_game->_main_panel.addChild(&_game_panel);
  _bg_timer.start();
}
void GameContext::onPause()
{
  if(_game)
  {
    _game->_main_panel.removeChild(&_game_panel);
  }
  _bg_timer.pause();
}

void GameContext::onResume()
{
  if(_game)
  {
    _game->_main_panel.addChild(&_game_panel);
  }
  _bg_timer.resume();
}

void GameContext::onStop()
{
  if(_game)
  {
    _game->_main_panel.removeChild(&_game_panel);
  }
  _bg_timer.stop();
}

void GameContext::onBgTimer()
{
  _game_bg._bgx++;
  _bg_timer.start();
}

void GameContext::onPteroAnim()
{
  _ptero.nextAnimFrame();
}

FlappyPteroGame::FlappyPteroGame()
: Application(), _main_panel(0,0,84,48), _game_context(this), _canvas(84,48), _display(system::System::getInstance()->_device_manager.getDevice<devices::displays::IDisplay>()),
  _screen_timer(&_timer_manager, 40,
                  std::bind(&FlappyPteroGame::onScreenUpdate, this), true)
{

}

FlappyPteroGame::~FlappyPteroGame()
{

}

void FlappyPteroGame::onUpdate()
{

}

void FlappyPteroGame::onStart()
{
  _canvas.clear();
  switchContext(&_game_context);
  _screen_timer.start();
}

void FlappyPteroGame::onPause()
{
  _screen_timer.pause();
}

void FlappyPteroGame::onResume()
{
  _screen_timer.resume();
}

void FlappyPteroGame::onStop()
{
  _screen_timer.stop();
}

void FlappyPteroGame::onScreenUpdate()
{
  _main_panel.paintOn(&_canvas);
  _canvas.repaint(0,0,_display);
  _screen_timer.start();
}

}
}
}
