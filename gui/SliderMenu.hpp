#ifndef __GUI_SLIDER_MENU_H__
#define __GUI_SLIDER_MENU_H__

#include "Container.hpp"

namespace ptm
{
namespace gui
{

class SliderMenu : public Container
{
public:
    SliderMenu(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    virtual ~SliderMenu();
    virtual void paintOn(Canvas * canvas);
    virtual void paintChildrens(Canvas* canvas);
    uint32_t getSelected();
    void selectNext();
    void selectPrevious();
protected:
    uint32_t _selected;
private:
};

}
}

#endif
