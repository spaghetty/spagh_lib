#include <stdio.h>
#include "spiller.h"

int
main(int argc, char *argv[]){
  char *dev;
  if(argc>1){
    /* MAIN BLOCK */
    dev = argv[1];
    start_listen(dev);
  }
  else{
    /* some errors in calling apps */
    printf("check for syntax you miss something\n");
  }
}
