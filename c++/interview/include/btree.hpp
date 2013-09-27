#ifndef __BTREE__
#define __BTREE__

#include<iostream>
#include<vector>
#include<queue>
#include<stack>



template<class T>
class Btree {
public:
  /* Exceptions */
  class Exception{};
  class IsEmpty:public Exception{};
  /***************** NODE CLASS ********************/
  class Node {
  public:
    Node():left(nullptr),right(nullptr),parent(nullptr){};
    Node(T d):data(d),left(nullptr),right(nullptr),parent(nullptr){};
    
    Node *getLeft() const { return left; };
    Node *getRight() const { return right; };
    T getData() const { return data; };
    
    bool appendLeft(Node *elem);
    bool appendRight(Node *elem);
    bool appendLeft(T elem);
    bool appendRight(T elem);
    Node *detachLeft();
    Node *detachRight();


  private:
    T data;
    Node *left,*right,*parent;
  };
  /***************** END NODE CLASS ********************/
  /***************** ITERATORS CLASS ********************/
  class Iterator {
  public:
    Iterator(Node *n, const Btree<T> *r):mynode(n), myroot(r) {};
    Iterator(const Iterator &i): mynode(i.mynode),myroot(i.myroot), recursion(i.recursion),status(i.status){};
    ~Iterator() {};

    T operator*() { return mynode->getData(); };
    Iterator &operator++(int i) { return ++(*this);};    
    bool operator!=(const Iterator &other) const { return (mynode != other.mynode); };
    virtual Iterator &operator++();

    Node *getNode() const { return mynode; };

    friend Btree<T>;

  protected:
    int status{0};
    Node *mynode;
    const Btree<T> *myroot;
    std::stack< std::pair<int,Node*> > recursion;
  };

  class PostIterator: public Iterator {
  public:
    PostIterator(Node *n, const Btree<T> *r): Iterator(n,r) {};
    PostIterator(const Iterator &i): Iterator(i) {};
  };

  class InIterator: public Iterator {
  public:
    InIterator(Node *n, const Btree<T> *r): Iterator(n,r) {};
    InIterator(const Iterator &i): Iterator(i) {};
    virtual InIterator &operator++();
  };

  class PreIterator: public Iterator {
  public:
    PreIterator(Node *n, const Btree<T> *r): Iterator(n,r) {};
    PreIterator(const Iterator &i): Iterator(i) {};
    virtual PreIterator &operator++();
  };

  /***************** END ITERATOR CLASS ********************/
  class BuildHelper {
  public:
    BuildHelper(Btree *b, Node *n):mynode(n),myroot(b){};
    BuildHelper operator()(T v) {
      BuildHelper *tmp = this;
      if(mynode==nullptr) {
	myroot->setRoot(v);
	mynode = myroot->getRoot();
      } else {
	while(1){
	  if(mynode->getLeft()==nullptr) {
	    mynode->appendLeft(v);
	    recursion.push(mynode->getLeft());
	    break;
	  } else if(mynode->getRight()==nullptr) {
	    mynode->appendRight(v);
	    recursion.push(mynode->getRight());
	    break;
	  } else {
	    if(recursion.size()>0){
	      mynode = recursion.front();
	      recursion.pop();
	    }
	  }
	}
      }
      return *tmp;
    };
  private:
    //int status{0};
    Node *mynode;
    Btree<T> *myroot;
    std::queue<Node*> recursion;
    
  };

  /** ALL THE TREE STUFF GOES UNDER HERE **/

  Btree():head(nullptr){};
  ~Btree();
  
  BuildHelper build(){
    return BuildHelper(this, getRoot());
  }

  bool isEmpty() const { return (head==nullptr); };

  Node *getRoot() const { return head; };
  
  bool setRoot(T val) {
    if(head!=nullptr) {
      return false;
    }
    head = new Node(val);
    return true;
  };
  
  bool setRoot(Node *b) { 
    if(head!=nullptr) {
      return false;
    }
    head = b;
    return true;
  };

  Iterator begin() const;
  PreIterator preBegin() const;
  PostIterator postBegin() const;
  InIterator inBegin() const;
  Iterator end() const;

private:
  Node *head;

protected:
};

template <class T>
Btree<T>::~Btree() {
  for(auto i = begin(); i!=end(); i++) {
    delete i.getNode();
  }
}

template <class T>
typename Btree<T>::Iterator &Btree<T>::Iterator::operator++() {
  while(true && mynode!=nullptr) {
    if(status==0){
      if(mynode->getLeft()!=nullptr) {
	std::pair<int,Node*> p;
	p = std::make_pair(status+1,mynode);
	recursion.push(p);
	mynode=mynode->getLeft();
	status=0;
	continue;
      }
      status=1;
    }
    if(status==1) {
      if(mynode->getRight()!=nullptr) {
	std::pair<int,Node*> p;
	p = std::make_pair(status+1,mynode);
	recursion.push(p);
	mynode=mynode->getRight();
	status=0;
	continue;
      }
      status=2;
    } 
    if (status==2) {
      status=3;
      break;
    } 
    if (status==3) {
      if(recursion.size()>0) {
	std::pair<int,Node*> tmp = recursion.top();
	recursion.pop();
	status = tmp.first;
	mynode = tmp.second;
      } else {
	status = 0;
	mynode = nullptr;
      }
    }
  }
  return *this;
};

