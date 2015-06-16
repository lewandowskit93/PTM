#include "FlappyPtero.hpp"
#include <stdlib.h>

namespace ptm
{
namespace applications
{
namespace flappy_ptero
{

Ptero::Ptero(uint32_t x, uint32_t y) :
    Ptero(x, y, false)
{

}

Ptero::Ptero(uint32_t x, uint32_t y, bool flip_x) :
    gui::Component(x, y, 10, 7), _current_frame(0), _reverse(false), _flip_x(
        flip_x)
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

bool Ptero::collidesWith(Wall *wall)
{
  if (!wall)
    return false;
  if (_flip_x)
  {
    for (uint32_t i = 1; i <= getWidth(); ++i)
    {
      for (uint32_t j = 0; j < getHeight(); ++j)
      {
        uint32_t x = getX() + i;
        uint32_t y = getY() + j;
        uint32_t k = j + _current_frame * getHeight();
        if (_ptero[getWidth() - i][k] && x >= wall->getX()
            && x < (wall->getX() + wall->getWidth())
            && ((y < wall->getGapY())
                || (y >= (wall->getGapY() + wall->getGapSize()))))
          return true;
      }
    }
  }
  else
  {
    for (uint32_t i = 0; i < getWidth(); ++i)
    {
      for (uint32_t j = 0; j < getHeight(); ++j)
      {
        uint32_t x = getX() + i;
        uint32_t y = getY() + j;
        if (_ptero[i][j + _current_frame * getHeight()] && x >= wall->getX()
            && x < (wall->getX() + wall->getWidth())
            && ((y < wall->getGapY())
                || (y >= (wall->getGapY() + wall->getGapSize()))))
          return true;
      }
    }
  }
  return false;
}

void Ptero::paintFrame(uint8_t frame, gui::Canvas *canvas)
{
  frame %= 6;
  if (canvas)
  {
    if (_flip_x)
    {
      for (uint32_t i = 1; i <= getWidth(); ++i)
      {
        for (uint32_t j = 0; j < getHeight(); ++j)
        {
          if (_ptero[getWidth() - i][j + frame * getHeight()])
            canvas->drawPixel(i, j);
        }
      }
    }
    else
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
}

Wall::Wall(uint32_t x, uint32_t y, uint32_t width, uint32_t height,
    uint32_t gap_size, uint32_t gap_y) :
    gui::Component(x, y, width, height), _gap_size(gap_size), _gap_y(gap_y)
{

}

Wall::~Wall()
{

}

void Wall::paintOn(gui::Canvas * canvas)
{
  if (!canvas)
    return;
  for (uint32_t i = 0; i < getWidth(); ++i)
  {
    for (uint32_t j = 0; j < getHeight(); ++j)
    {
      if (j < _gap_y || j >= (_gap_y + _gap_size))
      {
        canvas->drawPixel(i, j);
      }
    }
  }
}

uint32_t Wall::getGapSize()
{
  return _gap_size;
}

void Wall::setGapSize(uint32_t gap_size)
{
  _gap_size = gap_size;
}

uint32_t Wall::getGapY()
{
  return _gap_y;
}

void Wall::setGapY(uint32_t gap_y)
{
  _gap_y = gap_y;
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

MenuContext::MenuContext(FlappyPteroGame *game) :
    ApplicationContext(), _game(game), _ptero1(37, 14, false), _ptero2(37, 34,
        true), _ptero_anim_timer(&_timer_manager, 80,
        std::bind(&MenuContext::onPteroAnim, this), true), _menu_panel(0, 0, 84,
        48), _bg(0, 0, 84, 48), _bg_timer(&_timer_manager, 20,
        std::bind(&MenuContext::onBgTimer, this), true)
{
  _event_listener.registerEventHandler(
      events::EventMapping(events::EVENT_BUTTON,
          std::bind(&MenuContext::onButton, this, std::placeholders::_1)));
}

MenuContext::~MenuContext()
{

}

void MenuContext::onUpdate()
{

}

void MenuContext::onStart()
{
  _menu_panel.addChild(&_bg);
  _menu_panel.addChild(&_ptero1);
  _menu_panel.addChild(&_ptero2);
  _ptero2.setAnimFrame(5);
  if (_game)
    _game->_main_panel.addChild(&_menu_panel);
  _ptero_anim_timer.start();
  _bg_timer.start();
}

void MenuContext::onPause()
{

  if (_game)
  {
    _game->_main_panel.removeChild(&_menu_panel);
  }
  _ptero_anim_timer.pause();
  _bg_timer.pause();
}

void MenuContext::onResume()
{
  if (_game)
  {
    _game->_main_panel.addChild(&_menu_panel);
  }
  _ptero_anim_timer.resume();
  _bg_timer.resume();
}

void MenuContext::onStop()
{
  if (_game)
  {
    _game->_main_panel.removeChild(&_menu_panel);
  }
  _ptero_anim_timer.stop();
  _bg_timer.stop();
}

void MenuContext::onPteroAnim()
{
  _ptero1.nextAnimFrame();
  _ptero2.nextAnimFrame();
  _ptero_anim_timer.start();
}

void MenuContext::onBgTimer()
{
  _bg._bgx++;
  _bg._bgy++;
  _bg_timer.start();
}

void MenuContext::onButton(std::shared_ptr<events::Event> event)
{
  std::shared_ptr<events::ButtonEvent> b_event = std::static_pointer_cast
      < events::ButtonEvent > (event);
  if (!b_event->isPressed())
  {
    if (_game)
      _game->switchContext(&_game->_game_context);
  }
}

GameContext::GameContext(FlappyPteroGame *game) :
    ApplicationContext(), _game(game), _game_panel(0, 0, 84, 48), _game_bg(0, 0,
        84, 48), _bg_timer(&_timer_manager, 120,
        std::bind(&GameContext::onBgTimer, this), true), _ptero(15, 20), _ptero_anim_timer(
        &_timer_manager, 80, std::bind(&GameContext::onPteroAnim, this), true), _gravity_timer(
        &_timer_manager, 10, std::bind(&GameContext::onGravity, this), true), _current_y_speed(
        0), _current_x_speed(2), _ptero_current_y(20), _ptero_current_x(0), _game_state(
        GameState::PAUSED), _gravity(100), _pixels_per_meter(10), _flap_force_meters(
        12), _max_speed(10), _ptero_x_timer(&_timer_manager, 1,
        std::bind(&GameContext::onPteroX, this), true), _walls_distance(25)
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
  if (_ptero_current_y >= 48)
  {
    gameOver();
  }
  else
  {
    for (auto wall : _walls)
    {
      if (_ptero.collidesWith(wall))
      {
        gameOver();
      }
    }
  }
}

void GameContext::onStart()
{
  _game_state = GameState::PAUSED;
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
  _ptero_x_timer.start();
  for (auto wall : _walls)
  {
    if (wall->getParent())
      wall->getParent()->removeChild(wall);
    delete wall;
  }
  _walls.clear();
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
  _ptero_x_timer.pause();
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
  _ptero_x_timer.resume();
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
  _ptero_x_timer.stop();
  for (auto wall : _walls)
  {
    if (wall->getParent())
      wall->getParent()->removeChild(wall);
    delete wall;
  }
  _walls.clear();
  _game_state = GameState::PAUSED;
}

void GameContext::onBgTimer()
{
  if (_game_state != GameState::GAME_OVER)
  {
    _game_bg._bgx++;
    _bg_timer.start();
  }
}

void GameContext::onPteroAnim()
{
  if (_game_state != GameState::GAME_OVER)
  {
    _ptero.nextAnimFrame();
    _ptero_anim_timer.start();
  }
}

void GameContext::onGravity()
{
  if (_game_state == GameState::PLAYING)
  {
    _ptero_current_y += _current_y_speed
        / (1000.0 / _gravity_timer.getDuration());
    _ptero.setY((int32_t) _ptero_current_y);
    if (_current_y_speed < (_max_speed * _pixels_per_meter))
      _current_y_speed += _gravity / (1000.0 / _gravity_timer.getDuration())
          * _pixels_per_meter;
  }
  _gravity_timer.start();
}

void GameContext::onPteroX()
{
  if (_game_state == GameState::PLAYING)
  {
    uint32_t ptero_last_x = _ptero_current_x;
    _ptero_current_x += _current_x_speed
        * (_ptero_x_timer.getDuration() / 1000.0) * _pixels_per_meter;

    if ((uint32_t)(_ptero_current_x) > ptero_last_x)
    {

      uint32_t moved_by = (uint32_t)(_ptero_current_x) - ptero_last_x;
      std::list<Wall*> walls_to_delete;

      for (auto wall : _walls)
      {
        if (wall->getX() >= moved_by)
        {
          wall->setX(wall->getX() - moved_by);
        }
        else
        {
          walls_to_delete.push_back(wall);
        }
      }

      for (auto wall : walls_to_delete)
      {
        if (wall->getParent())
          wall->getParent()->removeChild(wall);
        _walls.remove(wall);
        delete wall;
      }

      uint32_t distance = _walls_distance;
      if (!_walls.empty())
        distance += _walls.back()->getWidth();

      if ((((uint32_t)(_ptero_current_x)) % (distance)) == 0)
      {
        Wall *w = new Wall(84, 0, 3, 48, 20, rand() % (18) + 5);
        _game_panel.addChild(w);
        _walls.push_back(w);
        _ptero_current_x = 0;
      }
    }
  }
  _ptero_x_timer.start();
}

void GameContext::gameOver()
{
  _game_state = GameState::GAME_OVER;
}

void GameContext::onButton(std::shared_ptr<events::Event> event)
{
  std::shared_ptr<events::ButtonEvent> b_event = std::static_pointer_cast
      < events::ButtonEvent > (event);
  if (!b_event->isPressed())
  {
    switch (_game_state)
    {
      case GameState::PAUSED:
      {
        _game_state = GameState::PLAYING;
      }
      case GameState::PLAYING:
      {
        _current_y_speed = -_flap_force_meters * _pixels_per_meter;
        break;
      }
      case GameState::GAME_OVER:
      default:
      {
        if (_game)
        {
          _game->switchContext(&_game->_menu_context);
          this->stop();
        }
      }
    }
  }
}

FlappyPteroGame::FlappyPteroGame() :
    Application(), _main_panel(0, 0, 84, 48), _game_context(this), _menu_context(
        this), _canvas(84, 48), _display(
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
  switchContext(&_menu_context);
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
