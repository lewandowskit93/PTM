#ifndef __DEVICES_DISPLAY_H__
#define __DEVICES_DISPLAY_H__

#include "../Devices/IDevice.hpp"
#include "../Utilities/Color.hpp"

namespace ptm
{
namespace devices
{
namespace displays
{

class IDisplay : public IDevice
{
  public:
    IDisplay(uint32_t width, uint32_t height);
    virtual ~IDisplay();
    void refresh();
    virtual void refreshArea(uint32_t x, uint32_t y, uint32_t width, uint32_t height)=0;
    void clear();
    void clearArea(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    virtual void clearPixel(uint32_t x, uint32_t y)=0;
    virtual void setPixelColor(uint32_t x, uint32_t y, utilities::colors::RGBA color)=0;
    virtual utilities::colors::RGBA getPixelColor(uint32_t x, uint32_t y)=0;
    uint32_t getWidth();
    uint32_t getHeight();
  private:
    uint32_t _width;
    uint32_t _height;
};

namespace monochromatic
{

class IMonochromaticDisplay : public IDisplay
{
  public:
    IMonochromaticDisplay(uint32_t width, uint32_t height);
    virtual ~IMonochromaticDisplay();
    void setPixel(uint32_t x, uint32_t y);
    void resetPixel(uint32_t x, uint32_t y);
    void togglePixel(uint32_t x, uint32_t y);
    virtual void clearPixel(uint32_t x, uint32_t y);
};

namespace PCD8544
{

namespace commands
{
enum Commands
{
  // COMMON
  NOP = 0x00,
  FUNCTION_SET = 0x20,
  DEVICE_MODE = FUNCTION_SET,
  // BASIC INSTRUCTION SET
  DISPLAY_MODE = 0x08,
  SET_Y = 0x40,
  SET_X = 0x80,
  // EXTENTED INSTRUCTION SET
  TEMPERATURE_CONTROL = 0x04,
  BIAS_SYSTEM = 0x10,
  SET_VOP = 0x80,
  SET_CONTRAST = SET_VOP
};
} // namespace commands

namespace display_modes
{
enum DisplayModes
{
  BLANK = 0x00, NORMAL = 0x04, ALL_ON = 0x01, INVERSE = 0x05
};
} // namespace display_modes

namespace temperature_modes
{
enum TemperatureModes
{
  COEF_0 = 0x00, COEF_1 = 0x01, COEF_2 = 0x02, COEF_3 = 0x03
};
} // namespace temperature_modes

class PCD8544 : public IMonochromaticDisplay
{
  public:
    PCD8544(SPI_TypeDef* spi, PinAFMapping spi_mosi, PinAFMapping spi_clk,
        Pin spi_data_command, Pin spi_chip_select, Pin spi_reset);
    ~PCD8544();

    virtual void refreshArea(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    virtual void setPixelColor(uint32_t x, uint32_t y, utilities::colors::RGBA color);
    virtual utilities::colors::RGBA getPixelColor(uint32_t x, uint32_t y);
  private:
    uint32_t getPixelByteOffset(uint32_t x, uint32_t y);
    uint32_t getPixelBitOffset(uint32_t x, uint32_t y);

    void initPins();
    void initSPI();
    void initDevice();
    void selectDevice();
    void deselectDevice();
    void resetDevice();
    void setData();
    void setCommand();
    void setX(uint32_t x);
    void setY(uint32_t y);
    void setXY(uint32_t x, uint32_t y);
    void extendedInstructions();
    void basicInstructions();
    void powerDown();
    void powerOn();
    void horizontalAddressing();
    void verticalAddresing();
    uint8_t getDeviceMode();
    void setDisplayMode(display_modes::DisplayModes mode);
    void setBiasSystem(uint8_t bias);
    void setContrast(uint8_t contrast);
    void setTemperatureMode(temperature_modes::TemperatureModes mode);
    void WriteCommand(uint8_t command);
    void WriteCommands(uint8_t *commands, uint32_t n);
    void WriteData(uint8_t data);
    void WriteData(uint8_t *data, uint32_t n);
    void Write(uint8_t value);
    void Write(uint8_t *values, uint32_t n);
    uint8_t _buffer[504];
    SPI_TypeDef* _spi;
    PinAFMapping _spi_mosi;
    PinAFMapping _spi_clk;
    Pin _spi_data_command;
    Pin _spi_chip_select;
    Pin _spi_reset;
    bool _powered_down;
    bool _vertical_addressing;
    bool _extended_functions;

};

} // namespace PCD8544
} // namespace monochromatic
} // namespace displays
} // namespace devices
} // namespace ptm

#endif
