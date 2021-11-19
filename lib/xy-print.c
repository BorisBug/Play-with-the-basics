#include "xy-print.h"

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

/*
// use napms(int milli)
void delay(int milli)
{
    if (milli <= 0)
        return;

    int now = clock() * 1000 / CLOCKS_PER_SEC;
    int end = now + milli;
    do { now = clock() * 1000 / CLOCKS_PER_SEC;
    } while (now <= end);
}
*/

void xy_goto(int x, int y)
{
    #if USE_NCURSES
    move(y,x);
    #else
    printf("%c[%d;%df", 0x1B, y+1, x+1);
    #endif
}

void xy_attr_set(int x, int y, int flag)
{
    #if USE_NCURSES
    move(y,x);
    attrset(flag);
    #endif
}

void xy_print_c(int x, int y, char c)
{
    if(x<0 || y<0)
        return;
        
    #if USE_NCURSES
    char str[]={c,0};
    mvaddstr(y,x,str);
//  mvaddch(y,x,(int)c);
    #else
    printf("%c[%d;%df%c", 0x1B, y+1, x+1, c);
    #endif
}

void xy_print_str(int x, int y, char *str)
{
    #if USE_NCURSES
    mvaddstr(y,x,str);
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
    mvprintw(y,x,"%s", buffer);
    #else
    printf("%c[%d;%df%s", 0x1B, y+1, x+1, buffer);
    #endif
    va_end(arg);
}

void xy_line_y(int px, int py, int sizeY, char c)
{
    #if USE_NCURSES
    mvvline(py,px,c,sizeY);
    #else
    while(sizeY-->0)
        xy_print_c(px,py++,c);
    #endif
}

void xy_line_x(int px, int py, int sizeX, char c)
{
    #if USE_NCURSES
    mvhline(py,px,c,sizeX);
    #else
    while(sizeX-->0)
        xy_print_c(px++,py,c);
    #endif
}