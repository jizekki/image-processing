#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

void usage(char *s)
{
  fprintf(stderr, "Usage: %s %s", s, "<num> <ims> <imd>\n");
  exit(EXIT_FAILURE);
}

#define PARAM 3
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);

  unsigned short num = atoi(argv[1]);
  char *ims_name = argv[2];
  char *imd_name = argv[3];
  pnm ims = pnm_load(ims_name);
  int cols = pnm_get_width(ims);
  int rows = pnm_get_height(ims);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short *channel = pnm_get_channel(ims, NULL, num);
  for (size_t i = 0; i < 3; i++)
  {
    pnm_set_channel(imd, channel, i);
  }

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims);
  pnm_free(imd);
  free(channel);

  return EXIT_SUCCESS;
}
