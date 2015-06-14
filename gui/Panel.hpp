#ifndef __GUI_PANEL_H__
#define __GUI_PANEL_H__

#include "../GUI/Container.hpp"

namespace ptm
{
namespace gui
{

class Panel : public Container
{
public:
  Panel(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
  void setBgColor(utilities::colors::RGBA color);
  virtual ~Panel();
  virtual void paintOn(Canvas * canvas);
protected:
  utilities::colors::RGBA _bg_color;
private:
};

}
}

#endif
