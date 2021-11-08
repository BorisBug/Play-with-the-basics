#include "xy-print.h"

#define MAX(a,b)    ((a)>(b)?(a):(b))
#define FITIN(a,size) (((a)<0?(a)+(size):(a))%(size)) // to use with offsets


#define USE_NCURSES 1

void clearscr()
{
    char clear[]={0x1B,'[','2','J',0};
    #if USE_NCURSES
    mvinstr(0,0, clear);
    #else
    printf("%s",clear);
    #endif
}

void delay(int milli)
{
    if (milli <= 0)
        return;

    int now = clock() * 1000 / CLOCKS_PER_SEC;
    int end = now + milli;
    do { now = clock() * 1000 / CLOCKS_PER_SEC;
    } while (now <= end);
}

void xy_goto(int x, int y)
{
    #if USE_NCURSES
    move(y+1,x+1);
    #else
    printf("%c[%d;%df", 0x1B, y+1, x+1);
    #endif
}

void xy_attr_set(int x, int y, int flag)
{
    #if USE_NCURSES
    move(y+1,x+1);
    attrset(flag);
    #endif
}

void xy_print_c(int x, int y, char c)
{
    #if USE_NCURSES
    char str[]={c,0};
    mvaddstr(y+1,x+1,str);
    #else
    printf("%c[%d;%df%c", 0x1B, y+1, x+1, c);
    #endif
}

void xy_print_str(int x, int y, char *str)
{
    #if USE_NCURSES
    mvaddstr(y+1,x+1,str);
    #else
    printf("%c[%d;%df%s", 0x1B, y+1, x+1, str);
    #endif
}

void xy_print_num(int x, int y, int num)
{
    xy_print_fmt(x,y,"%d", num);
}

void xy_print_fmt(int x, int y, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    char buffer[100];
    vsprintf(buffer, fmt, arg);
    #if USE_NCURSES
    mvprintw(y+1,x+1,"%s", buffer);
    #else
    printf("%c[%d;%df%s", 0x1B, y+1, x+1, buffer);
    #endif
    va_end(arg);
}

void xy_line_y(int px, int py, int sizeY, char c)
{
    #if USE_NCURSES
    mvvline(py+1,px+1,c,sizeY);
    #else
    while(sizeY-->0)
        xy_print_c(px,py++,c);
    #endif
}

void xy_line_x(int px, int py, int sizeX, char c)
{
    #if USE_NCURSES
    mvhline(py+1,px+1,c,sizeX);
    #else
    while(sizeX-->0)
        xy_print_c(px++,py,c);
    #endif
}

void xy_cross(int px, int py, int size, int offX, int offY)
{
    char line[200]={0};
    offX %= size;
    offY %= size;

    for(int y=-offY; y<size-offY; y++)
    {
        for(int x=-offX; x<size-offX; x++)
        {
            bool is_leg1 = FITIN(x,size)==FITIN(y,size);
            bool is_leg2 = FITIN(x,size)==size-FITIN(y,size)-1;
            bool is_x = is_leg1 || is_leg2;
            line[x+offX] = is_x ? 'x' : '.';
        }
        
        xy_print_str(px, py+y+offY, line);
    }
    
    #if USE_NCURSES==0
    printf("\n");
    #endif
}