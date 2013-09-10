#include <iostream>
#include <sllist.hpp>

using namespace std;

int main(int argc, char** argv) {
  auto a = new Node<int>(7);
  a->append(8);
  a->append(9);
  a->append(10);
  auto tmp = a;
  while (tmp != nullptr) {
    cout << tmp->getData() << " ";
    tmp = tmp->getNext();
  }
  a->getNext()->getNext()->remove(a);
  cout << endl;
  tmp = a;
  while (tmp != nullptr) {
    cout << tmp->getData() << " ";
    tmp = tmp->getNext();
  }
  cout << endl;
  a->append(5);
  a->append(6);
  a->append(8);
  a->append(9);
  a->append(5);
  a->append(6);
  a->append(8);
  a->append(9);
  tmp = a;
  while (tmp != nullptr) {
    cout << tmp->getData() << " ";
    tmp = tmp->getNext();
  }
  cout << endl;
  a->purge_dup();
  tmp = a;
  while (tmp != nullptr) {
    cout << tmp->getData() << " ";
    tmp = tmp->getNext();
  }
  cout << endl;
}
