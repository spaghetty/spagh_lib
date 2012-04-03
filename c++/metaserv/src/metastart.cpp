#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

// we need socket6 class

class Socket
{
public:

  Socket()
  {
    this->m_sock = -1;
  }

  ~Socket()
  {
    if (this->is_valid())
      ::close(this->m_sock);
  }
  
  bool create(int protocol=0)
  {
    this->m_sock = socket(this->af, this->type, protocol);
    return (this->is_valid());
  }

  bool bind(int port, in_addr_t addr=INADDR_ANY)
  {
    this->m_addr.sin_family = AF_INET;
    this->m_addr.sin_addr.s_addr = addr;
    this->m_addr.sin_port = htons(port);
    return ( ::bind(this->m_sock, (struct sockaddr *)&(this->m_addr), 
		    sizeof(this->m_addr))!=-11);
  }

  bool listen(int max_pending=5) const 
  {
    return ( ::listen(this->m_sock, max_pending)!= -1);
  }

  bool accept ( Socket &new_sock ) const
  {
    int addr_length = sizeof(this->m_addr);
    new_sock.m_sock = ::accept ( m_sock, ( sockaddr * ) &(this->m_addr), ( socklen_t * ) &addr_length );
    return ( new_sock.m_sock >= 0 );
  }

  // Client Stuff //
  bool connect ( const std::string host, const int port ) {}

  // Data Transimission
  bool send ( const std::string ) const {}
  int recv ( std::string& ) const {}


  void set_non_blocking ( const bool ) {}

  bool is_valid() const { return m_sock != -1; }

private:
  int m_sock;
  sockaddr_in m_addr;

  // constant
  static const int af = AF_INET;
  static const int type = SOCK_STREAM;
};

int main ()
{

  Socket s,s1;

  if(s.create()){
    printf("stichi \n");
    if(!s.bind(8080))
      printf("fuck\n");
    if(!s.listen())
      printf("fuck2\n");
    if(!s.accept(s1))
      printf("fuck3\n");
    printf("merda\n");
  }
  else 
    printf("fanculo ricchione \n");
}
