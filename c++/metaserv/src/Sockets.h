#ifndef Sockets
#define Sockets

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include "metaComponents.h"

class MetaElement;

class Socket
{
public:

  Socket(){}

  virtual ~Socket(){}
  
  bool create(int protocol=0);
  bool bind(int port, in_addr_t addr=INADDR_ANY);

  bool listen(int max_pending=5) const;

  bool accept ( Socket &new_sock );

  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send ( const std::string ) const;
  int recv ( std::string& ) const;
  void set_non_blocking ();

  bool is_valid() const;
 
  int get_descriptor();
  MetaElement *get_component();

protected:
  friend class MetaElement;
  virtual void set_socket(int val) = 0;
  virtual int &get_socket() = 0;
  virtual int get_sock_val() const = 0;
  virtual sockaddr_in &get_addr() = 0;
  void add_component(MetaElement *e)
  {
    m_component = e;
  }

  // constant
  static const int af = AF_INET;
  static const int type = SOCK_STREAM;
private:
  MetaElement *m_component;
};

class TcpSocket: public Socket 
{
public:
  
  TcpSocket()
  {
    this->m_sock = -1;
  }

  virtual ~TcpSocket()
  {
    if (this->is_valid())
      ::close(this->get_socket());
  }

protected:

  inline void set_socket(int val)
  {
    this->m_sock=val;
  }

  inline int &get_socket()
  {
    return this->m_sock;
  }

  inline int get_sock_val() const
  {
    return this->m_sock;
  }

  inline sockaddr_in &get_addr()
  {
    return this->m_addr;
  }

private:
  int m_sock;
  sockaddr_in m_addr;
};


#endif /* Sockets */
