#ifndef __4PLAYER__
#define __4PLAYER__


#include <iostream>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketStream.h>

#include "4board.hpp"

class Player {
public:
  Player(const char s): sign(s) {};
  
  virtual int play(Board &b) const;

  const char sign;
private:
};


int Player::play(Board &b) const {
  int i = -1;
  std::cout << sign << ") Insert column: ";
  std::cin >> i;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return i;
};

class ServPlayer: public Player {
public:
  ServPlayer(const char s):Player(s),srv(5099) {
    std::cout << "Waiting for client connection...\n" << std::flush;
    ss = srv.acceptConnection();
    ss.setNoDelay(true);
    str = new Poco::Net::SocketStream(ss);
  };

  virtual int play(Board &b) const;

private:
  mutable bool turn{true};
  Poco::Net::ServerSocket srv;
  Poco::Net::StreamSocket ss;
  Poco::Net::SocketStream *str;
};

int ServPlayer::play(Board &b) const {
  bool base = turn;
  Move m,n;
  int tmp=-1;
  if(turn) {
    m = b.getLastMove();
    std::string s;
    if(!m.has_column())
      std::cout << "stichi e' vuoto" << std::endl;
    m.SerializeToString(&s);
    (*str) << s << std::endl << std::endl << std::flush;
    turn=false;
  }
  if(!turn) {
    std::string s;
    (*str) >> s;
    std::cout << s.size() << std::endl;
    auto r = n.ParseFromString(s);
    while(!r && s.size()>0) {
      s.erase(0,1);
      r = n.ParseFromString(s);
    }
    tmp = n.column();
    turn = true;
  }
  return tmp;
};



class CliPlayer: public Player {
public:
  CliPlayer(const char s, const std::string addr):Player(s),sa(addr, 5099) {
    socket.connect(sa);
    str = new Poco::Net::SocketStream(socket);
  };

  virtual int play(Board &b) const;

private:
  mutable bool turn{false};
  Poco::Net::SocketAddress sa;
  Poco::Net::StreamSocket socket;
  Poco::Net::SocketStream *str;
};

int CliPlayer::play(Board &b) const {
    bool base = turn;
    Move m,n;
    int tmp=-1;
  if(turn) {
    m = b.getLastMove();
    std::string s;
    m.SerializeToString(&s);
    (*str) << s << std::endl << std::endl <<  std::flush;
    turn=false;
  }
  if(!turn) {
    std::string s;
    (*str) >> s;
    std::cout << s.size() << std::endl;
    auto r = n.ParseFromString(s);
    while(!r && s.size()>0) {
      s.erase(0,1);
      r = n.ParseFromString(s);
    }
    tmp = n.column();
    turn = true;
  }

  return tmp;
};

#endif /* __4PLAYER */
