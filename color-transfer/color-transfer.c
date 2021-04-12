/**
 * @file color-transfert
 * @brief transfert color from source image to target image.
 *        Method from Reinhard et al. : 
 *        Erik Reinhard, Michael Ashikhmin, Bruce Gooch and Peter Shirley, 
 *        'Color Transfer between Images', IEEE CGA special issue on 
 *        Applied Perception, Vol 21, No 5, pp 34-41, September - October 2001
 */

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <bcl.h>

#define D 3

float RGB2LMS[D][D] = {
    {0.3811, 0.5783, 0.0402},
    {0.1967, 0.7244, 0.0782},
    {0.0241, 0.1288, 0.8444}};

float LMS2RGB[D][D] = {
    {4.4679, -3.5873, 0.1193},
    {-1.2186, 2.3809, -0.1624},
    {0.0497, -0.2439, 1.2045}};

float LMS2tuv[D][D] = {
    {0.5773, 0.5773, 0.5773},
    {0.4082, 0.4082, -0.8165},
    {0.7071, -0.7071, 0}};

float tuv2LMS[D][D] = {
    {0.5773, 0.4082, 0.7071},
    {0.5773, 0.4082, -0.7071},
    {0.5773, -0.8165, 0}};

float standard_deviation(float *set, size_t size, float mean)
{
  float sd = 0.0;
  for (size_t i = 0; i < size; i++)
  {
    sd += powf((set[i] - mean), 2);
  }
  sd = sqrtf(sd / (float)size);
  return sd;
}

float mean(float *set, size_t size)
{
  float m = 0.0;
  for (size_t i = 0; i < size; i++)
  {
    m += set[i];
  }
  m = m / (float)size;
  return m;
}

unsigned short truncrgb(float v)
{
  unsigned short k = (unsigned short)roundf(v);
  if (k > 255)
    return 255;
  if (k < 0)
    return 0;
  return k;
}

void convert(float mconv[D][D], float *ch1, float *ch2, float *ch3, float *nch1, float *nch2, float *nch3, size_t im_cols, size_t im_rows)
{
  for (size_t i = 0; i < im_rows; i++)
  {
    for (size_t j = 0; j < im_cols; j++)
    {
      nch1[i * im_cols + j] = mconv[0][PnmRed] * ch1[i * im_cols + j] + mconv[0][PnmGreen] * ch2[i * im_cols + j] + mconv[0][PnmBlue] * ch3[i * im_cols + j];
      nch2[i * im_cols + j] = mconv[1][PnmRed] * ch1[i * im_cols + j] + mconv[1][PnmGreen] * ch2[i * im_cols + j] + mconv[1][PnmBlue] * ch3[i * im_cols + j];
      nch3[i * im_cols + j] = mconv[2][PnmRed] * ch1[i * im_cols + j] + mconv[2][PnmGreen] * ch2[i * im_cols + j] + mconv[2][PnmBlue] * ch3[i * im_cols + j];
    }
  }
}

void get_log(float *ch1, float *ch2, float *ch3, size_t im_cols, size_t im_rows)
{
  for (size_t i = 0; i < im_rows; i++)
  {
    for (size_t j = 0; j < im_cols; j++)
    {
      ch1[i * im_cols + j] = log1p(ch1[i * im_cols + j]);
      ch2[i * im_cols + j] = log1p(ch2[i * im_cols + j]);
      ch3[i * im_cols + j] = log1p(ch3[i * im_cols + j]);
    }
  }
}

void get_exp(float *ch1, float *ch2, float *ch3, size_t im_cols, size_t im_rows)
{
  for (size_t i = 0; i < im_rows; i++)
  {
    for (size_t j = 0; j < im_cols; j++)
    {
      ch1[i * im_cols + j] = expf(ch1[i * im_cols + j]);
      ch2[i * im_cols + j] = expf(ch2[i * im_cols + j]);
      ch3[i * im_cols + j] = expf(ch3[i * im_cols + j]);
    }
  }
}

float *make_channel(pnm im, pnmChannel c)
{
  size_t im_cols = pnm_get_width(im);
  size_t im_rows = pnm_get_height(im);
  float *im_c = (float *)malloc(sizeof(float) * im_cols * im_rows);
  for (size_t i = 0; i < im_rows; i++)
  {
    for (size_t j = 0; j < im_cols; j++)
    {
      im_c[i * im_cols + j] = (float)pnm_get_component(im, i, j, c);
    }
  }
  return im_c;
}

