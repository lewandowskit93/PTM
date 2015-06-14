#ifndef __GUI_CONTAINER_H__
#define __GUI_CONTAINER_H__

#include <c++/4.9.3/list>
#include <c++/4.9.3/vector>
#include "../GUI/Component.hpp"

namespace ptm
{
namespace gui
{

class Container : public Component
{
  public:
    Container(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    virtual ~Container();
    void addChild(Component *component);
    void removeChild(Component *component);
    std::vector<Component*> getChildrens();
    uint32_t getNumberOfChildrens();
    virtual void paintChildrens(Canvas* canvas);
  protected:
    std::list<Component*> _childrens;
  private:
};

}
}

#endif
