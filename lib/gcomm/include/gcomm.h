# if !defined(__GCOMM_H__)
# define __GCOMM_H__
#include <stdint.h>

struct GStream{
  public:
    virtual uint8_t get_byte() = 0;
    virtual void set_byte(uint8_t) = 0;
};

struct GConnector{
  public:
    virtual uint16_t read_buffer (void*, uint16_t) = 0;
    virtual void write_buffer (const void *, uint16_t) = 0;
};

template <typename TIN, typename TOUT>
struct GComm{
  GComm(GConnector &conn) : _conn (conn) {
  }
  bool read_object (TIN &i){
    return _conn.read_buffer(&i,sizeof(TIN))==sizeof(TIN);
  }
  void write_object (TOUT o){
    _conn.write_buffer(&o,sizeof(TOUT));
  }
  private:
  GConnector &_conn;
};
#endif