void process(char *ims_name, char *imt_name, char *imd_name)
{
  pnm imt = pnm_load(imt_name);
  pnm ims = pnm_load(ims_name);
  size_t imt_cols = pnm_get_width(imt);
  size_t imt_rows = pnm_get_height(imt);
  size_t ims_cols = pnm_get_width(ims);
  size_t ims_rows = pnm_get_height(ims);
  pnm imd = pnm_new(imt_cols, imt_rows, PnmRawPpm);

  float *imt_r = make_channel(imt, PnmRed);
  float *imt_g = make_channel(imt, PnmGreen);
  float *imt_b = make_channel(imt, PnmBlue);

  float *ims_r = make_channel(ims, PnmRed);
  float *ims_g = make_channel(ims, PnmGreen);
  float *ims_b = make_channel(ims, PnmBlue);

  float *imt_l = (float *)malloc(sizeof(float) * imt_cols * imt_rows);
  float *imt_m = (float *)malloc(sizeof(float) * imt_cols * imt_rows);
  float *imt_s = (float *)malloc(sizeof(float) * imt_cols * imt_rows);
  float *imt_t = (float *)malloc(sizeof(float) * imt_cols * imt_rows);
  float *imt_u = (float *)malloc(sizeof(float) * imt_cols * imt_rows);
  float *imt_v = (float *)malloc(sizeof(float) * imt_cols * imt_rows);

  float *ims_l = (float *)malloc(sizeof(float) * ims_cols * ims_rows);
  float *ims_m = (float *)malloc(sizeof(float) * ims_cols * ims_rows);
  float *ims_s = (float *)malloc(sizeof(float) * ims_cols * ims_rows);
  float *ims_t = (float *)malloc(sizeof(float) * ims_cols * ims_rows);
  float *ims_u = (float *)malloc(sizeof(float) * ims_cols * ims_rows);
  float *ims_v = (float *)malloc(sizeof(float) * ims_cols * ims_rows);

  convert(RGB2LMS, imt_r, imt_g, imt_b, imt_l, imt_m, imt_s, imt_cols, imt_rows);
  get_log(imt_l, imt_m, imt_s, imt_cols, imt_rows);
  convert(LMS2tuv, imt_l, imt_m, imt_s, imt_t, imt_u, imt_v, imt_cols, imt_rows);

  convert(RGB2LMS, ims_r, ims_g, ims_b, ims_l, ims_m, ims_s, ims_cols, ims_rows);
  get_log(ims_l, ims_m, ims_s, ims_cols, ims_rows);
  convert(LMS2tuv, ims_l, ims_m, ims_s, ims_t, ims_u, ims_v, ims_cols, ims_rows);

  //  <PORBLEM_HERE>

  float meantt = mean(imt_t, imt_cols * imt_rows);
  float meantu = mean(imt_u, imt_cols * imt_rows);
  float meantv = mean(imt_v, imt_cols * imt_rows);

  float meanst = mean(ims_t, ims_cols * ims_rows);
  float meansu = mean(ims_u, ims_cols * ims_rows);
  float meansv = mean(ims_v, ims_cols * ims_rows);

  float sdtt = standard_deviation(imt_t, imt_cols * imt_rows, meantt);
  float sdtu = standard_deviation(imt_u, imt_cols * imt_rows, meantu);
  float sdtv = standard_deviation(imt_v, imt_cols * imt_rows, meantv);

  float sdst = standard_deviation(ims_t, ims_cols * ims_rows, meanst);
  float sdsu = standard_deviation(ims_u, ims_cols * ims_rows, meansu);
  float sdsv = standard_deviation(ims_v, ims_cols * ims_rows, meansv);

  for (size_t i = 0; i < imt_rows; i++)
  {
    for (size_t j = 0; j < imt_cols; j++)
    {
      imt_t[i * imt_cols + j] = (sdst / sdtt) * (imt_t[i * imt_cols + j] - meantt) + meanst;
      imt_u[i * imt_cols + j] = (sdsu / sdtu) * (imt_u[i * imt_cols + j] - meantu) + meansu;
      imt_v[i * imt_cols + j] = (sdsv / sdtv) * (imt_v[i * imt_cols + j] - meantv) + meansv;
    }
  }

  //  <\PORBLEM_HERE>

  convert(tuv2LMS, imt_t, imt_u, imt_v, imt_l, imt_m, imt_s, imt_cols, imt_rows);
  get_exp(imt_l, imt_m, imt_s, imt_cols, imt_rows);
  convert(LMS2RGB, imt_l, imt_m, imt_s, imt_r, imt_g, imt_b, imt_cols, imt_rows);

  for (size_t i = 0; i < imt_rows; i++)
  {
    for (size_t j = 0; j < imt_cols; j++)
    {
      pnm_set_component(imd, i, j, PnmRed, truncrgb(imt_r[i * imt_cols + j]));
      pnm_set_component(imd, i, j, PnmGreen, truncrgb(imt_g[i * imt_cols + j]));
      pnm_set_component(imd, i, j, PnmBlue, truncrgb(imt_b[i * imt_cols + j]));
    }
  }

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(ims);
  pnm_free(imt);
  pnm_free(imd);
  free(imt_r);
  free(imt_g);
  free(imt_b);
  free(ims_r);
  free(ims_g);
  free(ims_b);
  free(imt_l);
  free(imt_m);
  free(imt_s);
  free(ims_l);
  free(ims_m);
  free(ims_s);
  free(imt_t);
  free(imt_u);
  free(imt_v);
  free(ims_t);
  free(ims_u);
  free(ims_v);
}

void usage(char *s)
{
  fprintf(stderr, "Usage: %s <ims> <imt> <imd> \n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 3
int main(int argc, char *argv[])
{
  if (argc != PARAM + 1)
    usage(argv[0]);
  process(argv[1], argv[2], argv[3]);
  return EXIT_SUCCESS;
}
