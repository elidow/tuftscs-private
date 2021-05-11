/*
 * HW2
 * Partners: Eli Dow, Daniel Alderman
 * uarray2.c
 * last edited 9/27/2019
 *
 * Implementation for uarray2
 */

#include "uarray2.h"

/*
 Struct that holds all the memory needed for our UArray2 implementation.
struct UArray2_T
*/
struct UArray2_T
{
        int width;
        int height;
        int size;
        UArray_T long_array;
};



A UArray2_new(int width, int height, int size)
{
        assert(width > 0);
        assert(height > 0);
        assert(size > 0);

        UArray2_T new_array = malloc(sizeof(*new_array));
        assert(new_array != NULL);
        UArray_T temp = UArray_new((width * height), size);
        new_array->long_array = temp;
        new_array->width = width;
        new_array->height = height;
        new_array->size = size;

        return new_array;
}


void UArray2_free(A *array)
{
        assert(array != NULL);
        A ray = *array;
        UArray_free(&ray->long_array);
        free(ray->long_array);
        free(ray);

}

int UArray2_width(A array)
{
        assert(array != NULL);
        return array->width;
}

int UArray2_height(A array)
{
        assert(array != NULL);
        return array->height;
}

int UArray2_size(A array)
{
        assert(array != NULL);
        return array->size;
}



void* UArray2_at(A array, int col, int row)
{
        int index; 
void* element = NULL;

        assert(array != NULL);
        assert(col >= 0);
        assert(row >= 0);
        assert(col < array->width);
        assert(row < array->height);

        index = UArray2_index(array, col, row);

        element = UArray_at(array->long_array, index);
        assert(element != NULL);
        
        return element;

}


int UArray2_index(A array, int col, int row)
{
        int index;

        assert(array != NULL);
        assert(col >= 0);
        assert(row >= 0);
        assert(col < array->width);
        assert(row < array->height);

        index = (row * array->width) + col;
        return index;
}


void UArray2_map_row_major(A array, void apply(int i, int j, UArray2_T a,
                                void *p1, void *p2), void*cl)
{
        assert(array != NULL);
        assert(apply);
        (void) cl;

        for (int row = 0; row < array->height; row++) {
                for (int col = 0; col < array->width; col++) {
                        apply(col, row, array, UArray2_at(array, col, row), cl);
                }
        }

        return;
}

void UArray2_map_col_major(A array, void apply(int i, int j, UArray2_T a,
                                void *p1, void *p2), void*cl)
{
        assert(array != NULL);
        assert(apply);
        (void) cl;

        for (int col = 0; col < array->width; col++) {
                for (int row = 0; row < array->height; row++) {
                        apply(col, row, array, UArray2_at(array, col, row), cl);
                }
        }

        return;
}
