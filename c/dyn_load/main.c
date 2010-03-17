#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mod_load.h"
#include "lac_module.h"

int main(int argc, char *argv)
{
  char **f;
  int size,i;
  void *plugin;
  lac_init_f init;
  printf("porca merda \n");
  size = module_search(".", &f);
  for(i=0; i<size; i++)
    {
      printf("ecco qua %s\n",f[i]);
      plugin = dlopen (f[i], RTLD_NOW);
      if (!plugin)
	{
	  printf ("Cannot load %s: %s", f[i], dlerror ());
	  exit(1);
	}
      init = dlsym (plugin, "init_f");
      init ();
    }
}