template <class T>
typename Btree<T>::InIterator &Btree<T>::InIterator::operator++() {
  while(true && this->mynode!=nullptr) {
    if(this->status==0){
      if(this->mynode->getLeft()!=nullptr) {
	std::pair<int,Node*> p;
	p = std::make_pair((this->status)+1,this->mynode);
	this->recursion.push(p);
	this->mynode=this->mynode->getLeft();
	this->status=0;
	continue;
      }
      this->status=1;
    }
    if (this->status==1) {
      this->status=2;
      break;
    } 
    if(this->status==2) {
      if(this->mynode->getRight()!=nullptr) {
	std::pair<int,Node*> p;
	p = std::make_pair((this->status)+1,this->mynode);
	this->recursion.push(p);
	this->mynode=this->mynode->getRight();
	this->status=0;
	continue;
      }
      this->status=3;
    } 
    if (this->status==3) {
      if(this->recursion.size()>0) {
	std::pair<int,Node*> tmp = this->recursion.top();
	this->recursion.pop();
	this->status = tmp.first;
	this->mynode = tmp.second;
      } else {
	this->status = 0;
	this->mynode = nullptr;
      }
    }
  }
  return *this;
};

template <class T>
typename Btree<T>::PreIterator &Btree<T>::PreIterator::operator++() {
  while(true && this->mynode!=nullptr) {
    if (this->status==0) {
      this->status=1;
      break;
    } 
    if(this->status==1){
      if(this->mynode->getLeft()!=nullptr) {
	std::pair<int,Node*> p;
	p = std::make_pair((this->status)+1,this->mynode);
	this->recursion.push(p);
	this->mynode=this->mynode->getLeft();
	this->status=0;
	continue;
      }
      this->status=2;
    }
    if(this->status==2) {
      if(this->mynode->getRight()!=nullptr) {
	std::pair<int,Node*> p;
	p = std::make_pair((this->status)+1,this->mynode);
	this->recursion.push(p);
	this->mynode=this->mynode->getRight();
	this->status=0;
	continue;
      }
      this->status=3;
    } 
    if (this->status==3) {
      if(this->recursion.size()>0) {
	std::pair<int,Node*> tmp = this->recursion.top();
	this->recursion.pop();
	this->status = tmp.first;
	this->mynode = tmp.second;
      } else {
	this->status = 0;
	this->mynode = nullptr;
      }
    }
  }
  return *this;
};

template<class T>
typename Btree<T>::Iterator Btree<T>::begin() const{
  if(head==nullptr) {
    Iterator t(nullptr, this);
    return t;
  }
  Node *tmp = head;
  Iterator t(tmp,this);
  ++t;
  return t;
};

template<class T>
typename Btree<T>::PreIterator Btree<T>::preBegin() const{
  if(head==nullptr) {
    Iterator t(nullptr, this);
    return t;
  }
  Node *tmp = head;
  PreIterator t(tmp,this);
  ++t;
  return t;
};

template<class T>
typename Btree<T>::PostIterator Btree<T>::postBegin() const{
  if(head==nullptr) {
    Iterator t(nullptr, this);
    return t;
  }
  Node *tmp = head;
  PostIterator t(tmp,this);
  ++t;
  return t;
};

template<class T>
typename Btree<T>::InIterator Btree<T>::inBegin() const{
  if(head==nullptr) {
    Iterator t(nullptr, this);
    return t;
  }
  Node *tmp = head;
  InIterator t(tmp,this);
  ++t;
  return t;
};

template<class T>
typename Btree<T>::Iterator Btree<T>::end() const{
  Iterator t(nullptr, this);
  return t;
};

template<class T>
bool Btree<T>::Node::appendLeft(Node *elem){
  if(left!=nullptr)
    return false;
  left = elem;
  return true;
};

template<class T>
bool Btree<T>::Node::appendLeft(T elem){
  if(left!=nullptr)
    return false;
  return appendLeft(new Node(elem));
};

template<class T>
bool Btree<T>::Node::appendRight(Node *elem){
  if(right!=nullptr)
	return false;
  right = elem;
  return true;
};

template<class T>
bool Btree<T>::Node::appendRight(T elem){
  if(right!=nullptr)
    return false;
  return appendRight(new Node(elem));
};

template<class T>
typename Btree<T>::Node *Btree<T>::Node::detachLeft() {
  auto tmp = getLeft();
  left = nullptr;
  return tmp;
};

template<class T>
typename Btree<T>::Node *Btree<T>::Node::detachRight() {
  auto tmp = getRight();
  right = nullptr;
  return tmp;
};


#endif /* __BTREE__ */
