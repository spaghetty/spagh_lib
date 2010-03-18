#ifndef __MOD_LOAD_H__
#define __MOD_LOAD_H__
#include "lac_module.h"


typedef void* plug_t;

int module_search(char *name,char ***list);
int module_load(char *fname, plug_t* plug);
#endif /* __MOD_LOAD_H__ */
