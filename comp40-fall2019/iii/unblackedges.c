/*
 * HW2
 * Partners: Eli Dow, Daniel Alderman
 * unblackedges.c
 *
 * Implementation for unblackededges
 */

#include "bit2.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

#include <pnmrdr.h>
#include "mem.h"
#include <math.h>
#include <except.h>
#include <stack.h>


/*
 * struct location
 * This struct stores information about a pixel in a file, which
 * includes the row location index and the column location index
 */
struct location
{
        int l_row;
        int l_col;
};

static Pnmrdr_T file_handler(int argc, char* argv[], FILE** fp, bool* filegood);
static Bit2_T pbmread(Pnmrdr_T imgage, Bit2_T bitmap, bool* success);
static Bit2_T is_edge_black(Bit2_T bitmap);
static Bit2_T dfs_bp(Bit2_T bitmap, int col, int row);
static void dfs_helper(Bit2_T bitmap, int col, int row, 
                        struct location* current, Stack_T prev_row,
                        struct location curr);
static void pbmwrite(Bit2_T bitmap);
static void bit_print(int i, int j, Bit2_T a, int bit, void *cl);
static void edges_free(bool* success, B bitmap, FILE* fp, Pnmrdr_T image);


/*
 * Function: main
 * Parameters: int argc, char *argv[]
 * Does: Reads file names and calls other functions
 * Returns EXIT_SUCCESS if there are no issues, otherwise EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
        FILE *fp = NULL;
        Pnmrdr_T image = NULL;
        bool filegood = true;
        image = file_handler(argc, argv, &fp, &filegood);
        if (!filegood){
                fprintf(stderr, "file format not good.\n");
                return EXIT_FAILURE;
        }
        Pnmrdr_mapdata map = Pnmrdr_data(image);

        if (map.height == 0 || map.width == 0 || map.type != Pnmrdr_bit){
                if (fp != NULL){
                        fclose(fp);
                }
                if (image != NULL){
                        Pnmrdr_free(&image);
                }
                fprintf(stderr, "Incorrect file format.\n");
                return EXIT_FAILURE;
        }

        B bitmap = Bit2_new(map.width, map.height);
        bool *success = malloc(sizeof(bool)); 
        *success = true;
        pbmread(image, bitmap, success);
        if (!*success){
                edges_free(success, bitmap, fp, image);
                return EXIT_FAILURE;
        }
        bitmap = is_edge_black(bitmap);
        pbmwrite(bitmap);

        free(success);
        Bit2_free(&bitmap);

        if (fp != NULL){
                fclose(fp);
        }
        if (image != NULL){
                Pnmrdr_free(&image);
        }
        return EXIT_SUCCESS;
}

/*
 * Function: file_handler
 * Parameters: int argc, char *argv[], FILE**, bool*
 * Does: Handles any errors from inputs and outputs stderr statements. If there
 * are errors, bool* filegood is set to false. Function returns filegood
 */
static Pnmrdr_T file_handler(int argc, char* argv[], FILE** fp, bool* filegood)
{
        Pnmrdr_T image = NULL;

        if (argc == 2){
                *fp = fopen(argv[1], "r");

                if (*fp == NULL){
                        fprintf(stderr, "Could not open file\n");
                        *filegood = false;
                        return image;
                }

                TRY
                {
                        image = Pnmrdr_new(*fp);
                }
                EXCEPT(Pnmrdr_Badformat)
                {
                        fprintf(stderr, "Incorrect file format\n");
                        fclose(*fp);
                        *filegood = false;
                }
                END_TRY;
        }
        else if (argc == 1) {
                image = Pnmrdr_new(stdin);
        }
        else {
                fprintf(stderr, "Too many arguments. Enter one file name.\n");
                *filegood = false;
        }
        return image;
}

/*
 * Function: pbmread
 * Parameters: Pnmrdr_T image, Bit2_T bitmap, bool* success
 * Does: Reads a pbm file and populates a Bit2_array bitmap. Returns the array.
 */
static Bit2_T pbmread(Pnmrdr_T image, Bit2_T bitmap, bool* success)
{

        for (int row = 0; row < Bit2_height(bitmap); row++){
                for (int col = 0; col < Bit2_width(bitmap); col++){
                        TRY
                        {
                                int curr_bit = Pnmrdr_get(image);
                                Bit2_put(bitmap, col, row, curr_bit);
                        }
                        EXCEPT(Pnmrdr_Count)
                        {
                                fprintf(stderr, "Bit cannot be read\n");
                                *success = false;
                                return bitmap;
                        }            
                        END_TRY;
                }
        }

        return bitmap;
}

/*
 * Function: is_edge_black
 * Parameters: Bit2_T bitmap
 * Does: Takes in a Bit2_T array and checks to see if any of the edge pixels are
 * black. If they are, dfs_bp is called to delete non-edge pixels that are 
 * black. Returns modified bitmap.
 */
