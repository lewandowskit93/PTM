#include "../utilities/Color.hpp"

namespace ptm
{
namespace utilities
{
namespace colors
{

RGBA::RGBA() :
    RGBA(0xFFFFFFFF)
{

}
RGBA::RGBA(uint8_t r, uint8_t g, uint8_t b) :
    RGBA(r, g, b, 0xFF)
{

}

RGBA::RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
    RGBA(
        (((uint32_t) r) << 24) | (((uint32_t) g) << 24) | (((uint32_t) b) << 24)
            | ((uint32_t) a))
{
}

RGBA::RGBA(uint32_t rgba) :
    _rgba(rgba)
{

}

RGBA::~RGBA()
{

}

uint32_t RGBA::getRGBA() const
{
  return _rgba;
}

uint8_t RGBA::getR() const
{
  return (uint8_t)((_rgba >> 24) & 0xFF);
}

uint8_t RGBA::getG() const
{
  return (uint8_t)((_rgba >> 16) & 0xFF);
}

uint8_t RGBA::getB() const
{
  return (uint8_t)((_rgba >> 8) & 0xFF);
}

uint8_t RGBA::getA() const
{
  return (uint8_t)((_rgba) & 0xFF);
}

bool RGBA::operator==(const RGBA & rgba)
{
  return this->_rgba==rgba._rgba;
}

}
}
}
