#ifndef SSTACK_H
#define SSTACK_H

#include <cstddef>

struct Selement {
  Selement (int i)
  {
    stacked = NULL;
    sup = NULL;
    inf = NULL;
    payload = i;
  }

  Selement *stacked;
  Selement *sup;
  Selement *inf;
  int payload;
};


class Sstack 
{
public:
  Sstack(): start(NULL), sorted(NULL) {}

  bool empty() const;
  void push(const int i);
  int pop();
  int min() const;

  //private section
private:
  Selement *start;
  Selement *sorted;
};


#endif /* SSTACK_H */
