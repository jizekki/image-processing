#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pnm.h>

float filter_box(float x)
{
  if (x >= -0.5 && x < 0.5)
  {
    return 1;
  }
  return 0;
}

float filter_tent(float x)
{
  if (x >= -1 && x <= 1)
  {
    return 1 - fabsf(x);
  }
  return 0;
}

float filter_bell(float x)
{
  float abs_x = fabsf(x);
  if (abs_x <= 0.5)
  {
    return -(x * x) + 0.75;
  }
  else if (abs_x > 0.5 && abs_x <= 1.5)
  {
    return 0.5 * (abs_x - 1.5) * (abs_x - 1.5);
  }

  return 0;
}

float filter_mitchell_netravali(float x)
{
  float abs_x = fabsf(x);
  if (x > -1 && x < 1)
  {
    return 1.16667 * (abs_x * abs_x * abs_x) - 2 * x * x + 0.88889;
  }
  else if ((x > -2 && x <= -1) || (x >= 1 && x < 2))
  {
    return -0.38889 * (abs_x * abs_x * abs_x) + 2 * x * x - 3.33333 * abs_x + 1.77778;
  }

  return 0;
}

void interpolation(pnm ims, pnm imd, int factor, float (*filter)(float), float wf, int orientation, int channel)
{

  int imd_cols = pnm_get_width(imd);
  int imd_rows = pnm_get_height(imd);
  int max_index = orientation ? pnm_get_height(ims) : pnm_get_width(ims);

  float new_j, pixel, val;
  int left, right;
  for (int i = 0; i < imd_rows; i++)
  {
    for (int j = 0; j < imd_cols; j++)
    {
      new_j = orientation ? (float)i / factor : (float)j / factor;

      left = floor(new_j - wf);
      right = floor(new_j + wf);
      val = 0;

      for (int k = left; k <= right; k++)
      {

        if (k < 0)
        {
          pixel = orientation ? pnm_get_component(ims, 0, j, channel) : pnm_get_component(ims, i, 0, channel);
        }
        else if (k >= max_index)
        {
          pixel = orientation ? pnm_get_component(ims, max_index - 1, j, channel) : pnm_get_component(ims, i, max_index - 1, channel);
        }
        else
        {
          pixel = orientation ? pnm_get_component(ims, k, j, channel) : pnm_get_component(ims, i, k, channel);
        }
        val += pixel * filter((float)k - new_j);
      }

      pnm_set_component(imd, i, j, channel, (unsigned short)val);
    }
  }
}

void process(char *ims_name, char *imd_name, int factor, float (*filter)(float), float wf)
{
  pnm ims = pnm_load(ims_name);
  int ims_cols = pnm_get_width(ims);
  int ims_rows = pnm_get_height(ims);
  int imd_cols = ims_cols * factor;
  int imd_rows = ims_rows * factor;
  pnm imt = pnm_new(imd_cols, ims_rows, PnmRawPpm);
  pnm imd = pnm_new(imd_cols, imd_rows, PnmRawPpm);

  for (int c = 0; c < 3; c++)
  {
    interpolation(ims, imt, factor, filter, wf, 0, c);
    interpolation(imt, imd, factor, filter, wf, 1, c);
  }

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims);
  pnm_free(imt);
  pnm_free(imd);
}

void usage(char *s)
{
  fprintf(stderr, "Usage: %s <factor> <filter> <ims> <imd> \n", s);
  exit(EXIT_FAILURE);
}

#define param 4
int main(int argc, char *argv[])
{
  if (argc != param + 1)
    usage(argv[0]);

  int factor = atoi(argv[1]);
  char *filter_name = argv[2];
  char *ims_name = argv[3];
  char *imd_name = argv[4];
  float (*filter)(float) = NULL;
  float wf = 0;
  if (strcmp(filter_name, "box") == 0)
  {
    filter = filter_box;
    wf = 0.5;
  }
  else if (strcmp(filter_name, "tent") == 0)
  {
    filter = filter_tent;
    wf = 1;
  }
  else if (strcmp(filter_name, "bell") == 0)
  {
    filter = filter_bell;
    wf = 1.5;
  }
  else if (strcmp(filter_name, "mitch") == 0)
  {
    filter = filter_mitchell_netravali;
    wf = 2;
  }
  else
  {
    usage(argv[0]);
  }

  process(ims_name, imd_name, factor, filter, wf);
  return EXIT_SUCCESS;
}
