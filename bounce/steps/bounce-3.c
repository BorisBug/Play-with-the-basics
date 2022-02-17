#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// colors for printf
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET   "\x1B[0m"

// screen variables
int screen_x = 0; // screen reference position left
int screen_y = 0; // screen reference position right
int screen_sx = 50; // screen size: horizontal axis
int screen_sy = 10; // screen size: vertical axis

// point variables
float px = 1; // ball position: horizontal axis
float py = 1; // ball position: vertical axis
float px_increment = 1; // movement: horizontal axis
float py_increment = 1; // movement: vertical axis

void set_color(const char *color)
{
    if(!color)
        color = RESET;
    printf("%s", color);
}

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
        set_color(py==screen_y ? RED : RESET);
        xy_print(x, screen_y, ".");
        set_color(py==screen_y+screen_sy-1 ? RED : RESET);
        xy_print(x, screen_y+screen_sy-1, ".");
    }

    // vertical lines
    for(int y=screen_y+1; y<screen_y+screen_sy; y++)
    {
        set_color(px==screen_x ? RED : RESET);
        xy_print(screen_x, y, ":");
        set_color(px==screen_x+screen_sx-1 ? RED : RESET);
        xy_print(screen_x+screen_sx-1, y, ":");
    }

    set_color(RESET);
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
void play()
{
    // delete the previous position
    xy_print(px, py, " ");

    // increment positions
    px += px_increment; 
    py += py_increment;

    // do not go over the top side
    if(py<=screen_y)
        py_increment *= -1;
    // do not go over the left side
    if(px<=screen_x)
        px_increment *= -1;
    // do not go over the right side
    if(px>=screen_x+screen_sx-1)
        px_increment *= -1;
    // do not go over the bottom side
    if(py>=screen_y+screen_sy-1)
        py_increment *= -1;

    // show the new position
    xy_print(px, py, "O");
}

int main()
{   
    while(1)
    {
        play(); 
        print_borders();
        xy_print(screen_x, screen_y+screen_sy, "Press Ctrl-C to exit\n");
        delay(50);  
    } 

    return 0;
}