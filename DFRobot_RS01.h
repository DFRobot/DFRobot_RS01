/*!
 * @file  DFRobot_RS01.h
 * @brief  Define infrastructure of DFRobot_RS01 class
 * @details  Get and configure the sensor basic information and measurement parameters, and the sensor measurement information
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2021-07-06
 * @url  https://github.com/DFRobot/DFRobot_RS01
 */
#ifndef __DFROBOT_RS01_H__
#define __DFROBOT_RS01_H__

#include <Arduino.h>
#include <Stream.h>
#include <DFRobot_RTU.h>

// #define ENABLE_DBG   //!< Open the macro and you can see the details of the program
#ifdef ENABLE_DBG
  #define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
  #define DBG(...)
#endif

#define RS01_PID                     uint16_t(0x01E9)   ///< module PID (The highest two of 16-bit data are used to determine SKU type: 00: SEN, 01: DFR, 10: TEL, the next 14 are numbers.)(SEN0489)

/* RS01 register address for basic information */
#define RS01_PID_REG                 uint16_t(0x0000)   ///< module PID memory register, the default value is 0x01E9 (The highest two of 16-bit data are used to determine SKU type: 00: SEN, 01: DFR, 10: TEL, the next 14 are numbers.)(SEN0489)
#define RS01_VID_REG                 uint16_t(0x0001)   ///< module VID memory register, the default value is 0x3343 (represent manufacturer DFRobot)
#define RS01_ADDR_REG                uint16_t(0x0002)   ///< memory register for module communication address, the default value is 0x000E, module device address(1~247)
#define RS01_BAUDRATE_REG            uint16_t(0x0003)   ///< module baud rate memory register, the default value is 0x0008
#define RS01_CHECKBIT_STOPBIT_REG    uint16_t(0x0004)   ///< module check bit and stop bit memory register, the default value is 0x0001
#define RS01_VERSION_REG             uint16_t(0x0005)   ///< memory register for firmware revision number:0x1000 represents V1.0.0.0

/* RS01 register address of measurement data */
#define RS01_TARGETS_NUMBER        uint16_t(0x0006)   ///< detect the current object numbers
#define RS01_DISTANCE_TARGET1      uint16_t(0x0007)   ///< distance to object 1
#define RS01_INTENSITY_TARGET1     uint16_t(0x0008)   ///< intensity of object 1
#define RS01_DISTANCE_TARGET2      uint16_t(0x0009)   ///< distance to object 2
#define RS01_INTENSITY_TARGET2     uint16_t(0x000A)   ///< intensity of object 2
#define RS01_DISTANCE_TARGET3      uint16_t(0x000B)   ///< distance to object 3
#define RS01_INTENSITY_TARGET3     uint16_t(0x000C)   ///< intensity of object 3
#define RS01_DISTANCE_TARGET4      uint16_t(0x000D)   ///< distance to object 4
#define RS01_INTENSITY_TARGET4     uint16_t(0x000E)   ///< intensity of object 4
#define RS01_DISTANCE_TARGET5      uint16_t(0x000F)   ///< distance to object 5
#define RS01_INTENSITY_TARGET5     uint16_t(0x0010)   ///< intensity of object 5

/* RS01 configure register address */
#define MEASUREMENT_START_POSITION         uint16_t(0x0011)   ///< measurement start position config register, the default value is 0x00C8
#define MEASUREMENT_END_POSITION           uint16_t(0x0012)   ///< measurement stop position config register, the default value is 0x1770
#define RS01_START_THRESHOLD               uint16_t(0x0013)   ///< initial threshold config register, the default value is 0x0190
#define RS01_END_THRESHOLD                 uint16_t(0x0014)   ///< end threshold config register, the default value is 0x0190
#define RS01_MODULE_SENSITIVITY            uint16_t(0x0015)   ///< module sensitivity config register, the default value is 0x0002
#define RS01_COMPARISON_OFFSET             uint16_t(0x0016)   ///< comparison offset config register, the default value is 0x0000
#define RS01_RESET_FACTORY                 uint16_t(0x0017)   ///< restore to factory setting

class DFRobot_RS01
{
public:
  #define NO_ERROR           0     ///< No error
  #define ERR_DATA_BUS      (-1)   ///< data bus error
  #define ERR_IC_VERSION    (-2)   ///< the chip version isn't suitable
  /**
   * @struct sBasicInfo_t
   * @brief device information structure in modbus mode
   */
  typedef struct
  {
    uint16_t PID;   /**< module PID, the default value is 0x01E9 (The highest two of 16-bit data are used to determine SKU type: 00: SEN, 01: DFR, 10: TEL, the next 14 are numbers.)(SEN0489) */
    uint16_t VID;   /**< module VID, the default value is 0x3343 (represent manufacturer DFRobot) */
    uint16_t modbusAddr;   /**< module communication address, the default value is 0x000E, module device address(1~247) */
    uint16_t baudrate;   /**< module baud rate, the default value is 0x0009(1000000) */
    uint8_t stopbit;   /**< stop bit: 1bit; 2bit */
    uint8_t checkbit;   /**< check bit: 0 represents none; 1 represents even; 2 represents odd */
    uint16_t versions;   /**< firmware revision number: 0x1000 represents V1.0.0.0 */
  }sBasicInfo_t;

