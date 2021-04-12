#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

void usage(char *s)
{
  fprintf(stderr, "Usage: %s %s", s, "<ims> <imd>\n");
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
  size_t cols = pnm_get_width(ims);
  size_t rows = pnm_get_height(ims);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short total;
  for (size_t k = 0; k < rows; k++)
  {
    for (size_t l = 0; l < cols; l++)
    {
      total = 0;
      for (size_t m = 0; m < 3; m++)
      {
        total += pnm_get_component(ims, k, l, m);
      }
      for (size_t m = 0; m < 3; m++)
      {
        pnm_set_component(imd, k, l, m, ((float)total) / 3);
      }
    }
  }

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims);
  pnm_free(imd);

  return EXIT_SUCCESS;
}
