#ifndef ARDUINO_H
#define ARDUINO_H

#include <pthread.h>

#define FB_PIN 11
#define LR_PIN 12

#ifndef MEGA
#define DEVTYPE_ADDRESS 0x00
#define ANALOGUE_MODE_ADDRESS 0x01
#define DIGITAL_MODE_ADDRESS 0x07
#else
#define DEVTYPE_ADDRESS 0x00
#define ANALOGUE_MODE_ADDRESS 0x01
#define DIGITAL_MODE_ADDRESS 0x0E
#endif

#define ANALOGUE_OFF  0
#define ANALOGUE_IN   1
#define ANALOGUE_IIC  2

#define DIGITAL_OFF   0
#define DIGITAL_IN    1
#define DIGITAL_OUT   2
#define DIGITAL_PWM   3
#define DIGITAL_TONE  4
#define DIGITAL_UART  5
#define DIGITAL_SPI   6
#define DIGITAL_PULSE 7
#define DIGITAL_SERVO 8

class arduino {

public:
  arduino(const char* port = "/dev/ttyACM0");
  arduino(const arduino& source);
  virtual ~arduino(void);
  long int getPulse(int pin);
  int getAnalog(int pin);
  int setPWM(int pin, int value);
  int setDigital(int pin, int value);
  
  void operator () (void);
private:

  int commPort;
  long int* channels;
  pthread_mutex_t* m;
  
};

#endif
