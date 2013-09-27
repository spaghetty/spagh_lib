#ifndef SLLIST
#define SLLIST

#include<iostream>

template<class T> 
class Node {
private:
  T Data;
  Node* Next;

protected: 
  void append(Node *n) {
    Next = n;
  }

public:
  Node(T data): Data(data), Next(nullptr) {};
  T getData(){return Data;};
  void setData(T data) { Data = data; };
  Node* getNext() { return Next; };
  void append(T d){
    Node* t = new Node<T>(d);
    Node* a = this;
    while( a->getNext() != nullptr ) {
      a = a->getNext();
    }
    a->append(t);
    t=nullptr;
  }

  ~Node(){
  }

  void DeleteList() {
    if (Next!=nullptr) {
      Next->DeleteList();
    }
    delete Next;
  }

  bool remove(){
    // removing node just having access to this one
    if(Next == nullptr) {
      return false;
    }
    auto tmp = Next;
    Data = tmp->getData();
    Next = tmp->getNext();
    delete tmp;
    return true;
  }

  void print() {
    auto tmp = this;
    while (tmp != nullptr) {
      std::cout << tmp->getData() << " ";
      tmp = tmp->getNext();
    }
    std::cout << std::endl;
  }

  Node* getFromLast(int n) {
    auto tmp = this;
    auto nth = this;
    int dist = 0;
    while(tmp!=nullptr) {
      if (dist==n) {
	nth = nth->getNext();
	dist--;
      }
      dist++;
      tmp = tmp->getNext();
    }
    if (dist < n) {
      return nullptr;
    }
    return nth;
  }

  Node* remove(Node *head) {
    if (this == head) {
      auto tmp = head->getNext();
      delete head;
      return tmp;
    }
    auto tmp = head;
    while(tmp != nullptr && tmp->getNext() != this){
      tmp = tmp->getNext();
    }
    if (tmp != nullptr) {
      tmp->append(getNext());
      Next = nullptr;
    }
    return head;
  };

  void purge_dup(){
    auto tmp = this;
    while(tmp != nullptr) {
      auto x = tmp->getData();
      auto next = tmp->getNext();
      auto prev = tmp;
      while(next != nullptr) {
	if (x == next->getData()) {
	  prev->append(next->getNext());
	  delete next;
	  next = prev->getNext();
	  continue;
	}
	prev = next;
	next = next->getNext();
      }
      tmp = tmp->getNext();

    }
  };
};

#endif /* SLLIST */
