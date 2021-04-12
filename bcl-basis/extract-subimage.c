#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

void usage(char *s)
{
  fprintf(stderr, "Usage: %s %s", s, "<i> <j> <rows> <cols> <ims> <imd>\n");
  exit(EXIT_FAILURE);
}

#define PARAM 6
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);

  int i = atoi(argv[1]);
  int j = atoi(argv[2]);
  size_t rows = atoi(argv[3]);
  size_t cols = atoi(argv[4]);
  char *ims_name = argv[5];
  char *imd_name = argv[6];
  pnm ims = pnm_load(ims_name);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  for (size_t k = 0; k < rows; k++)
  {
    for (size_t l = 0; l < cols; l++)
    {
      for (size_t m = 0; m < 3; m++)
      {
        const unsigned short val = pnm_get_component(ims, k + i, l + j, m);
        pnm_set_component(imd, k, l, m, val);
      }
    }
  }

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims);
  pnm_free(imd);

  return EXIT_SUCCESS;
}
