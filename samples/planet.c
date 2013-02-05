#include <stdio.h>
#include "geometry.h"
#include "solid.h"

int main()
{
  init_geometry();
  shape_t *planet = new_sphere(50, 20, 20);
  shape_t *sp1 = new_sphere(10, 16, 16);
  shape_t *sp2 = new_sphere(10, 16, 16);
  shape_t *t1 = new_taurus(30, 4, 50, 10);
  shape_t *t2 = new_taurus(30, 4, 40, 10);
  shape_translate(sp1, 80, 70, 40);
  shape_translate(sp2, -90, 10, -60);
  shape_scale(t1, 5, 1.2, 5);
  shape_scale(t2, 3.5, 1.2, 3.5);
  shape_rotate(t1, -20, 0, 0, 1);
  shape_rotate(t2, 20, 0, 0, 1);
  flushOBJ(stdout);
  free_shape(planet);
  free_shape(t1);
  free_shape(t2);
  finalize_geometry();
  return 0;
}

