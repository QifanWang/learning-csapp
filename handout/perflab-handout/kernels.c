/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "abc",              /* Team name */

    "Harry Q. Bovik",     /* First member full name */
    "bovik@nowhere.edu",  /* First member email address */

    "Qifan Wang",                   /* Second member full name (leave blank if none) */
    "qifanwang@whu.edu.com"                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}


/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version, 8 * 8 block";
void rotate(int dim, pixel *src, pixel *dst) 
{
    //naive_rotate(dim, src, dst);
    int dim_1 = dim - 1;
    int seg = 8;
    int i, j, A, B;
    
    for(A = 0; A < dim; A += seg)
    	for(B = 0; B < dim; B += seg)
    	    for(i = A; i <A + seg; ++i)
    	    	for(j = B; j <B + seg; ++j) {
    	    	    dst[RIDX(dim_1-j, i, dim)] = src[RIDX(i, j, dim)];
    	    	}

    // loop unrolling 
    // for(A = 0; A < dim; A += seg)
    // 	for(B = 0; B < dim; B += seg) {
    //         dst[RIDX(dim_1-B-0, A+0, dim)] = src[RIDX(A+0, B+0, dim)];
    //         dst[RIDX(dim_1-B-1, A+0, dim)] = src[RIDX(A+0, B+1, dim)];
    //         dst[RIDX(dim_1-B-2, A+0, dim)] = src[RIDX(A+0, B+2, dim)];
    //         dst[RIDX(dim_1-B-3, A+0, dim)] = src[RIDX(A+0, B+3, dim)];
    //         dst[RIDX(dim_1-B-4, A+0, dim)] = src[RIDX(A+0, B+4, dim)];
    //         dst[RIDX(dim_1-B-5, A+0, dim)] = src[RIDX(A+0, B+5, dim)];
    //         dst[RIDX(dim_1-B-6, A+0, dim)] = src[RIDX(A+0, B+6, dim)];
    //         dst[RIDX(dim_1-B-7, A+0, dim)] = src[RIDX(A+0, B+7, dim)];
    //         dst[RIDX(dim_1-B-0, A+1, dim)] = src[RIDX(A+1, B+0, dim)];
    //         dst[RIDX(dim_1-B-1, A+1, dim)] = src[RIDX(A+1, B+1, dim)];
    //         dst[RIDX(dim_1-B-2, A+1, dim)] = src[RIDX(A+1, B+2, dim)];
    //         dst[RIDX(dim_1-B-3, A+1, dim)] = src[RIDX(A+1, B+3, dim)];
    //         dst[RIDX(dim_1-B-4, A+1, dim)] = src[RIDX(A+1, B+4, dim)];
    //         dst[RIDX(dim_1-B-5, A+1, dim)] = src[RIDX(A+1, B+5, dim)];
    //         dst[RIDX(dim_1-B-6, A+1, dim)] = src[RIDX(A+1, B+6, dim)];
    //         dst[RIDX(dim_1-B-7, A+1, dim)] = src[RIDX(A+1, B+7, dim)];
    //         dst[RIDX(dim_1-B-0, A+2, dim)] = src[RIDX(A+2, B+0, dim)];
    //         dst[RIDX(dim_1-B-1, A+2, dim)] = src[RIDX(A+2, B+1, dim)];
    //         dst[RIDX(dim_1-B-2, A+2, dim)] = src[RIDX(A+2, B+2, dim)];
    //         dst[RIDX(dim_1-B-3, A+2, dim)] = src[RIDX(A+2, B+3, dim)];
    //         dst[RIDX(dim_1-B-4, A+2, dim)] = src[RIDX(A+2, B+4, dim)];
    //         dst[RIDX(dim_1-B-5, A+2, dim)] = src[RIDX(A+2, B+5, dim)];
    //         dst[RIDX(dim_1-B-6, A+2, dim)] = src[RIDX(A+2, B+6, dim)];
    //         dst[RIDX(dim_1-B-7, A+2, dim)] = src[RIDX(A+2, B+7, dim)];
    //         dst[RIDX(dim_1-B-0, A+3, dim)] = src[RIDX(A+3, B+0, dim)];
    //         dst[RIDX(dim_1-B-1, A+3, dim)] = src[RIDX(A+3, B+1, dim)];
    //         dst[RIDX(dim_1-B-2, A+3, dim)] = src[RIDX(A+3, B+2, dim)];
    //         dst[RIDX(dim_1-B-3, A+3, dim)] = src[RIDX(A+3, B+3, dim)];
    //         dst[RIDX(dim_1-B-4, A+3, dim)] = src[RIDX(A+3, B+4, dim)];
    //         dst[RIDX(dim_1-B-5, A+3, dim)] = src[RIDX(A+3, B+5, dim)];
    //         dst[RIDX(dim_1-B-6, A+3, dim)] = src[RIDX(A+3, B+6, dim)];
    //         dst[RIDX(dim_1-B-7, A+3, dim)] = src[RIDX(A+3, B+7, dim)];
    //         dst[RIDX(dim_1-B-0, A+4, dim)] = src[RIDX(A+4, B+0, dim)];
    //         dst[RIDX(dim_1-B-1, A+4, dim)] = src[RIDX(A+4, B+1, dim)];
    //         dst[RIDX(dim_1-B-2, A+4, dim)] = src[RIDX(A+4, B+2, dim)];
    //         dst[RIDX(dim_1-B-3, A+4, dim)] = src[RIDX(A+4, B+3, dim)];
    //         dst[RIDX(dim_1-B-4, A+4, dim)] = src[RIDX(A+4, B+4, dim)];
    //         dst[RIDX(dim_1-B-5, A+4, dim)] = src[RIDX(A+4, B+5, dim)];
    //         dst[RIDX(dim_1-B-6, A+4, dim)] = src[RIDX(A+4, B+6, dim)];
    //         dst[RIDX(dim_1-B-7, A+4, dim)] = src[RIDX(A+4, B+7, dim)];
    //         dst[RIDX(dim_1-B-0, A+5, dim)] = src[RIDX(A+5, B+0, dim)];
    //         dst[RIDX(dim_1-B-1, A+5, dim)] = src[RIDX(A+5, B+1, dim)];
    //         dst[RIDX(dim_1-B-2, A+5, dim)] = src[RIDX(A+5, B+2, dim)];
    //         dst[RIDX(dim_1-B-3, A+5, dim)] = src[RIDX(A+5, B+3, dim)];
    //         dst[RIDX(dim_1-B-4, A+5, dim)] = src[RIDX(A+5, B+4, dim)];
    //         dst[RIDX(dim_1-B-5, A+5, dim)] = src[RIDX(A+5, B+5, dim)];
    //         dst[RIDX(dim_1-B-6, A+5, dim)] = src[RIDX(A+5, B+6, dim)];
    //         dst[RIDX(dim_1-B-7, A+5, dim)] = src[RIDX(A+5, B+7, dim)];
    //         dst[RIDX(dim_1-B-0, A+6, dim)] = src[RIDX(A+6, B+0, dim)];
    //         dst[RIDX(dim_1-B-1, A+6, dim)] = src[RIDX(A+6, B+1, dim)];
    //         dst[RIDX(dim_1-B-2, A+6, dim)] = src[RIDX(A+6, B+2, dim)];
    //         dst[RIDX(dim_1-B-3, A+6, dim)] = src[RIDX(A+6, B+3, dim)];
    //         dst[RIDX(dim_1-B-4, A+6, dim)] = src[RIDX(A+6, B+4, dim)];
    //         dst[RIDX(dim_1-B-5, A+6, dim)] = src[RIDX(A+6, B+5, dim)];
    //         dst[RIDX(dim_1-B-6, A+6, dim)] = src[RIDX(A+6, B+6, dim)];
    //         dst[RIDX(dim_1-B-7, A+6, dim)] = src[RIDX(A+6, B+7, dim)];
    //         dst[RIDX(dim_1-B-0, A+7, dim)] = src[RIDX(A+7, B+0, dim)];
    //         dst[RIDX(dim_1-B-1, A+7, dim)] = src[RIDX(A+7, B+1, dim)];
    //         dst[RIDX(dim_1-B-2, A+7, dim)] = src[RIDX(A+7, B+2, dim)];
    //         dst[RIDX(dim_1-B-3, A+7, dim)] = src[RIDX(A+7, B+3, dim)];
    //         dst[RIDX(dim_1-B-4, A+7, dim)] = src[RIDX(A+7, B+4, dim)];
    //         dst[RIDX(dim_1-B-5, A+7, dim)] = src[RIDX(A+7, B+5, dim)];
    //         dst[RIDX(dim_1-B-6, A+7, dim)] = src[RIDX(A+7, B+6, dim)];
    //         dst[RIDX(dim_1-B-7, A+7, dim)] = src[RIDX(A+7, B+7, dim)];
    //     }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version, loop unrolling.";
void smooth(int dim, pixel *src, pixel *dst)
{
    int i, j;
    // reducing procedure calls
    // use temp var
    pixel current_pixel;
    pixel *pcurrent_pixel = &current_pixel;

    // four corners
    i = 0;
    j = 0;
    pcurrent_pixel->red =
        (unsigned short)(((int)(src[RIDX(0, 0, dim)].red + src[RIDX(0, 1, dim)].red +
                                src[RIDX(1, 0, dim)].red + src[RIDX(1, 1, dim)].red)) /
                         4);
    pcurrent_pixel->green =
        (unsigned short)(((int)(src[RIDX(0, 0, dim)].green + src[RIDX(0, 1, dim)].green +
                                src[RIDX(1, 0, dim)].green + src[RIDX(1, 1, dim)].green)) /
                         4);
    pcurrent_pixel->blue =
        (unsigned short)(((int)(src[RIDX(0, 0, dim)].blue + src[RIDX(0, 1, dim)].blue +
                                src[RIDX(1, 0, dim)].blue + src[RIDX(1, 1, dim)].blue)) /
                         4);
    dst[RIDX(0, 0, dim)] = current_pixel;

    i = 0;
    j = dim - 1;
    pcurrent_pixel->red =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].red + src[RIDX(i + 1, j, dim)].red +
                                src[RIDX(i, j - 1, dim)].red + src[RIDX(i + 1, j - 1, dim)].red)) /
                         4);
    pcurrent_pixel->green =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].green + src[RIDX(i + 1, j, dim)].green +
                                src[RIDX(i, j - 1, dim)].green + src[RIDX(i + 1, j - 1, dim)].green)) /
                         4);
    pcurrent_pixel->blue =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].blue + src[RIDX(i + 1, j, dim)].blue +
                                src[RIDX(i, j - 1, dim)].blue + src[RIDX(i + 1, j - 1, dim)].blue)) /
                         4);
    dst[RIDX(i, j, dim)] = current_pixel;

    i = dim - 1;
    j = 0;
    pcurrent_pixel->red =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].red + src[RIDX(i - 1, j, dim)].red +
                                src[RIDX(i, j + 1, dim)].red + src[RIDX(i - 1, j + 1, dim)].red)) /
                         4);
    pcurrent_pixel->green =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].green + src[RIDX(i - 1, j, dim)].green +
                                src[RIDX(i, j + 1, dim)].green + src[RIDX(i - 1, j + 1, dim)].green)) /
                         4);
    pcurrent_pixel->blue =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].blue + src[RIDX(i - 1, j, dim)].blue +
                                src[RIDX(i, j + 1, dim)].blue + src[RIDX(i - 1, j + 1, dim)].blue)) /
                         4);
    dst[RIDX(i, j, dim)] = current_pixel;

    i = dim - 1;
    j = dim - 1;
    pcurrent_pixel->red =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].red + src[RIDX(i - 1, j, dim)].red +
                                src[RIDX(i, j - 1, dim)].red + src[RIDX(i - 1, j - 1, dim)].red)) /
                         4);
    pcurrent_pixel->green =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].green + src[RIDX(i - 1, j, dim)].green +
                                src[RIDX(i, j - 1, dim)].green + src[RIDX(i - 1, j - 1, dim)].green)) /
                         4);
    pcurrent_pixel->blue =
        (unsigned short)(((int)(src[RIDX(i, j, dim)].blue + src[RIDX(i - 1, j, dim)].blue +
                                src[RIDX(i, j - 1, dim)].blue + src[RIDX(i - 1, j - 1, dim)].blue)) /
                         4);
    dst[RIDX(i, j, dim)] = current_pixel;

    // four edges
    j = 0;
    for (i = 1; i < dim - 1; i++)
    {
        pcurrent_pixel->red =
            (unsigned short)(((int)(src[RIDX(i - 1, j, dim)].red + src[RIDX(i - 1, j + 1, dim)].red +
                                    src[RIDX(i, j, dim)].red + src[RIDX(i, j + 1, dim)].red +
                                    src[RIDX(i + 1, j, dim)].red + src[RIDX(i + 1, j + 1, dim)].red)) /
                             6);
        pcurrent_pixel->green =
            (unsigned short)(((int)(src[RIDX(i - 1, j, dim)].green + src[RIDX(i - 1, j + 1, dim)].green +
                                    src[RIDX(i, j, dim)].green + src[RIDX(i, j + 1, dim)].green +
                                    src[RIDX(i + 1, j, dim)].green + src[RIDX(i + 1, j + 1, dim)].green)) /
                             6);
        pcurrent_pixel->blue =
            (unsigned short)(((int)(src[RIDX(i - 1, j, dim)].blue + src[RIDX(i - 1, j + 1, dim)].blue +
                                    src[RIDX(i, j, dim)].blue + src[RIDX(i, j + 1, dim)].blue +
                                    src[RIDX(i + 1, j, dim)].blue + src[RIDX(i + 1, j + 1, dim)].blue)) /
                             6);
        dst[RIDX(i, j, dim)] = current_pixel;
    }

    i = dim - 1;
    for (j = 1; j < dim - 1; j++)
    {
        pcurrent_pixel->red =
            (unsigned short)(((int)(src[RIDX(i, j, dim)].red + src[RIDX(i - 1, j, dim)].red +
                                    src[RIDX(i, j - 1, dim)].red + src[RIDX(i - 1, j - 1, dim)].red +
                                    src[RIDX(i, j + 1, dim)].red + src[RIDX(i - 1, j + 1, dim)].red)) /
                             6);
        pcurrent_pixel->green =
            (unsigned short)(((int)(src[RIDX(i, j, dim)].green + src[RIDX(i - 1, j, dim)].green +
                                    src[RIDX(i, j - 1, dim)].green + src[RIDX(i - 1, j - 1, dim)].green +
                                    src[RIDX(i, j + 1, dim)].green + src[RIDX(i - 1, j + 1, dim)].green)) /
                             6);
        pcurrent_pixel->blue =
            (unsigned short)(((int)(src[RIDX(i, j, dim)].blue + src[RIDX(i - 1, j, dim)].blue +
                                    src[RIDX(i, j - 1, dim)].blue + src[RIDX(i - 1, j - 1, dim)].blue +
                                    src[RIDX(i, j + 1, dim)].blue + src[RIDX(i - 1, j + 1, dim)].blue)) /
                             6);
        dst[RIDX(i, j, dim)] = current_pixel;
    }

    j = dim - 1;
    for (i = 1; i < dim - 1; i++)
    {
        pcurrent_pixel->red =
            (unsigned short)(((int)(src[RIDX(i - 1, j, dim)].red + src[RIDX(i - 1, j - 1, dim)].red +
                                    src[RIDX(i, j, dim)].red + src[RIDX(i, j - 1, dim)].red +
                                    src[RIDX(i + 1, j, dim)].red + src[RIDX(i + 1, j - 1, dim)].red)) /
                             6);
        pcurrent_pixel->green =
            (unsigned short)(((int)(src[RIDX(i - 1, j, dim)].green + src[RIDX(i - 1, j - 1, dim)].green +
                                    src[RIDX(i, j, dim)].green + src[RIDX(i, j - 1, dim)].green +
                                    src[RIDX(i + 1, j, dim)].green + src[RIDX(i + 1, j - 1, dim)].green)) /
                             6);
        pcurrent_pixel->blue =
            (unsigned short)(((int)(src[RIDX(i - 1, j, dim)].blue + src[RIDX(i - 1, j - 1, dim)].blue +
                                    src[RIDX(i, j, dim)].blue + src[RIDX(i, j - 1, dim)].blue +
                                    src[RIDX(i + 1, j, dim)].blue + src[RIDX(i + 1, j - 1, dim)].blue)) /
                             6);
        dst[RIDX(i, j, dim)] = current_pixel;
    }

    i = 0;
    for (j = 1; j < dim - 1; j++)
    {
        pcurrent_pixel->red =
            (unsigned short)(((int)(src[RIDX(i, j, dim)].red + src[RIDX(i + 1, j, dim)].red +
                                    src[RIDX(i, j - 1, dim)].red + src[RIDX(i + 1, j - 1, dim)].red +
                                    src[RIDX(i, j + 1, dim)].red + src[RIDX(i + 1, j + 1, dim)].red)) /
                             6);
        pcurrent_pixel->green =
            (unsigned short)(((int)(src[RIDX(i, j, dim)].green + src[RIDX(i + 1, j, dim)].green +
                                    src[RIDX(i, j - 1, dim)].green + src[RIDX(i + 1, j - 1, dim)].green +
                                    src[RIDX(i, j + 1, dim)].green + src[RIDX(i + 1, j + 1, dim)].green)) /
                             6);
        pcurrent_pixel->blue =
            (unsigned short)(((int)(src[RIDX(i, j, dim)].blue + src[RIDX(i + 1, j, dim)].blue +
                                    src[RIDX(i, j - 1, dim)].blue + src[RIDX(i + 1, j - 1, dim)].blue +
                                    src[RIDX(i, j + 1, dim)].blue + src[RIDX(i + 1, j + 1, dim)].blue)) /
                             6);
        dst[RIDX(i, j, dim)] = current_pixel;
    }


    for (i = 1; i < dim - 1; i++)
    {
        for (j = 1; j < dim - 1; j++)
        {
            pcurrent_pixel->red =
                (unsigned short)(((int)(src[RIDX(i + 1, j, dim)].red + src[RIDX(i + 1, j - 1, dim)].red +
                                        src[RIDX(i, j, dim)].red + src[RIDX(i - 1, j, dim)].red +
                                        src[RIDX(i, j - 1, dim)].red + src[RIDX(i - 1, j - 1, dim)].red +
                                        src[RIDX(i, j + 1, dim)].red + src[RIDX(i - 1, j + 1, dim)].red +
                                        src[RIDX(i + 1, j + 1, dim)].red)) /
                                 9);
            pcurrent_pixel->green =
                (unsigned short)(((int)(src[RIDX(i + 1, j, dim)].green + src[RIDX(i + 1, j - 1, dim)].green +
                                        src[RIDX(i, j, dim)].green + src[RIDX(i - 1, j, dim)].green +
                                        src[RIDX(i, j - 1, dim)].green + src[RIDX(i - 1, j - 1, dim)].green +
                                        src[RIDX(i, j + 1, dim)].green + src[RIDX(i - 1, j + 1, dim)].green +
                                        src[RIDX(i + 1, j + 1, dim)].green)) /
                                 9);
            pcurrent_pixel->blue =
                (unsigned short)(((int)(src[RIDX(i + 1, j, dim)].blue + src[RIDX(i + 1, j - 1, dim)].blue +
                                        src[RIDX(i, j, dim)].blue + src[RIDX(i - 1, j, dim)].blue +
                                        src[RIDX(i, j - 1, dim)].blue + src[RIDX(i - 1, j - 1, dim)].blue +
                                        src[RIDX(i, j + 1, dim)].blue + src[RIDX(i - 1, j + 1, dim)].blue +
                                        src[RIDX(i + 1, j + 1, dim)].blue)) /
                                 9);
            dst[RIDX(i, j, dim)] = current_pixel;
        }
    }
}



/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

