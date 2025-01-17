/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MMA8451Q_H
#define MMA8451Q_H
#include <fsl_i2c.h>

/**
* MMA8451Q accelerometer example
*
* @code
* #include "mbed.h"
* #include "MMA8451Q.h"
* 
* #define MMA8451_I2C_ADDRESS (0x1d<<1)
* 
* int main(void) {
* 
* MMA8451Q acc(P_E25, P_E24, MMA8451_I2C_ADDRESS);
* PwmOut rled(LED_RED);
* PwmOut gled(LED_GREEN);
* PwmOut bled(LED_BLUE);
* 
*     while (true) {       
*         rled = 1.0 - abs(acc.getAccX());
*         gled = 1.0 - abs(acc.getAccY());
*         bled = 1.0 - abs(acc.getAccZ());
*         wait(0.1);
*     }
* }
* @endcode
*/
class MMA8451Q
{
public:
  /**
  * MMA8451Q constructor
  *
  * @param sda SDA pin
  * @param sdl SCL pin
  * @param addr addr of the I2C peripheral
  */
  MMA8451Q(I2C_Type *I2CBase, unsigned char  addr);

  /**
  * MMA8451Q destructor
  */
  ~MMA8451Q();

  /**
   * Get the value of the WHO_AM_I register
   *
   * @returns WHO_AM_I value
   */
  uint8_t getWhoAmI();

  /**
   * Get X axis acceleration
   *
   * @returns X axis acceleration
   */
  float getAccX();

  /**
   * Get Y axis acceleration
   *
   * @returns Y axis acceleration
   */
  float getAccY();

  /**
   * Get Z axis acceleration
   *
   * @returns Z axis acceleration
   */
  float getAccZ();

  /**
   * Get XYZ axis acceleration
   *
   * @param res array where acceleration data will be stored
   */


  //! Ziskanie informácie všetkých troch osí akcelerometra.
  /*! Parameter je adresa na pole troch floatov kde sa uloźia hodnoty troch osí akcelerometra */
  void getAccAllAxis(float * res);

private:
  unsigned char m_addr;
  I2C_Type * I2CBase;
  i2c_master_config_t masterConfig;
  //! Čítanie z I2C.
  /*! Číta z I2C Dáta na adrese addr o dĺžke len */
  void readRegs(int addr, uint8_t * data, int len);
  //! Zapisovanie do I2C
  /*! Zapisuje dáta do I2C */
  void writeRegs(uint8_t * data, int len);

  int16_t getAccAxis(uint8_t addr);

};

#endif
