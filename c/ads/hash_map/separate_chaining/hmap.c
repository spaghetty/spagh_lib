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
  __htab->table = (struct _ENTRY *)malloc(sizeof(struct _ENTRY));
  for(i = 0; i< __base; i++)
      __htab->table[i].prv = __htab->table[i].nxt = &(__htab->table[i]);
  __htab->hf = std_hf;
  return 1;
};

void hdestroy (hsearch_data *__htab)
{
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
  if (elm->data.key != NULL)
    printf ("%s ----%s\n", __item.key, elm->data.key);
  if (__is_clean(elm))  /* placement clean as was just crated */
    {
      if (__action==ENTER)
        {
          elm->data = __item;
          __make_dirty (elm);
          *__retval = &__item;
        }
      else
        {
          return -3;
        }
    }
  else      /* something in here ... let see */
    {
      printf ("no so clean\n");
      while (__conflicted_stuff (elm) && *__retval == NULL)
        {
          if (strcmp (elm->data.key, __item.key) == 0)
            {
              printf ("i shouldn't be here %s , %s \n", elm->data.key, __item.key);
              *__retval = &(elm->data);
            }
          else
            {
              printf ("i should be here \n");
              if (__action == ENTER)
                {
                  if (elm->nxt == NULL)
                    printf ("i should insert now\n");
                }
            }
          elm = elm->nxt;
        }
    }
  printf("placement: %d\n", idx);
  return 1;
}

int std_hf(char *key,unsigned int range)
{
  int len = strlen(key); /* just a place holder */
  return len%range;
}
