#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mod_load.h"
#include "lac_module.h"

int main(int argc, char *argv)
{
  char **f;
  int size,i;
  plug_t p;
  size = module_search(".", &f);
  for(i=0; i<size; i++)
    {
      printf("ecco qua %s\n",f[i]);
      if(!module_load(f[i],&p))
	printf("i miss something\n");
    }
}
