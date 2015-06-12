#include "../Devices/Display.hpp"
#include "../cmsis_lib/include/stm32f4xx_spi.h"
#include "../cmsis_lib/include/stm32f4xx_gpio.h"
#include "../cmsis_lib/include/stm32f4xx_rcc.h"

namespace ptm
{
namespace devices
{
namespace displays
{

IDisplay::IDisplay(uint32_t width, uint32_t height) :
    IDevice(), _width(width), _height(height)
{

}

IDisplay::~IDisplay()
{

}

uint32_t IDisplay::getWidth()
{
  return _width;
}
uint32_t IDisplay::getHeight()
{
  return _height;
}

void IDisplay::refresh()
{
  refreshArea(0, 0, _width, _height);
}

void IDisplay::clear()
{
  clearArea(0, 0, _width, _height);
}

void IDisplay::clearArea(uint32_t x, uint32_t y, uint32_t width,
    uint32_t height)
{
  if(x>_width)x=_width;
  if(y>_height)y=_height;
  if((width+x) > _width)width=_width-x;
  if((height+y) > _height)height=_height-y;
  for (uint32_t i=0; i < width; ++i)
  {
    for (uint32_t j = 0; j < height; ++j)
    {
      clearPixel(x+i, y+j);
    }
  }
}

namespace monochromatic
{

IMonochromaticDisplay::IMonochromaticDisplay(uint32_t width, uint32_t height) :
    IDisplay(width, height)
{

}

IMonochromaticDisplay::~IMonochromaticDisplay()
{

}

void IMonochromaticDisplay::setPixel(uint32_t x, uint32_t y)
{
  setPixelColor(x, y, utilities::colors::RGBA(0xFFFFFFFF));
}

void IMonochromaticDisplay::resetPixel(uint32_t x, uint32_t y)
{
  clearPixel(x, y);
}

void IMonochromaticDisplay::togglePixel(uint32_t x, uint32_t y)
{
  if (getPixelColor(x, y) == utilities::colors::RGBA(0xFFFFFFFF))
  {
    resetPixel(x, y);
  }
  else
    setPixel(x, y);
}

void IMonochromaticDisplay::clearPixel(uint32_t x, uint32_t y)
{
  setPixelColor(x, y, utilities::colors::RGBA(0x00000000));
}

namespace PCD8544
{

PCD8544::PCD8544(SPI_TypeDef* spi, PinAFMapping spi_mosi, PinAFMapping spi_clk,
    Pin spi_data_command, Pin spi_chip_select, Pin spi_reset) :
    IMonochromaticDisplay(84, 48), _spi(spi), _spi_mosi(spi_mosi), _spi_clk(
        spi_clk), _spi_data_command(spi_data_command), _spi_chip_select(
        spi_chip_select), _spi_reset(spi_reset), _powered_down(false), _vertical_addressing(
        false), _extended_functions(false)
{
  for (uint32_t i = 0; i < 504; ++i)
  {
    _buffer[i] = 0;
  }

  initPins();
  initSPI();
  initDevice();
}

void PCD8544::initPins()
{
  GPIO_InitTypeDef pin_init;
  pin_init.GPIO_Mode = GPIO_Mode_AF;
  pin_init.GPIO_OType = GPIO_OType_PP;
  pin_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  pin_init.GPIO_Speed = GPIO_Speed_100MHz;

  //mosi pin
  pin_init.GPIO_Pin = _spi_mosi.pin.pin;
  GPIO_PinAFConfig(_spi_mosi.pin.port, _spi_mosi.pin_source,
      _spi_mosi.alternate_function);
  GPIO_Init(_spi_mosi.pin.port, &pin_init);

  //clk pin
  pin_init.GPIO_Pin = _spi_clk.pin.pin;
  GPIO_PinAFConfig(_spi_clk.pin.port, _spi_clk.pin_source,
      _spi_clk.alternate_function);
  GPIO_Init(_spi_clk.pin.port, &pin_init);

  pin_init.GPIO_Mode = GPIO_Mode_OUT;
  pin_init.GPIO_OType = GPIO_OType_PP;
  pin_init.GPIO_Speed = GPIO_Speed_100MHz;

  //data/command pin
  pin_init.GPIO_Pin = _spi_data_command.pin;
  GPIO_Init(_spi_data_command.port, &pin_init);

  //chip select pin
  pin_init.GPIO_Pin = _spi_chip_select.pin;
  GPIO_Init(_spi_chip_select.port, &pin_init);

  //reset pin
  pin_init.GPIO_Pin = _spi_reset.pin;
  GPIO_Init(_spi_reset.port, &pin_init);

  deselectDevice();
}

void PCD8544::initSPI()
{
  SPI_I2S_DeInit(_spi);
  SPI_InitTypeDef spi_init;
  spi_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spi_init.SPI_Mode = SPI_Mode_Master;
  spi_init.SPI_DataSize = SPI_DataSize_8b;
  spi_init.SPI_CPOL = SPI_CPOL_Low;
  spi_init.SPI_CPHA = SPI_CPHA_1Edge;
  spi_init.SPI_NSS = SPI_NSS_Soft;
  spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  spi_init.SPI_FirstBit = SPI_FirstBit_MSB;
  spi_init.SPI_CRCPolynomial = 7;
  SPI_Init(_spi, &spi_init);
  SPI_Cmd(_spi, ENABLE);
}

void PCD8544::initDevice()
{
  resetDevice();
  powerOn();
  extendedInstructions();
  setBiasSystem(0x04);
  setContrast(0x3f);
  setTemperatureMode(temperature_modes::COEF_2);
  basicInstructions();
  horizontalAddressing();
  setDisplayMode(display_modes::NORMAL);
  setXY(0, 0);
  refresh();
}

PCD8544::~PCD8544()
{
}

void PCD8544::refreshArea(uint32_t x, uint32_t y, uint32_t width,
    uint32_t height)
{
  //ToDo support area refreshing when x!=0 || y!=0 || width!=_width || height!=_height, otherwise full refresh
  setXY(0, 0);
  WriteData(_buffer, 504);
}

uint32_t PCD8544::getPixelByteOffset(uint32_t x, uint32_t y)
{
  return x + (y / 8) * 84;
}

uint32_t PCD8544::getPixelBitOffset(uint32_t x, uint32_t y)
{
  return y % 8;
}

void PCD8544::setPixelColor(uint32_t x, uint32_t y, utilities::colors::RGBA color)
{
  if (color == utilities::colors::RGBA(0x00000000))
  {
    _buffer[getPixelByteOffset(x, y)] &= ~(1 << (getPixelBitOffset(x,y)));
  }
  else
  {
    _buffer[getPixelByteOffset(x, y)] |= 1 << (getPixelBitOffset(x,y));
  }
}

utilities::colors::RGBA PCD8544::getPixelColor(uint32_t x, uint32_t y)
{
  if ((_buffer[getPixelByteOffset(x, y)] & (1 << (getPixelBitOffset(x,y)))) == (1 << (getPixelBitOffset(x,y))))
  {
    return utilities::colors::RGBA(0xFFFFFFFF);
  }
  else
  {
    return utilities::colors::RGBA(0x00000000);
  }
}

void PCD8544::selectDevice()
{
  GPIO_ResetBits(_spi_chip_select.port, _spi_chip_select.pin);
}

void PCD8544::deselectDevice()
{
  GPIO_SetBits(_spi_chip_select.port, _spi_chip_select.pin);
}

void PCD8544::resetDevice()
{
  GPIO_ResetBits(_spi_reset.port, _spi_reset.pin);
  GPIO_SetBits(_spi_reset.port, _spi_reset.pin);
}

void PCD8544::setData()
{
  GPIO_SetBits(_spi_data_command.port, _spi_data_command.pin);
}

void PCD8544::setCommand()
{
  GPIO_ResetBits(_spi_data_command.port, _spi_data_command.pin);
}

void PCD8544::setX(uint32_t x)
{
  if (x >= 0 && x < 84)
  {
    WriteCommand(commands::SET_X | x);
  }
}

void PCD8544::setY(uint32_t y)
{
  if (y >= 0 && y < 6)
  {
    WriteCommand(commands::SET_Y | y);
  }
}

void PCD8544::setXY(uint32_t x, uint32_t y)
{
  if (x >= 0 && x < 84 && y >= 0 && y < 6)
  {
    uint8_t commands[2] =
    { (uint8_t)(commands::SET_X | (uint8_t) x), (uint8_t)(
        commands::SET_Y | (uint8_t) y) };
    WriteCommands(commands, 2);
  }
}

void PCD8544::extendedInstructions()
{
  _extended_functions = true;
  WriteCommand(commands::DEVICE_MODE | getDeviceMode());
}

void PCD8544::basicInstructions()
{
  _extended_functions = false;
  WriteCommand(commands::DEVICE_MODE | getDeviceMode());
}

void PCD8544::powerDown()
{
  _powered_down = true;
  WriteCommand(commands::DEVICE_MODE | getDeviceMode());
}

void PCD8544::powerOn()
{
  _powered_down = false;
  WriteCommand(commands::DEVICE_MODE | getDeviceMode());
}

void PCD8544::horizontalAddressing()
{
  _vertical_addressing = false;
  WriteCommand(commands::DEVICE_MODE | getDeviceMode());
}

void PCD8544::verticalAddresing()
{
  _vertical_addressing = true;
  WriteCommand(commands::DEVICE_MODE | getDeviceMode());
}

uint8_t PCD8544::getDeviceMode()
{
  uint8_t mode = 0x00;
  if (_extended_functions)
    mode |= 0x01;
  if (_vertical_addressing)
    mode |= 0x02;
  if (_powered_down)
    mode |= 0x04;
  return mode;
}

void PCD8544::setDisplayMode(display_modes::DisplayModes mode)
{
  WriteCommand(commands::DISPLAY_MODE | mode);
}

void PCD8544::setBiasSystem(uint8_t bias)
{
  if (bias >= 0 && bias < 8)
  {
    WriteCommand(commands::BIAS_SYSTEM | bias);
  }
}

void PCD8544::setContrast(uint8_t contrast)
{
  if (contrast >= 0 && contrast < 0x80)
  {
    WriteCommand(commands::SET_CONTRAST | contrast);
  }
}

void PCD8544::setTemperatureMode(temperature_modes::TemperatureModes mode)
{
  WriteCommand(commands::TEMPERATURE_CONTROL | mode);
}

void PCD8544::WriteCommand(uint8_t command)
{
  setCommand();
  Write(command);
}

void PCD8544::WriteCommands(uint8_t *commands, uint32_t n)
{
  setCommand();
  Write(commands, n);
}

void PCD8544::WriteData(uint8_t data)
{
  setData();
  Write(data);
}

void PCD8544::WriteData(uint8_t *data, uint32_t n)
{
  setData();
  Write(data, n);
}

void PCD8544::Write(uint8_t value)
{
  selectDevice();
  while (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_TXE) == RESET)
    ;
  SPI_I2S_SendData(_spi, value);
  while (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_RXNE) == RESET)
    ;
  SPI_I2S_ReceiveData(_spi);
  deselectDevice();
}

void PCD8544::Write(uint8_t *values, uint32_t n)
{
  selectDevice();
  for (uint32_t i = 0; i < n; ++i)
  {
    while (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_TXE) == RESET)
      ;
    SPI_I2S_SendData(_spi, values[i]);
    while (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_RXNE) == RESET)
      ;
    SPI_I2S_ReceiveData(_spi);
  }
  deselectDevice();
}

} // namespace PCD8544
} // namespace monochromatic
} // namespace displays
} // namespace devices
} // namespace ptm
