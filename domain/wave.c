
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <bcl.h>

#include "domain.h"

pnm wave(pnm *pims, float tx, float ax, float ty, float ay)
{
  size_t cols = pnm_get_width(*pims);
  size_t rows = pnm_get_height(*pims);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  float x;
  float y;
  for (size_t i = 0; i < rows; i++)
  {
    for (size_t j = 0; j < cols; j++)
    {
      x = (float)j + ax * sin(2 * M_PI * i / tx);
      y = (float)i + ay * sin(2 * M_PI * j / ty);

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
  fprintf(stderr, "%s <tx> <ax> <ty> <ay> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 6
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);

  float tx = atof(argv[1]);
  float ax = atof(argv[2]);
  float ty = atof(argv[3]);
  float ay = atof(argv[4]);
  char *ims_name = argv[5];
  char *imd_name = argv[6];

  pnm ims = pnm_load(ims_name);

  pnm *pims = &ims;

  pnm imd = wave(pims, tx, ax, ty, ay);

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims);
  pnm_free(imd);

  return EXIT_SUCCESS;
}
