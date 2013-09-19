#include<iostream>
#include<math.h>


#define DISKS 3

const int EMPTY = -1;

class Rod {
private:
  int road_slot[DISKS+1];
  int top;
public:
  Rod(){
    for(int i=0; i<DISKS+1; i++ ) 
      road_slot[i] = EMPTY;
    top = 0;
  }

  bool push(int d) {
    if(top!=0){
      if(top==DISKS) 
	return false;
      if( road_slot[top-1]<d && road_slot[top-1]>-1 )
	return false;
    }
    road_slot[top] = d;
    top++;
    return true;
  }

  int pop() {
    if(top==0) 
      return -1;
    top--;
    auto tmp = road_slot[top];
    road_slot[top] = EMPTY;
    return tmp;
  }

  void print() {
    int i = 0;
    std::cout << "--";
    for(; i<DISKS+1; i++) {
      if(road_slot[i]==EMPTY) {
	std::cout << "---";
      } else {
	std::cout << road_slot[i] << "--";
      }
    }
    std::cout << std::endl;
  }

  bool is_complete() {
    return (top==DISKS);
  }
};

int check(char a) {
  int value = -1;
  switch(a) {
  case 'A':
  case 'a':
    value = 0;
    break;
  case 'B':
  case 'b':
    value = 1;
    break;
  case 'C':
  case 'c':
    value = 2;
    break;
  case 'Q':
  case 'q':
    value = 3;
    break;
  }
  return value;
}



int main(int argn, char **argv) {
  Rod* a[3] = { new Rod(),new Rod(),new Rod()};
  for(int i = DISKS; i>0; i-- ) {
    a[0]->push(i);
  }
  int exact_moves = pow(2,DISKS)-1;
  int max_moves = exact_moves + 3;
  char from, to;
  int f,t;
  bool success = false;
  int steps = 0;
  while(!success && steps<max_moves) { 
    steps++;
    std::cout << "A) ";
    a[0]->print();
    std::cout << "B) ";
    a[1]->print();
    std::cout << "C) ";
    a[2]->print();
    if( a[1]->is_complete() || a[2]->is_complete()){
      std::cout << "You did it using " << steps << " steps" << "on exact match of " << exact_moves <<std::endl;
      success = true;
      continue;
    }
    while(true) {
      std::cout << "select From:";
      std::cin >> from;
      std::cout << " select To:";
      std::cin >> to;
      std::cout << std::endl;
      f = check(from);
      t = check(to);
      
      if(f==3 || t==3) { 
	success = true;
	break;
      }
      if(f>=0 && f<=2 && t>=0 && t<=2) {
	break;
      } else {
	std::cout << "errore di immissione" << std::endl;
      }
    }
    if(!success) {
      std::cout << steps << "/" << exact_moves << ")"<< "move from " << from << " to " << to << std::endl;
      auto tmp = a[f]->pop();
      if(!tmp) {
	std::cout << "error estracting from queue" << std::endl;
	steps--;
	continue;
      }
      auto res = a[t]->push(tmp);
      if(!res) {
	std::cout << "error pushing element" << std::endl;
	a[f]->push(tmp);
	steps--;
      }
      //success = true;
    }
  }
  if(steps==max_moves) {
    std::cout << "----=====YoU LOose====-----" << std::endl;
  }
  for(int i = 0; i<3; i++ ) {
    delete a[i];
  }

}