  /**
   * @struct sMeasurementConfig_t
   * @brief Device information structure in modbus mode
   */
  typedef struct
  {
    uint16_t startPosition;   /**< current measurement start position set value, 0x0046~0x19C8, can't be greater than the set value at stop position */
    uint16_t stopPosition;   /**< current measurement stop position set value, 0x0046~0x19C8, can't be less than the set value at start position */
    uint16_t initialThreshold;   /**< current initial threshold set value */
    uint16_t endThreshold;   /**< current end threshold set value */
    uint16_t moduleSensitivity;   /**< current module sensitivity set value */
    int16_t comparisonOffset;   /**< current comparison offset set value */
  }sMeasurementConfig_t;

  /**
   * @enum  eBaudrateMode_t
   * @brief Available baud rate for the module
   */
  typedef enum
  {
    eBaudrate2400 = 0x0001,
    eBaudrate4800 = 0x0002,
    eBaudrate9600 = 0x0003,
    eBaudrate14400 = 0x0004,
    eBaudrate19200 = 0x0005,
    eBaudrate38400 = 0x0006,
    eBaudrate57600 = 0x0007,
    eBaudrate115200 = 0x0008,
    eBaudrate_1000000 = 0x0009,
  }eBaudrateMode_t;

  /**
   * @enum  eCheckBitMode_t
   * @brief  Available check bit mode for the module 
   */
  typedef enum
  {
    eCheckBitNone = 0x0000<<8,
    eCheckBitEven = 0x0001<<8,
    eCheckBitOdd = 0x0002<<8,
  }eCheckBitMode_t;

  /**
   * @enum  eStopBitMode_t
   * @brief Available stop bit mode for the module 
   */
  typedef enum
  {
    eStopBit1 = 0x0001,
    eStopBit2 = 0x0003,
  }eStopBitMode_t;

public:
  /**
   * @fn DFRobot_RS01
   * @brief constructor
   * @param addr RS485 communication device address
   * @return None
   */
  DFRobot_RS01(uint8_t addr);

  /**
   * @fn begin
   * @brief init function
   * @param _serial serial ports for communication, supporting hard and soft serial ports
   * @return int type, means returning initialization status
   * @retval 0 NO_ERROR
   * @retval -1 ERR_DATA_BUS
   * @retval -2 ERR_IC_VERSION
   */
  int begin(Stream *_serial);

  /**
   * @fn refreshBasicInfo
   * @brief Retrieve the basic information from the sensor and buffer it into the structure basicInfo that stores information
   * @note basicInfo structure members:
   * @n      PID: module PID
   * @n      VID: module VID
   * @n      modbusAddr: module communication address
   * @n      baudrate: module baud rate
   * @n      checkbit: check bit
   * @n      stopbit: stop bit
   * @n      versions: firmware version number
   * @return returning 0 means read succeeds
   */
  int refreshBasicInfo(void);

  /**
   * @fn refreshMeasurementData
   * @brief Re-read the measured data from the sensor and buffer it into the array dataBuf[11] that stores information
   * @note dataBuf[11] array members:
   * @n      dataBuf[0]: the number of objects currently detected
   * @n      dataBuf[1]: measured distance to the first object; dataBuf[2]: measured intensity of the first object
   * @n      dataBuf[3]: measured distance to the second object; dataBuf[4]: measured intensity of the second object
   * @n      dataBuf[5]: measured distance to the third object; dataBuf[6]: measured intensity of the third object
   * @n      dataBuf[7]: measured distance to the fourth object; dataBuf[8]: measured intensity of the fourth object
   * @n      dataBuf[9]: measured distance to the fifth object; dataBuf[10]: measured intensity of the fifth object
   * @return returning 0 means read succeeds
   */
  int refreshMeasurementData(void);

  /**
   * @fn refreshMeasurementConfig
   * @brief Retrieve the currently configured parameters from the sensor and buffer it into the structure measurementConfig that stores information
   * @note measurementConfig structure members:
   * @n      startPosition: current measurement start position set value
   * @n      stopPosition: current measurement stop position set value
   * @n      initialThreshold: current initial threshold set value
   * @n      endThreshold: current end threshold set value
   * @n      moduleSensitivity: current module sensitivity set value
   * @n      comparisonOffset: current comparison offset set value
   * @return returning 0 means reading succeeds
   */
  int refreshMeasurementConfig(void);

  /**
   * @fn setADDR
   * @brief Set the module communication address
   * @param addr Device address to be set, (1~247 is 0x0001~0x00F7)
   * @return None
   */
  void setADDR(uint16_t addr);

