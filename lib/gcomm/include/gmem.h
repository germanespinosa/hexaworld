# if !defined(__GMEM_H__)
# define __GMEM_H__

#include"gcomm.h"
#include <thread>
#include <mutex>

struct MemGStream : GStream{
  public:
    enum class mode{
      master,
      slave
    };
    MemGStream(uint16_t, mode);
    virtual uint8_t get_byte() override;
    virtual void set_byte(uint8_t) override;
};
#endif