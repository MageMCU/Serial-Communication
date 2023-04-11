//  * Adafruit_LIS2MDL.h
//  *
//  * This is a library for the LIS2MDL magnentometer/compass
//  *
//  * Designed specifically to work with the Adafruit LSM303AGR and LIS2MDL
//  * Breakouts
//  *
//  * These displays use I2C to communicate, 2 pins are required to interface.
//  *
//  * Adafruit invests time and resources providing this open source code,
//  * please support Adafruit and open-source hardware by purchasing products
//  * from Adafruit!
//  *
//  * Written by Bryan Siepert for Adafruit Industries.
//  * BSD license, all text above must be included in any redistribution
//
// Adapted by Carpenter Software
//

#ifndef LIS2MDL_MAG_H
#define LIS2MDL_MAG_H

#include <Arduino.h>


#include <limits.h>
#include <Adafruit_BusIO_Register.h>

#include <Adafruit_I2CDevice.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define _ADDRESS_MAG 0x1E   //!< Default address
#define _CHIP_ID 0x40       //!< Chip ID from WHO_AM_I register
#define LIS2MDL_MAG_LSB 1.5 //!< Sensitivity
#define LIS2MDL_MILLIGAUSS_TO_MICROTESLA                                       \
  0.1 //!< Conversion rate of Milligauss to Microtesla
/*=========================================================================*/

/*!
 * @brief LIS2MDL I2C register address bits
 */
typedef enum {
  LIS2MDL_OFFSET_X_REG_L = 0x45,
  LIS2MDL_OFFSET_X_REG_H = 0x46,
  LIS2MDL_OFFSET_Y_REG_L = 0x47,
  LIS2MDL_OFFSET_Y_REG_H = 0x48,
  LIS2MDL_OFFSET_Z_REG_L = 0x49,
  LIS2MDL_OFFSET_Z_REG_H = 0x4A,
  LIS2MDL_WHO_AM_I = 0x4F,
  LIS2MDL_CFG_REG_A = 0x60,
  LIS2MDL_CFG_REG_B = 0x61,
  LIS2MDL_CFG_REG_C = 0x62,
  LIS2MDL_INT_CRTL_REG = 0x63,
  LIS2MDL_INT_SOURCE_REG = 0x64,
  LIS2MDL_INT_THS_L_REG = 0x65,
  LIS2MDL_STATUS_REG = 0x67,
  LIS2MDL_OUTX_L_REG = 0x68,
  LIS2MDL_OUTX_H_REG = 0x69,
  LIS2MDL_OUTY_L_REG = 0x6A,
  LIS2MDL_OUTY_H_REG = 0x6B,
  LIS2MDL_OUTZ_L_REG = 0x6C,
  LIS2MDL_OUTZ_H_REG = 0x6D,
} lis2mdl_register_t;
/*=========================================================================*/

/*!
 * @brief Magnetometer update rate settings
 */
typedef enum {
  LIS2MDL_RATE_10_HZ,  //!< 10 Hz
  LIS2MDL_RATE_20_HZ,  //!< 20 Hz
  LIS2MDL_RATE_50_HZ,  //!< 50 Hz
  LIS2MDL_RATE_100_HZ, //!< 100 Hz
} lis2mdl_rate_t;
/*=========================================================================*/

/**************************************************************************/
/*!
    @brief  a data thing
*/
/**************************************************************************/
typedef struct lis2mdl_data {
  int16_t x; ///< x-axis raw data
  int16_t y; ///< y-axis raw data
  int16_t z; ///< z-axis raw data
} lis2mdl_data_t;
/*=========================================================================*/

/**************************************************************************/
/*!
    @brief  Unified sensor driver for the magnetometer
*/
/**************************************************************************/
class Adafruit_LIS2MDL : public Adafruit_Sensor {
public:
  Adafruit_LIS2MDL(int32_t sensorID = -1);

  bool begin(uint8_t i2c_addr = _ADDRESS_MAG, TwoWire *wire = &Wire);
  bool begin_SPI(uint8_t cs_pin, SPIClass *theSPI = &SPI);
  bool begin_SPI(int8_t cs_pin, int8_t sck_pin, int8_t miso_pin,
                 int8_t mosi_pin);

  lis2mdl_rate_t getDataRate();
  void setDataRate(lis2mdl_rate_t rate);
  bool getEvent(sensors_event_t *);
  void getSensor(sensor_t *);
  void reset(void);

  void enableInterrupts(bool);
  void interruptsActiveHigh(bool);
  lis2mdl_data_t raw; ///< struct instance to hold raw data

private:
  bool _init(void);

  int32_t _sensorID;
  Adafruit_BusIO_Register *config_a;

  void read(void);

  Adafruit_I2CDevice *i2c_dev = NULL;
  Adafruit_SPIDevice *spi_dev = NULL;
};


/***************************************************************************
 MAGNETOMETER
 ***************************************************************************/
