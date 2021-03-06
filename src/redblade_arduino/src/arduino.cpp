#include "arduino.hpp"

#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

arduino::arduino(const char* port) {
  struct termios tios;
  
  commPort = open(port, O_RDWR);
  usleep (100000);
  cout << "Port number: " << commPort << endl;
  if (commPort < 0) {
    cout << "Error opening the comm port\n";
  } else {
    tcgetattr(commPort, &tios);
    tios.c_cflag = CS8|B57600;//flow control: 8 bit bytes, 57 kbaud
    tios.c_oflag = 0;//output params
    tios.c_iflag = IGNPAR;//input params: ignore parity
    tios.c_lflag = 0;//
    //set input and output speed
    cfsetispeed(&tios, B57600);
    cfsetospeed(&tios, B57600);
    //store updated params for the tty at fd1
    tcsetattr(commPort, TCSANOW, &tios);
    usleep(100000);
    cout << "Comm port open, and parameters set to 57600 baud\n";
    m = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(m,NULL);
    usleep(1500000);
  }
}

arduino::arduino(const arduino& source) {
  m = source.m;
  commPort = source.commPort;
  channels = source.channels;
}
    
arduino::~arduino(void) {
  close(commPort);
  free(m);
  
}

void arduino::operator() () {
  while(1) {
    
    usleep(100000);
  }
}

long int arduino::getPulse(int pin) {
  char outbuf[5];
  char inbuf[16];
  int rd = 0;
  long int ret;
  sprintf(outbuf,"u%d\r",pin);
  outbuf[4] = '\0';
  pthread_mutex_lock(m);
  tcflush(commPort, TCIOFLUSH);
  write(commPort, outbuf, strlen(outbuf));
  usleep(41000);
  rd = read(commPort, inbuf, 15);
  pthread_mutex_unlock(m);
  inbuf[rd] = '\0';
  sscanf(inbuf, "u%dv%ld\r",&rd,&ret);
  if(ret < 800 || ret > 2200) 
    ret = 1420;
  return ret;
}

int arduino::getAnalog(int pin) {
  char outbuf[5];
  char inbuf[16];
  int rd = 0;
  int ret;
  sprintf(outbuf,"a%d\r",pin);
  
  outbuf[4] = '\0';
  pthread_mutex_lock(m);
  tcflush(commPort, TCIOFLUSH);
  write(commPort, outbuf, strlen(outbuf));
  usleep(8000);
  rd = read(commPort, inbuf, 15);
  pthread_mutex_unlock(m);
  inbuf[rd] = '\0';
  sscanf(inbuf, "a%dv%d\r",&rd,&ret);
  
  
  return ret;
}


int arduino::setPWM(int pin, int value) {
  char outbuf[9];
  char inbuf[9];
  int rd;
  if(value<0)
    value=0;
  if(value>255)
    value=255;
  sprintf(outbuf,"p%dv%d\r",pin,value);
  
  pthread_mutex_lock(m);
  tcflush(commPort, TCIOFLUSH);
  write(commPort, outbuf, strlen(outbuf));
  
  usleep(10000);
  
  rd = read(commPort, inbuf, strlen(outbuf));
  
  pthread_mutex_unlock(m);
  inbuf[rd] = '\0';
  if(strncmp(outbuf,inbuf,strlen(outbuf)))
    return -1;
  return value;
}

int arduino::setDigital(int pin, int value) {
  char outbuf[9];
  char inbuf[9];
  int rd;
  if(value<0)
    value=0;
  if(value>1)
    value=1;
  sprintf(outbuf,"d%dv%d\r",pin,value);
  
  pthread_mutex_lock(m);
  tcflush(commPort, TCIOFLUSH);
  write(commPort, outbuf, strlen(outbuf));
  
  usleep(10000);
  
  rd = read(commPort, inbuf, strlen(outbuf));
  
  pthread_mutex_unlock(m);
  inbuf[rd] = '\0';
  if(strncmp(outbuf,inbuf,strlen(outbuf)))
    return -1;
  return value;
}
