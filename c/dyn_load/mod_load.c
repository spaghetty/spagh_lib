#include "mod_load.h"
#include <dirent.h>
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
