/**
 * @file starry-night.c
 * @author Boris Snäll (borissnaell@gmail.com)
 * @brief Demonstration of coding
 *          - Use of arrays (and its [] operator)
 *          - Use of structs
 *          - Use of pointers to structs (-> operator)
 *          - Use of array of structs
 *          - Use of mod in equations (% operator)
 *          - Use of dynamic memory (malloc, free, etc)
 * @version 0.1
 * @date 2021-11-02
 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "xy-print.h"

typedef struct
{   int x, y; // position
    int state; // state
} star_t;

typedef struct
{   int x, y; // position
    int sx, sy; // size of frame
    star_t *star; // the array of stars
    unsigned size; // amount of stars
    unsigned fadein; // fade in counter
} starry_night_t;

const char star_state[] =
"................. .........................."\
"........................... ................"\
"........... ................................"\
"........................ ............*......"\
"............................................"\
"......+....................................."\
"............................. ............x."\
"*.....          .                           ";
#define STAR_SIZE sizeof(star_state)
#define DIST(n1,n2) abs((n2)-(n1))

// place the star in the sky
void night_star_print(star_t *star)
{
    assert(star);
    assert(star->state<STAR_SIZE);

    int col = (star->state%95!=0) ? (star->state%70!=0 ? 1 : 2) : 3;
    attron(COLOR_PAIR(col));
    xy_print_c(star->x, star->y, star_state[star->state]);
    attroff(COLOR_PAIR(col));
}

// remove the star from the sky
void night_star_clear(star_t *star)
{
    assert(star);
    xy_print_c(star->x, star->y, ' ');
    star->state = star->x = star->y = -1;
}

// initialize a star
void night_star_init(starry_night_t *night, star_t *star)
{
    assert(night && star);
    int x, y, try = 15;
    star->state = 0;
    bool repeat = false;

    // set a random position that keeps a minimum distance to another star

    do
    {   repeat = false;
        x = night->x + rand()%night->sx;
        y = night->y + rand()%night->sy;

        int minX = 4; // minimum distance
        int minY = 2; // minimum distance

        for(int i=0; i<night->size; i++)
        if(star != &night->star[i] && night->star[i].x>=0 && night->star[i].y>=0)
        if(DIST(x, night->star[i].x) < minX && DIST(y, night->star[i].y) < minY)
        {
            repeat = true;
            break;
        }
    } while(repeat && try-->0);

    star->x = x;
    star->y = y;
}

// increment the state of a star
void night_star_inc(starry_night_t *night, star_t *star)
{
    assert(night && star);
    star->state = (star->state+1)%STAR_SIZE;

    // start of a new cycle?
    if(star->state==0)
    {
        night_star_clear(star);         // clear
        night_star_init(night, star);   // relocate
    }
}

/**
 * @brief Initialize the variables for the "starry night"
 * @param night the night sky window
 * @param size amount of stars in the sky
 * @param x position x on screen
 * @param y position y on screen
 * @param sx size x of window
 * @param sy size y of window
 */
void night_init(starry_night_t *night, unsigned size, int x, int y, int sx, int sy)
{
    assert(night);
    assert(sx>0 && sy>0 && size>0);

    night->x = x;
    night->y = y;
    night->sx = sx;
    night->sy = sy;
    night->fadein = 0;
    night->size = size;
    night->star = (star_t*)malloc(size*sizeof(star_t));

    if(!night->star)
        exit(1); // no memory!

    // stars start with invisible state (-1)
    for(int i=0; i<size; i++)
        night->star[i].state = night->star[i].x = night->star[i].y = -1;

    // initialize each star with random values
    for(int i=0; i<size; i++)
    {
        night_star_init(night, &night->star[i]);
        night->star[i].state = rand()%STAR_SIZE;
    }
}

// free memory of the night container
void night_destroy(starry_night_t **night)
{
    assert(night && *night && (*night)->star);
    free((*night)->star);
    *night = NULL;
}

// animate the stars in night
void night_run(starry_night_t *night)
{
    assert(night);

    // print frame
    // xy_print_c(night->x, night->y,'@');
    // xy_print_c(night->x+night->sx-1, night->y, '@');
    // xy_print_c(night->x, night->y+night->sy-1,'@');
    // xy_print_c(night->x+night->sx-1, night->y+night->sy-1, '@');

    // increment fade in counter
    night->fadein++;

    // print starts and increment state
    for(int i=0; i<night->size && i<night->fadein/7; i++)
    {
        night_star_print(&night->star[i]);
        night_star_inc(night, &night->star[i]);
    }
}

int main()
{
    // allocate the starry night object
    starry_night_t *night = (starry_night_t*)malloc(sizeof(starry_night_t));

    if(!night)
        exit(1);

    initscr();      // initialize ncurses
    start_color();  // start the color handling
    noecho();       // no echo on getch
    curs_set(0);    // cursor hidden
    nodelay(stdscr, TRUE); // no wait for key to be pressed

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);

    int x = getbegx(stdscr);
    int y = getbegy(stdscr);
    int sx = getmaxx(stdscr);
    int sy = getmaxy(stdscr);
    
    srand(time(NULL));  // seed for randomizer
    night_init(night,30, x, y, sx, sy); // initialize the "starry night" variables

    do {
        night_run(night);   // run
        xy_print_fmt(x, sy-1,"Press any key to exit..");
        wrefresh(stdscr);   // refresh screen
        napms(150);         // make a pause (0.15s)
    } while(getch()==-1);   // finish if a key is pressed

    night_destroy(&night);  // free memory

    endwin(); 
    return 0;
}