/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Reads the raw data from the sensor
*/
/**************************************************************************/
void Adafruit_LIS2MDL::read() {

  Adafruit_BusIO_Register data_reg = Adafruit_BusIO_Register(
      i2c_dev, spi_dev, ADDRBIT8_HIGH_TOREAD, LIS2MDL_OUTX_L_REG, 6);

  uint16_t buffer[3];
  data_reg.read((uint8_t *)buffer, 6);

  raw.x = buffer[0];
  raw.y = buffer[1];
  raw.z = buffer[2];
}

/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Instantiates a new Adafruit_LIS2MDL class
    @param sensorID an option ID to differentiate the sensor from others
*/
/**************************************************************************/
Adafruit_LIS2MDL::Adafruit_LIS2MDL(int32_t sensorID) {
  _sensorID = sensorID;

  // Clear the raw mag data
  raw.x = 0;
  raw.y = 0;
  raw.z = 0;
}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/
/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  i2c_address
 *            The I2C address to be used.
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_LIS2MDL::begin(uint8_t i2c_address, TwoWire *wire) {
  if (!i2c_dev) {
    i2c_dev = new Adafruit_I2CDevice(i2c_address, wire);
  }
  spi_dev = NULL;

  if (!i2c_dev->begin()) {
    return false;
  }

  return _init();
}

/*!
 *    @brief  Sets up the hardware and initializes hardware SPI
 *    @param  cs_pin The arduino pin # connected to chip select
 *    @param  theSPI The SPI object to be used for SPI connections.
 *    @return True if initialization was successful, otherwise false.
 */
boolean Adafruit_LIS2MDL::begin_SPI(uint8_t cs_pin, SPIClass *theSPI) {
  i2c_dev = NULL;
  if (!spi_dev) {
    spi_dev = new Adafruit_SPIDevice(cs_pin,
                                     1000000,               // frequency
                                     SPI_BITORDER_MSBFIRST, // bit order
                                     SPI_MODE0,             // data mode
                                     theSPI);
  }
  if (!spi_dev->begin()) {
    return false;
  }
  return _init();
}

/*!
 *    @brief  Sets up the hardware and initializes software SPI
 *    @param  cs_pin The arduino pin # connected to chip select
 *    @param  sck_pin The arduino pin # connected to SPI clock
 *    @param  miso_pin The arduino pin # connected to SPI MISO
 *    @param  mosi_pin The arduino pin # connected to SPI MOSI
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_LIS2MDL::begin_SPI(int8_t cs_pin, int8_t sck_pin, int8_t miso_pin,
                                 int8_t mosi_pin) {
  i2c_dev = NULL;
  if (!spi_dev) {
    spi_dev = new Adafruit_SPIDevice(cs_pin, sck_pin, miso_pin, mosi_pin,
                                     1000000,               // frequency
                                     SPI_BITORDER_MSBFIRST, // bit order
                                     SPI_MODE0);            // data mode
  }
  if (!spi_dev->begin()) {
    return false;
  }
  return _init();
}

/*!
 *    @brief  Common initialization code for I2C & SPI
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_LIS2MDL::_init(void) {
  if (spi_dev) {
    // enable 4-wire SPI and disable I2C
    Adafruit_BusIO_Register cfg_reg =
        Adafruit_BusIO_Register(spi_dev, 0x62, ADDRBIT8_HIGH_TOREAD);
    cfg_reg.write(0b00100100);
    delay(10);
  }

  // Check connection
  Adafruit_BusIO_Register chip_id = Adafruit_BusIO_Register(
      i2c_dev, spi_dev, ADDRBIT8_HIGH_TOREAD, LIS2MDL_WHO_AM_I, 1);

  // make sure we're talking to the right chip
  if (chip_id.read() != _CHIP_ID) {
    // No LIS2MDL detected ... return false
    return false;
  }

  config_a = new Adafruit_BusIO_Register(i2c_dev, spi_dev, ADDRBIT8_HIGH_TOREAD,
                                         LIS2MDL_CFG_REG_A, 1);

  // enable int latching
  reset();

  return true;
}
/*!
 *    @brief  Resets the sensor to an initial state
 */
void Adafruit_LIS2MDL::reset(void) {

  Adafruit_BusIO_Register config_c = Adafruit_BusIO_Register(
      i2c_dev, spi_dev, ADDRBIT8_HIGH_TOREAD, LIS2MDL_CFG_REG_C, 1);

  Adafruit_BusIO_RegisterBits reset =
      Adafruit_BusIO_RegisterBits(config_a, 1, 5);

  Adafruit_BusIO_RegisterBits reboot =
      Adafruit_BusIO_RegisterBits(config_a, 1, 6);

  Adafruit_BusIO_RegisterBits mode =
      Adafruit_BusIO_RegisterBits(config_a, 2, 0);

  Adafruit_BusIO_RegisterBits temp_compensation =
      Adafruit_BusIO_RegisterBits(config_a, 1, 7);

  Adafruit_BusIO_RegisterBits bdu =
      Adafruit_BusIO_RegisterBits(&config_c, 1, 4);

  reset.write(1);
  delay(100);
  reboot.write(1);
  delay(100);

  if (spi_dev) {
    // enable 4-wire SPI and disable I2C
    Adafruit_BusIO_Register cfg_reg =
        Adafruit_BusIO_Register(spi_dev, 0x62, ADDRBIT8_HIGH_TOREAD);
    cfg_reg.write(0b00100100);
    delay(10);
  }

  bdu.write(1);
  temp_compensation.write(1);

  mode.write(0x00); // set to continuous mode

  setDataRate(LIS2MDL_RATE_100_HZ);
}

