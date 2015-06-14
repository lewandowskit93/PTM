#include "../GUI/Component.hpp"

namespace ptm
{
namespace gui
{
Component::Component(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
: _x(x), _y(y), _width(width), _height(height), _parent(0)
{
  _attributes[attributes::ACTIVE]=attributes::states::INHERIT;
  _attributes[attributes::VISIBILITY]=attributes::states::INHERIT;
}
Component::~Component()
{

}

void Component::setX(uint32_t x)
{
  _x=x;
}

uint32_t Component::getX()
{
  return _x;
}
void Component::setY(uint32_t y)
{
  _y=y;;
}
uint32_t Component::getY()
{
  return _y;
}
uint32_t Component::getWidth()
{
  return _width;
}
uint32_t Component::getHeight()
{
  return _height;
}
Container* Component::getParent()
{
  return _parent;
}
void Component::setParent(Container* parent)
{
  _parent = parent;
}
attributes::states::AttributeState Component::getAttributeState(
    attributes::Attribute attribute)
{
  try{
    attributes::states::AttributeState state = _attributes.at(attribute);
    return state;
  }
  catch(std::out_of_range e)
  {
    return attributes::states::UNDEFINED;
  }
}

void Component::setAttributeState(attributes::Attribute attribute, attributes::states::AttributeState state)
{
  if(state==attributes::states::UNDEFINED)
  {
    removeAttribute(attribute);
  }
  _attributes[attribute]=state;
}

void Component::removeAttribute(attributes::Attribute attribute)
{
  _attributes.erase(attribute);
}

}
}
