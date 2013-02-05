#include <stdio.h>
#include "geometry.h"
#include "solid.h"

int main()
{
  init_geometry();
  shape_t *s = new_taurus(30, 15, 30, 30);
  flushOBJ(stdout);
  free_shape(s);
  finalize_geometry();
  return 0;
}

