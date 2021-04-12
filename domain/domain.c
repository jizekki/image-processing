#include <math.h>

#include <domain.h>

#include <stdlib.h>
#include <stdio.h>

unsigned short
bilinear_interpolation(float x, float y, pnm ims, int c)
{
  int i = (int)y;
  int j = (int)x;
  //fprintf(stderr, "INTERP : i = %d, j = %d\n", i, j);

  float dx = x - (float)j;
  float dy = y - (float)i;
  unsigned short val = (1 - dx) * (1 - dy) * pnm_get_component(ims, i, j, c);
  // fprintf(stderr, "done 1\n");
  val += dx * (1 - dy) * pnm_get_component(ims, i, j + 1, c);
  // fprintf(stderr, "done 2\n");
  val += dy * (1 - dx) * pnm_get_component(ims, i + 1, j, c);
  // fprintf(stderr, "done 3\n");
  val += dx * dy * pnm_get_component(ims, i + 1, j + 1, c);
  // fprintf(stderr, "done 4\n");
  return val;
}
