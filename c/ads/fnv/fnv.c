#include "fnv.h"
#include <string.h>
#include <sys/types.h>
#include <stdio.h>

unsigned int fnv_32(char *str, unsigned int max)
{
  u_int32_t hash,
    offset_basis = 2166136261,
    fnv_prime = 16777619,
    length = strlen(str),
    i;
  
  hash = offset_basis;
  for(i=0; i<length; i++)
    {
      hash = hash * fnv_prime;
      hash = hash ^ str[i];
    }
  return hash%max;
}

