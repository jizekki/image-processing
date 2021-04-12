#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <bcl.h>

#include "domain.h"

pnm shear_h(pnm *pims, float angle)
{
  float tang = tan(angle * M_PI / 180.0);
  size_t cols = pnm_get_width(*pims);
  size_t rows = pnm_get_height(*pims);
  ssize_t bx = (ssize_t)(rows * tang);
  size_t ncols = cols + labs(bx);
  size_t nrows = rows;
  pnm imd = pnm_new(ncols, nrows, PnmRawPpm);

  float x;
  float y;

  for (size_t i = 0; i < nrows; i++)
  {
    for (size_t j = 0; j < ncols; j++)
    {
      x = j - tang * i;
      y = i;
      if (angle < 0)
      {
        x -= labs(bx);
      }
      if (0 < x && x + 1 < cols && 0 < y && y + 1 < rows)
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

pnm shear_v(pnm *pims, float angle)
{
  float tang = tan(angle * M_PI / 180.0);
  size_t cols = pnm_get_width(*pims);
  size_t rows = pnm_get_height(*pims);
  ssize_t bx = (ssize_t)(cols * tang);
  size_t ncols = cols;
  size_t nrows = rows + labs(bx);
  pnm imd = pnm_new(ncols, nrows, PnmRawPpm);

  float x;
  float y;

  for (size_t i = 0; i < nrows; i++)
  {
    for (size_t j = 0; j < ncols; j++)
    {
      x = j;
      y = i - tang * j;
      if (angle < 0)
      {
        y -= labs(bx);
      }
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
  fprintf(stderr, "%s <dir>{h ,v} <angle> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);

  char *dir = argv[1];
  float angle = atof(argv[2]);
  char *ims_name = argv[3];
  char *imd_name = argv[4];

  pnm ims = pnm_load(ims_name);

  pnm *pims = &ims;
  pnm imd;

  switch (dir[0])
  {
  case 'v':
    imd = shear_v(pims, angle);
    break;
  case 'h':
    imd = shear_h(pims, angle);
    break;

  default:
    exit(-1);
    break;
  }

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims);
  pnm_free(imd);

  return EXIT_SUCCESS;
}
