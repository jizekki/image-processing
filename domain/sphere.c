
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <bcl.h>

#include "domain.h"

#define GHO 1.6

pnm sphere(pnm *pims)
{
  size_t cols = pnm_get_width(*pims);
  size_t rows = pnm_get_height(*pims);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  float xc = cols / 2;
  float yc = rows / 2;
  float rmax = cols / 2;

  float x;
  float y;
  float dx;
  float dy;
  float bx;
  float by;
  float r;
  float z;
  for (size_t i = 0; i < rows; i++)
  {
    for (size_t j = 0; j < cols; j++)
    {

      dx = j - xc;
      dy = i - yc;
      r = sqrt(dx * dx + dy * dy);
      z = sqrt(rmax * rmax - r * r);
      bx = (1 - 1 / GHO) * asin(dx / sqrt(dx * dx + z * z));
      by = (1 - 1 / GHO) * asin(dy / sqrt(dy * dy + z * z));
      if (r <= rmax)
      {
        x = j - z * tan(bx);
        y = i - z * tan(by);
        if ((x - xc) * (x - xc) + (y - yc) * (y - yc) < rmax * rmax)
        {
          for (size_t k = 0; k < 3; k++)
          {
            unsigned short val = bilinear_interpolation(x, y, *pims, k);
            pnm_set_component(imd, i, j, k, val);
          }
        }
      }
    }
  }

  return imd;
}

static void usage(char *s)
{
  fprintf(stderr, "%s <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 2
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);

  char *ims_name = argv[1];
  char *imd_name = argv[2];

  pnm ims = pnm_load(ims_name);

  pnm *pims = &ims;

  pnm imd = sphere(pims);

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims);
  pnm_free(imd);

  return EXIT_SUCCESS;
}
