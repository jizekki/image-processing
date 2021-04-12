/**
 * @file copy.c
 * @brief resize a given image with an increasing factor by copying existing
 *        pixels. The resize factor is considered as an interger.
 * @author Vinh-Thong Ta <vinh-thong.ta@labri.fr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <pnm.h>

void process(char *ims_name, char *imd_name, int factor)
{
  pnm ims = pnm_load(ims_name);
  int ims_cols = pnm_get_width(ims);
  int ims_rows = pnm_get_height(ims);
  int imd_cols = ims_cols * factor;
  int imd_rows = ims_rows * factor;
  pnm imd = pnm_new(imd_cols, imd_rows, PnmRawPpm);

  for (int i = 0; i < ims_rows; i++)
  {
    for (int j = 0; j < ims_cols; j++)
    {
      for (int l = 0; l < factor; l++)
      {
        for (int m = 0; m < factor; m++)
        {
          for (int k = 0; k < 3; k++)
          {
            pnm_set_component(imd, i * factor + l, j * factor + m, k, pnm_get_component(ims, i, j, k));
          }
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
  fprintf(stderr, "Usage: %s <factor> <ims> <imd> \n", s);
  exit(EXIT_FAILURE);
}

#define param 3
int main(int argc, char *argv[])
{
  if (argc != param + 1)
    usage(argv[0]);

  int factor = atoi(argv[1]);
  char *ims_name = argv[2];
  char *imd_name = argv[3];
  process(ims_name, imd_name, factor);
  return EXIT_SUCCESS;
}
