#ifndef SOLID_H_
#define SOLID_H_
#include "geometry.h"

typedef struct {
  double x;
  double y;
} point_t;

typedef struct {
  vertex_id *vertices;
  unsigned num_vertices;
  unsigned capacity_vertices;
} shape_t;

#define INIT_BUFSIZE_SHAPE 1024

shape_t *new_shape();
shape_t *copy_shape(const shape_t *s);
vertex_id add_vertex(shape_t *s, const vertex_id v);
bool free_shape(shape_t *s);

bool make_face(const vertex_id* restrict a, const vertex_id* restrict b,
  const unsigned len);


shape_t *new_revolution(const shape_t *standard, const unsigned m, const unsigned n);
shape_t *new_cylinder(const double r, const double height, const unsigned m);
shape_t *new_taurus(const double offset, const double r,
  const unsigned m, const unsigned n);
shape_t *new_sphere(const double r, const unsigned m, const unsigned n);

bool shape_translate(shape_t *s,
  const double x, const double y, const double z);
bool shape_scale(shape_t *s,
  const double x, const double y, const double z);
bool shape_rotate(shape_t *s, const double degree,
  const double nx, const double ny, const double nz);

#endif

