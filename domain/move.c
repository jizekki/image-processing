#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>

void process(int dx, int dy, char *ims_name, char *imd_name)
{
  pnm ims = pnm_load(ims_name);
  size_t cols = pnm_get_width(ims);
  size_t rows = pnm_get_height(ims);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  for (size_t k = 0; k < rows; k++)
  {
    for (size_t l = 0; l < cols; l++)
    {
      for (size_t m = 0; m < 3; m++)
      {
        const unsigned short val = pnm_get_component(ims, k, l, m);
        if (k + dy > 0 && l + dx > 0 && k + dy < rows && l + dx < cols)
        {
          pnm_set_component(imd, k + dy, l + dx, m, val);
        }
      }
    }
  }

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims);
  pnm_free(imd);
}

void usage(char *s)
{
  fprintf(stderr, "%s <dx> <dy> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);

  int dx = atoi(argv[1]);
  int dy = atoi(argv[2]);
  char *ims_name = argv[3];
  char *imd_name = argv[4];

  process(dx, dy, ims_name, imd_name);

  return EXIT_SUCCESS;
}
