#ifndef __4INAROW__
#define __4INAROW__

#include <boost/circular_buffer.hpp>
#include "board.pb.h"
#include <iostream>
#include <vector>

class Board {
public:
  enum Status { Ready=1, Full, Win };
  class Exception{
  public:
    virtual std::string name(){ return "Board::Exception";}
  };
  class ColumnExceeded:public Exception{
  public:
    virtual std::string name(){ return "ColumnExceeded"; }
  };
  class ColumnFull:public Exception {
  public:
    virtual std::string name(){ return  "ColumnFull"; }
  };
  

  Board();
  ~Board();

  void play(int column,const char sign);
  Move getLastMove() const;

  Status status() const;

  std::string toString() const;
  
  SBoard &toProtoBuf() const;

private:

  bool check_rows() const;
  bool check_columns() const;
  bool check_right() const;
  bool check_left() const;
  bool check_full() const;

  static const int MAX_COLUMNS{7};
  static const int MAX_ROWS{6};
  const char EMPTY{' '};
  mutable SBoard *sb;
  mutable boost::circular_buffer<Move> undo{7};
  mutable bool validsb;
  int height[MAX_COLUMNS];

  std::vector<std::vector<char>> board;
};

Board::Board() {
  board.resize(MAX_ROWS);
  for( auto i=0; i<MAX_ROWS; i++ ) {
    board[i].resize(MAX_COLUMNS,EMPTY);
  }
  for( auto i=0; i<MAX_COLUMNS; i++) {
    height[i]=0;
  }
  sb=nullptr;
  validsb=false;
};

Board::~Board() { 
  board.clear();
  if(sb!=nullptr)
    delete sb;
};

Move Board::getLastMove() const {
    return undo[0];
};

SBoard &Board::toProtoBuf() const{
  if(!validsb){
    if(sb!=nullptr){
      delete sb;
    }
    sb = new SBoard();
    for( auto i=0; i<MAX_COLUMNS; i++){
      sb->add_height(height[i]);
    }
    for( auto i: board) {
      for( auto j: i) {
	sb->add_board(j);
      }
    }
    validsb=true;
  }
  return *sb;
};

void Board::play(int column, const char sign) {
  column -= 1;
  if(column>=MAX_COLUMNS) {
    throw ColumnExceeded();
  }
  if(height[column]==MAX_ROWS) {
    throw ColumnFull();
  }
  board[height[column]][column]= sign;
  height[column]+= 1;
  auto tmp = new Move();
  tmp->set_column(column+1);
  undo.push_front(*tmp);
  delete tmp;
  validsb=false;
};

Board::Status Board::status() const {
  if (check_rows()) {
    return Status::Win;
  }
  if (check_columns()) {
    return Status::Win;
  }
  if (check_right()) {
    return Status::Win;
  }
  if (check_left()) {
    return Status::Win;
  }
  if (check_full()) {
    return Status::Full;
  }
  return Status::Ready;
}

bool Board::check_full() const {
  for(auto i: height) {
    if(i<MAX_ROWS)
      return false;
  }
  return true;
}

bool Board::check_rows() const {
  bool winning=false;
  for( auto i: board ) {
    int max = 0;
    char t = EMPTY;
    for (int j=0; j<MAX_COLUMNS && max<4; j++) {
      if (t==i[j] && i[j]!=EMPTY) {
	max+=1;
      } else {
	t = i[j];
	max=1;
      }
    }
    if(max==4) {
      winning = true;
      break;
    }
  }
  return winning;
}

bool Board::check_columns() const {
  bool winning=false;
  for( auto i=0; i<MAX_COLUMNS; i++ ) {
    int max = 0;
    char t = EMPTY;
    for (int j=0; j<MAX_ROWS && max<4; j++) {
      if (t==board[j][i] && board[j][i]!=EMPTY) {
	max+=1;
      } else {
	t = board[j][i];
	max=1;
      }
    }
    if(max==4) {
      winning = true;
      break;
    }
  }
  return winning;
}

bool Board::check_right() const {
  bool winning=false;
  for( auto i = -(MAX_ROWS-4); i<(MAX_COLUMNS-3); i++) {
    auto tmp = i;
    int max = 0;
    char t = EMPTY;
    for(auto j=0; j<MAX_ROWS && max<4; j++) {
      if(tmp>=0 && j>=0) {
	if (t==board[j][tmp] && board[j][tmp]!=EMPTY) {
	  max+=1;
	} else {
	  t = board[j][tmp];
	  max=1;
	}	
      }
      tmp++;
    }
    if(max==4) {
      winning=true;
      break;
    }
  }
  return winning;
}

bool Board::check_left() const {
  bool winning=false;
  for( auto i = 3; i<(MAX_COLUMNS+2); i++) {
    auto tmp = i;
    int max = 0;
    char t = EMPTY;
    for(auto j=0; j<MAX_ROWS && max<4; j++) {
      if((tmp>=0 && j>=0) && tmp<MAX_COLUMNS) {
	if (t==board[j][tmp] && board[j][tmp]!=EMPTY) {
	  max+=1;
	} else {
	  t = board[j][tmp];
	  max=1;
	}	
      }
      tmp--;
    }
    if(max==4) {
      winning=true;
      break;
    }
  }
  return winning;
}

std::string Board::toString() const {
  std::ostringstream res;
  res << " _____________" << std::endl;
  for( auto row = board.rbegin(); row!=board.rend(); ++row ) {
    for ( auto field: *row) {
      res << "|" << field;
    }
    res << "|" << std::endl;
  }
  res << " -------------" << std::endl;
  res << " 1 2 3 4 5 6 7 " << std::endl;
  return res.str();
};

#endif /* __4INAROW__ */
