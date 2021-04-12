#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>
#include <string.h>

pnm flip_h(pnm *pims)
{
  size_t cols = pnm_get_width(*pims);
  size_t rows = pnm_get_height(*pims);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);
  for (size_t i = 0; i < rows; i++)
  {
    for (size_t j = 0; j < cols; j++)
    {
      for (size_t k = 0; k < 3; k++)
      {
        pnm_set_component(imd, i, j, k, pnm_get_component(*pims, i, cols - j - 1, k));
      }
    }
  }
  return imd;
}

pnm flip_v(pnm *pims)
{
  size_t cols = pnm_get_width(*pims);
  size_t rows = pnm_get_height(*pims);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);
  for (size_t i = 0; i < rows; i++)
  {
    for (size_t j = 0; j < cols; j++)
    {
      for (size_t k = 0; k < 3; k++)
      {
        pnm_set_component(imd, i, j, k, pnm_get_component(*pims, rows - i - 1, j, k));
      }
    }
  }
  pnm_free(*pims);
  return imd;
}

pnm flip_t(pnm *pims)
{
  size_t cols = pnm_get_width(*pims);
  size_t rows = pnm_get_height(*pims);
  pnm imd = pnm_new(rows, cols, PnmRawPpm);
  for (size_t i = 0; i < rows; i++)
  {
    for (size_t j = 0; j < cols; j++)
    {
      for (size_t k = 0; k < 3; k++)
      {
        pnm_set_component(imd, j, i, k, pnm_get_component(*pims, i, j, k));
      }
    }
  }
  return imd;
}

void usage(char *s)
{
  fprintf(stderr, "%s <dir>{h,v,t}* <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 3
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);

  char *dirs = argv[1];
  char *ims_name = argv[2];
  char *imd_name = argv[3];
  size_t dirs_len = strlen(dirs);
  pnm ims = pnm_load(ims_name);

  pnm *pims = &ims;
  pnm imd;

  for (size_t i = 0; i < dirs_len; i++)
  {
    switch (dirs[i])
    {
    case 'h':
      imd = flip_h(pims);
      break;
    case 'v':
      imd = flip_v(pims);
      break;
    case 't':
      imd = flip_t(pims);
      break;

    default:
      break;
    }
    pims = &imd;
  }
  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims);
  pnm_free(imd);

  return EXIT_SUCCESS;
}
