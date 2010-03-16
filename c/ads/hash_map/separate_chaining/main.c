#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmap.h"


#define SIZE 100

char *rand_str()
{
  int len = (rand()%15)+2;
  //int len = 5;
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
  struct timeval t1, t2;
  ENTRY *e,e1, *r;
  hsearch_data d;
  hcreate(50, &d);
  srand(time(NULL));
  int i;
  char* list[SIZE];
  for(i=0; i<SIZE;i++)
    {
      e = malloc(sizeof(ENTRY));
      e->key = rand_str();
      list[i] = malloc(sizeof(char)*(strlen(e->key)+1));
      strcpy(list[i],(e->key));// , strlen(e->key)+1);
      e->data = NULL;
      gettimeofday(&t1, NULL);
      if(hsearch(*e, ENTER, &r, &d)> 0)
	{
	  gettimeofday(&t2, NULL);
	  printf("yep  %s\n",r->key);
	}
      else
	{
	  gettimeofday(&t2, NULL);
	  printf("cool\n");
	}
      printf("Insertion took %ld seconds and %ld microseconds.\n",
	     t2.tv_sec - t1.tv_sec, 
	     t2.tv_usec >= t1.tv_usec ? t2.tv_usec - t1.tv_usec : t2.tv_usec + 1000000L - t1.tv_usec);
    }
  printf("bon\n");
  for(i=0;i<SIZE;i++)
    {
      e1.key=list[i];
      if(hsearch(e1, FIND, &r, &d))
	{
	  printf(" %s yapp  %s\n",list[i],r->key);
	}
    }
  printf("\n");
  hdestroy(&d);
}
