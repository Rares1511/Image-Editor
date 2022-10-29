#include "image_editor.h"

/* functions which saves the current opened image at the location offered */
void save(char *path, Bmp *img) {
    // we create our file with the intention of writing the image information
    FILE *file = fopen(path, "wb");
    // we write the information of the file header and info header
    fwrite(img->fh, 1, BmpFileheaderSize, file);
    fwrite(img->ih, 1, BmpInfoheaderSize, file);
    int height = img->ih->height;
    int width = img->ih->width;
    int rest = (width * 3) % 4;
    if ( rest != 0 )
        rest = 4 - rest;
    // we write the information of the image itself with caution to the extra empty bytes
    // needed to complete
    for ( int i = 0; i < height; i++ ) {
        for ( int j = 0; j < width; j++ )
            fwrite(&img->bitmap[i][j], 1, sizeof(Pixel), file);
        if ( rest != 0 )
            fwrite(&img->bitmap[i][width], 1, rest, file);
    }
    // finally we close the file since we've put the image inside
    fclose(file);
}

/* functions which loads the image from the path offered inside our programm
   so it is ready to be edited */
Bmp *edit(char *path) {
    // we create an empty image type variable
    Bmp *img = malloc(sizeof(Bmp));
    // we open the file from which to read the image
    FILE *file = fopen(path, "rb");
    // allocate the space for the info and file header
    img->fh = malloc(BmpFileheaderSize);
    img->ih = malloc(BmpInfoheaderSize);
    // read the image info and file header information
    fread(img->fh, 1, BmpFileheaderSize, file);
    fread(img->ih, 1, BmpInfoheaderSize, file);
    int height = img->ih->height;
    int width = img->ih->width;
    int rest = (width * 3) % 4;
    if ( rest != 0 ) {
        rest = 4 - rest;
        width++;
    }
    // allocate the space for the image matrix
    img->bitmap = malloc(height * sizeof(Pixel*));
    for ( int i = 0; i < height; i++ )
        img->bitmap[i] = malloc(width * sizeof(Pixel));
    if ( rest != 0 )
        width--;
    // we read the image matrix with caution to the extra empty bytes
    for ( int i = 0; i < height; i++ ) {
        for ( int j = 0; j < width; j++ )
            fread(&img->bitmap[i][j], 1, sizeof(Pixel), file);
        if ( rest != 0 )
            fread(&img->bitmap[i][width], 1, rest, file);
    }
    // we close the file
    fclose(file);
    // we return the read image
    return img;
}

/* functions which takes the image from path and inserts in our current loaded image
   from point (x, y) and then returns the new image formed */
void insert(char *path, int y, int x, Bmp *old) {
    int i = 0, j = 0;
    // we read our image from the path
    Bmp *new = edit ( path );
    int height = old->ih->height;
    int width = old->ih->width;
    int newHeight = new->ih->height;
    int newWidth = new->ih->width;
    // we then insert it with caution not to go over the width and height of either image
    for ( i = 0; i < newHeight && i + x < height; i++ )
        for ( j = 0; j < newWidth && j + y < width; j++ )
            old->bitmap[i + x][j + y]= new->bitmap[i][j];
}

/* function which draws a filled square with the center (x, y) with the color of the line */
void draw(int x, int y, Bmp *img, Line line) {
    int height = img->ih->height;
    int width = img->ih->width;
    for ( int i = x - line.width; i <= x + line.width; i++ )
        for ( int j = y - line.width; j <= y + line.width; j++ )
            if ( check(height, width, i, j) == 1 )
                img->bitmap[i][j] = line.color;
}

/* function which draws a line from point (x1, y1) to point (x2, y2) 
   by having one variable either x or y incremented by 1 and the other 
   calculated with the line's formula; we choose which variable to increment
   by which distance on OX or OY is bigger */
void draw_line(int x1, int y1, int x2, int y2, Line line, Bmp *img) {
    int disx = x2 - x1;
    int disy = y2 - y1;
    line.width = line.width / 2;
    if ( mod(disx) > mod(disy) ) {
        int x = min(x1, x2);
        int end = max(x1, x2);
        do {
            int y = ((x - x1) * disy + disx * y1) / disx;
            draw(x, y, img, line);
            x++;
        } while ( x <= end );
    } else {
        int end = max(y1, y2);
        int y = min(y1, y2);
        do {
            int x = ((y - y1) * disx + x1 * disy) / disy;
            draw(x, y, img, line);
            y++;
        } while ( y <= end );
    }
}

/* functions which draws a rectangle from point (x, y) which has a height of h and
   width of w by using 4 independent calls of the draw_line funciton */
void draw_rectangle(int x1, int y1, int h, int w, Line line, Bmp *img) {
    int x2 = x1 + h;
    int y2 = y1 + w;
    draw_line(x1, y1, x2, y1, line, img);
    draw_line(x2, y1, x2, y2, line, img);
    draw_line(x2, y2, x1, y2, line, img);
    draw_line(x1, y2, x1, y1, line, img);
}

