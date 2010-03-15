#ifndef __HMAP_H__
#define __HMAP_H__

/* Action which shall be performed in the call the hsearch.  */

#define __is_clean(f)                           \
  (f->nxt == f->prv &&                          \
   f == f->nxt)
#define __make_dirty(f)                         \
  f->prv = NULL;                                \
  f->nxt = NULL;

#define __conflicted_stuff(f)                   \
  (f!=NULL)

typedef enum
  {
    FIND,
    ENTER
  } ACTION;

typedef struct entry
{
  char *key;
  void *data;
} ENTRY;

/* Opaque type for internal use.  */
struct _ENTRY;

typedef struct hsearch_data
{
  struct _ENTRY *table;
  unsigned int size;
  int (*hf) (char*, unsigned int);
} hsearch_data;


extern int hsearch (ENTRY __item, ACTION __action, ENTRY **__retval,
                     hsearch_data *__htab);
/* extern int hremove(ENTRY __item, hsearch_data *__htab); */
extern int hcreate (int __base, hsearch_data *__htab);
extern void hdestroy (hsearch_data *__htab);

#endif /* __HMAP_H__ */
