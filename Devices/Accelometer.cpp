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
	    IDevice(), _timer(500,std::bind(&Accelometer::updateAccelometerAxis,this), true),
	    _spi(spi),  _sck(sck), _miso(miso),
	    _mosi(mosi), _cs(cs), _int1(int1), _int2(int2)
{

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

void Accelometer::updateAccelometerAxis()
{
	LIS302DL_Read(&axis.ACCX, LIS302DL_OUT_X_ADDR, 1);
	if(axis.ACCX > 60 || axis.ACCX < -60)
	{
		axis.ACCX = axis.ACCX - 1;
		axis.ACCX = ( ~axis.ACCX ) & 0xFF;
		axis.ACCX = -axis.ACCX;

	}
	LIS302DL_Read(&axis.ACCY, LIS302DL_OUT_Y_ADDR, 1);
	if(axis.ACCY >60 || axis.ACCY < -60)
	{
		axis.ACCY = axis.ACCY - 1;
		axis.ACCY= ( ~axis.ACCY ) & 0xFF;
		axis.ACCY= -axis.ACCY;

	}
	LIS302DL_Read(&axis.ACCZ, LIS302DL_OUT_Z_ADDR, 1);
	if( axis.ACCZ > 60 || axis.ACCZ < -60 )
	{
		axis.ACCZ = axis.ACCZ - 1;
		axis.ACCZ = ( ~axis.ACCZ ) & 0xFF;
		axis.ACCZ = -axis.ACCZ;
	}
	_timer.start();
}

ptm::Moved Accelometer::getAccelometerAxis()
{
	LIS302DL_Read(&axis.ACCX, LIS302DL_OUT_X_ADDR, 1);
	if(axis.ACCX > 60 || axis.ACCX < -60)
	{
		axis.ACCX = axis.ACCX - 1;
		axis.ACCX = ( ~axis.ACCX ) & 0xFF;
		axis.ACCX = -axis.ACCX;
		return ptm::movedLeft;
	}
	LIS302DL_Read(&axis.ACCY, LIS302DL_OUT_Y_ADDR, 1);
	if(axis.ACCY >60 || axis.ACCY < -60)
	{
		axis.ACCY = axis.ACCY - 1;
		axis.ACCY= ( ~axis.ACCY ) & 0xFF;
		axis.ACCY= -axis.ACCY;
		return ptm::movedFront;
	}
	LIS302DL_Read(&axis.ACCZ, LIS302DL_OUT_Z_ADDR, 1);
	if( axis.ACCZ > 60 || axis.ACCZ < -60 )
	{
		axis.ACCZ = axis.ACCZ - 1;
		axis.ACCZ = ( ~axis.ACCZ ) & 0xFF;
		axis.ACCZ = -axis.ACCZ;
		return ptm::movedBack;
	}
	return ptm::movedBack;

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
  LIS302DL_SendByte(this, WriteAddr);
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    LIS302DL_SendByte(this, *pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }

  /* Set chip select High at the end of the transmission */
  LIS302DL_CS_HIGH();
}

uint8_t Accelometer::LIS302DL_SendByte(Accelometer *a, uint8_t byte)
{
  /* Loop while DR register in not emplty */
  a->LIS302DLTimeout = LIS302DL_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(a->_spi, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((a->LIS302DLTimeout--) == 0) return a->LIS302DL_TIMEOUT_UserCallback();
  }

  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(a->_spi, byte);

  /* Wait to receive a Byte */
  a->LIS302DLTimeout = LIS302DL_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(a->_spi, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((a->LIS302DLTimeout--) == 0) return a->LIS302DL_TIMEOUT_UserCallback();
  }

  /* Return the Byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(a->_spi);
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
  LIS302DL_SendByte(this, ReadAddr);

  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to LIS302DL (Slave device) */
    *pBuffer = LIS302DL_SendByte(this, DUMMY_BYTE);
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
////Menu events
AccelometerMenuEvents::AccelometerMenuEvents(Moved moved, std::weak_ptr<devices::Accelometer> device) :
    Event(EventType::EVENT_EXTI0_IRQn, device.lock().get()), _moved(moved), _device(device)
{

}

AccelometerMenuEvents::~AccelometerMenuEvents()
{

}

ptm::Moved AccelometerMenuEvents::isMoved() const
{
  return _moved;
}

std::weak_ptr<devices::Accelometer> AccelometerMenuEvents::getDevice() const
{
  return _device;
}

//// In Game events
AccelometerInGameEvents::AccelometerInGameEvents(bool moved, std::weak_ptr<devices::Accelometer> device) :
    Event(EventType::EVENT_EXTI0_IRQn, device.lock().get()), _moved(moved), _device(device)
{

}

AccelometerInGameEvents::~AccelometerInGameEvents()
{

}

bool AccelometerInGameEvents::isMoved() const
{
  return _moved;
}

std::weak_ptr<devices::Accelometer>AccelometerInGameEvents::getDevice() const
{
  return _device;
}

} // namespace events


} // ptm namespace
