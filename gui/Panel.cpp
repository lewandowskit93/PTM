#include "../GUI/Panel.hpp"

namespace ptm
{
namespace gui
{
Panel::Panel(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
 : Container(x,y,width,height), _bg_color(0x00000000)
{

}

Panel::~Panel()
{

}

void Panel::setBgColor(utilities::colors::RGBA color)
{
  _bg_color=color;
}

void Panel::paintOn(Canvas * canvas)
{
  if(canvas)
  {
    utilities::colors::RGBA bg_color = canvas->getBgColor();
    canvas->setBgColor(_bg_color);
    canvas->clear();
    paintChildrens(canvas);
    canvas->setBgColor(bg_color);
  }
}

}
}
