#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

// colors for printf
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET   "\x1B[0m"

typedef struct
{    int x, y; // coordinates on axis X & Y
} point_t;

typedef struct
{   point_t pos; // position
    point_t size; // size
} box_t;

typedef struct
{   point_t pos; // position
    point_t inc; // increment
} ball_t;

box_t screen = {.pos.x=0, .pos.y=0, .size.x=50, .size.y=10}; 
ball_t ball = {.pos.x=1, .pos.y=1, .inc.x=1, .inc.y=1};

// bounces
int bounces = 0;

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
    for(int x=screen.pos.x; x<screen.pos.x+screen.size.x; x++)
    {
        set_color(ball.pos.y==screen.pos.y ? RED : RESET);
        xy_print(x, screen.pos.y, ".");
        set_color(ball.pos.y==screen.pos.y+screen.size.y-1 ? RED : RESET);
        xy_print(x, screen.pos.y+screen.size.y-1, ".");
    }

    // vertical lines
    for(int y=screen.pos.y+1; y<screen.pos.y+screen.size.y; y++)
    {
        set_color(ball.pos.x==screen.pos.x ? RED : RESET);
        xy_print(screen.pos.x, y, ":");
        set_color(ball.pos.x==screen.pos.x+screen.size.x-1 ? RED : RESET);
        xy_print(screen.pos.x+screen.size.x-1, y, ":");
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
    xy_print(ball.pos.x, ball.pos.y, " ");

    // increment positions
    ball.pos.x += ball.inc.x; 
    ball.pos.y += ball.inc.y;

    // do not go over the top side
    if(ball.pos.y<=screen.pos.y)
    {
        bounces++;
        ball.inc.y *= -1;
    }
    // do not go over the left side
    if(ball.pos.x<=screen.pos.x)
    {
        bounces++;
        ball.inc.x *= -1;
    }
    // do not go over the right side
    if(ball.pos.x>=screen.pos.x+screen.size.x-1)
    {
        bounces++;
        ball.inc.x *= -1;
    }
    // do not go over the bottom side
    if(ball.pos.y>=screen.pos.y+screen.size.y-1)
    {
        bounces++;
        ball.inc.y *= -1;
    }

    // show the new position
    xy_print(ball.pos.x, ball.pos.y, "O");
}

int main()
{   
    char status[30];

    while(1)
    {
        play(); 
        print_borders();
        sprintf(status, "Bounces: %d", bounces);
        xy_print(screen.pos.x+screen.size.x-strlen(status), screen.pos.y+screen.size.y, status);
        xy_print(screen.pos.x, screen.pos.y+screen.size.y, "Press Ctrl-C to exit\n");
        delay(50);  
    } 

    return 0;
}