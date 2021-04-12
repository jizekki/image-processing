#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

void usage(char *s)
{
  fprintf(stderr, "Usage: %s %s", s, "<ims0> <ims1> <ims2> <imd>\n");
  exit(EXIT_FAILURE);
}

#define PARAM 4
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);

  char *ims0_name = argv[1];
  char *ims1_name = argv[2];
  char *ims2_name = argv[3];
  char *imd_name = argv[4];
  pnm ims0 = pnm_load(ims0_name);
  pnm ims1 = pnm_load(ims1_name);
  pnm ims2 = pnm_load(ims2_name);
  int cols = pnm_get_width(ims0);
  int rows = pnm_get_height(ims1);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short *channel;
  channel = pnm_get_channel(ims0, NULL, 0);
  pnm_set_channel(imd, channel, 0);
  channel = pnm_get_channel(ims1, NULL, 1);
  pnm_set_channel(imd, channel, 1);
  channel = pnm_get_channel(ims2, NULL, 2);
  pnm_set_channel(imd, channel, 2);

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims0);
  pnm_free(ims1);
  pnm_free(ims2);
  pnm_free(imd);
  free(channel);

  return EXIT_SUCCESS;
}
