#include <stdio.h>
#include <stdlib.h>
#include "hmap.h"


char *rand_str()
{
  //int len = (rand()%15)+2;
  int len = 5;
  int ch,i;
  char *str = malloc(sizeof(char)*len);
  for(i=0; i < len-1;i++)
    {
      str[i]=(rand()%25)+97;
    }
  str[len-1]='\0';
  return str;
}


int main(int argc, char *argv)
{
  ENTRY *e, *r;
  hsearch_data d;
  hcreate(50, &d);
  srand(time(NULL));
  int i;
  for(i=0; i<100;i++)
    {
      e = malloc(sizeof(ENTRY));
      e->key = rand_str();
      e->data = NULL;
      if(hsearch(*e, ENTER, &r, &d)> 0)
	printf("yep  %s\n",r->key);
      else
	printf("cool\n");
    }
  hdestroy(&d);
  char *b = rand_str();
}
