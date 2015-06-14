#include "../GUI/Container.hpp"

namespace ptm
{
namespace gui
{

Container::Container(uint32_t x, uint32_t y, uint32_t width, uint32_t height) :
    Component(x, y, width, height)
{

}

Container::~Container()
{

}

void Container::addChild(Component *component)
{
  if (component->getParent())
    component->getParent()->removeChild(component);
  component->setParent(this);
  _childrens.push_back(component);
}

void Container::removeChild(Component *component)
{
  if (component->getParent() == this)
  {
    component->setParent(0);
    _childrens.remove(component);
  }

}

std::vector<Component*> Container::getChildrens()
{
  return std::vector<Component*>(_childrens.begin(), _childrens.end());
}

uint32_t Container::getNumberOfChildrens()
{
  return _childrens.size();
}

void Container::paintChildrens(Canvas* canvas)
{
  for (auto child : _childrens)
  {
    attributes::states::AttributeState child_visibility = child->getAttributeState(
        attributes::VISIBILITY);
    if (child_visibility == attributes::states::ENABLED)
    {
      Canvas* subcanvas = canvas->getSubCanvas(child->getX(), child->getY(), child->getWidth(),
          child->getHeight());
      child->paintOn(subcanvas);
      delete subcanvas;
    }
  }
}

}
}