/* function which draws a triangle formed by the 3 points offered in the 2 vectors
   Point i = (x[i], y[i]) by using 3 independent calls of the draw_line function */
void draw_triangle(int *x, int *y, Line line, Bmp *img) {
    for ( int i = 0; i < 2; i++ )
        draw_line(x[i], y[i], x[i + 1], y[i + 1], line, img);
    draw_line(x[2], y[2], x[0], y[0], line, img);
}

/* function which fills the image from the point (x, y) and goes to the neightboors
   that have the same color and so on until it can't go on, it then replaces all of 
   the visited points colors with the new color offered */
void fill(int x, int y, Pixel color, Bmp *img) {
    int H = img->ih->height;
    int W = img->ih->width;
    int dirx[] = { 0, -1, 0, 1 };
    int diry[] = { -1, 0, 1, 0 };
    // we can use a queue to input our points and add their neighboors
    Coord *queue = malloc(H * W * sizeof(Coord));
    // we also use a visited vector so that we do not visit the same point twice
    // the points (x, y) are transformed with the formula into one number
    // (x, y) -> x * width + y
    short int *visited = malloc(H * W * sizeof(short int));
    Pixel oldClr = img->bitmap[x][y];
    // we add the first point and mark it as visited
    queue[0] = (Coord){ x, y };
    visited[x * W + y] = 1;
    // until the queue is empty we add the unvisited neighboors that have the same color 
    // as the inital point and the swap their color with the given one
    int st = 0, dr = 0;
    while ( st <= dr ) {
        int currentx = queue[st].x;
        int currenty = queue[st].y;
        img->bitmap[currentx][currenty] = color;
        for ( int i = 0; i < 4; i++ ) {
            int newx = currentx + dirx[i];
            int newy = currenty + diry[i];
            if ( check(H, W, newx, newy) ) {
                if ( compare(oldClr, img->bitmap[newx][newy]) ) {
                    if ( visited[newx * W + newy] == 0 ) {
                        queue[++dr] = (Coord){ newx, newy };
                        visited[newx * W + newy] = 1;
                    }
                }
            }
        }
        st++;
    }
}

/* main function which reads the input untill the quit command is given */
int main() {
    char S[N];
    Bmp *img = NULL;
    Line line = { 0, 0, 0, 1 };
    while (1) {
        fgets(S, N, stdin);
        int len = (int)strlen(S) - 1;
        if ( S[len] == '\n' )
            S[len] = 0;
        char *command = strtok(S, " ");
        if ( strcmp(command, "save") == 0 ) {
            char *path = strtok(NULL, " ");
            save(path, img);
        } else if ( strcmp(command, "edit") == 0 ) {
            char *path = strtok(NULL, " ");
            img = edit(path);
        } else if ( strcmp(command, "insert") == 0 ) {
            char *path = strtok(NULL, " ");
            int y = num(strtok(NULL, " "));
            int x = num(strtok(NULL, " "));
            insert(path, y, x, img);
        } else if ( strcmp(command, "set") == 0 ) {
            char *option = strtok(NULL, " ");
            if ( strcmp(option, "draw_color") == 0 ) {
                char *R = strtok(NULL, " ");
                char *G = strtok(NULL, " ");
                char *B = strtok(NULL, " ");
                line.color = (Pixel) { num(B), num(G), num(R) };
            }
            if ( strcmp(option, "line_width") == 0 ) {
                line.width = num(strtok(NULL, " "));
                if ( line.width == 0 )
                    line.width = 1;
            }
        } else if ( strcmp(command, "draw") == 0 ) {
            char *option = strtok(NULL, " ");
            if ( strcmp(option, "line") == 0 ) {
                int y1 = num(strtok(NULL, " "));
                int x1 = num(strtok(NULL, " "));
                int y2 = num(strtok(NULL, " "));
                int x2 = num(strtok(NULL, " "));
                draw_line(x1, y1, x2, y2, line, img);
            } else if ( strcmp(option, "rectangle") == 0 ) {
                int y1 = num(strtok(NULL, " "));
                int x1 = num(strtok(NULL, " "));
                int height = num(strtok(NULL, " "));
                int width = num(strtok(NULL, " "));
                draw_rectangle(x1, y1, width, height, line, img);
            } else if ( strcmp(option, "triangle") == 0 ) {
                int *x = malloc(3 * sizeof(int)), *y = malloc(3 * sizeof (int));
                for ( int i = 0; i < 3; i++ ) {
                    y[i] = num(strtok(NULL, " "));
                    x[i] = num(strtok(NULL, " "));
                }
                draw_triangle(x, y, line, img);
            }
        } else if ( strcmp(command, "fill") == 0 ) {
                int y = num(strtok(NULL, " "));
                int x = num(strtok(NULL, " "));
                if ( compare(line.color, img->bitmap[x][y]) == 0 )
                    fill(x, y, line.color, img);
        } else if ( strcmp(command, "quit") == 0 ) {
            break;
        }
    }
    return 0;
}
