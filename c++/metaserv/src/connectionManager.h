#ifndef ConnectionManager
#define ConnectionManager

#include "Sockets.h"
#include <sys/epoll.h>
#include "metaEvent.h"

class Socket;
class Scheduler;

class epollManager
{
public:

  epollManager(int max_event, Scheduler &q);
  ~epollManager();

  void create();
  bool add(Socket &s);
  int wait();
  void main_loop();
  bool isValid();
  struct epoll_event *get_events();

private:
  int m_epoll_df;
  struct epoll_event *m_events;
  int MAXEVENTS;
  Scheduler &m_queue;
};

#endif /* ConnectionManager */
