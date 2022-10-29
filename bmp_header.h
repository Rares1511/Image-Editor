#pragma pack(1)

/* -------------- CONSTANTS -------------- */

typedef struct {
    unsigned char  fileMarker1;
    unsigned char  fileMarker2;
    unsigned int   bfSize;
    unsigned short unused1;
    unsigned short unused2;
    unsigned int   imageDataOffset;
} bmp_fileheader;

typedef struct {
    unsigned int   biSize;
    signed int     width;
    signed int     height;
    unsigned short planes;
    unsigned short bitPix;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
} bmp_infoheader;

#pragma pack()
