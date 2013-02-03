#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "geometry.h"
#include "solid.h"

shape_t *new_shape()
{
  shape_t *s = malloc(sizeof(shape_t));
  if(s == NULL) {
    error_printf("out of memory\n");
    exit(EXIT_FAILURE);
  }
  s->num_vertices = 0;
  s->capacity_vertices = INIT_BUFSIZE_SHAPE;
  if((s->vertices = malloc(s->capacity_vertices * sizeof(vertex_id))) == NULL) {
    error_printf("out of memory\n");
    FREE(s);
    exit(EXIT_FAILURE);
  }
  return s;
}

shape_t *copy_shape(const shape_t *s)
{
  shape_t *s2 = new_shape();
  for(unsigned i = 0; i < s->num_vertices; ++i) {
    add_vertex(s2, copy_vertex(s->vertices[i]));
  }
  return s2;
}

vertex_id add_vertex(shape_t *s, const vertex_id v)
{
  if(!is_valid_id(v)) return false;
  if(s->num_vertices >= s->capacity_vertices) {
    s->capacity_vertices *= 2;
    s->vertices = realloc(s->vertices, s->capacity_vertices * sizeof(vertex_id));
    if(s->vertices == NULL) {
      error_printf("out of memory\n");
      FREE(s);
      exit(EXIT_FAILURE);
    }
  }
  s->vertices[s->num_vertices] = v;
  s->num_vertices++;
  return v;
}

bool free_shape(shape_t *s)
{
  if(s == NULL) {
    error_printf("already released\n");
    return false;
  }
  FREE(s->vertices);
  FREE(s);
  return true;
}

bool make_face(const vertex_id* restrict a, const vertex_id* restrict b,
  const unsigned len)
{
  for(unsigned i = 0; i < len-1; ++i) {
    new_face(a[i], a[i+1], b[i]);
    new_face(b[i], a[i+1], b[i+1]);
  }
  return true;
}

shape_t *new_revolution(const shape_t *standard, const unsigned m, const unsigned n)
{
  shape_t *ret = new_shape();
  shape_t *prev = new_shape();
  for(unsigned i = 0; i < n+1; ++i) {
    const vertex_id v = standard->vertices[i];
    add_vertex(prev, v);
    add_vertex(ret, v);
  }
  for(unsigned rot = 0; rot < m; ++rot) {
    shape_t *current = new_shape();
    const double theta = rot * (2 * M_PI / m);
    for(unsigned i = 0; i < n+1; ++i) {
      const vertex_t vs = get_point(standard->vertices[i]);
      const double x = vs.x * cos(theta);
      const double z = vs.x * sin(theta);
      const vertex_id v = new_vertex(x, vs.y, z);
      add_vertex(current, v);
      add_vertex(ret, v);
    }
    make_face(prev->vertices, current->vertices, n+1);
    free_shape(prev);
    prev = current;
  }
  make_face(prev->vertices, standard->vertices, n+1);
  free_shape(prev);
  return ret;
}

shape_t *new_taurus(const double offset, const double r,
  const unsigned m, const unsigned n)
{
  shape_t *standard = new_shape();
  for(unsigned i = 0; i < n+1; ++i) {
    const double theta = i * (2*M_PI/n);
    const vertex_id v = new_vertex(
      r*cos(theta)+offset,
      r*sin(theta),
      0
    );
    add_vertex(standard, v);
  }
  shape_t *ret = new_revolution(standard, m, n);
  free_shape(standard);
  return ret;
}

shape_t *new_sphere(const double r, const unsigned m, const unsigned n)
{
  shape_t *standard = new_shape();
  for(unsigned i = 0; i < n+1; ++i) {
    const double theta = i * (M_PI / n) - M_PI / 2;
    const vertex_id v = new_vertex(r * cos(theta), r * sin(theta), 0);
    add_vertex(standard, v);
  }
  shape_t *ret = new_revolution(standard, m, n);
  free_shape(standard);
  return ret;
}

bool shape_translate(shape_t *s,
  const double x, const double y, const double z)
{
  return vertex_translate(s->vertices, s->num_vertices, x, y, z);
}

bool shape_scale(shape_t *s,
  const double x, const double y, const double z)
{
  return vertex_scale(s->vertices, s->num_vertices, x, y, z);
}

bool shape_rotate(shape_t *s, const double degree,
  const double x, const double y, const double z)
{
  return vertex_rotate(s->vertices, s->num_vertices, degree, x, y, z);
}
