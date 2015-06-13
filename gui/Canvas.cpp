#include "../GUI/Canvas.hpp"

namespace ptm
{
namespace gui
{

Canvas::Canvas(uint32_t x_offset, uint32_t y_offset, uint32_t width, uint32_t height, bool subcanvas)
:  _x_offset(x_offset), _y_offset(y_offset), _width(width), _height(height), _color(utilities::colors::RGBA(0xFFFFFFFF)), _bg_color(utilities::colors::RGBA(0x00000000)), _subcanvas(subcanvas)
{
}

Canvas::~Canvas()
{
}

void Canvas::setColor(utilities::colors::RGBA color)
{
  _color=color;
}

void Canvas::setBgColor(utilities::colors::RGBA color)
{
  _bg_color = color;
}

void Canvas::drawPixel(uint32_t x, uint32_t y)
{
  drawCPixel(x,y,_color);
}

void Canvas::drawBgPixel(uint32_t x, uint32_t y)
{
  drawCPixel(x,y,_bg_color);
}

void Canvas::clear()
{
  clearArea(0,0,_width,_height);
}

void Canvas::clearArea(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
  for(uint32_t i=0;i<_width;++i)
  {
    for(uint32_t j=0;j<_height;++j)
    {
      drawBgPixel(x+i,y+j);
    }
  }
}


MonochromaticCanvas::MonochromaticCanvas(uint32_t width, uint32_t height)
: Canvas(0,0,width,height,false)
{
  _canvas = new bool*[_width];
  for(uint32_t i=0;i<_width;++i)
  {
    _canvas[i] = new bool[_height];
    for(uint32_t j=0;j<_height;++j)
    {
      _canvas[i][j]=false;
    }
  }
}

MonochromaticCanvas::MonochromaticCanvas(uint32_t x, uint32_t y, uint32_t width, uint32_t height, MonochromaticCanvas &canvas)
: Canvas(x,y,width,height,true), _canvas(canvas._canvas)
{
}

MonochromaticCanvas::~MonochromaticCanvas()
{
  if(!_subcanvas)
  {
    for(uint32_t i=0;i<_width;++i)
    {
      delete [] _canvas[i];
    }
    delete [] _canvas;
  }
}

void MonochromaticCanvas::repaint(uint32_t x, uint32_t y, std::weak_ptr<devices::displays::IDisplay> display)
{
  auto display_s = display.lock();
  if(display_s)
  {
    for(uint32_t i=0; i<_width;++i)
    {
      for(uint32_t j=0;j<_height;++j)
      {
        display_s->setPixelColor(x+i,y+j,utilities::colors::RGBA(_canvas[i][j] ? 0xFFFFFFFF : 0x00000000));
      }
    }
    display_s->refreshArea(x,y,_width,_height);
  }
}

MonochromaticCanvas* MonochromaticCanvas::getSubCanvas(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
  if(x>=_width)return 0;
  if(y>=_height)return 0;
  if(x+width>_width)return 0;
  if(y+height>_height)return 0;
  return new MonochromaticCanvas(x,y,width,height,*this);
}

void MonochromaticCanvas::drawCPixel(uint32_t x, uint32_t y,utilities::colors::RGBA color)
{
  if(x>=_width || y>=_height) return;
  if(color==utilities::colors::RGBA(0x00000000))
  {
    _canvas[x+_x_offset][y+_y_offset]=false;
  }
  else
  {
    _canvas[x+_x_offset][y+_y_offset]=true;
  }
}

}
}

