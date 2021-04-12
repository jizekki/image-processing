/**
 * @file test-fft.c
 * @brief test the behaviors of functions in fft module
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fft.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_NAME_SIZE 40
#define FREQUENCY_VALUE 8
#define AMPLITULE_FACTOR 0.25

/**
 * @brief fills the grey-scaled image 'imd' with the values of 'values'
 * @param pnm imd, the input image to fill
 * @param unsigned short *values, the values to fill the image with
*/
void set_image(pnm imd, unsigned short *values)
{
  int im_cols = pnm_get_width(imd);
  int im_rows = pnm_get_height(imd);
  for (int i = 0; i < im_rows; i++)
  {
    for (int j = 0; j < im_cols; j++)
    {
      for (int k = 0; k < 3; k++)
      {
        pnm_set_component(imd, i, j, k, values[i * im_cols + j]);
      }
    }
  }
}

/**
 * @brief returns the max value of an array 'd' of size 'size'
 * @param float* d, the input array
 * @param int size, the length of the array d 
*/
float max_value(float *d, int size)
{
  float max = d[0];
  for (int i = 1; i < size; i++)
  {
    if (d[i] > max)
    {
      max = d[i];
    }
  }
  return (max == 0) ? 0.5 : max;
}

/**
 * @brief normalizes the values of d by bringing them to the interval [0,255]
 * @param float* d, the input array
 * @param int size, the length of the array d 
*/
unsigned short *bring2rgb(float *d, int size)
{
#define P 0.1
  float max = max_value(d, size);
  unsigned short *new_d = malloc(sizeof(unsigned short) * size);
  for (int j = 0; j < size; j++)
  {
    new_d[j] = 255 * powf(d[j] / max, P);
  }
  return new_d;
}

/**
 * @brief test the forward and backward functions
 * @param char* name, the input image file name
 */
void test_forward_backward(char *name)
{
  fprintf(stderr, "test_forward_backward: ");
  pnm im = pnm_load(name);
  int im_cols = pnm_get_width(im);
  int im_rows = pnm_get_height(im);
  pnm imd = pnm_new(im_cols, im_rows, PnmRawPpm);
  unsigned short *im_buffer = pnm_get_channel(im, NULL, PnmRed);
  fftw_complex *fwd = forward(im_rows, im_cols, im_buffer);
  unsigned short *bwd = backward(im_rows, im_cols, fwd);
  set_image(imd, bwd);

  char imd_name[MAX_NAME_SIZE] = "FB-";
  char *ims_name = strrchr(name, '/');
  strcat(imd_name, ims_name + 1);
  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(im);
  pnm_free(imd);
  fprintf(stderr, "OK\n");
}

/**
 * @brief test image reconstruction from of magnitude and phase spectrum
 * @param char *name: the input image file name
 */
void test_reconstruction(char *name)
{
  fprintf(stderr, "test_reconstruction: ");
  pnm im = pnm_load(name);
  int im_cols = pnm_get_width(im);
  int im_rows = pnm_get_height(im);
  pnm imd = pnm_new(im_cols, im_rows, PnmRawPpm);
  unsigned short *im_buffer = pnm_get_channel(im, NULL, PnmRed);

  fftw_complex *fwd = forward(im_rows, im_cols, im_buffer);

  float *ps = (float *)malloc(sizeof(float) * im_rows * im_cols);
  float *as = (float *)malloc(sizeof(float) * im_rows * im_cols);

  freq2spectra(im_rows, im_cols, fwd, as, ps);
  spectra2freq(im_rows, im_cols, as, ps, fwd);

  unsigned short *bwd = backward(im_rows, im_cols, fwd);
  set_image(imd, bwd);

  char imd_name[MAX_NAME_SIZE] = "FB-ASPS-";
  char *ims_name = strrchr(name, '/');
  strcat(imd_name, ims_name + 1);
  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(im);
  pnm_free(imd);
  free(ps);
  free(as);
  fprintf(stderr, "OK\n");
}

/**
 * @brief test construction of magnitude and phase images in ppm files
 * @param char* name, the input image file name
 */
