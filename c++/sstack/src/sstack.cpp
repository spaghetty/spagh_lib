#include "sstack.h"
#include <iostream>

void Sstack::push(const int i)
{
  Selement *n1 = new Selement(i);
  if(this->start==NULL) { // we are here alone //
    this->start = n1;
    this->sorted = n1;
  }
  else { // try to get stuff ordered //
    // stack stuff //
    n1->stacked = this->start;
    this->start = n1;
    // managed double linked sorted list //
    Selement *next = this->sorted;
    while ( next->payload < n1->payload and next->sup!=NULL)
      {
	next = next->sup;
      }
    if ( next->payload > n1->payload ) {
      n1->sup = next;
      n1->inf = next->inf;
      if (next->inf!= NULL) {
	next->inf->sup = n1;
      } else{
	this->sorted = n1;
      }
      next->inf = n1;
    }
    else{ // new number is bigger then the last
      next->sup = n1;
      n1->inf = next;
    }
  }
}
  
int Sstack::pop()
{
  if( this->start != NULL)
    {
      Selement *tmp = start;
      this->start = tmp->stacked;
      if (tmp->inf != NULL)
	tmp->inf->sup = tmp->sup;
      else
	this->sorted = tmp->sup;
      if (tmp->sup != NULL)
	tmp->sup->inf = tmp->inf;
      int i = tmp->payload;
      delete tmp;
      return i;
    }
  else
    return -1;
}

int Sstack::min() const
{
  if( this->sorted!= NULL)
    return this->sorted->payload;
  else
    return -1;
}


bool Sstack::empty() const
{
  return (this->start == NULL);
}
