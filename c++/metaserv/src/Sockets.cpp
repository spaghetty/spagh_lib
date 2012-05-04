#include "Sockets.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

bool Socket::create(int protocol)
{
  this->get_socket() = socket(this->af, this->type, protocol);
  return (this->is_valid());
}

bool Socket::bind(int port, in_addr_t addr)
{
  sockaddr_in &tmp = this->get_addr();
  tmp.sin_family = AF_INET;
  tmp.sin_addr.s_addr = addr;
  tmp.sin_port = htons(port);
  return ( ::bind(this->get_socket(), (struct sockaddr *)&(tmp), 
		  sizeof(tmp))!=-1);
}

bool Socket::listen(int max_pending) const
{
  return ( ::listen(this->get_sock_val(), 
		    max_pending)!= -1);
}

bool Socket::accept ( Socket &new_sock )
{
  sockaddr_in &tmp = this->get_addr();
  int addr_length = sizeof(tmp);
  new_sock.get_socket() = ::accept ( this->get_socket(),
				     ( sockaddr * ) &(tmp),
				     ( socklen_t * ) &addr_length );
  return new_sock.is_valid();
}

// Client Stuff //
bool Socket::connect ( const std::string host, const int port ) 
{
}

// Data Transimission
bool Socket::send ( const std::string ) const {}

int Socket::recv ( std::string& s) const 
{
  char buffer[4096] = "";
  int read_size = 0;
  read_size = ::recv(get_sock_val(), buffer, 4096, 0);
  if ( -1 == read_size) {
    // manage errors here thake care of EMSGSIZE (errno);
  }
  s += buffer;
  return read_size;
}

void Socket::set_non_blocking () 
{
  if (not this->is_valid()) {
    return;
  }
  int SOCK_OPT = fcntl(this->get_socket(), F_GETFL);
  fcntl(this->get_socket(), F_SETFD, SOCK_OPT | O_NONBLOCK);
}

bool Socket::is_valid() const { return (this->get_sock_val() != -1); }

int Socket::get_descriptor() 
{
  return get_sock_val();
}

MetaElement *Socket::get_component()
{
  return m_component;
}

