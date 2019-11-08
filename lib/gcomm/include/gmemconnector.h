# if !defined(__GMEM_H__)
# define __GMEM_H__

#include"gcomm.h"
#include <thread>
#include <mutex>

struct GMemConnector : GConnector{
  public:
    enum class mode{
      master,
      slave
    };
    GMemConnector(uint32_t, mode);
    virtual uint16_t read_buffer(void *, uint16_t);
    virtual void write_buffer(const void *, uint16_t);
    ~GMemConnector();
    private:
      uint16_t _buffer_size;
      struct _Buffer{
        uint16_t offset;
        uint16_t next;
        uint8_t data[1024];
      };
      _Buffer *_local;
      _Buffer *_remote;
      int _shmid;
      mode _mode;
};
#endif