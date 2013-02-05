#include <stdio.h>
#include "geometry.h"
#include "solid.h"

int main()
{
  init_geometry();
  shape_t *s = new_shape();
  add_vertex(s, new_vertex(0, 300, 0));
  add_vertex(s, new_vertex(20, 300, 0));
  add_vertex(s, new_vertex(20, 290, 0));
  add_vertex(s, new_vertex(300, 220, 0));
  add_vertex(s, new_vertex(295, 218, 0));
  add_vertex(s, new_vertex(60, 260, 0));
  add_vertex(s, new_vertex(5, 220, 0));
  add_vertex(s, new_vertex(5, 0, 0));
  add_vertex(s, new_vertex(0, 0, 0));
  shape_t *u = new_revolution(s, 60);
  flushOBJ(stdout);
  free_shape(s);
  free_shape(u);
  finalize_geometry();
  return 0;
}

