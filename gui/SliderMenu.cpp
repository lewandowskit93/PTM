#include "SliderMenu.hpp"

namespace ptm
{
namespace gui
{


SliderMenu::SliderMenu(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
: Container(x,y,width,height), _selected(0)
{

}

SliderMenu::~SliderMenu()
{

}

void SliderMenu::paintOn(Canvas * canvas)
{
  paintChildrens(canvas);
}

void SliderMenu::paintChildrens(Canvas* canvas)
{
  if(canvas)
  {
    uint32_t i = getSelected();
    if(i!=0)
    {
      --i;
      auto childrens = getChildrens();
      childrens[i]->setX(getWidth()/2-childrens[i]->getWidth()/2);
      childrens[i]->setY(getHeight()/2-childrens[i]->getHeight()/2);
      Canvas* subcanvas = canvas->getSubCanvas(childrens[i]->getX(), childrens[i]->getY(), childrens[i]->getWidth(),
          childrens[i]->getHeight());
      childrens[i]->paintOn(subcanvas);
      delete subcanvas;
    }
  }
}

uint32_t SliderMenu::getSelected()
{
  if(_selected>getNumberOfChildrens())
  {
    _selected=getNumberOfChildrens();
  }
  return _selected;
}

void SliderMenu::selectNext()
{
  ++_selected;
  if(_selected>getNumberOfChildrens())
  {
    _selected=getNumberOfChildrens();
  }
}

void SliderMenu::selectPrevious()
{
  if(_selected>1)
  {
    --_selected;
  }
  if(_selected>getNumberOfChildrens())
  {
    _selected=getNumberOfChildrens();
  }
}

}
}
