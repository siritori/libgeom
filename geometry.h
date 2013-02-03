#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#define INIT_BUFSIZE_VERTICES 1024
#define INIT_BUFSIZE_FACES    1024
#define ERROR_VERTEX UINT_MAX

// Utility
#define FREE(P) do { if(P) free(P); P = NULL; } while(0)
#define error_printf(...) do {\
  fprintf(stderr, "ERROR(%s) : ", __func__);\
  fprintf(stderr, __VA_ARGS__);\
} while(0)

typedef struct {
  double x;
  double y;
  double z;
} vertex_t;

typedef unsigned vertex_id;

typedef struct {
  vertex_id v1;
  vertex_id v2;
  vertex_id v3;
} face_t;

void init_geometry();
void finalize_geometry();
bool is_valid_id(const vertex_id id);

// Constructors
vertex_id new_vertex(const double x, const double y, const double z);
vertex_id copy_vertex(const vertex_id v);
void new_face(const vertex_id v1, const vertex_id v2, const vertex_id v3);

// Accessor
double get_x(const vertex_id v);
double get_y(const vertex_id v);
double get_z(const vertex_id v);
vertex_t get_point(const vertex_id v);

// Affine transformations
bool vertex_translate(const vertex_id *vs, const unsigned len,
  const double x, const double y, const double z);
bool vertex_scale(const vertex_id *vs, const unsigned len,
  const double x, const double y, const double z);
bool vertex_rotate(const vertex_id *vs,
  const unsigned len, const double degree,
  const double nx, const double ny, const double nz);

// Flush as Wavefront OBJ file format
bool flushOBJ(FILE *fp);

#endif