  /**
   * @fn setBaudrateMode
   * @brief Set the module baud rate, the setting takes effect after power fail and restart
   * @param mode The baud rate to be set:
   * @n       eBaudrate2400---2400, eBaudrate4800---4800, eBaudrate9600---9600, 
   * @n       eBaudrate14400---14400, eBaudrate19200---19200, eBaudrate38400---38400, 
   * @n       eBaudrate57600---57600, eBaudrate115200---115200, eBaudrate_1000000---1000000
   * @return None
   */
  void setBaudrateMode(eBaudrateMode_t mode);

  /**
   * @fn setCheckbitStopbit
   * @brief set check bit and stop bit of the module
   * @param mode the mode to be set, perform OR operation on the following to get mode:
   * @n       check bit:
   * @n             eCheckBitNone
   * @n             eCheckBitEven
   * @n             eCheckBitOdd
   * @n       stop bit:
   * @n             eStopBit1
   * @n             eStopBit2
   * @return None
   */
  void setCheckbitStopbit(uint16_t mode);

  /**
   * @fn setAllMeasurementParameters
   * @brief configure the value at measurement start position, configure the value at measurement stop position,
   * @n     configure the initial threshold, configure the end threshold,
   * @n     configure the module sensitivity, configure the comparison offset
   * @param startingPosition value at start position, 70~6600(0x0046~0x19C8)
   * @param stopPosition value at stop position, 70~6600(0x0046~0x19C8)
   * @param initialThreshold initial threshold, 100~10000(0x0064~0x2710)
   * @param endThreshold end threshold, 100~10000(0x0064~0x2710)
   * @param moduleSensitivity module sensitivity, 0x0000~0x0004
   * @param comparisonOffset comparison offset, -32768~32767(0~0xFFFF)
   * @return None
   */
  void setAllMeasurementParameters(uint16_t startingPosition, uint16_t stopPosition,
                                   uint16_t initialThreshold, uint16_t endThreshold,
                                   uint16_t moduleSensitivity, uint16_t comparisonOffset);

  /**
   * @fn restoreFactorySetting
   * @brief Restore to factory setting
   * @return None
   */
  void restoreFactorySetting(void);

protected:

/***************** register reading and writing interface ******************************/
  /**
   * @fn readData
   * @brief Read holding register function(read one or more register data)
   * @param reg  Register address 16bits
   * @param pBuf Read data storage and buffer
   * @param size Read data length
   * @return uint8_t, Exception code:
   * @retval   0 : sucess.
   * @retval   1 or eRTU_EXCEPTION_ILLEGAL_FUNCTION : Illegal function.
   * @retval   2 or eRTU_EXCEPTION_ILLEGAL_DATA_ADDRESS: Illegal data address.
   * @retval   3 or eRTU_EXCEPTION_ILLEGAL_DATA_VALUE:  Illegal data value.
   * @retval   4 or eRTU_EXCEPTION_SLAVE_FAILURE:  Slave failure.
   * @retval   8 or eRTU_EXCEPTION_CRC_ERROR:  CRC check error.
   * @retval   9 or eRTU_RECV_ERROR:  Receive packet error.
   * @retval   10 or eRTU_MEMORY_ERROR: Memory error.
   * @retval   11 or eRTU_ID_ERROR: Broadcasr address or error ID
   */
  virtual uint8_t readData(uint16_t reg, uint16_t * pBuf, uint8_t size);

  /**
   * @fn writeData
   * @brief Write holding register function(write one or more register data)
   * @param reg  Register address 16bits
   * @param pBuf Write data storage and buffer
   * @param size Write data length
   * @return uint8_t, Exception code:
   * @retval   0 : sucess.
   * @retval   1 or eRTU_EXCEPTION_ILLEGAL_FUNCTION : Illegal function.
   * @retval   2 or eRTU_EXCEPTION_ILLEGAL_DATA_ADDRESS: Illegal data address.
   * @retval   3 or eRTU_EXCEPTION_ILLEGAL_DATA_VALUE:  Illegal data value.
   * @retval   4 or eRTU_EXCEPTION_SLAVE_FAILURE:  Slave failure.
   * @retval   8 or eRTU_EXCEPTION_CRC_ERROR:  CRC check error.
   * @retval   9 or eRTU_RECV_ERROR: Receive packet error.
   * @retval   10 or eRTU_MEMORY_ERROR: Memory error.
   * @retval   11 or eRTU_ID_ERROR: Broadcasr address or error ID
   */
  virtual uint8_t writeData(uint16_t reg, const void * pBuf, uint8_t size);

public:
  /* variable for storing the information obtained by users */
  uint16_t dataBuf[11];   // the array storing the measured data
  sBasicInfo_t basicInfo;   // the array storing the sensor basic information
  sMeasurementConfig_t measurementConfig;   // the array storing the sensor measurement parameters

private:
  /* private variables */
  DFRobot_RTU *_DFRobot_RTU;   // the pointer to RS485 communication mode instance
};

#endif
