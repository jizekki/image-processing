#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

void usage(char *s)
{
  fprintf(stderr, "Usage: %s %s", s, "<min> <max> <ims> <imd>\n");
  exit(EXIT_FAILURE);
}

void normalize_channel(unsigned short *channel, size_t size, int min, int max)
{
  int minI = 0;
  int maxI = pnm_maxval;

  for (size_t i = 0; i < size; i++)
  {
    if (channel[i] < minI)
    {
      minI = channel[i];
    }
    if (channel[i] > maxI)
    {
      maxI = channel[i];
    }
  }

  for (size_t i = 0; i < size; i++)
  {
    channel[i] = (float)((float)(max - min) / (float)(maxI - minI)) * channel[i] + ((float)(min * maxI + max * minI) / (float)(maxI - minI));
  }
}

#define PARAM 4
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);

  int min = atoi(argv[1]);
  int max = atoi(argv[2]);
  char *ims_name = argv[3];
  char *imd_name = argv[4];
  pnm ims = pnm_load(ims_name);
  size_t cols = pnm_get_width(ims);
  size_t rows = pnm_get_height(ims);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short *channelr = pnm_get_channel(ims, NULL, 0);
  unsigned short *channelg = pnm_get_channel(ims, NULL, 1);
  unsigned short *channelb = pnm_get_channel(ims, NULL, 2);

  normalize_channel(channelr, cols * rows, min, max);
  normalize_channel(channelg, cols * rows, min, max);
  normalize_channel(channelb, cols * rows, min, max);

  pnm_set_channel(imd, channelr, 0);
  pnm_set_channel(imd, channelg, 1);
  pnm_set_channel(imd, channelb, 2);

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims);
  pnm_free(imd);
  free(channelr);
  free(channelg);
  free(channelb);

  return EXIT_SUCCESS;
}
