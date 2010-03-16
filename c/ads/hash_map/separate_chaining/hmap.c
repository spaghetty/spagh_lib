#include "hmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct _ENTRY {
  ENTRY data;
  struct _ENTRY *nxt;
  struct _ENTRY *prv;
};

int std_hf(char *,unsigned int range);

int hcreate (int __base, hsearch_data *__htab)
{
  int i;
  __htab->size = __base;
  __htab->table = (struct _ENTRY *)malloc(sizeof(struct _ENTRY)*__base);
  for(i = 0; i< __base; i++)
      __htab->table[i].prv = __htab->table[i].nxt = &(__htab->table[i]);
  __htab->hf = std_hf;
  return 1;
};

void hdestroy (hsearch_data *__htab)
{
  printf("strart transhing\n");
  free(__htab->table);
  __htab->size = 0;
}

int hsearch (ENTRY __item, ACTION __action, ENTRY **__retval,
              hsearch_data *__htab)
{
  int idx = -1;
  struct _ENTRY *elm;
  *__retval= NULL;
  if (__htab->size == 0 || __item.key == NULL)
    return -1;
  idx = __htab->hf (__item.key, __htab->size);
  if (idx == -1)
    return -2;
  elm = &(__htab->table[idx]);
  if (__is_clean (elm))  /* placement clean as was just crated */
    {
      if (__action==ENTER)
        {
          elm->data.key = __item.key;
	  elm->data.data = __item.data;
          __make_dirty (elm);
          *__retval = &(elm->data);
        }
      else
        {
          return -3;
        }
    }
  else      /* something in here ... let see */
    {
      while (__conflicted_stuff (elm) && *__retval == NULL)
        {
          if (strcmp (elm->data.key, __item.key) == 0)
            {
              *__retval = &(elm->data);
            }
          else
            {
              if (__action == ENTER && elm->nxt == NULL)
                {
		  elm->nxt = malloc(sizeof(struct _ENTRY));
		  __initialize_follower (elm);
		  elm->nxt->data = __item;
		  *__retval = &(elm->nxt->data);
                }
            }
          elm = elm->nxt;
        }
    }
  return 1;
}

int std_hf(char *key,unsigned int range)
{
  int len = strlen(key); /* just a place holder */
  return len%range;
}
