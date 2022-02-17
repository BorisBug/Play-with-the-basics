#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// screen variables
int screen_x = 0; // screen left
int screen_y = 0; // screen right
int screen_sx = 50; // screen size horizontal
int screen_sy = 10; // screen size vertical

// point variables
float px = 1; // start on the left
float py = 1; // start on the top of the window
float px_increment = 1; // move to the left
float py_increment = 0; // no vertical movement

void xy_print(int x, int y, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    char buffer[100];
    vsprintf(buffer, fmt, arg);
    printf("%c[%d;%df%s", 0x1B, y+1, x+1, buffer);
    va_end(arg);
}

void print_borders()
{
    // horizontal lines
    for(int x=screen_x; x<screen_x+screen_sx; x++)
    {
        xy_print(x, screen_y, ".");
        xy_print(x, screen_y+screen_sy-1, ".");
    }

    // vertical lines
    for(int y=screen_y+1; y<screen_y+screen_sy; y++)
    {
        xy_print(screen_x, y, ":");
        xy_print(screen_x+screen_sx-1, y, ":");
    }
}

void delay(int milli)
{
    clock_t now,start;
    long pause = milli * CLOCKS_PER_SEC / 1000;
    now = start = clock();
    while(now-start < pause)
        now = clock();
}

// modify this function
void game()
{
    // delete the previous position
    xy_print(px, py, " ");

    // increment positions
    px += px_increment; 
    py += py_increment;

    // do not go over the right side
    if(px>=screen_x+screen_sx)
        px = screen_x;
    // do not go over the left side
    if(px<screen_x)
        px = screen_x+screen_sx-1;
    // do not go over the bottom side
    if(py>=screen_y+screen_sy)
        py = screen_y;
    // do not go over the top side
    if(py<screen_y)
        py = screen_y+screen_sy-1;

    // show the new position
    xy_print(px, py, "O");
}

int main()
{   
    while(1)
    {
        game(); 
        print_borders();
        xy_print(screen_x, screen_y+screen_sy, "Press Ctrl-C to exit\n");
        delay(50);  
    } 

    return 0;
}