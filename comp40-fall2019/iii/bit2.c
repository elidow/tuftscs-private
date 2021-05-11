/*
 * HW2
 * Partners: Eli Dow, Daniel Alderman
 * bit2.c
 *
 * Implementation for bit2
 */

#include "bit2.h"

struct Bit2_T
{
        int width;
        int height;
        Bit_T long_array;
};

Bit2_T Bit2_new(int width, int height)
{
        assert(width > 0);
        assert(height > 0);

        B new_array = malloc(sizeof(*new_array));
        assert(new_array != NULL);
        Bit_T temp = Bit_new(width * height);
        new_array->long_array = temp;
        new_array->width = width;
        new_array->height = height;

        return new_array;

}

int Bit2_index(Bit2_T array, int col, int row)
{
        assert(array != NULL);
        assert(col >= 0);
        assert(row >= 0);
        assert(col < array->width);
        assert(row < array->height);

        int index = (row * array->width) + col;
        return index;
}


void Bit2_free(Bit2_T* array)
{
        assert(array != NULL);

        Bit2_T ray = *array;
        Bit_free(&ray->long_array);
        free(ray->long_array);
        free(ray);
}


int Bit2_width(Bit2_T array)
{
        assert(array != NULL);
        return array->width;
}


int Bit2_height(Bit2_T array)
{
        assert(array != NULL);
        return array->height;
}


void Bit2_map_row_major(Bit2_T array, void apply(int i, int j, Bit2_T array,
                                                int bit, void *p1), void*cl)
{
        assert(array != NULL);
        assert(apply);
        (void) cl;

        for (int row = 0; row < array->height; row++){
                for (int col = 0; col < array->width; col++){
                        apply(col, row, array, Bit2_get(array, col, row), cl);
                }
        }

        return;
}

void Bit2_map_col_major(Bit2_T array, void apply(int i, int j, Bit2_T array,
                                                int bit, void *p1), void*cl)
{
        assert(array != NULL);
        assert(apply);
        (void) cl;

        for (int col = 0; col < array->width; col++){
                for (int row = 0; row < array->height; row++){
                        apply(col, row, array, Bit2_get(array, col, row), cl);
                }
        }

        return;
}

int Bit2_get(Bit2_T array, int col, int row)
{
        assert(array != NULL);
        assert(row >= 0 && row < array->height);
        assert(col >= 0 && col < array->width);

        int index = Bit2_index(array, col, row);
        int element;
        element = Bit_get(array->long_array, index);
        assert(element == 0 || element == 1);

        return element;

}


int Bit2_put(Bit2_T array, int col, int row, int bit)
{
        assert(array != NULL);
        assert(row >= 0 && row < array->height);
        assert(col >= 0 && col < array->width);
        assert(bit == 0 || bit == 1);

        int index = Bit2_index(array, col, row);
        int prev;
        prev = Bit_put(array->long_array, index, bit);
        assert(prev == 0 || prev == 1);

        return prev;


}