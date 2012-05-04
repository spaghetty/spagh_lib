#ifndef MetaEvents
#define MetaEvents

class Socket;

class MetaEvent
{
 public:
  MetaEvent() 
    {
      m_typecode=0x0;
    }
  int get_type() { return m_typecode; }
 protected:
  int m_typecode;
};

class EpollEvent: public MetaEvent
{
 public:
  EpollEvent(int epolle, Socket *s)
    {
      m_typecode=0x1;
      m_event = epolle;
      m_sock = s;
    }

  int get_event() { return m_event; }
  Socket *get_socket() { return m_sock; }

 private:
  int m_event;
  Socket *m_sock;
};



#endif /* MetaEvent */
