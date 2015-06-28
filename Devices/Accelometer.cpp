#include "../Devices/Accelometer.hpp"
#include "../System/System.hpp"
#include "../Events/Events.hpp"
namespace ptm
{
namespace devices
{
inline void Accelometer::LIS302DL_CS_LOW()
{
	GPIO_ResetBits(_cs.port, _cs.pin);

}

inline void Accelometer::LIS302DL_CS_HIGH()
{
	GPIO_SetBits(_cs.port, _cs.pin);
}

Accelometer::Accelometer(SPI_TypeDef* spi, PinAFMapping sck, PinAFMapping miso, PinAFMapping mosi,
		Pin cs, Pin int1, Pin int2) :
		 IDevice(), _timer(100,std::bind(&Accelometer::updateAccelometerAxis,this), true),
	    _spi(spi),  _sck(sck), _miso(miso),
	    _mosi(mosi), _cs(cs), _int1(int1), _int2(int2)

{

	 	 GPIO_PinAFConfig(_sck.pin.port, _sck.pin_source, _sck.alternate_function);
	 	 GPIO_PinAFConfig(_miso.pin.port, _miso.pin_source, _miso.alternate_function);
	 	 GPIO_PinAFConfig(_mosi.pin.port, _mosi.pin_source, _mosi.alternate_function);

	    GPIO_InitTypeDef GPIO_InitStructure;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	    /* SPI SCK pin configuration */
	    GPIO_InitStructure.GPIO_Pin = _sck.pin.pin;
	    GPIO_Init(_sck.pin.port, &GPIO_InitStructure);

	    /* SPI  MOSI pin configuration */
	    GPIO_InitStructure.GPIO_Pin =  _mosi.pin.pin;
	    GPIO_Init(_mosi.pin.port, &GPIO_InitStructure);

	    /* SPI MISO pin configuration */
	    GPIO_InitStructure.GPIO_Pin = _miso.pin.pin;
	    GPIO_Init(_miso.pin.port, &GPIO_InitStructure);

	    /* SPI configuration -------------------------------------------------------*/
	    SPI_InitTypeDef  SPI_InitStructure;
	    SPI_I2S_DeInit(_spi);
	    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	    SPI_InitStructure.SPI_CRCPolynomial = 7;
	    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	    SPI_Init(_spi, &SPI_InitStructure);

	    SPI_Cmd(_spi, ENABLE);


	    /* Configure GPIO PIN for Lis Chip select */
	    GPIO_InitStructure.GPIO_Pin = _cs.pin;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(_cs.port, &GPIO_InitStructure);

	    /* Deselect : Chip Select high */
	    GPIO_SetBits(_cs.port, _cs.pin);

	    /* Configure GPIO PINs to detect Interrupts */
	    GPIO_InitStructure.GPIO_Pin = _int1.pin;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	    GPIO_Init(_int1.port, &GPIO_InitStructure);

	    GPIO_InitStructure.GPIO_Pin = _int2.pin;
	    GPIO_Init(_int2.port, &GPIO_InitStructure);


	    LIS302DL_InitTypeDef  LIS302DL_InitStruct;

	    /* Set configuration of LIS302DL*/
	    LIS302DL_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	    LIS302DL_InitStruct.Output_DataRate = LIS302DL_DATARATE_100;
	    LIS302DL_InitStruct.Axes_Enable = LIS302DL_X_ENABLE | LIS302DL_Y_ENABLE | LIS302DL_Z_ENABLE;
	    LIS302DL_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
	    LIS302DL_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
	    LIS302DL_Init(&LIS302DL_InitStruct);

	    LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;
	    /* Set configuration of Internal High Pass Filter of LIS302DL*/
	    LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_LATCHED;
	    LIS302DL_InterruptStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_Z_ENABLE;
	    LIS302DL_InterruptStruct.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_Z_ENABLE;
	    LIS302DL_InterruptConfig(&LIS302DL_InterruptStruct);
	    _timer.start();

}


Accelometer::~Accelometer()
{
	//ToDo
}
void Accelometer::setAccDirection(Moved a)
{
	accelDirection = a;
}

bool Accelometer::isMovedTop()
{
	if (accelDirection == movedFront)
		return true;
	return false;
}

bool Accelometer::isMovedDown()
{
	if (accelDirection == movedBack)
			return true;
		return false;
}
bool Accelometer::isMovedRight()
{
	if (accelDirection == movedRight)
		return true;
	return false;
}

bool Accelometer::isMovedLeft()
{
	if (accelDirection == movedLeft)
			return true;
		return false;
}

void Accelometer::resetAllifEvent()
{
	accelDirection = none;
}

void Accelometer::checkForMenuEvents()
{
	std::weak_ptr<devices::Accelometer> _device;
	auto x = _device.lock();
	auto a = std::dynamic_pointer_cast < devices::Accelometer > (x);

	if (isMovedDown())
	{
		system::System::getInstance()->_event_manager.raiseEvent(
		  std::shared_ptr < events::AccelometerMenuEvent
			  > (new events::AccelometerMenuEvent(this->isMovedDown(), this->isMovedTop(),a)));

	 }
	else if (isMovedTop())
	{
		 	system::System::getInstance()->_event_manager.raiseEvent(
			  std::shared_ptr < events::AccelometerMenuEvent
				  > (new events::AccelometerMenuEvent(this->isMovedDown(), this->isMovedTop(),a)));
	}

	ptm::axisXY axis;
	axis.x = this->axis.ACCX;
	axis.y = this->axis.ACCY;
	system::System::getInstance()->_event_manager.raiseEvent(
				  std::shared_ptr < events::AccelometerGetPositionEvent
					  > (new events::AccelometerGetPositionEvent(axis,a)));

	resetAllifEvent();

}


void Accelometer::updateAccelometerAxis()
{
	uint8_t x, y, z;
	LIS302DL_Read(&x, LIS302DL_OUT_X_ADDR, 1);
	x = x - 1;
	x = ( ~x ) & 0xFF;
	x = -x;

	LIS302DL_Read(&y, LIS302DL_OUT_Y_ADDR, 1);
	y = y - 1;
	y = ( ~y ) & 0xFF;
	y= -y;

	LIS302DL_Read(&z, LIS302DL_OUT_Z_ADDR, 1);
	z = z- 1;
	z = ( ~z ) & 0xFF;
	z = -z;
	axis.ACCX = (int8_t) x; // values are from -66 to 66 approximtally
	axis.ACCY = (int8_t) y;
	axis.ACCZ = (int8_t) z;

	if (axis.ACCY > 15)
		setAccDirection(movedBack);
	else if (axis.ACCY < -15)
		setAccDirection(movedFront);
	else if (axis.ACCX > 15)
		setAccDirection(movedLeft);
	else if (axis.ACCX < - 15)
		setAccDirection(movedRight);

	checkForMenuEvents();


	_timer.start();
}




void Accelometer::LIS302DL_Init(LIS302DL_InitTypeDef *LIS302DL_InitStruct)
{
  uint8_t ctrl = 0x00;

  /* Configure the low level interface ---------------------------------------*/
//  LIS302DL_LowLevel_Init();

  /* Configure MEMS: data rate, power mode, full scale, self test and axes */
  ctrl = (uint8_t) (LIS302DL_InitStruct->Output_DataRate | LIS302DL_InitStruct->Power_Mode | \
                    LIS302DL_InitStruct->Full_Scale | LIS302DL_InitStruct->Self_Test | \
                    LIS302DL_InitStruct->Axes_Enable);

  /* Write value to MEMS CTRL_REG1 regsister */
  LIS302DL_Write(&ctrl, LIS302DL_CTRL_REG1_ADDR, 1);
}

void Accelometer::LIS302DL_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Configure the MS bit:
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  LIS302DL_CS_LOW();

