#ifndef __GUI_CANVAS_H__
#define __GUI_CANVAS_H__

#include "utilities/Color.hpp"
#include "Devices/Display.hpp"
#include <c++/4.9.3/memory>

namespace ptm
{
namespace gui
{

class Canvas
{
  public:
    Canvas(uint32_t x_offset, uint32_t y_offset, uint32_t width,
        uint32_t height, bool subcanvas);
    virtual ~Canvas();
    void clear();
    void clearArea(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    virtual void repaint(uint32_t x, uint32_t y,
        std::weak_ptr<devices::displays::IDisplay> display)=0;
    utilities::colors::RGBA getColor();
    void setColor(utilities::colors::RGBA color);
    utilities::colors::RGBA getBgColor();
    void setBgColor(utilities::colors::RGBA color);
    virtual utilities::colors::RGBA getPixelColor(uint32_t x, uint32_t y)=0;
    virtual void drawPixel(uint32_t x, uint32_t y);
    virtual void drawBgPixel(uint32_t x, uint32_t y);
    virtual Canvas* getSubCanvas(uint32_t x, uint32_t y, uint32_t width,
        uint32_t height)=0;
  protected:
    virtual void drawCPixel(uint32_t x, uint32_t y,
        utilities::colors::RGBA color)=0;
    uint32_t _x_offset;
    uint32_t _y_offset;
    uint32_t _width;
    uint32_t _height;
    utilities::colors::RGBA _color;
    utilities::colors::RGBA _bg_color;
    bool _subcanvas;
  private:
};

class MonochromaticCanvas : public Canvas
{
  public:
    MonochromaticCanvas(uint32_t width, uint32_t height);
    virtual ~MonochromaticCanvas();
    virtual void repaint(uint32_t x, uint32_t y,
        std::weak_ptr<devices::displays::IDisplay> display);
    virtual MonochromaticCanvas* getSubCanvas(uint32_t x, uint32_t y,
        uint32_t width, uint32_t height);
    virtual utilities::colors::RGBA getPixelColor(uint32_t x, uint32_t y);
  protected:
    MonochromaticCanvas(uint32_t x, uint32_t y, uint32_t width, uint32_t height,
        MonochromaticCanvas &canvas);
    virtual void drawCPixel(uint32_t x, uint32_t y,
        utilities::colors::RGBA color);
  private:
    bool **_canvas;
};

}

}

#endif
