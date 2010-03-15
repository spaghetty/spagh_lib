#include <stdio.h>
#include "hmap.h"

int main(int argc, char *argv)
{
  ENTRY e, e1, *r;
  e.key = "strunz";
  e1.key = "strinz";
  hsearch_data d;
  hcreate(50, &d);
  if(hsearch(e, ENTER, &r, &d)> 0)
    printf("yep 1  %s\n",r->key);
  else
    printf("cool\n");
  hsearch(e, ENTER, &r, &d);
  printf("yep 2 %s\n",r->key);
  hsearch(e1, ENTER, &r, &d);
  if(r!= NULL)
    printf("yep 3 %s\n",r->key);
  hdestroy(&d);
  if(hsearch(e, ENTER, &r, &d)>0)
    printf("yep 4 \n");
  else
    printf("cool\n");
}
