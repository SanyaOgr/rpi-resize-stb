#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include "bicubic.h"
#include "gauss.h"

#define RESIZE_VERSION "1.8.1"

void resize_usage(char* prog, int resize_height, int resize_width, float ratio, int quality)
{
    printf("stb-resize version %s.\n", RESIZE_VERSION);
    printf("usage: %s [options] image_in.xxx image_out.xxx\n", prog);
    printf("extension: jpg(jpeg)\n");
    printf("method: bicubic (with Gauss prefilter for downsize)\n");
    printf("options:\n");
    printf("  -H px     resize height (default %d)\n", resize_height);
    printf("  -W px     resize width (default %d)\n", resize_width);
    printf("  -r X.X    sample ratio (default %f)\n", ratio);
    printf("  -q %%      jpeg quality (default %d)\n", quality);
    printf("  -h        show this help message and exit\n");
}

void resize(unsigned char *src, int height, int width, int channels, int resize_height, int resize_width, unsigned char *res)
{
    int gaussrx = 0, gaussry = 0;
    float vgauss;

    // Use Gauss prefilter if image downsizes only
    if ((resize_height < height) || (resize_width < width))
    {
        printf("prefilter: Gauss\n");
        gaussry = (resize_height < height) ? (0.5f * (float)height / (float)resize_height) : 0.0f;
        gaussrx = (resize_width < width) ? (0.5f * (float)width / (float)resize_width) : 0.0f;
        vgauss = GaussBlurFilter(src, height, width, channels, gaussry, gaussrx);
        printf("  gauss-value: %f\n", vgauss);
    }
    else
    {
        printf("prefilter: none\n");
    }

    printf("resize: %dx%d:%d\n", resize_width, resize_height, channels);

    printf("method: bicubic\n");
    ResizeImageBiCubic(src, height, width, channels, resize_height, resize_width, res);
}

int main(int argc, char **argv)
{
    int height, width, channels, y, x, d;
    int resize_height = 0, resize_width = 0;
    bool resizing = true;
    float ratio = 1.0f;
    int quality = 100;

    int fhelp = 0;
    int opt;
    size_t ki, kd;
    unsigned char *data = NULL, *resize_data = NULL, *save_data = NULL;
    stbi_uc *img = NULL;

    // Getting CLI options
    while ((opt = getopt(argc, argv, ":H:W:r:q:h")) != -1)
    {
        switch(opt)
        {
        case 'H':
            resize_height = atoi(optarg);
            break;

        case 'W':
            resize_width = atoi(optarg);
            break;

        case 'r':
            ratio = atof(optarg);
            if (ratio < 0.0f)
            {
                fprintf(stderr, "Bad argument\n");
                fprintf(stderr, "ratio = %f\n", ratio);
                return 1;
            }
            break;

        case 'q':
            quality = atoi(optarg);
            if (quality <= 0 || 100 < quality)
            {
                fprintf(stderr, "Bad argument\n");
                fprintf(stderr, "quality = %d\n", quality);
                return 1;
            }
            break;

        case 'h':
            fhelp = 1;
            break;

        case ':':
            fprintf(stderr, "ERROR: option needs a value\n");
            return 2;
            break;

        case '?':
            fprintf(stderr, "ERROR: unknown option: %c\n", optopt);
            return 3;
            break;
        }
    }
    if(optind + 2 > argc || fhelp)
    {
        resize_usage(argv[0], resize_height, resize_width, ratio, quality);
        return 0;
    }
    const char *src_name = argv[optind];
    const char *dst_name = argv[optind + 1];

    // Check if image extension is supported
    char* dst_ext = strrchr(dst_name, '.');
    if(!(strcmp(dst_ext, ".jpg") == 0 || 
        strcmp(dst_ext, ".jpeg") == 0))
    {
        printf("ERROR: saving to this format is not supported\n");
        return 1;
    }

    // Check if user doesn't resize image in fact
    if ((ratio == 1.f && resize_width == 0 && resize_height == 0) ||
        (resize_width == width && resize_height == height))
    {
        resizing = false;
    }

    // Load image from file
    printf("load: %s\n", src_name);
    if (!(img = stbi_load(src_name, &width, &height, &channels, STBI_rgb_alpha)))
    {
        fprintf(stderr, "ERROR: not read image: %s\n", src_name);
        return 1;
    }

    // Allocate memory for image data
    printf("image: %dx%d:%d\n", width, height, channels);
    if (!(data = (unsigned char*)malloc(height * width * channels * sizeof(unsigned char))))
    {
        fprintf(stderr, "ERROR: not use memmory\n");
        return 1;
    }

    // Get image data
    ki = 0;
    kd = 0;
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            for (d = 0; d < channels; d++)
            {
                data[kd + d] = (unsigned char)img[ki + d];
            }
            ki += STBI_rgb_alpha;
            kd += channels;
        }
    }
    stbi_image_free(img);

    // Check if width or height must be resized by ratio
    if (resize_height == 0)
    {
        resize_height = (int)((float)height * ratio + 0.5f);
    }

    if (resize_width == 0)
    {
        resize_width = (int)((float)width * ratio + 0.5f);
    }

    // Making resize stuff if need only
    if(resizing)
    {
        if ((resize_height == 0) || (resize_width == 0))
        {
            fprintf(stderr, "ERROR: bad target size %dx%d:%d\n", resize_width, resize_height, channels);
            return 1;
        }

        if (!(resize_data = (unsigned char*)malloc(resize_height * resize_width * channels * sizeof(unsigned char))))
        {
            fprintf(stderr, "ERROR: failed to allocate memory\n");
            return 2;
        }

        resize(data, height, width, channels, resize_height, resize_width, resize_data);

        save_data = resize_data;
    }
    else
    {
        save_data = data;
        printf("saving with original size\n");
    }

    // Checking target format and saving image to file
    bool retval = true;
    if(strcmp(dst_ext, ".jpg") == 0 || 
        strcmp(dst_ext, ".jpeg") == 0)
    {
        printf("quality: %d%%\n", quality);
        printf("save: %s\n", dst_name);
        retval = stbi_write_jpg(dst_name, resize_width, resize_height, channels, save_data, quality);
    }

    if (!retval)
    {
        fprintf(stderr, "ERROR: not write image: %s\n", dst_name);
        return 1;
    }

    // Free allocated memory
    if(resizing)
    {
        free(resize_data);
    }
    free(data);

    return 0;
}
