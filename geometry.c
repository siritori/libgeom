#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "geometry.h"

static vertex_t *vertices = NULL;
static face_t *faces = NULL;
static unsigned capacity_vertices;
static unsigned capacity_faces;
static unsigned num_vertices;
static unsigned num_faces;

void init_geometry()
{
  num_vertices = num_faces = 0;
  capacity_vertices = INIT_BUFSIZE_VERTICES;
  capacity_faces = INIT_BUFSIZE_FACES;
  if((vertices = malloc(capacity_vertices * sizeof(vertex_t))) == NULL) {
    error_printf("out of memory(faces)\n");
    exit(EXIT_FAILURE);
  }
  if((faces = malloc(capacity_faces * sizeof(face_t))) == NULL) {
    error_printf("out of memory(vertices)\n");
    FREE(vertices);
    exit(EXIT_FAILURE);
  }
}

void finalize_geometry()
{
  FREE(vertices);
  FREE(faces);
}

bool is_valid_id(const vertex_id id)
{
  if(id > num_vertices) {
    error_printf("invalid vertex_id %d\n", id);
    return false;
  } else {
    return true;
  }
}

vertex_id new_vertex(const double x, const double y, const double z)
{
  // initialized check
  if(vertices == NULL) {
    error_printf("not initialized\n");
    exit(EXIT_FAILURE);
  }
  // expand vertices buffer
  if(num_vertices >= capacity_vertices) {
    capacity_vertices *= 2;
    vertices = realloc(vertices, capacity_vertices * sizeof(vertex_t));
    if(vertices == NULL) {
      error_printf("failed to expand buffer(vertices)\n");
      FREE(faces);
      exit(EXIT_FAILURE);
    }
  }
  // put on record
  vertices[num_vertices].x = x;
  vertices[num_vertices].y = y;
  vertices[num_vertices].z = z;
  num_vertices++;
  return num_vertices - 1;
}

vertex_id copy_vertex(const vertex_id v)
{
  if(!is_valid_id(v)) return ERROR_VERTEX;
  return new_vertex(vertices[v].x, vertices[v].y, vertices[v].z);
}

void new_face(const vertex_id v1, const vertex_id v2, const vertex_id v3)
{
  // initialized check
  if(faces == NULL) {
    error_printf("not initialized\n");
    exit(EXIT_FAILURE);
  }
  // expand faces buffer
  if(num_faces >= capacity_faces) {
    capacity_faces *= 2;
    faces = realloc(faces, capacity_faces * sizeof(face_t));
    if(faces == NULL) {
      error_printf("failed to expand buffer(faces)\n");
      FREE(vertices);
      exit(EXIT_FAILURE);
    }
  }
  // put on record
  faces[num_faces].v1 = v1;
  faces[num_faces].v2 = v2;
  faces[num_faces].v3 = v3;
  num_faces++;
}

double get_x(const vertex_id v)
{
  return vertices[v].x;
}
double get_y(const vertex_id v)
{
  return vertices[v].y;
}
double get_z(const vertex_id v)
{
  return vertices[v].z;
}
vertex_t get_point(const vertex_id v)
{
  return vertices[v];
}

bool flushOBJ(FILE *fp)
{
  // initialized check
  if(vertices == NULL || faces == NULL) {
    error_printf("not initialized\n");
    exit(EXIT_FAILURE);
  }
  // flush vertices
  for(unsigned i = 0; i < num_vertices; ++i) {
    vertex_t *v = &vertices[i];
    if(fprintf(fp, "v %f %f %f\n", v->x, v->y, v->z) < 0) {
      error_printf("failed to write file\n");
      goto ON_ERROR;
    }
  }
  // flush faces
  for(unsigned i = 0; i < num_faces; ++i) {
    face_t *f = &faces[i];
    if(fprintf(fp, "f %d %d %d\n", f->v1 + 1, f->v2 + 1, f->v3 + 1) < 0) {
      error_printf("failed to write file\n");
      goto ON_ERROR;
    }
  }
  return true;
ON_ERROR:
    FREE(vertices);
    FREE(faces);
    fclose(fp);
    return false;
}

static inline vertex_t *vp(const vertex_id id)
{
  return &vertices[id];
}

bool vertex_translate(const vertex_id *vs, const unsigned len,
  const double x, const double y, const double z)
{
  for(unsigned i = 0; i < len; ++i) {
    if(!is_valid_id(vs[i])) return false;
    vertex_t *v = vp(vs[i]);
    v->x += x;
    v->y += y;
    v->z += z;
  }
  return true;
}

bool vertex_scale(const vertex_id *vs, const unsigned len,
  const double x, const double y, const double z)
{
  for(unsigned i = 0; i < len; ++i) {
    if(!is_valid_id(vs[i])) return false;
    vertex_t *v = vp(vs[i]);
    v->x *= x;
    v->y *= y;
    v->z *= z;
  }
  return true;
}

bool vertex_rotate(const vertex_id *vs,
  const unsigned len, const double degree,
  const double nx, const double ny, const double nz)
{
  const double rad = M_PI_2 * degree / 180.0;
  const double x = nx * sin(rad);
  const double y = ny * sin(rad);
  const double z = nz * sin(rad);
  const double w = cos(rad / 2.0);
  const double m[3][3] = {
    {1-2*(y*y+z*z), 2*(x*y-z*w)  , 2*(x*z+y*w)  },
    {2*(x*y+z*w)  , 1-2*(z*z+x*x), 2*(y*z-x*w)  },
    {2*(x*y-y*w)  , 2*(y*z+x*w)  , 1-2*(x*x+y*y)},
  };
  for(unsigned i = 0; i < len; ++i) {
    if(!is_valid_id(vs[i])) return false;
    vertex_t *v = vp(vs[i]);
    const vertex_t t = {v->x, v->y, v->z};
    v->x = t.x * m[0][0] + t.y * m[0][1] + t.z * m[0][2];
    v->y = t.x * m[1][0] + t.y * m[1][1] + t.z * m[1][2];
    v->z = t.x * m[2][0] + t.y * m[2][1] + t.z * m[2][2];
  }
  return true;
}

