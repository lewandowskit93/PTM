#ifndef __GUI_COMPONENT_H__
#define __GUI_COMPONENT_H__

#include <c++/4.9.3/map>
#include "../GUI/Canvas.hpp"

namespace ptm
{
namespace gui
{

namespace attributes
{
  enum Attribute
  {
    UNKNOWN=0,
    VISIBILITY=1,
    ACTIVE=2
  };

  namespace states
  {
    enum AttributeState
    {
      DISABLED=0,
      ENABLED=1,
      INHERIT=1, // no inherit support
      UNDEFINED=3
    };
  }
}

class Container;

class Component
{
  public:
    Component(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    virtual ~Component();
    void setX(uint32_t x);
    uint32_t getX();
    void setY(uint32_t y);
    uint32_t getY();
    uint32_t getWidth();
    uint32_t getHeight();
    Container* getParent();
    void setParent(Container* parent);
    attributes::states::AttributeState getAttributeState(attributes::Attribute attribute);
    void setAttributeState(attributes::Attribute attribute, attributes::states::AttributeState state);
    void removeAttribute(attributes::Attribute attribute);
    virtual void paintOn(Canvas * canvas)=0;
  protected:
  private:
    uint32_t _x;
    uint32_t _y;
    uint32_t _width;
    uint32_t _height;
    Container *_parent;
    std::map<attributes::Attribute, attributes::states::AttributeState> _attributes;
};
}
}

#endif
