#include "gsocket.h"
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>

SocketGStream::SocketGStream(std::string remote, uint16_t port){
}

uint8_t SocketGStream::get_byte(){
  while (!connected);
  uint8_t byte=0;
  read(_socket, &byte, 1);
  return byte;
}

void SocketGStream::set_byte(uint8_t byte){
  send(_socket, &byte, 1 , 0 );
}

