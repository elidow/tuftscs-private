/* 
 * ppmdiff.c
 *
 * COMP40
 * Lab in preparation for arith
 *
 * Eli Dow and Zetty Cho
 */

#include <stdio.h>
#include <stdlib.h>
#include "pnmrdr.h"


#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>

#include <stdbool.h>
#include "assert.h"
#include "mem.h"
#include "pnmrdr.h"

#include <math.h>
#include <except.h>


static double compare_images(Pnmrdr_T, Pnmrdr_T);

static int compare_size(Pnmrdr_mapdata, Pnmrdr_mapdata);

static int get_small(int, int);

static int identify_image(int, int);

static double sum_pixel(double, double, double, double, double, double);

/*
* Function: main
* Parameters: int argc, char *argv[]
* Reads in a file either from a argc/argv or from stdin.
* Handles errors for too many arguments, incorrect file format, and
* file opening issues. Calls brightness_calculator funtion and prints average
* brightness. Closes all files and frees all memory.
*/
int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    Pnmrdr_T image = NULL;
    FILE *fp2 = NULL;
    Pnmrdr_T image2 = NULL;
    if(argc == 3)
    {

        fp = fopen(argv[1], "r");

        if(fp == NULL)
        {
            fprintf(stderr, "Could not open file\n");
            return EXIT_FAILURE;
        }

        TRY
        {
            image = Pnmrdr_new(fp);
        }
        EXCEPT(Pnmrdr_Badformat)
        {
            fprintf(stderr, "Incorrect file format\n");
            fclose(fp);
            return EXIT_FAILURE;
        }

        END_TRY;

        fp2 = fopen(argv[2], "r");

        if(fp2 == NULL)
        {
            fprintf(stderr, "Could not open file\n");
            return EXIT_FAILURE;
        }

        TRY
        {
            image2 = Pnmrdr_new(fp2);
        }
        EXCEPT(Pnmrdr_Badformat)
        {
            fprintf(stderr, "Incorrect file format\n");
            fclose(fp2);
            return EXIT_FAILURE;
        }

        END_TRY;


    }
   // else if(argc == 1)
   //     image = Pnmrdr_new(stdin);
   // else
   // {
   //     fprintf(stderr, "Too many arguments. Enter one file name.\n");
   //     return EXIT_FAILURE;
   // }

   // double avg_bright = brightness_calculator(image);

    Pnmrdr_mapdata map = Pnmrdr_data(image);
    Pnmrdr_mapdata map2 = Pnmrdr_data(image2);


    if (compare_size(map, map2) == 0) 
    {
        fprintf(stderr, "Image size difference is greater than 1\n");
        printf("1.0\n");
        if( image != NULL)
            Pnmrdr_free(&image);
        if(fp != NULL)
            fclose(fp);
        if(image2 != NULL)
            Pnmrdr_free(&image2);
        if(fp2 != NULL)
            fclose(fp2);
        return EXIT_FAILURE;
    }



    double diff = compare_images(image, image2);
    printf("%f\n", diff);
    
    if(image != NULL) {
        fprintf(stderr, "test1\n");
        Pnmrdr_free(&image);
    }
    if(fp != NULL) {
        fprintf(stderr, "test2\n");
        fclose(fp);
    }
    if(image2 != NULL) {
        fprintf(stderr, "test3\n");
        Pnmrdr_free(&image2);
    }
    if(fp2 != NULL) {
        fprintf(stderr, "test4\n");
        fclose(fp2);
    }

    fprintf(stderr, "test\n");
    
    return EXIT_SUCCESS;
}

static int get_small(int n, int n1)
{
    if (n > n1)
        return n1;
    else 
        return n;
}

static int compare_size(Pnmrdr_mapdata map, Pnmrdr_mapdata map2)
{


    if (abs((int)map.width - (int)map2.width) > 1 || 
        abs((int)map.height - (int)map2.height) > 1)
    {
        return 0;
    }
    return 1;
}

static int identify_image(int n, int n2)
{
    if (n > n2)
        return 1;
    else if (n < n2)
        return 0;
    else
        return -1;
}

static double compare_images(Pnmrdr_T image, Pnmrdr_T image2)
{

    Pnmrdr_mapdata map = Pnmrdr_data(image);
    Pnmrdr_mapdata map2 = Pnmrdr_data(image2);

    int width = get_small((int)map.width, (int)map2.width);
    int height = get_small((int)map.height, (int)map2.height);

    // 0 means 1st image, 1 means second
    // These identify the smaller image
    int boolw = identify_image((int)map.width, (int)map2.width);
    int boolh = identify_image((int)map.height, (int)map2.height);

    double sum = 0;
    for (int i = 0; i < width; i++) 
    {
        for (int j = 0; j < height; j++)
        {
            int red1 = Pnmrdr_get(image);
            int green1 = Pnmrdr_get(image);
            int blue1 = Pnmrdr_get(image);

            int red2 = Pnmrdr_get(image2);
            int green2 = Pnmrdr_get(image2);
            int blue2 = Pnmrdr_get(image2);

            // sum += sum_pixel(red1/255, red2/255, green1/255, green2/255, 
            //                 blue1/255, blue2/255);

            sum += sum_pixel(red1, red2, green1, green2, blue1, blue2);
            if (boolh != -1)
            {
                int r;
                int g;
                int b;
                if (boolh == 1)
                {
                    r = Pnmrdr_get(image);
                    g = Pnmrdr_get(image);
                    b = Pnmrdr_get(image);
                }
                else
                {
                    r = Pnmrdr_get(image2);
                    g = Pnmrdr_get(image2);
                    b = Pnmrdr_get(image2);
                }
                (void) r;
                (void) g;
                (void) b;
                j++;
            }
        }
        if (boolw != -1)
        {
            int r;
            int g;
            int b;
            if (boolw == 1)
            {
                r = Pnmrdr_get(image);
                g = Pnmrdr_get(image);
                b = Pnmrdr_get(image);
            }
            else
            {
                r = Pnmrdr_get(image2);
                g = Pnmrdr_get(image2);
                b = Pnmrdr_get(image2);
            }
            (void) r;
            (void) g;
            (void) b;
            i++;
        }
    }
    
    double final = sqrt(sum/(3 * width * height)) / 255;

    return final;
}


static double sum_pixel(double red1, double red2, double green1, double green2, 
                    double blue1, double blue2)
{
    double sum = 0;
    // sum = pow(red1 - red2, 2) + pow(green1 - green2, 2) + pow(blue1 - blue2, 2);

    sum = ((red1 - red2) * (red1 - red2))
         + ((green1 - green2) * (green1 - green2)) 
         + ((blue1 - blue2) * (blue1 - blue2));
    return sum;
}

