#include "SunSpaceGame.hpp"

namespace ptm
{
namespace applications
{
namespace spacegame
{

Spaceship::Spaceship(uint32_t x, uint32_t y) :
    gui::Component(x, y, 12, 11), _current_frame(0), _reverse(false)
{

}

Spaceship::~Spaceship()
{

}

void Spaceship::paintOn(gui::Canvas *canvas)
{
  paintFrame(_current_frame, canvas);
}

void Spaceship::nextAnimFrame()
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
    if (_current_frame < 4)
      setAnimFrame(_current_frame + 1);
    else
    {
      _reverse = true;
      setAnimFrame(_current_frame - 1);
    }
  }
}

void Spaceship::setAnimFrame(uint8_t i)
{
  i %= 5;
  _current_frame = i;
}

void Spaceship::paintFrame(uint8_t frame, gui::Canvas *canvas)
{
  frame %= 5;
  if (canvas)
  {
    for (uint32_t i = 0; i < getWidth(); ++i)
    {
      for (uint32_t j = 0; j < getHeight(); ++j)
      {
        if (_spaceship[i][j + frame * getHeight()])
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
                _bg[(_bgx + i) % 800][(_bgy + j) % 48] == 0 ?
                    0x00000000 : 0xFFFFFFFF));
        canvas->drawBgPixel(i, j);
      }
    }
  }
}

GameContext::GameContext(SpaceshipGame *game) :
    ApplicationContext(), _game(game), _game_panel(0, 0, 84, 48), _game_bg(0, 0,
        84, 48), _bg_timer(&_timer_manager, 120,
        std::bind(&GameContext::onBgTimer, this), true), _spaceship(15, 20), _spaceship_anim_timer(
        &_timer_manager, 80, std::bind(&GameContext::onSpaceshipAnim, this), true),
        _spaceship_current_y(20), _playing(false)
{
  _event_listener.registerEventHandler(
      events::EventMapping(events::EVENT_BUTTON,
          std::bind(&GameContext::onButton, this, std::placeholders::_1)));


  _event_listener.registerEventHandler(
        events::EventMapping(events::EVENT_ACC_IN_GAME,
            std::bind(&GameContext::getAccelometerAxis, this, std::placeholders::_1)));

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
  _game_panel.addChild(&_spaceship);
  _spaceship.setX(15);
  _spaceship.setY(20);
  _spaceship_current_y = 20;
  if (_game)
    _game->_main_panel.addChild(&_game_panel);
  _bg_timer.start();
  _spaceship_anim_timer.start();
}
void GameContext::onPause()
{
  if (_game)
  {
    _game->_main_panel.removeChild(&_game_panel);
  }
  _bg_timer.pause();
  _spaceship_anim_timer.pause();
}

void GameContext::onResume()
{
  if (_game)
  {
    _game->_main_panel.addChild(&_game_panel);
  }
  _bg_timer.resume();
  _spaceship_anim_timer.resume();
}

void GameContext::onStop()
{
  if (_game)
  {
    _game->_main_panel.removeChild(&_game_panel);
  }
  _bg_timer.stop();
  _spaceship_anim_timer.stop();
  _playing = false;
}

void GameContext::onBgTimer()
{
  _game_bg._bgx++;
  _bg_timer.start();
}

void GameContext::onSpaceshipAnim()
{
  _spaceship.nextAnimFrame();
  _spaceship_anim_timer.start();
}

void GameContext::getAccelometerAxis(std::shared_ptr<events::Event> event)
{
	std::shared_ptr<events::AccelometerGetPositionEvent> a_event = std::static_pointer_cast
	      < events::AccelometerGetPositionEvent > (event);
	ptm::axisXY xy = a_event->getPosition();

	_spaceship_current_y = xy.y;
	_spaceship.setY((int32_t) _spaceship_current_y);

	checkSpaceshipCollision();

}

void GameContext::checkSpaceshipCollision()
{
	uint32_t x = _spaceship.getX();
	uint32_t y = _spaceship.getY();
	for (int i = 0; i < _spaceship.getWidth(); i++)
	{
		for (int j = 0; j < _spaceship.getHeight() ; j++)
		{
			if ((_spaceship._spaceship[i][j + _spaceship._current_frame * _spaceship.getHeight()] ==
					_game_bg._bg[(_game_bg._bgx+i+x)%750][(_game_bg._bgy+j+y)%48]) && _spaceship._spaceship[i][j + _spaceship._current_frame * _spaceship.getHeight()] != 0 )
			{
				_game->stop();
			}
		}
	}
}

/*void GameContext::onGravity()
{
  if(_playing)
  {
    //here comes accelometer
    _ptero.setY((int32_t) _ptero_current_y);
  }
  _gravity_timer.start();
}*/

void GameContext::onButton(std::shared_ptr<events::Event> event)
{
  std::shared_ptr<events::ButtonEvent> b_event = std::static_pointer_cast
      < events::ButtonEvent > (event);
  if (!b_event->isPressed())
  {
   if (_game)
   {
	   _game->stop();
   }

  }
}

SpaceshipGame::SpaceshipGame() :
    Application(), _main_panel(0, 0, 84, 48), _game_context(this), _canvas(84,
        48), _display(
        system::System::getInstance()->_device_manager.getDevice<
            devices::displays::IDisplay>()), _screen_timer(&_timer_manager, 40,
        std::bind(&SpaceshipGame::onScreenUpdate, this), true)
{

}

SpaceshipGame::~SpaceshipGame()
{

}

void SpaceshipGame::onUpdate()
{

}

void SpaceshipGame::onStart()
{
  _canvas.clear();
  switchContext(&_game_context);
  _screen_timer.start();
}

void SpaceshipGame::onPause()
{
  _screen_timer.pause();
}

void SpaceshipGame::onResume()
{
  _screen_timer.resume();
}

void SpaceshipGame::onStop()
{
  _screen_timer.stop();
}

void SpaceshipGame::onScreenUpdate()
{
  _main_panel.paintOn(&_canvas);
  _canvas.repaint(0, 0, _display);
  _screen_timer.start();
}

}
}
}
