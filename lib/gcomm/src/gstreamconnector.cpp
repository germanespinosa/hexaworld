#include "gstreamconnector.h"
#include <sys/time.h>
#include <iostream>
using namespace std;
GStreamConnector::GStreamConnector(GStream &stream) : GStreamConnector(stream,0){
}

GStreamConnector::GStreamConnector(GStream &stream, uint64_t timeout) : _stream(stream){
  _rx_length = _rx_offset = _rx_next = _tx_offset = _tx_next = 0;
  _rx_thread = std::thread (_rx_update_, std::ref(*this));
  _tx_thread = std::thread (_tx_update_, std::ref(*this));
  _timeout = timeout;
}

uint16_t GStreamConnector::read_buffer(void *dest, uint16_t max_size){
  _rx_buffer_access.lock();
  if (!_rx_length && _pending() >= 2){
    uint16_t l = get_byte();
    uint16_t h = get_byte();
    _rx_length =  (h << 8) + l ;
  }
  if( _rx_length && _pending() > _rx_length ) 
  {
    uint8_t *dest_c = (uint8_t *) dest;
    uint8_t check_sum = 0;
    for ( int index = 0 ; index < _rx_length ; index++ ){
      uint8_t byte = get_byte();
      if ( index < max_size) dest_c[index] = byte;
      check_sum = check_sum ^ byte;
    }
    uint8_t length = _rx_length;
    _rx_length = 0;
    _rx_buffer_access.unlock();
    return check_sum == get_byte() ? length : 0;
  }
  _rx_buffer_access.unlock();
  return 0;  
}

void GStreamConnector::write_buffer(const void *data, uint16_t size){
  uint8_t* buff = (uint8_t*)data;
  uint8_t check_sum = 0;
  set_byte(*((uint8_t *)&size));
  set_byte(*(((uint8_t *)&size)+1));
  for ( uint8_t index = 0 ; index < size ; index++ ){
    set_byte(buff[index]);
    check_sum = check_sum ^ buff[index];
  }
  set_byte(check_sum);
}

void GStreamConnector::_rx_update_(GStreamConnector& comm){
  timeval current_time, last_byte;
  gettimeofday(&last_byte, NULL);
  while(true){
    uint8_t byte = comm._stream.get_byte();
    gettimeofday(&current_time, NULL);
    uint64_t elapsed = (current_time.tv_usec - last_byte.tv_usec) + (current_time.tv_sec - last_byte.tv_sec) * 1000000;
    last_byte = current_time;
    if (comm._timeout && elapsed > comm._timeout){
      cout << "timeout" << elapsed << endl;
      comm._rx_buffer_access.lock();
      comm._rx_next=comm._rx_offset;
      comm._rx_length=0;
      comm._rx_buffer_access.unlock();
    }
    comm._rx_buffer[comm._rx_offset] = byte;
    comm._rx_offset=(++comm._rx_offset % BUFFERLENGTH); 
  }
}

void GStreamConnector::_tx_update_(GStreamConnector& comm){
  while(true){
    if (comm._tx_next != comm._tx_offset){
      uint8_t byte = comm._tx_buffer[comm._tx_next];
      comm._stream.set_byte(byte);
      comm._tx_next=(++comm._tx_next % BUFFERLENGTH); 
    }
  }
}

uint16_t GStreamConnector::_pending(){
  if (_rx_offset >=_rx_next) return _rx_offset - _rx_next;
  return BUFFERLENGTH - _rx_next + _rx_offset;
}

uint8_t GStreamConnector::get_byte(){
  uint8_t byte = _rx_buffer[_rx_next];
  _rx_next=(++_rx_next % BUFFERLENGTH);
  return byte;
}
bool GStreamConnector::set_byte(uint8_t byte){
  _tx_buffer[_tx_offset] = byte;
  _tx_offset=(++_tx_offset % BUFFERLENGTH);
  return true;
}

