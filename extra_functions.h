#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* -------------- CONSTANTS -------------- */

#define base 10

/* -------------- DATA STRUCTURES -------------- */

typedef struct {
    unsigned char blue, green, red;
} Pixel;

/* -------------- FUNCTIONS -------------- */

int mod(int x);
void swap(int *x, int *y);
int min(int x, int y);
int max(int x, int y);
int check(int height, int width, int x, int y);
int num(char *S);
int compare(Pixel color1, Pixel color2);