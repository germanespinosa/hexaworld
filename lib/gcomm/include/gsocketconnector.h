# if !defined(__GSOCKETCONNECTOR_H__)
# define __GSOCKETCONNECTOR_H__

#include"gcomm.h"
#include <thread>
#include <mutex>

struct GSocketConnector : GConnector{
  public:
    GSocketConnector(uint16_t);
    GSocketConnector(std::string, uint16_t);
    virtual uint16_t read_buffer(void *, uint16_t);
    virtual void write_buffer(const void *, uint16_t);
    bool connected;
  private:
    static void _connect_server(GSocketConnector&, uint16_t);
    int32_t _socket;
    std::thread _connect;     
  };
#endif