/**************************************************************************/
/*!
    @brief  Sets the magnetometer's update rate
    @param rate The new `lis2mdl_rate_t` to set
*/
/**************************************************************************/
void Adafruit_LIS2MDL::setDataRate(lis2mdl_rate_t rate) {
  Adafruit_BusIO_RegisterBits data_rate =
      Adafruit_BusIO_RegisterBits(config_a, 2, 2);

  data_rate.write(rate);
}

/**************************************************************************/
/*!
    @brief  Gets the magnetometer's update rate
    @returns The current data rate as a `lis2mdl_rate_t`
*/
/**************************************************************************/
lis2mdl_rate_t Adafruit_LIS2MDL::getDataRate(void) {
  Adafruit_BusIO_RegisterBits data_rate =
      Adafruit_BusIO_RegisterBits(config_a, 2, 2);

  return (lis2mdl_rate_t)data_rate.read();
}

/**************************************************************************/
/*!
    @brief  Gets the most recent sensor event
    @param event The `sensors_event_t` to fill with event data
    @returns true, always
*/
/**************************************************************************/
bool Adafruit_LIS2MDL::getEvent(sensors_event_t *event) {

  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));

  /* Read new data */
  read();

  event->version = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type = SENSOR_TYPE_MAGNETIC_FIELD;
  event->timestamp = millis();
  event->magnetic.x =
      (float)raw.x * LIS2MDL_MAG_LSB * LIS2MDL_MILLIGAUSS_TO_MICROTESLA;
  event->magnetic.y =
      (float)raw.y * LIS2MDL_MAG_LSB * LIS2MDL_MILLIGAUSS_TO_MICROTESLA;
  event->magnetic.z =
      (float)raw.z * LIS2MDL_MAG_LSB * LIS2MDL_MILLIGAUSS_TO_MICROTESLA;

  return true;
}

/**************************************************************************/
/*!
    @brief  Gets the sensor_t data
*/
/**************************************************************************/
void Adafruit_LIS2MDL::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy(sensor->name, "LIS2MDL", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name) - 1] = 0;
  sensor->version = 1;
  sensor->sensor_id = _sensorID;
  sensor->type = SENSOR_TYPE_MAGNETIC_FIELD;
  sensor->min_delay = 0;
  sensor->max_value = 5000;  // 50 gauss = 5000 uTesla
  sensor->min_value = -5000; // -50 gauss = -5000 uTesla
  sensor->resolution = 0.15; // 1.65 gauss = 0.15 uTesla
}

/*************************************************************************/
/*!
    @brief Enable interrupts
    @param enable Set to True to enable interrupts, set to False to disable
*/
void Adafruit_LIS2MDL::enableInterrupts(bool enable) {
  Adafruit_BusIO_Register int_ctrl = Adafruit_BusIO_Register(
      i2c_dev, spi_dev, ADDRBIT8_HIGH_TOREAD, LIS2MDL_INT_CRTL_REG);
  Adafruit_BusIO_Register cfg_c = Adafruit_BusIO_Register(
      i2c_dev, spi_dev, ADDRBIT8_HIGH_TOREAD, LIS2MDL_CFG_REG_C);

  Adafruit_BusIO_RegisterBits enable_ints =
      Adafruit_BusIO_RegisterBits(&int_ctrl, 1, 0);
  Adafruit_BusIO_RegisterBits int_pin_output =
      Adafruit_BusIO_RegisterBits(&cfg_c, 1, 6);

  enable_ints.write(enable);
  int_pin_output.write(enable);
}

/*************************************************************************/
/*!
    @brief Sets the polarity of the interrupt pin
    @param active_high Set to true to make the int pin active high, false
    to set as active low
*/
void Adafruit_LIS2MDL::interruptsActiveHigh(bool active_high) {
  Adafruit_BusIO_Register int_ctrl = Adafruit_BusIO_Register(
      i2c_dev, spi_dev, ADDRBIT8_HIGH_TOREAD, LIS2MDL_INT_CRTL_REG);

  Adafruit_BusIO_RegisterBits active_high_bit =
      Adafruit_BusIO_RegisterBits(&int_ctrl, 1, 2);
  active_high_bit.write(active_high);
}

#endif
