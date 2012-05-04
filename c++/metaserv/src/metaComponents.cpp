#include "metaComponents.h"

void MetaElement::add_socket(Socket &s)
{
  s.add_component(this);
}


void MetaClient::handle_msg(MetaEvent& event)
{
  if( event.get_type() == 0x1 ) {
    EpollEvent *m = static_cast<EpollEvent*>(&event);
    if( m->get_event() == EPOLLIN ){
      std::string s = "";
      m->get_socket()->recv(s);
      std::cout << s << std::endl;
    }
  }
}

MetaServer::MetaServer(int port, epollManager &e)
{
  //TcpSocket &s = new TcpSocket();
  m_pollm = &e;
  m_pollm->create();
  m_sock[0] = new TcpSocket();
  if(m_sock[0]->create()){
    std::cout << "stichi " << std::endl;
    if(!m_sock[0]->bind(port))
      std::cout << "fuck" << std::endl;
    m_sock[0]->set_non_blocking();
    if(!m_sock[0]->listen())
      std::cout << "fuck2" << std::endl;
    add_socket(*m_sock[0]);
    m_pollm->add(*(m_sock[0]));
  }
  else
    std::cout << "fanculo ricchione"<< std::endl;
}

void MetaServer::handle_msg(struct epoll_event& event)
{
  MetaClient *cc = new MetaClient();
  TcpSocket *s1 = new TcpSocket();
  if(!((Socket*)(event.data.ptr))->accept(*s1))
    std::cout << "fuck3" << std::endl;
  s1->set_non_blocking();
  cc->add_socket(*s1);
  m_pollm->add(*s1);
}
