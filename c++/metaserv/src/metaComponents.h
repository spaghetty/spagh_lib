#ifndef MetaComponents
#define MetaComponents

#include <sys/epoll.h>
#include "connectionManager.h"
#include "metaEvent.h"

class Socket;
class epollManager;

class MetaElement
{
 public:
  virtual void handle_msg(struct epoll_event&){};
  virtual void handle_msg(MetaEvent&){};
  void add_socket(Socket &s);
  virtual bool is_server() = 0;
 protected:
  Socket *m_sock[4];
};

class MetaClient: public MetaElement
{
  void handle_msg(MetaEvent& event);
  bool is_server() {return false; };
};

class MetaServer: public MetaElement
{
 public:
  MetaServer(int port, epollManager &e);
  void handle_msg(struct epoll_event& event);
  bool is_server() { return true; };
 private:
  epollManager *m_pollm;
};

#endif /*MetaComponents*/
