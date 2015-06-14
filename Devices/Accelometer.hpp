#ifndef __ACCELOMETER_H__
#define __ACCELOMETER_H__

#include "../Devices/IDevice.hpp"
#include "../Interrupts/Interrupt.hpp"
#include "../Events/Events.hpp"
#include "../System/ManagedTimer.hpp"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_spi.h"

namespace ptm
{

struct axisXY
{
	int8_t x;
	int8_t y;
};

enum Moved
{
	movedFront = 1,
	movedBack = 2,
	movedLeft = 3,
	movedRight = 4,
	none = 5
};


namespace system
{
class DeviceManager;
class InterruptManager;
}

namespace devices
{




enum AccelometerVariables{
	 LIS302DL_FLAG_TIMEOUT				= 	((uint32_t)0x1000),
	 LIS302DL_WHO_AM_I_ADDR 			= 	0x0F,
	 LIS302DL_CTRL_REG1_ADDR 			= 	0x20,
	 LIS302DL_CTRL_REG2_ADDR 			=	0x21,
	 LIS302DL_CTRL_REG3_ADDR			=	0x22,
	 LIS302DL_HP_FILTER_RESET_REG_ADDR	=	0x23,
	 LIS302DL_STATUS_REG_ADDR			=	0x27,
	 LIS302DL_OUT_X_ADDR 				=  	0x29,
	 LIS302DL_OUT_Y_ADDR 				=  	0x2B,
	 LIS302DL_OUT_Z_ADDR 				=  	0x2D,
	 LIS302DL_FF_WU_CFG1_REG_ADDR		=	0x30,
	 LIS302DL_FF_WU_SRC1_REG_ADDR		=	0x31,
	 LIS302DL_FF_WU_THS1_REG_ADDR       =	0x32,
	 LIS302DL_FF_WU_DURATION1_REG_ADDR  =	0x33,
	 LIS302DL_FF_WU_CFG2_REG_ADDR       =	0x34,
	 LIS302DL_FF_WU_SRC2_REG_ADDR       =	0x35,
	 LIS302DL_FF_WU_THS2_REG_ADDR       =	0x36,
	 LIS302DL_FF_WU_DURATION2_REG_ADDR  =	0x37,
	 LIS302DL_CLICK_CFG_REG_ADDR    	= 	0x38,
	 LIS302DL_CLICK_SRC_REG_ADDR      	=	0x39,
	 LIS302DL_CLICK_THSY_X_REG_ADDR     =	0x3B,
	 LIS302DL_CLICK_THSZ_REG_ADDR       =	0x3C,
	 LIS302DL_CLICK_TIMELIMIT_REG_ADDR  =	0x3D,
	 LIS302DL_CLICK_LATENCY_REG_ADDR    =	0x3E,
	 LIS302DL_CLICK_WINDOW_REG_ADDR     =	0x3F,
	 LIS302DL_SENSITIVITY_2_3G         	=	18,  /* 18 mg/digit*/
	 LIS302DL_SENSITIVITY_9_2G          =	72,  /* 72 mg/digit*/
	 LIS302DL_DATARATE_100 				=   ((uint8_t)0x00),
	 LIS302DL_DATARATE_400 				=	((uint8_t)0x80),
	 LIS302DL_LOWPOWERMODE_POWERDOWN 	=   ((uint8_t)0x00),
	 LIS302DL_LOWPOWERMODE_ACTIVE 		=	((uint8_t)0x40),
	 LIS302DL_FULLSCALE_2_3 			=	((uint8_t)0x00),
	 LIS302DL_FULLSCALE_9_2 			=	((uint8_t)0x20),
	 LIS302DL_SELFTEST_NORMAL 			=	((uint8_t)0x00),
	 LIS302DL_SELFTEST_P 				=	((uint8_t)0x10),
	 LIS302DL_SELFTEST_M 				=	((uint8_t)0x08),
	 LIS302DL_X_ENABLE 					=	((uint8_t)0x01),
	 LIS302DL_Y_ENABLE 					=	((uint8_t)0x02),
	 LIS302DL_Z_ENABLE 					=	((uint8_t)0x04),
	 LIS302DL_XYZ_ENABLE 				=	((uint8_t)0x07),
	 LIS302DL_SERIALINTERFACE_4WIRE 	=	((uint8_t)0x00),
	 LIS302DL_SERIALINTERFACE_3WIRE 	=	((uint8_t)0x80),
	 LIS302DL_BOOT_NORMALMODE 			=	((uint8_t)0x00),
	 LIS302DL_BOOT_REBOOTMEMORY 		=	((uint8_t)0x40),
	 LIS302DL_FILTEREDDATASELECTION_BYPASSED		=	((uint8_t)0x00),
	 LIS302DL_FILTEREDDATASELECTION_OUTPUTREGISTER  =   ((uint8_t)0x20),
	 LIS302DL_HIGHPASSFILTERINTERRUPT_OFF           =	((uint8_t)0x00),
	 LIS302DL_HIGHPASSFILTERINTERRUPT_1             =	((uint8_t)0x04),
	 LIS302DL_HIGHPASSFILTERINTERRUPT_2             =	((uint8_t)0x08),
	 LIS302DL_HIGHPASSFILTERINTERRUPT_1_2           =	((uint8_t)0x0C),
	 LIS302DL_HIGHPASSFILTER_LEVEL_0 				=   ((uint8_t)0x00),
	 LIS302DL_HIGHPASSFILTER_LEVEL_1 				=   ((uint8_t)0x01),
	 LIS302DL_HIGHPASSFILTER_LEVEL_2 				=   ((uint8_t)0x02),
	 LIS302DL_HIGHPASSFILTER_LEVEL_3 				=   ((uint8_t)0x03),
	 LIS302DL_INTERRUPTREQUEST_NOTLATCHED           =   ((uint8_t)0x00),
	 LIS302DL_INTERRUPTREQUEST_LATCHED 				= 	((uint8_t)0x40),
	 LIS302DL_CLICKINTERRUPT_XYZ_DISABLE            =	((uint8_t)0x00),
	 LIS302DL_CLICKINTERRUPT_X_ENABLE 				=   ((uint8_t)0x01),
	 LIS302DL_CLICKINTERRUPT_Y_ENABLE 				=   ((uint8_t)0x04),
	 LIS302DL_CLICKINTERRUPT_Z_ENABLE 				=   ((uint8_t)0x10),
	 LIS302DL_CLICKINTERRUPT_XYZ_ENABLE				=	((uint8_t)0x15),
	 LIS302DL_DOUBLECLICKINTERRUPT_XYZ_DISABLE      =   ((uint8_t)0x00),
	 LIS302DL_DOUBLECLICKINTERRUPT_X_ENABLE         =   ((uint8_t)0x02),
	 LIS302DL_DOUBLECLICKINTERRUPT_Y_ENABLE         =   ((uint8_t)0x08),
	 LIS302DL_DOUBLECLICKINTERRUPT_Z_ENABLE         =   ((uint8_t)0x20),
	 LIS302DL_DOUBLECLICKINTERRUPT_XYZ_ENABLE       =   ((uint8_t)0x2A),
	 READWRITE_CMD									=	((uint8_t)0x80),
	 MULTIPLEBYTE_CMD								=	((uint8_t)0x40),
	 DUMMY_BYTE 									=	((uint8_t)0x00)
};

class Accelometer : public IDevice {
		public:

