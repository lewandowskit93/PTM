#ifndef __UTILITIES_COLOR_H__
#define __UTILITIES_COLOR_H__

#include <stdint.h>

namespace ptm
{
namespace utilities
{
namespace colors
{
class RGBA
{
  public:
    RGBA();
    RGBA(uint8_t r, uint8_t g, uint8_t b);
    RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    RGBA(uint32_t rgba);
    ~RGBA();
    uint32_t getRGBA() const;
    uint8_t getR() const;
    uint8_t getG() const;
    uint8_t getB() const;
    uint8_t getA() const;
    bool operator==(const RGBA & rgba);
  private:
    const uint32_t _rgba;
};
}
}
}

#endif
