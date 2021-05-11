/*
 * HW1
 * Partners: Eli Dow, Daniel Alderman
 * brightness.c
 *
 * This program reads a grayscale image and 
 * prints its average brightness
 */


#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>

#include <stdbool.h>
#include "assert.h"
#include "mem.h"
#include "pnmrdr.h"

#include <math.h>
#include <except.h>


//calculates brightness of Pnmrdr object
static double brightness_calculator(Pnmrdr_T);


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
    if(argc == 2)
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

    }
    else if(argc == 1)
        image = Pnmrdr_new(stdin);
    else
    {
        fprintf(stderr, "Too many arguments. Enter one file name.\n");
        return EXIT_FAILURE;
    }

    double avg_bright = brightness_calculator(image);

    if(fp != NULL)
        fclose(fp);
    if(image != NULL)
        Pnmrdr_free(&image);

    if(avg_bright == -1.0)
        return EXIT_FAILURE;

    printf("%.3lf\n", avg_bright);
    return EXIT_SUCCESS;
}


/*
 * Function: brightness_calculator
 * Parameters: Pnmrdr_T object
 * Does: Responsible for calculating the brightness of a 
 * Pnmrdr object. Once it takes the image in, it
 * retrieves important data from it. Then, it calculates the average 
 * brightness and catches exception for issues with reading
 * the pixels. Returns a double.
 */
static double brightness_calculator(Pnmrdr_T image)
{
    double avg_bright = 0.000;

    Pnmrdr_mapdata map = Pnmrdr_data(image);

    double total_bright = 0;
    double total_pix = 0;

    for(unsigned i = 0; i < map.width; i++)
    {
        for(unsigned j = 0; j < map.height; j++)
        {
            TRY
            {
                total_bright+= Pnmrdr_get(image);
                total_pix++;
            }
            EXCEPT(Pnmrdr_Count)
            {
                fprintf(stderr, "Pixel cannot be read\n");
                return -1.0;
            }
            
            END_TRY;
        }
    }
    avg_bright = total_bright / total_pix / map.denominator;

    double rounded_bright = roundf(avg_bright * 1000) / 1000;

    if(total_pix == 0)
        return -1.0;
    else
        return rounded_bright;
}
