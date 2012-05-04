#include "connectionManager.h"
#include "metaEvent.h"
#include "scheduler.h"
#include <stdlib.h>

epollManager::epollManager(int max_event, Scheduler &q) : m_queue(q)
{
  this->MAXEVENTS=max_event;
  m_events = (struct epoll_event*)calloc(MAXEVENTS, sizeof(struct epoll_event));
  m_epoll_df = -1;
}

epollManager::~epollManager()
{
  free(m_events);
}

void epollManager::create()
{
  m_epoll_df = epoll_create(1);
}

bool epollManager::add(Socket &s)
{
  struct epoll_event *event = (struct epoll_event*) calloc(1,sizeof(struct epoll_event));
  event->data.fd = s.get_descriptor();
  event->data.ptr = &s;
  event->events = EPOLLIN | EPOLLET;
  if (epoll_ctl (m_epoll_df, EPOLL_CTL_ADD, s.get_descriptor(), event)!=-1) {
    return false;
  }
  return true;
}

int epollManager::wait()
{
  return epoll_wait(m_epoll_df, m_events, MAXEVENTS, -1);
}

void epollManager::main_loop()
{
  int num_event;
  while(1) {
    num_event = epoll_wait(m_epoll_df, m_events, MAXEVENTS, -1);
    for (int i = 0; i< num_event; i++){
      if ((m_events[i].events & EPOLLERR)) {
	std::cout << "error from epoll "<< m_events[i].events << std::endl;
	}
      else if (m_events[i].events & EPOLLHUP) {
	std::cout << "sign hup from epoll "<< m_events[i].events << std::endl;
      }
      else if ((m_events[i].events & EPOLLIN)) {
	//std::cout << "is not Epollin " << std::endl;
	//}
	//else {
	std::cout << "something new here " << std::endl;
	MetaElement *tmp = ((Socket*)(m_events[i].data.ptr))->get_component();
	if (tmp->is_server()) // priorize server
	  tmp->handle_msg(m_events[i]);
	else {
	  EpollEvent *e = new EpollEvent( EPOLLIN, ((Socket*)(m_events[i].data.ptr)) );
	  m_queue.push(*e);
	}
      }
    }
  }
}

bool epollManager::isValid()
{
  return (m_epoll_df!=-1);
}

struct epoll_event *epollManager::get_events()
{
  return m_events;
}
