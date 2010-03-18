#include "mod_load.h"
#include <dirent.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int f_select(const struct dirent *d)
{
  if( d != NULL)
    {
      if( *(d->d_name)!='.')
	{
	  char *ext = strchr(d->d_name, '.');
	  if (ext != NULL && strcmp(ext,".lac")==0)
	    return 1;
	}
    }
  return 0;
}

int module_search(char *name, char ***list)
{
  struct dirent **f1;
  int size,i;
  size = scandir (name, &f1, f_select, NULL);
  if ( size == -1)
    return -1;
  (*list) = malloc(sizeof(char*)*size);
  for (i=0; i<size; i++)
    {
      (*list)[i] = malloc(sizeof(char)*strlen(f1[i]->d_name));
      strcpy((*list)[i],(f1[i]->d_name));
    } 
  free(f1);
  return size;
}

int module_load(char *fname, plug_t* plug)
{
  lac_init_f init;
  char *tmp = malloc(sizeof(char)*(strlen(fname)+2));
  *tmp = '.';
  *(tmp+1) = '/';
  strcpy(tmp+2,fname);
  (*plug) = dlopen (tmp, RTLD_NOW);
  if (!(*plug))
    {
      printf ("Cannot load %s: %s\n", tmp, dlerror ());
      return 0;
    }
  init = dlsym ((*plug), "init_f");
  init ();
  return 1;
}