			typedef struct
			{
			  uint8_t Power_Mode;                         /* Power-down/Active Mode */
			  uint8_t Output_DataRate;                    /* OUT data rate 100 Hz / 400 Hz */
			  uint8_t Axes_Enable;                        /* Axes enable */
			  uint8_t Full_Scale;                         /* Full scale */
			  uint8_t Self_Test;                          /* Self test */
			}LIS302DL_InitTypeDef;

			/* LIS302DL High Pass Filter struct */
			typedef struct
			{
			  uint8_t HighPassFilter_Data_Selection;      /* Internal filter bypassed or data from internal filter send to output register*/
			  uint8_t HighPassFilter_CutOff_Frequency;    /* High pass filter cut-off frequency */
			  uint8_t HighPassFilter_Interrupt;           /* High pass filter enabled for Freefall/WakeUp #1 or #2 */
			}LIS302DL_FilterConfigTypeDef;

			/* LIS302DL Interrupt struct */
			typedef struct
			{
			  uint8_t Latch_Request;                      /* Latch interrupt request into CLICK_SRC register*/
			  uint8_t SingleClick_Axes;                   /* Single Click Axes Interrupts */
			  uint8_t DoubleClick_Axes;                   /* Double Click Axes Interrupts */
			}LIS302DL_InterruptConfigTypeDef;


			friend class DeviceManager;
			~Accelometer();
			__IO uint32_t  LIS302DLTimeout = LIS302DL_FLAG_TIMEOUT;

			struct Axis {
				int8_t ACCX;
				int8_t ACCY;
				int8_t ACCZ;
			} axis;

			Moved accelDirection;
			void setAccDirection(Moved a);

			void checkForMenuEvents();
			void resetAllifEvent();

			bool isMovedTop();
			bool isMovedDown();
			bool isMovedRight();
			bool isMovedLeft();


			system::SystemTimer _timer;
			SPI_TypeDef* _spi;
			PinAFMapping _sck;
			PinAFMapping _miso;
			PinAFMapping _mosi;
			Pin _cs;
			Pin _int1;
			Pin _int2;

			void updateAccelometerAxis();
			bool ifAccelometerMoved();

			inline void LIS302DL_CS_HIGH();
			inline void LIS302DL_CS_LOW();
			void LIS302DL_Init(LIS302DL_InitTypeDef *LIS302DL_InitStruct);
			void LIS302DL_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
			uint8_t LIS302DL_SendByte(uint8_t byte);
			void LIS302DL_InterruptConfig(LIS302DL_InterruptConfigTypeDef *LIS302DL_IntConfigStruct);
			void LIS302DL_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
			uint32_t LIS302DL_TIMEOUT_UserCallback(void);


			Accelometer(
						SPI_TypeDef* spi,
						PinAFMapping sck,
						PinAFMapping miso,
						PinAFMapping mosi,
						Pin cs,
						Pin int1,
						Pin int2
						);

		  protected:
		  private:






};
} // namespace devices


namespace events
{
class AccelometerMenuEvent : public Event
{
 public:
   AccelometerMenuEvent(bool top, bool down, std::weak_ptr<devices::Accelometer> device);
   ~AccelometerMenuEvent();

   bool movedTop() const;
   bool movedDown() const;
   std::weak_ptr<devices::Accelometer> getDevice() const;
 protected:
   std::weak_ptr<devices::Accelometer> _device;
   bool _top;
   bool _down;
};

class AccelometerGetPositionEvent : public Event
{
 public:
	AccelometerGetPositionEvent(axisXY xy, std::weak_ptr<devices::Accelometer> device);
   ~AccelometerGetPositionEvent();

   axisXY getPosition() const;
   std::weak_ptr<devices::Accelometer> getDevice() const;
 protected:
   std::weak_ptr<devices::Accelometer> _device;
   axisXY _xy;
};

} // namespace events


} // namespace ptm
#endif
