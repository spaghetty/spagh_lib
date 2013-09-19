#include <iostream>
#include <sllist.hpp>

using namespace std;

Node<int>* sum(Node<int> *a, Node<int> *b) {
  // works just for int
  Node<int>* c = nullptr;
  auto tmp = a;
  auto tmp2 = b;
  int ret = 0;
  while( tmp!=nullptr || tmp2!=nullptr ) {
    int value;
    if (tmp!=nullptr && tmp2!=nullptr) {
      value = tmp->getData() + tmp2->getData() + ret;
      tmp = tmp->getNext();
      tmp2 = tmp2->getNext();
    } else {
      if ( tmp!=nullptr ) {
	value = tmp->getData() + ret;
	tmp = tmp->getNext();
      }
      if ( tmp2!=nullptr ) {
	value = tmp2->getData() + ret;
	tmp2 = tmp2->getNext();
      }
    } 
    if( value > 9 ){
      ret = (value - (value % 10)) /10;
      value = value % 10;
    } else {
      ret = 0;
    }
    if ( c == nullptr ) {
      c = new Node<int>(value);
    } else {
      c->append(value);
    }
  }
  if (ret != 0 ) {
    c->append(ret);
  }
  return c;
}

int main(int argc, char** argv) {
  for(auto i=0; i<1; i++) { 
  auto a = new Node<int>(7);
  a->append(8);
  a->append(9);
  a->append(10);
  a->print();
  
  auto test = a->getNext()->getNext();
  a = test->remove(a);
  delete test;

  a->print();

  a->append(5);
  a->append(6);
  a->append(8);
  a->append(9);
  a->append(5);
  a->append(6);
  a->append(8);
  a->append(9);

  a->print();

  a->purge_dup();

  a->print();

  a->getNext()->getNext()->remove();
  a->print();
  a->append(1);
  a->append(2);
  a->append(3);
  a->append(4);
  a->print();
  auto tmp = a->getFromLast(3);
  if (tmp!=nullptr) {
    cout << tmp->getData() << endl;
  }
  tmp = a->getFromLast(5);
  if (tmp!=nullptr) {
    cout << tmp->getData() << endl;
  }
  tmp = a->getFromLast(12);
  if (tmp!=nullptr) {
    cout << tmp->getData() << endl;
  }
  
  auto b =  new Node<int>(4);   //702
  b->append(0);
  b->append(7);
  auto c =  new Node<int>(8);   //98
  c->append(9);
  auto d = sum(b, c);
  d->print();
  a->DeleteList();
  delete a;
  b->DeleteList();
  delete b;
  c->DeleteList();
  delete c;
  d->DeleteList();
  delete d;
  }
}
