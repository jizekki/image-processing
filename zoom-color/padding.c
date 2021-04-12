/**
 * @file zero_padding.c
 * @brief resize a given image with an increasing factor with
 *        frequency zero-padding method.
 *        The resize factor is considered as an interger.
 * @author Vinh-Thong Ta <vinh-thong.ta@labri.fr>
 * @date 2010-2011  
 */

#include <stdio.h>
#include <stdlib.h>

#include <pnm.h>
#include "fft.h"

/**
 * @brief fills the grey-scaled image 'imd' with the values of 'values'
 * @param pnm imd, the input image to fill
 * @param unsigned short *values, the values to fill the image with
*/
void set_image_channel(pnm imd, unsigned short *values, int channel)
{
    int im_cols = pnm_get_width(imd);
    int im_rows = pnm_get_height(imd);
    for (int i = 0; i < im_rows; i++)
    {
        for (int j = 0; j < im_cols; j++)
        {
            pnm_set_component(imd, i, j, channel, values[i * im_cols + j]);
        }
    }
}

void process_channel(pnm ims, pnm imd, int factor, int channel)
{
    int ims_cols = pnm_get_width(ims);
    int ims_rows = pnm_get_height(ims);
    int imd_cols = ims_cols * factor;
    int imd_rows = ims_rows * factor;
    unsigned short *im_buffer = pnm_get_channel(ims, NULL, channel);

    fftw_complex *fwd = forward(ims_rows, ims_cols, im_buffer);
    fftw_complex *new_fwd = fftw_alloc_complex(imd_rows * imd_cols);
    int half_imd_rows = imd_rows / 2;
    int half_imd_cols = imd_cols / 2;
    int half_ims_rows = ims_rows / 2;
    int half_ims_cols = ims_cols / 2;

    int k = 0;
    fftw_complex *tmp = new_fwd;
    for (int i = 0; i < imd_rows; i++)
    {
        for (int j = 0; j < imd_cols; j++)
        {
            *tmp++ = (i >= (half_imd_rows - half_ims_rows) && i < (half_imd_rows + half_ims_rows) && j >= (half_imd_cols - half_ims_cols) && j < (half_imd_cols + half_ims_cols)) ? fwd[k++] : 0;
        }
    }

    unsigned short *bwd = backward(factor, imd_rows, imd_cols, new_fwd);
    set_image_channel(imd, bwd, channel);

    fftw_free(fwd);
    fftw_free(new_fwd);
    free(bwd);
    free(im_buffer);
}

void process(char *ims_name, char *imd_name, int factor)
{
    pnm ims = pnm_load(ims_name);
    int ims_cols = pnm_get_width(ims);
    int ims_rows = pnm_get_height(ims);
    int imd_cols = ims_cols * factor;
    int imd_rows = ims_rows * factor;
    pnm imd = pnm_new(imd_cols, imd_rows, PnmRawPpm);

    for (int c = 0; c < 3; c++)
    {
        process_channel(ims, imd, factor, c);
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
