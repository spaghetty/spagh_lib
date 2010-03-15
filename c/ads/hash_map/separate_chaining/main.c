#include <stdio.h>
#include "hmap.h"

int main(int argc, char *argv)
{
  ENTRY e, e1, e2, e3, e4, *r;
  e.key = "provola";
  e1.key = "proveta";
  e2.key = "provola";
  e3.key = "burbero";
  e4.key = "poppopo";
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
  hsearch(e2, ENTER, &r, &d);
  if(r!= NULL)
    printf("yep 4 %s\n",r->key);
  hsearch(e3, ENTER, &r, &d);
  if(r!= NULL)
    printf("yep 5 %s\n",r->key);
  hdestroy(&d);
  if(hsearch(e, ENTER, &r, &d)>0)
    printf("yep 6 \n");
  else
    printf("cool\n");
}
