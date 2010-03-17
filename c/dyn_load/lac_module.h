#ifndef __LAC_MODULE_H__
#define __LAC_MODULE_H__

typedef void (*lac_init_f) ();

#define LAC_INIT_F				\
  void init_f ();				\
  void init_f ()

#endif /* __LAC_MODULE_H__ */
