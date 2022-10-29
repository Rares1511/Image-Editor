#include "extra_functions.h"

/* return the absolute value of the number offered */
int mod(int x) {
    if ( x < 0 )
        x = -x;
    return x;
}

/* swaps the 2 numbers */
void swap(int *x, int *y) {
    int t = *x;
    *x = *y;
    *y = t;
}

/* returns the minimum value of the 2 numbers offered */
int min(int x, int y) {
    if ( x < y )
        return x;
    return y;
}

/* returns the maximum value of the 2 numbers offered */
int max(int x, int y) {
    if ( x < y )
        return y;
    return x;
}

/* returns whether the point (x, y) is inside the rectangle (0, 0, H, W) */
int check(int H, int W, int x, int y) {
    if ( x >= 0 && x < H && 0 <= y && y < W )
        return 1;
    return 0;
}

/* returns the numberical value that is contained in a vector of characters */
int num(char *S) {
    int nr = 0;
    if ( S == NULL )
        return 0;
    for ( int i = 0; S[i] != 0; i++ )
        nr = nr * base + (S[i] - '0');
    return nr;
}

/* returns whether the 2 colors offered are the same or not */
int compare(Pixel color1, Pixel color2) {
    if ( color1.red != color2.red )
        return 0;
    if ( color1.blue != color2.blue )
        return 0;
    if ( color1.green != color2.green )
        return 0;
    return 1;
}