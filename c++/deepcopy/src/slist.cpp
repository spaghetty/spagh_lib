#include <map>
#include <iostream>
#include <ostream>

struct Selem {
  Selem(int v): next(NULL), related(NULL), payload(v) {}

  Selem *next;
  Selem *related;
  const int payload;
};

class Slist {
public:
  Slist(): start(NULL) {}
  
  Slist(Slist &s): start(NULL){
    std::map<Selem*,Selem*> translate_map;
    std::map<Selem*,Selem*>::iterator it;
    Selem *tmp = s.start;
    while (tmp != NULL) {
      Selem *n = NULL;
      it = translate_map.find(tmp);
      if ( it!= translate_map.end()) {
	n = (*it).second;
      }
      else{
	n = new Selem(tmp->payload);
	translate_map.insert(std::pair<Selem*,Selem*>(tmp,n));
      }
      this->append(*n);
      it = translate_map.find(tmp->related);
      if( it!= translate_map.end()) {
	n->related = (*it).second;
      }
      else {
	Selem *pre_build = new Selem(tmp->related->payload);
	translate_map.insert(std::pair<Selem*,Selem*>(tmp->related,pre_build));
	n->related = pre_build;
      }
      tmp = tmp->next;
    }
  }

  ~Slist() {
    Selem *elem = this->start;
    Selem *old;
    while (elem != NULL) {
      old = elem;
      elem = elem->next;
      delete old;
    }
  }

  int insert(int pos, Selem &node)
  {
    Selem *place = this->start;
    bool is_first = true;
    pos=pos-1;
    while( pos>1 and place->next != NULL) {
      is_first=false;
      place = place->next;
      pos -= 1;
    }
    if( is_first) {
      node.next = this->start;
      this->start = &node;
    }
    else if( pos==1 ) {  // the selected position is out of the range 
      node.next = place->next;
      place->next = &node;
    }
    else {
      return -1;
    }
  }

  void append(Selem &node) 
  {
    Selem *tmp = this->start;
    if (tmp == NULL){
      this->start = &node;
    }
    else {
      while (tmp->next != NULL){
	tmp = tmp->next;
      }
      tmp->next = &node;
    }
  }

  Selem &get(int pos) {
    Selem *place = this->start;
    pos=pos-1;
    while( pos>1 and place->next != NULL) {
      place = place->next;
      pos -= 1;
    }
    if (pos ==1 ){
      return *place;
    }
  }

  friend std::ostream& operator<< (std::ostream& out, const Slist &b){
    Selem *elem = b.start;
    while (elem != NULL){
      out << elem->payload << " :";
      if (elem->related != NULL)
	out << elem->related->payload; 
      out << std::endl;
      elem = elem->next;
    }
    return out;
  }

protected:
  Selem  *start;
};


int main ( int argn, char **argv) {

  Slist s;
  Selem *e = new Selem(1);
  Selem *e1 = new Selem(2);
  Selem *e2 = new Selem(3);
  e->related=e2;
  e1->related=e;
  e2->related=e1;
  s.insert(1,*e);
  s.insert(1,*e1);
  s.insert(1,*e2);
  std::cout << s << std::endl;
  Slist s2(s);
  std::cout << "copyed:" << std::endl;
  std::cout << s2 << std::endl;
  
}
