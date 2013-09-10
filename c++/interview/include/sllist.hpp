#ifndef SLLIST
#define SLLIST


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
    auto t = new Node<T>(d);
    auto a = this;
    while( a->getNext() != nullptr ) {
      a = a->getNext();
    }
    a->append(t);
  };

  Node* remove(Node *head) {
    if (this == head) {
      return head->getNext();
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
	}
	prev = next;
	next = next->getNext();
      }
      tmp = tmp->getNext();
    }
  };

};



#endif /* SLLIST */
