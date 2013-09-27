#include "btree.hpp"

#include <iostream>



int main(int argn, char **argv) {
  Btree<int> t;
  // if(t.isEmpty()) {
  //   t.setRoot(1);
  //}

  t.build()(1)
        (2)   (3)
      (4)(5)(6)(7)
    (8)(9);
  std::cout << "Pre odine:" << std::endl;
  for( auto i = t.preBegin(); i!=t.end(); ++i) {
    std::cout << (*i) << " ";
  }
  std::cout << std::endl;
  std::cout << "Post odine:" << std::endl;
  for( auto i = t.postBegin(); i!=t.end(); ++i) {
    std::cout << (*i) << " ";
  }
  std::cout << std::endl;
  std::cout << "In Odine:" << std::endl;
  for( auto i = t.inBegin(); i!=t.end(); ++i) {
    std::cout << (*i) << " ";
  }
  std::cout << std::endl;
}
