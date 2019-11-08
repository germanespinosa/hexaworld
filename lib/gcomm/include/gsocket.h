# if !defined(__GSOCKET_H__)
# define __GSOCKET_H__

#include"gcomm.h"
#include <thread>
#include <mutex>

struct SocketGStream : GStream{
  public:
    SocketGStream(uint16_t);
    SocketGStream(std::string, uint16_t);
    virtual uint8_t get_byte() override;
    virtual void set_byte(uint8_t) override;
    bool connected;
    private:
    static void _connect_server(SocketGStream&, uint16_t);
    int32_t _socket;
    std::thread _connect;    
};
#endif