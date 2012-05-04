#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <iostream>
#include "Sockets.h"
#include "metaEvent.h"
#include "scheduler.h"
#include "metaComponents.h"
#include "connectionManager.h"

#define MAX_WORKERS 5

class Worker {

public:
  Worker(Scheduler &q): m_queue(q), m_shutdown(false) {
    int ret;
    ret = pthread_create(&m_id, NULL, (Worker::start) , this );
  }

  void exit () {
    m_shutdown = true;
    m_queue.wake_up_all();
  }

  static void *start(void *udata){
    ((Worker*)udata)->main_loop();
  }
  
  void main_loop(){
    while ( not m_shutdown) {
      MetaEvent *m = m_queue.pop();
      if ( NULL != m ) {
	if (m->get_type() == 0x1 ) {
	  if ( ((EpollEvent*)m)->get_event() == EPOLLIN )
	    ((EpollEvent*)m)->get_socket()->get_component()->handle_msg(*m);
	  std::cout << "something cool is happening here" << std::endl;
	}
      }
      else {
	if (m_queue.is_empty())
	  {
	    m_queue.wait_for_data();
	  }
      }
    }
    std::cout << "provolone 123" << std::endl;
    pthread_exit(0);
  }

private:
  pthread_t m_id;
  Scheduler &m_queue;
  bool m_shutdown;
};

Worker *workers[MAX_WORKERS];

class ControlSignals {

public:
  ControlSignals() {
    //signal(SIGINT, sign_handler);
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = ControlSignals::sign_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &act, NULL);
  }

  static void sign_handler(int signal, siginfo_t *info, void *ptr) {
    std::cout << "fico" << std::endl;
    for( int i; i<4 ; i++){
      if ( workers[i]!= NULL )
	workers[i]->exit();
    }
    sleep(2);
    exit(0);
  }
};

int main ()
{
  Scheduler q;
  ControlSignals();
  epollManager m(10, q);
  std::cout << "sticazzi 123" << std::endl;
  for ( int i=0; i<MAX_WORKERS; i++ ) 
    workers[i] = new Worker(q);
  MetaServer mServ(8080, m);
  //struct epoll_event event;
  //struct epoll_event *events;
  m.main_loop();
}
