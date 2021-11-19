/**
 * @file animate.c
 * @author Boris Snäll (borissnaell@gmail.com)
 * @brief Demonstration of reusability
 * @version 0.1
 * @date 2021-11-02
 */
#include "xy-print.h"

#define FITIN(a,size) (((a)<0?(a)+(size):(a))%(size)) // to use with offsets

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
}

void xy_label_cross(int px, int py, int size, int offX, int offY)
{
    xy_print_fmt(px, py, "cross(%d,%d,%d,%d,%d)   ", px, py, size, offX, offY);
    xy_cross(px, py+1, size, offX, offY);
}

int play_with_xy_cross(int i, int pause)
{
    for(int j=0; j<3; j++)
    {
        if(j==0)
            xy_label_cross(44+j*7, 0, 7, i/2, 0);
        else
            xy_cross(44+j*7, 1, 7, i/2, 0);
        xy_cross(44+j*7, 7, 7, i/2, 0);
    }

    xy_label_cross( 0, 0, 13, i%13, 0);
    xy_label_cross(14, 2, 11, 0, i%11);
    xy_label_cross(26, 4, 9, i%9,(i/2)%9);

    int ss = 7+i/5;
    xy_label_cross(66, 0, ss, 0, 0); 
    xy_line_x(66, ss+1, ss+1, ' ');
    xy_line_y(66+ss, 1, ss+1, ' ');

    xy_line_y(36+i-1, 6, 8, ' '); // clean pre
    xy_label_cross(36+i, 6, 7, 0, 0); 
    xy_line_y(36+i+7, 7, 8, ' '); // clean post

    xy_print_fmt(0,15, "Press any key to exit..");
    napms(pause); // 10 frames per sec
    wrefresh(stdscr);
    return wgetch(stdscr)!=-1;
}

int main()
{
    initscr();      // initialize ncurses
    noecho();       // no echo on getch
    curs_set(0);    // cursor hidden
    nodelay(stdscr, TRUE); // no wait for key to be pressed

    int i=0, j=0, pause=80;
    int exit=0, inc=-15;

    while(!exit)
    {
        // adjust speed & direction
        if(++j%3==0)
        {
            pause += inc;
            if(pause<30 || pause >200)
                inc = -inc;
        }

        // move to one side
        for(; i<34 && !exit; i++)
            exit = play_with_xy_cross(i, pause);

        // move to oposite side
        for(; i>=0 && !exit; i--)
            exit = play_with_xy_cross(i, pause);
    }

    fflush(stdin);
    endwin();
    return 0;
}