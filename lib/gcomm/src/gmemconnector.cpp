#include "gmemconnector.h"
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>

GMemConnector::GMemConnector(uint32_t key, mode mode){
  _mode = mode;
  key_t *key_p = (key_t *) &key;
  _shmid = shmget( key, 2 * sizeof(_Buffer), 0666|IPC_CREAT); 
  if (_mode == mode::master){
    _local = (_Buffer*) shmat(_shmid,(void*)0,0);
    _remote = _local + 1;
    _remote->offset =  _remote->next =_local->offset =_local->next = 0;
  }else{
    _remote = (_Buffer*) shmat(_shmid,(void*)0,0);
    _local = _remote + 1;
  }
}

uint16_t GMemConnector::read_buffer(void *buffer, uint16_t max_size){
  uint16_t pending = _remote->offset >= _remote->next ? _remote->offset - _remote->next : 1024 + _remote->offset - _remote->next;
  if (pending>=max_size){
    uint8_t *buffer_c = (uint8_t *)buffer;
    for (int index =0; index<max_size; index ++){
      buffer_c[index] = _remote->data[_remote->next];
      _remote->next = ++_remote->next % 1024;
    }
    return max_size;
  }
  return 0;
}

void GMemConnector::write_buffer(const void *buffer, uint16_t size){
  uint8_t *buffer_c = (uint8_t *)buffer;
  for (int index =0; index<size; index ++){
    _local->data[_local->offset] = buffer_c[index];
    _local->offset = ++_local->offset % 1024;
  }
}

GMemConnector::~GMemConnector(){
    if (_mode == mode::master){
      shmdt(_local);
      shmctl(_shmid,IPC_RMID,NULL); 
    }else{
      shmdt(_remote);
    
    }
}