void test_display(char *name)
{
  fprintf(stderr, "test_display: ");
  pnm im = pnm_load(name);
  int im_cols = pnm_get_width(im);
  int im_rows = pnm_get_height(im);
  int size = im_rows * im_cols;
  pnm imd_a = pnm_new(im_cols, im_rows, PnmRawPpm);
  pnm imd_p = pnm_new(im_cols, im_rows, PnmRawPpm);
  unsigned short *im_buffer = pnm_get_channel(im, NULL, PnmRed);
  for (int i = 0; i < size; i++)
  {
    //im_buffer[i] *= powf(-1, i);
  }

  fftw_complex *fwd = forward(im_rows, im_cols, im_buffer);

  float *ps = (float *)malloc(sizeof(float) * size);
  float *as = (float *)malloc(sizeof(float) * size);

  freq2spectra(im_rows, im_cols, fwd, as, ps);
  unsigned short *us_as = bring2rgb(as, size);
  unsigned short *us_ps = bring2rgb(ps, size);
  set_image(imd_a, us_as);
  set_image(imd_p, us_ps);

  char imd_a_name[MAX_NAME_SIZE] = "AS-";
  char imd_p_name[MAX_NAME_SIZE] = "PS-";
  char *ims_name = strrchr(name, '/');
  strcat(imd_a_name, ims_name + 1);
  strcat(imd_p_name, ims_name + 1);
  pnm_save(imd_a, PnmRawPpm, imd_a_name);
  pnm_save(imd_p, PnmRawPpm, imd_p_name);
  pnm_free(im);
  pnm_free(imd_a);
  pnm_free(imd_p);
  free(ps);
  free(as);
  free(us_ps);
  free(us_as);
  fprintf(stderr, "OK\n");
}

/**
 * @brief test the modification of magnitude by adding a periodic functions
          on both vertical and horizontal axis, and 
 *        construct output images
 * @param char* name, the input image file name
 */
void test_add_frequencies(char *name)
{
  fprintf(stderr, "test_add_frequencies: ");
  pnm im = pnm_load(name);
  int im_cols = pnm_get_width(im);
  int im_rows = pnm_get_height(im);
  int size = im_rows * im_cols;
  pnm imd_freq = pnm_new(im_cols, im_rows, PnmRawPpm);
  pnm imd_fas = pnm_new(im_cols, im_rows, PnmRawPpm);
  unsigned short *im_buffer = pnm_get_channel(im, NULL, PnmRed);
  float *ps = (float *)malloc(sizeof(float) * size);
  float *as = (float *)malloc(sizeof(float) * size);

  fftw_complex *fwd = forward(im_rows, im_cols, im_buffer);

  freq2spectra(im_rows, im_cols, fwd, as, ps);
  //float max = max_value(as, size);

  float max = max_value(as, size);
  int im_cols2 = im_cols / 2;
  int im_rows2 = im_rows / 2;
  as[im_cols * (im_rows2 - FREQUENCY_VALUE + 1) - im_cols2] = AMPLITULE_FACTOR * max;
  as[im_cols * (im_rows2 + FREQUENCY_VALUE + 1) - im_cols2] = AMPLITULE_FACTOR * max;
  as[im_cols * (im_rows2 + 1) - im_cols2 - FREQUENCY_VALUE] = AMPLITULE_FACTOR * max;
  as[im_cols * (im_rows2 + 1) - im_cols2 + FREQUENCY_VALUE] = AMPLITULE_FACTOR * max;

  spectra2freq(im_rows, im_cols, as, ps, fwd);
  unsigned short *us_as = bring2rgb(as, im_cols * im_rows);
  unsigned short *bwd = backward(im_rows, im_cols, fwd);
  set_image(imd_freq, bwd);
  set_image(imd_fas, us_as);

  char imd_freq_name[MAX_NAME_SIZE] = "FREQ-";
  char imd_fas_name[MAX_NAME_SIZE] = "FAS-";
  char *ims_name = strrchr(name, '/');
  strcat(imd_freq_name, ims_name + 1);
  strcat(imd_fas_name, ims_name + 1);
  pnm_save(imd_freq, PnmRawPpm, imd_freq_name);
  pnm_save(imd_fas, PnmRawPpm, imd_fas_name);
  pnm_free(im);
  pnm_free(imd_freq);
  pnm_free(imd_fas);
  free(ps);
  free(as);
  free(us_as);
  fprintf(stderr, "OK\n");
}

void run(char *name)
{
  test_forward_backward(name);
  test_reconstruction(name);
  test_display(name);
  test_add_frequencies(name);
}

void usage(const char *s)
{
  fprintf(stderr, "Usage: %s <ims> \n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 1
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);
  run(argv[1]);
  return EXIT_SUCCESS;
}
