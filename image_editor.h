#include "bmp_header.h"
#include "extra_functions.h"

/* -------------- CONSTANTS -------------- */

#define N 1001
#define IniHeight 768
#define IniWidth 1024
#define plane 1
#define BmpFileheaderSize 14
#define BmpInfoheaderSize 40
#define BitPix 24

/* -------------- DATA STRUCTURES -------------- */

typedef struct {
    int x, y;
} Coord;

typedef struct {
    Pixel color;
    int width;
} Line;

typedef struct {
    bmp_fileheader *fh;
    bmp_infoheader *ih;
    Pixel **bitmap;
} Bmp;
