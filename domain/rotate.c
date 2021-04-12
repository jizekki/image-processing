#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <bcl.h>

#include "domain.h"

pnm rotate(pnm *pims, float angle, int x0, int y0)
{
  size_t cols = pnm_get_width(*pims);
  size_t rows = pnm_get_height(*pims);
  float costheta = cos(M_PI * angle / 180);
  float sintheta = sin(M_PI * angle / 180);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  float x;
  float y;
  for (size_t i = 0; i < rows; i++)
  {
    for (size_t j = 0; j < cols; j++)
    {
      x = (float)x0 + ((float)j - x0) * costheta - ((float)i - y0) * sintheta;
      y = (float)y0 + ((float)j - x0) * sintheta + ((float)i - y0) * costheta;

      if (0 <= x && x + 1 < cols && 0 <= y && y + 1 < rows)
      {
        for (size_t k = 0; k < 3; k++)
        {
          unsigned short val = bilinear_interpolation(x, y, *pims, k);
          pnm_set_component(imd, i, j, k, val);
        }
      }
    }
  }

  return imd;
}

static void usage(char *s)
{
  fprintf(stderr, "%s <x> <y> <angle> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 5
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);

  int x0 = atoi(argv[1]);
  int y0 = atoi(argv[2]);
  float angle = atof(argv[3]);
  char *ims_name = argv[4];
  char *imd_name = argv[5];

  pnm ims = pnm_load(ims_name);

  pnm *pims = &ims;

  pnm imd = rotate(pims, angle, x0, y0);

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims);
  pnm_free(imd);

  return EXIT_SUCCESS;
}
