#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <fft.h>
#include <assert.h>

fftw_complex *
forward(int rows, int cols, unsigned short *g_img)
{
  int size = rows * cols;
  fftw_complex *im_complex_in = (fftw_complex *)fftw_malloc(size * sizeof(fftw_complex));
  fftw_complex *im_complex_out = (fftw_complex *)fftw_malloc(size * sizeof(fftw_complex));
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      im_complex_in[i * cols + j] = (i + j) % 2 == 0 ? g_img[i * cols + j] + I * 0 : -g_img[i * cols + j] + I * 0;
    }
  }
  fftw_plan plan = fftw_plan_dft_2d(rows, cols, im_complex_in, im_complex_out, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(plan);
  fftw_destroy_plan(plan);
  fftw_cleanup();
  fftw_free(im_complex_in);
  return im_complex_out;
}

unsigned short *
backward(int normalisation, int rows, int cols, fftw_complex *freq_repr)
{
  int size = rows * cols;
  fftw_complex *im_complex_out = fftw_malloc(size * sizeof(fftw_complex));
  unsigned short *im_out = malloc(size * sizeof(unsigned short));
  fftw_plan plan = fftw_plan_dft_2d(rows, cols, freq_repr, im_complex_out, FFTW_BACKWARD, FFTW_ESTIMATE);
  fftw_execute(plan);
  for (int i = 0; i < size; i++)
  {
    float real = creal(im_complex_out[i]) / size * normalisation * normalisation;
    im_out[i] = (real < 0) ? -(unsigned short)real : (unsigned short)real;
  }
  fftw_destroy_plan(plan);
  fftw_cleanup();
  fftw_free(im_complex_out);
  return im_out;
}
