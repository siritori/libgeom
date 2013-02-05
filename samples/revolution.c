#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "geometry.h"
#include "solid.h"

#define INITIAL_CAPACITY_POINTS 100
#define DIVISION_NUMBER 60
#define WINDOW_HEIGHT 600

static point_t *pt = NULL;
static unsigned numpoints = 0;
static unsigned capacity = INITIAL_CAPACITY_POINTS;
static FILE *file_out = NULL;

void push_point(const double x, const double y)
{
  if(numpoints >= capacity) {
    capacity *= 2;
    if((pt = realloc(pt, capacity * sizeof(point_t))) == NULL) {
      error_printf("out of memory\n");
      exit(EXIT_FAILURE);
    }
  }
  pt[numpoints].x = x;
  pt[numpoints].y = y;
  numpoints++;
}

void discard_top()
{
  if(numpoints == 0) return;
  numpoints--;
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  // Draw points
  glLineWidth(3.0);
  glBegin(GL_POINTS);
    for(unsigned i = 0; i < numpoints; ++i) glVertex2d(pt[i].x, pt[i].y);
  glEnd();
  // Draw lines
  glLineWidth(1.0);
  glBegin(GL_LINE_STRIP);
    for(unsigned i = 0; i < numpoints; ++i) glVertex2d(pt[i].x, pt[i].y);
  glEnd();
  glFlush();
}

void handle_mouse(const int btn, const int state, const int x, const int y)
{
  if(state != GLUT_DOWN) return;
  switch(btn) {
  case GLUT_LEFT_BUTTON:
    push_point(x, y);
    break;
  case GLUT_RIGHT_BUTTON:
    discard_top();
    break;
  default: break;
  }
  glutPostRedisplay();
}

void to_revolution()
{
  init_geometry();
  shape_t *s = new_shape();
  for(unsigned i = 0; i < numpoints; ++i) {
    add_vertex(s, new_vertex(pt[i].x, WINDOW_HEIGHT-pt[i].y, 0));
  }
  new_revolution(s, DIVISION_NUMBER);
  flushOBJ(file_out);
  finalize_geometry();
}

void handle_keyboard(const unsigned char key, const int x, const int y)
{
#define KEY_ESC   27
#define KEY_ENTER 13
  switch (key) {
  case KEY_ESC:
  case KEY_ENTER:
    to_revolution();
    exit(EXIT_SUCCESS);
  default:
    break;
  }
}

void reshape(const int w, const int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (float)w, (float)h, 0.0, -10.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[])
{
  if(argc < 2) {
    error_printf("missing file argument\n");
    fprintf(stderr, "usage : %s output_filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  if((pt = malloc(capacity * sizeof(point_t))) == NULL) {
    error_printf("out of memory\n");
    exit(EXIT_FAILURE);
  }
  if((file_out = fopen(argv[1], "w")) == NULL) {
    error_printf("failed to open %s for write\n");
    FREE(pt);
    exit(EXIT_FAILURE);
  }
  glutInitWindowSize(600, WINDOW_HEIGHT);
  glutInitWindowPosition(10, 10);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutCreateWindow(argv[0]);
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glColor3f(0.0, 0.0, 0.0);
  glPointSize(7.0);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(handle_mouse);
  glutKeyboardFunc(handle_keyboard);
  glutMainLoop();
  return 0;
}