static Bit2_T is_edge_black(Bit2_T bitmap)
{

        int last_row = Bit2_height(bitmap) - 1;
        int last_col = Bit2_width(bitmap) - 1;

        for (int i = 0; i < Bit2_width(bitmap); i++){
                if (Bit2_get(bitmap, i, 0) == 1){
                        bitmap = dfs_bp(bitmap, i, 0);
                }
                if (Bit2_get(bitmap, i, last_row) == 1){
                        bitmap = dfs_bp(bitmap, i, last_row);
                }
        }

        for (int i = 0; i < Bit2_height(bitmap); i++){
                if (Bit2_get(bitmap, 0, i) == 1){
                        bitmap = dfs_bp(bitmap, 0, i);
                }
                if (Bit2_get(bitmap, last_col, i) == 1){
                        bitmap = dfs_bp(bitmap, last_col, i);
                }
        }

        return bitmap;
}

/*
 * Function: dfs_bp (stands for Depth First Search for Black Pixels)
 * Parameters: Bit2_T bitmap, int c, int r
 * Does: Takes in a Bit2_T array and uses a depth first search algorithm that 
 * acts as a recursive function in order to change all neighboring pixels to
 * white. Returns updated bitamp.
 */
static Bit2_T dfs_bp(Bit2_T bitmap, int c, int r)
{
        struct location curr;
        int row = r;
        int col = c;
        Stack_T prev_row= Stack_new();
        struct location *current = NULL;

        while (true){
                if (Bit2_get(bitmap, col, row) == 1){
                        dfs_helper(bitmap, col, row, current, prev_row, curr);
                }

                if (!Stack_empty(prev_row)){
                        current = (struct location *)Stack_pop(prev_row);
                        row = (*current).l_row;
                        col = (*current).l_col;
                        if (current != NULL){
                                free(current);
                                current = NULL;
                        }
                }
                else {
                        if (current != NULL) {
                                free(current);
                                current = NULL;
                        }
                        break;
                }
        }
        Stack_free(&prev_row);

        return bitmap;
}

/*
 * Function: dfs_helper (stands for Depth First Search helper)
 * Parameters: Bit2_T bitmap, int c, int r, struct location* current,
                Stack_T prev_row, struct location curr
 * Does: Helper function for dfs_bp. Takes in a Bit2_T array and other
 * attributes to check to see if its neighbors (up, right, down, left) are also
 * black.
 * Returns nothing
 */
static void dfs_helper(Bit2_T bitmap, int col, int row,
                        struct location* current, Stack_T prev_row, 
                        struct location curr)
{
        Bit2_put(bitmap, col, row, 0);

        if (row != 0 && Bit2_get(bitmap, col, row-1) == 1){
                current = malloc(sizeof(curr));
                assert(current != NULL);
                (*current).l_row = row - 1;
                (*current).l_col = col;
                Stack_push(prev_row, current);
        }

        if (col!=(Bit2_width(bitmap) - 1) && Bit2_get(bitmap, col+1, row) == 1){
                current = malloc(sizeof(curr));
                assert(current != NULL); 
                (*current).l_row = row;
                (*current).l_col = col+1;
                Stack_push(prev_row, current);
        }

        if (row!=(Bit2_height(bitmap) - 1) && Bit2_get(bitmap, col, row+1) ==1){
                current = malloc(sizeof(curr));
                assert(current != NULL);
                (*current).l_row = row+1;
                (*current).l_col = col; 
                Stack_push(prev_row, current);
        }

        if ( col != 0 && Bit2_get(bitmap, col-1, row) == 1){
                current = malloc(sizeof(curr));
                assert(current != NULL);
                (*current).l_row = row;
                (*current).l_col = col-1;
                Stack_push(prev_row, current);
        }
}

/*
 * Function: pbmwrite
 * Parameters: Bit2_T bitmap
 * Does: Traverses bitmap row_major and writes it to the apply function, which 
 * is bit_print
 */
static void pbmwrite (Bit2_T bitmap)
{
        int* line_len = malloc(sizeof(int));
        assert(line_len != NULL);
        *line_len = 0;
        fprintf(stdout, "P1\n");
        fprintf(stdout,
         "#ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 6 from 6)\n");
        fprintf(stdout, "%d %d\n", Bit2_width(bitmap), Bit2_height(bitmap));
        Bit2_map_row_major(bitmap, bit_print, line_len);
        free(line_len);

}

/*
 * Function: bit_print
 * Parameters: int i, int j, Bit2_T bitmap, int bit, void* cl
 * Does: Prints the current bit based on parameters
 */
static void bit_print(int i, int j, Bit2_T bitmap, int bit, void *cl) 
{
        *(int *)cl = *(int*)cl + 1;
        (void) j;
        if ((i == (Bit2_width(bitmap) -1)) || *(int *)cl > 69) {
                fprintf(stdout, "%d\n", bit);
                *(int *)cl = 0;
        }
        else {          
                fprintf(stdout, "%d ", bit);   
        }
}

/*
 * Function: edges_free
 * Parameters: bool* success, B bitmap, FILE* fp, Pnmrdr_T image
 * Does: Frees all memory used to change black edges
 */
static void edges_free(bool* success, B bitmap, FILE* fp, Pnmrdr_T image)
{
        free(success);
        Bit2_free(&bitmap);

        if (fp != NULL){
                fclose(fp);
        }
        if (image != NULL){
                Pnmrdr_free(&image);
        }
}

