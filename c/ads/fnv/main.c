#include "fnv.h"
#include <stdio.h>

int main(int argc, char *argv)
{
  printf("%u,\n",fnv_32("pluto",50));
  printf("%u,\n",fnv_32("pippo",50));
  printf("%u,\n",fnv_32("paper",50));
  printf("%u,\n",fnv_32("paper",50));
  printf("%u,\n",fnv_32("ploto",50));
  printf("%u,\n",fnv_32("plito",50));
  return 1;
}
