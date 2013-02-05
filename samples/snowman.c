#include <stdio.h>
#include "geometry.h"
#include "solid.h"

shape_t *new_umbrella()
{
  shape_t *u = new_shape();
  add_vertex(u, new_vertex(0, 300, 0));
  add_vertex(u, new_vertex(20, 300, 0));
  add_vertex(u, new_vertex(20, 290, 0));
  add_vertex(u, new_vertex(300, 220, 0));
  add_vertex(u, new_vertex(295, 218, 0));
  add_vertex(u, new_vertex(60, 260, 0));
  add_vertex(u, new_vertex(5, 220, 0));
  add_vertex(u, new_vertex(5, 0, 0));
  add_vertex(u, new_vertex(0, 0, 0));
  shape_t *ret = new_revolution(u, 70);
  free_shape(u);
  return ret;
}

int main()
{
  init_geometry();
  // umbrella
  shape_t *u = shape_rotate(
    shape_translate(
      shape_rotate(new_umbrella(), -20, 1, 0, 0)
    , 0, 60, -100),
    45, 0, 1, 0
  );
  const double r_head = 60.0;
  const double r_body = 100.0;
  // snowman
  shape_t *head = shape_translate(new_sphere(r_head, 10, 10), 0, r_body*2+r_head, 0);
  shape_t *body = shape_translate(new_sphere(r_body, 15, 15), 0, r_body, 0);
  shape_t *arm_l = shape_translate(
    shape_rotate(
      shape_translate(new_cylinder(5, 80, 6, 2, 3), 0, 40, 0)
      , -45, 0, 0, 1
    ),
    r_body-20, r_body, 0
  );
  shape_t *arm_r = shape_rotate(
    shape_translate(
      shape_rotate(
        shape_translate(new_cylinder(5, 80, 6, 2, 3), 20, 40, 0)
        , -45, 0, 0, 1
      ),
      r_body-20, r_body, 0
    ),
    180, 0, 1, 0
  );
  shape_t *hat = shape_translate(
    shape_rotate(
      shape_translate(new_cylinder(30, 30, 10, 3, 2), 0, r_head, 0),
      -20, 0, 0, 1
    ), 0, r_body*2+r_head, 0
  );
  shape_t *nose = shape_translate(
    shape_rotate(new_cone(8, 30, 5), 90, 1, 0, 0),
    0, r_body*2+r_head, r_head
  );
  // stage
  shape_t *stage = shape_translate(
    new_cylinder(500, 60, 50, 3, 2),
  0, -30, 0);
  // flush
  flushOBJ(stdout);
  // finalize
  free_shape(u);
  free_shape(head);
  free_shape(body);
  free_shape(hat);
  free_shape(nose);
  free_shape(stage);
  finalize_geometry();
  return 0;
}

