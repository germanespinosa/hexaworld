#include "guart.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <fstream>


UartGStream::UartGStream(baudrate baudrate){
  _file = -1;
  _file = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);//Open in non blocking read/write mode
  if ( _file < 0 ) exit(EXIT_FAILURE);
  struct termios options;
  tcgetattr(_file, &options);
  options.c_cflag = ((int)baudrate) | CS8 | CLOCAL | CREAD;//<Set baud rate
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(_file, TCIFLUSH);
  tcsetattr(_file, TCSANOW, &options);
}

uint8_t UartGStream::get_byte(){
  uint8_t byte;
  while(read(_file, &byte, 1) != 1);
  return byte;
}

void UartGStream::set_byte(uint8_t byte){
  write(_file, &byte, 1);
}
