#include "FlappyPtero.hpp"

namespace ptm
{
namespace applications
{
namespace flappy_ptero
{

Ptero::Ptero(uint32_t x, uint32_t y) :
    gui::Component(x, y, 10, 7), _current_frame(0), _reverse(false)
{

}

Ptero::~Ptero()
{

}

void Ptero::paintOn(gui::Canvas *canvas)
{
  paintFrame(_current_frame, canvas);
}

void Ptero::nextAnimFrame()
{
  if (_reverse)
  {
    if (_current_frame > 0)
      setAnimFrame(_current_frame - 1);
    else
    {
      _reverse = false;
      setAnimFrame(_current_frame + 1);
    }
  }
  else
  {
    if (_current_frame < 5)
      setAnimFrame(_current_frame + 1);
    else
    {
      _reverse = true;
      setAnimFrame(_current_frame - 1);
    }
  }
}

void Ptero::setAnimFrame(uint8_t i)
{
  i %= 6;
  _current_frame = i;
}

void Ptero::paintFrame(uint8_t frame, gui::Canvas *canvas)
{
  frame %= 6;
  if (canvas)
  {
    for (uint32_t i = 0; i < getWidth(); ++i)
    {
      for (uint32_t j = 0; j < getHeight(); ++j)
      {
        if (_ptero[i][j + frame * getHeight()])
          canvas->drawPixel(i, j);
      }
    }
  }
}

GameBackground::GameBackground(uint32_t x, uint32_t y, uint32_t width,
    uint32_t height) :
    Component(x, y, width, height), _bgx(0), _bgy(0)
{

}

GameBackground::~GameBackground()
{

}

void GameBackground::paintOn(gui::Canvas *canvas)
{
  if (canvas)
  {
    for (uint32_t i = 0; i < getWidth(); ++i)
    {
      for (uint32_t j = 0; j < getHeight(); ++j)
      {
        canvas->setBgColor(
            utilities::colors::RGBA(
                _bg[(_bgx + i) % 168][(_bgy + j) % 48] == 0 ?
                    0x00000000 : 0xFFFFFFFF));
        canvas->drawBgPixel(i, j);
      }
    }
  }
}

GameContext::GameContext(FlappyPteroGame *game) :
    ApplicationContext(), _game(game), _game_panel(0, 0, 84, 48), _game_bg(0, 0,
        84, 48), _bg_timer(&_timer_manager, 120,
        std::bind(&GameContext::onBgTimer, this), true), _ptero(15, 20), _ptero_anim_timer(
        &_timer_manager, 80, std::bind(&GameContext::onPteroAnim, this), true), _gravity_timer(
        &_timer_manager, 10, std::bind(&GameContext::onGravity, this), true), _current_y_speed(
        0), _ptero_current_y(20), _playing(false), _gravity(100), _pixels_per_meter(10), _flap_force_meters(12), _max_speed(10)
{
  _event_listener.registerEventHandler(
      events::EventMapping(events::EVENT_BUTTON,
          std::bind(&GameContext::onButton, this, std::placeholders::_1)));
}
GameContext::~GameContext()
{

}
void GameContext::onUpdate()
{
}
void GameContext::onStart()
{
  _playing = false;
  _game_bg._bgx = 0;
  _game_bg._bgy = 0;
  _current_y_speed = 0;
  _game_panel.addChild(&_game_bg);
  _game_panel.addChild(&_ptero);
  _ptero.setX(15);
  _ptero.setY(20);
  _ptero_current_y = 20;
  if (_game)
    _game->_main_panel.addChild(&_game_panel);
  _bg_timer.start();
  _ptero_anim_timer.start();
  _gravity_timer.start();
}
void GameContext::onPause()
{
  if (_game)
  {
    _game->_main_panel.removeChild(&_game_panel);
  }
  _bg_timer.pause();
  _ptero_anim_timer.pause();
  _gravity_timer.pause();
}

void GameContext::onResume()
{
  if (_game)
  {
    _game->_main_panel.addChild(&_game_panel);
  }
  _bg_timer.resume();
  _ptero_anim_timer.resume();
  _gravity_timer.resume();
}

void GameContext::onStop()
{
  if (_game)
  {
    _game->_main_panel.removeChild(&_game_panel);
  }
  _bg_timer.stop();
  _ptero_anim_timer.stop();
  _gravity_timer.stop();
  _playing = false;
}

void GameContext::onBgTimer()
{
  _game_bg._bgx++;
  _bg_timer.start();
}

void GameContext::onPteroAnim()
{
  _ptero.nextAnimFrame();
  _ptero_anim_timer.start();
}

void GameContext::onGravity()
{
  if(_playing)
  {
    _ptero_current_y += _current_y_speed / (1000.0/_gravity_timer.getDuration());
    _ptero.setY((int32_t) _ptero_current_y);
    if (_current_y_speed < (_max_speed*_pixels_per_meter))
      _current_y_speed += _gravity/(1000.0/_gravity_timer.getDuration()) * _pixels_per_meter;
  }
  _gravity_timer.start();
}

void GameContext::onButton(std::shared_ptr<events::Event> event)
{
  std::shared_ptr<events::ButtonEvent> b_event = std::static_pointer_cast
      < events::ButtonEvent > (event);
  if (!b_event->isPressed())
  {
    if(!_playing)_playing=true;
    _current_y_speed = -_flap_force_meters*_pixels_per_meter;
  }
}

FlappyPteroGame::FlappyPteroGame() :
    Application(), _main_panel(0, 0, 84, 48), _game_context(this), _canvas(84,
        48), _display(
        system::System::getInstance()->_device_manager.getDevice<
            devices::displays::IDisplay>()), _screen_timer(&_timer_manager, 40,
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
  _canvas.repaint(0, 0, _display);
  _screen_timer.start();
}

}
}
}
