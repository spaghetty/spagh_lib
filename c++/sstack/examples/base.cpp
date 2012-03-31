#include <iostream>
#include <sstack.h>

int main ( int argn, char **argv)
{
  Sstack *s = new Sstack();
  s->push(7);
  std::cout << "insert new element:" << s->min() << std::endl;
  s->push(8);
  std::cout << "insert new element:" << s->min() << std::endl;
  s->push(5);
  std::cout << "insert new element:" << s->min() << std::endl;
  s->push(4);
  std::cout << "insert new element:" << s->min() << std::endl;
  s->push(1);
  std::cout << "insert new element:" << s->min() << std::endl;

  while(! s->empty()){
    std::cout << " new min: " << s->min() << std::endl;
    std::cout << "poped: " << s->pop();
  }
}
