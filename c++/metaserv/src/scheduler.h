#ifndef SCHEDULER
#define SCHEDULER

#include "metaEvent.h"
#include <queue>
#include <pthread.h>


typedef std::queue<MetaEvent*> event_queue;

class Scheduler {

 public:
  Scheduler() {
    pthread_mutex_init( &m_mutex, NULL);
    pthread_cond_init( &m_wait_for_data , NULL);
    m_waiting = 0;
  };

  ~Scheduler() {};

  void push( MetaEvent &e)
    {
      pthread_mutex_lock( &m_mutex );
      m_queue.push(&e);
      if (m_waiting > 0) {
	m_waiting -= 1;
	pthread_cond_signal( &m_wait_for_data );
      }
      pthread_mutex_unlock( &m_mutex );
    }
  void wake_up_all()
  {
    pthread_mutex_lock( &m_mutex );
    if (m_waiting > 0 ) {
      m_waiting = 0;
      pthread_cond_broadcast( &m_wait_for_data );
    }
    pthread_mutex_unlock( & m_mutex );
  }

  MetaEvent *pop(){
    MetaEvent *ret = NULL;
    if ( not m_queue.empty() ) {
      if ( 0 == pthread_mutex_trylock( &m_mutex ) ) {
	ret = m_queue.front();
	m_queue.pop();
	pthread_mutex_unlock( &m_mutex );
      }
    }
    return ret;
  }

  void wait_for_data()
  {
    pthread_mutex_lock( &m_mutex );
    if ( m_queue.empty() )
      m_waiting += 1;
      pthread_cond_wait( &m_wait_for_data, &m_mutex );
    pthread_mutex_unlock( &m_mutex );
  }

  bool is_empty() // useless
  {
    return m_queue.empty();
  }

 private:
  int m_waiting;
  event_queue m_queue;
  pthread_mutex_t m_mutex;
  pthread_cond_t m_wait_for_data;
};

#endif /* SCHEDULER */
