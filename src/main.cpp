#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define RESIZE_VERSION "1.0"

void resize_usage(char* prog, int resize_height, int resize_width, float ratio, int quality)
{
    printf("stb-resize version %s.\n", RESIZE_VERSION);
    printf("usage: %s [options] image_in.xxx image_out.xxx\n", prog);
    printf("extension: jpg(jpeg)\n");
    //printf("method: bicubic (with Gauss prefilter for downsize)\n");
    printf("options:\n");
    printf("  -H px     resize height (default %d)\n", resize_height);
    printf("  -W px     resize width (default %d)\n", resize_width);
    printf("  -r X.X    sample ratio (default %f)\n", ratio);
    printf("  -q %%      jpeg quality (default %d)\n", quality);
    printf("  -h        show this help message and exit\n");
}

int resize(stbi_uc *src, int height, int width, int channels, int resize_height, int resize_width, stbi_uc *dst)
{
    printf("resize: %dx%d:%d\n", resize_width, resize_height, channels);
    
    int ret = stbir_resize(src, width, height, 0, 
                            dst, resize_width, resize_height, 0,
                            STBIR_TYPE_UINT8,
                            channels, STBIR_ALPHA_CHANNEL_NONE, 0,
                            STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
                            STBIR_FILTER_DEFAULT, STBIR_FILTER_DEFAULT,
                            STBIR_COLORSPACE_SRGB, NULL);
    
    return ret;
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

    stbi_uc *img = NULL, *resize_img = NULL, *save_img = NULL;

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
    if (!(img = stbi_load(src_name, &width, &height, &channels, 0)))
    {
        fprintf(stderr, "ERROR: not read image: %s\n", src_name);
        return 1;
    }

    printf("image: %dx%d:%d\n", width, height, channels);

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

        if (!(resize_img = (unsigned char*)malloc(resize_height * resize_width * channels * sizeof(unsigned char))))
        {
            fprintf(stderr, "ERROR: failed to allocate memory\n");
            return 2;
        }

        if(!resize(img, height, width, channels, resize_height, resize_width, resize_img))
        {
            fprintf(stderr, "ERROR: failed to resize image\n");
            return 2;
        }

        save_img = resize_img;
    }
    else
    {
        save_img = img;
        printf("saving with original size\n");
    }

    // Checking target format and saving image to file
    bool retval = true;
    if(strcmp(dst_ext, ".jpg") == 0 || 
        strcmp(dst_ext, ".jpeg") == 0)
    {
        printf("quality: %d%%\n", quality);
        printf("save: %s\n", dst_name);
        retval = stbi_write_jpg(dst_name, resize_width, resize_height, channels, save_img, quality);
    }

    if (!retval)
    {
        fprintf(stderr, "ERROR: not write image: %s\n", dst_name);
        return 1;
    }

    // Free allocated memory
    if(resizing)
    {
        stbi_image_free(resize_img);
    }
    stbi_image_free(img);

    return 0;
}