  /* Send the Address of the indexed register */
  LIS302DL_SendByte(WriteAddr);
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    LIS302DL_SendByte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }

  /* Set chip select High at the end of the transmission */
  LIS302DL_CS_HIGH();
}

uint8_t Accelometer::LIS302DL_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  LIS302DLTimeout = LIS302DL_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((LIS302DLTimeout--) == 0) return LIS302DL_TIMEOUT_UserCallback();
  }

  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(_spi, byte);

  /* Wait to receive a Byte */
  LIS302DLTimeout = LIS302DL_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((LIS302DLTimeout--) == 0) return LIS302DL_TIMEOUT_UserCallback();
  }

  /* Return the Byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(_spi);
}

void Accelometer::LIS302DL_InterruptConfig(LIS302DL_InterruptConfigTypeDef *LIS302DL_IntConfigStruct)
{
  uint8_t ctrl = 0x00;

  /* Read CLICK_CFG register */
  LIS302DL_Read(&ctrl, LIS302DL_CLICK_CFG_REG_ADDR, 1);

  /* Configure latch Interrupt request, click interrupts and double click interrupts */
  ctrl = (uint8_t)(LIS302DL_IntConfigStruct->Latch_Request| \
                   LIS302DL_IntConfigStruct->SingleClick_Axes | \
                   LIS302DL_IntConfigStruct->DoubleClick_Axes);

  /* Write value to MEMS CLICK_CFG register */
  LIS302DL_Write(&ctrl, LIS302DL_CLICK_CFG_REG_ADDR, 1);
}

void Accelometer::LIS302DL_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  LIS302DL_CS_LOW();

  /* Send the Address of the indexed register */
  LIS302DL_SendByte(ReadAddr);

  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to LIS302DL (Slave device) */
    *pBuffer = LIS302DL_SendByte(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }

  /* Set chip select High at the end of the transmission */
  LIS302DL_CS_HIGH();
}

/**
  * @brief  MEMS accelerometre management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t Accelometer::LIS302DL_TIMEOUT_UserCallback(void)
{
  /* MEMS Accelerometer Timeout error occured */
 return 0;
}

} // device namespace




namespace events
{

AccelometerMenuEvent::AccelometerMenuEvent(bool movedTop, bool movedDown, std::weak_ptr<devices::Accelometer> device) :
		Event(EventType::EVENT_ACC_IN_MENU, device.lock().get()), _top(movedTop), _down(movedDown), _device(device)
{

}

AccelometerMenuEvent::~AccelometerMenuEvent()
{

}
bool AccelometerMenuEvent::movedTop() const
{
  return _top;
}

bool AccelometerMenuEvent::movedDown() const
{
  return _down;
}

std::weak_ptr<devices::Accelometer> AccelometerMenuEvent::getDevice() const
{
  return _device;
}
/////////////////////////////////////////////////
//AccelInGame

AccelometerGetPositionEvent::AccelometerGetPositionEvent(ptm::axisXY xy, std::weak_ptr<devices::Accelometer> device) :
		Event(EventType::EVENT_ACC_IN_GAME, device.lock().get()), _xy(xy), _device(device)
{

}

AccelometerGetPositionEvent::~AccelometerGetPositionEvent()
{

}
ptm::axisXY AccelometerGetPositionEvent::getPosition() const
{
  return _xy;
}



std::weak_ptr<devices::Accelometer> AccelometerGetPositionEvent::getDevice() const
{
  return _device;
}

} // event namespace

} // ptm namespace
