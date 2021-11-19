/**
 * @file fireworks.c
 * @author Boris Snäll (borissnaell@gmail.com)
 * @brief Demonstration of coding
 *          - Use of switch/case
 *          - Use of structs
 *          - Use of pointers to structs (-> operator)
 *          - Use of dynamic memory (malloc, free, etc)
 *          - Use of linked list
 * @version 0.1
 * @date 2021-11-06
 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "xy-print.h"

const char tail_chars[] = "0x::'";
#define TAIL_SIZE (sizeof(tail_chars)-1)
#define STATE_RECYCLE (-1)

typedef struct pos
{   int x;  // x axis position
    int y;  // y axis position
} pos_t;

typedef struct rocket
{   float x, y;     // precise position
    float vx, vy;   // speed
    int state;      // state flying=0, explotion>0
    int boom;       // position for explotion
    pos_t tail[TAIL_SIZE]; // position of the tail
    struct rocket *next; // next rocket on the list
} rocket_t;

typedef struct
{   int x, y;       // position
    int sx, sy;     // size of frame
    int count;      // amount of flying rockets
    rocket_t *rocket; // the list of rockets
} fireworks_t;


#define DIST(n1,n2) ((n2)-(n1)>0?(n2)-(n1):(n1)-(n2))

// get a random color/pair
int get_rand_color()
{
    return rand()%6+1;
}

// place the flying rocket in the sky
void fireworks_rocket_print_tail(rocket_t *rocket)
{
    assert(rocket);
    attron(COLOR_PAIR(1));
    for(int i=0; i<TAIL_SIZE; i++)
        xy_print_c(rocket->tail[i].x, rocket->tail[i].y, tail_chars[i]);
}

// remove the flying rocket from the sky
void fireworks_rocket_clear_tail(rocket_t *rocket)
{
    assert(rocket);    
    for(int i=0; i<TAIL_SIZE; i++)
        xy_print_c(rocket->tail[i].x, rocket->tail[i].y, ' ');
}

// shift the tail the rocket
void fireworks_rocket_shift_tail(rocket_t *rocket)
{
    for(int i=TAIL_SIZE-1; i>0; i--)
    {
        rocket->tail[i].x = rocket->tail[i-1].x; 
        rocket->tail[i].y = rocket->tail[i-1].y; 
    }

    rocket->tail[0].x = rocket->x;
    rocket->tail[0].y = rocket->y;
}

// helper print explosion
void fireworks_rocket_print_explosion_star(rocket_t *rocket, int t)
{
    int col = get_rand_color();
    attron(COLOR_PAIR(col));
    xy_print_c(rocket->x  , rocket->y  , 'X');  // center
    xy_print_c(rocket->x-t, rocket->y-t, '\\'); // top left
    xy_print_c(rocket->x  , rocket->y-t, '|');  // top
    xy_print_c(rocket->x+t, rocket->y-t, '/');  // top right
    xy_print_c(rocket->x-t, rocket->y  , '-');  // left
    xy_print_c(rocket->x+t, rocket->y  , '-');  // right
    xy_print_c(rocket->x-t, rocket->y+t, '/');  // bottom left
    xy_print_c(rocket->x  , rocket->y+t, '|');  // bottom
    xy_print_c(rocket->x+t, rocket->y+t, '\\'); // bottom right
    attroff(COLOR_PAIR(col));
}

// helper print explosion
void fireworks_rocket_print_explosion_debris(rocket_t *rocket, int t)
{
    int col = get_rand_color();
    attron(COLOR_PAIR(col));
    xy_print_c(rocket->x  , rocket->y  , '*');  // center
    xy_print_c(rocket->x-t, rocket->y-t, '/');  // top left
    xy_print_c(rocket->x  , rocket->y-t, '|');  // top
    xy_print_c(rocket->x+t, rocket->y-t, '\\'); // top right
    xy_print_c(rocket->x-t, rocket->y  , '-');  // left
    xy_print_c(rocket->x+t, rocket->y  , '-');  // right
    xy_print_c(rocket->x-t, rocket->y+t, '\\'); // bottom left
    xy_print_c(rocket->x  , rocket->y+t, '|');  // bottom
    xy_print_c(rocket->x+t, rocket->y+t, '/');  // bottom right
    attroff(COLOR_PAIR(col));
}

// helper print explosion
void fireworks_rocket_print_explosion_block(rocket_t *rocket, int t, char c)
{
    int col = get_rand_color();
    attron(COLOR_PAIR(col));

    for(int x=-t; x<=t; x++)
    for(int y=-t; y<=t; y++)
        xy_print_c(rocket->x+x, rocket->y+y, c);

    attroff(COLOR_PAIR(col));
}

// print explosion stages
void fireworks_rocket_print_explosion(rocket_t *rocket)
{
    assert(rocket);
 
    switch(rocket->state-TAIL_SIZE)
    {
    case 1:
        fireworks_rocket_print_explosion_star(rocket, 1);
        break;
    case 2:
        fireworks_rocket_print_explosion_star(rocket, 1);
        fireworks_rocket_print_explosion_star(rocket, 2);
        break;
    case 3:
        fireworks_rocket_print_explosion_block(rocket, 1, ' ');
        fireworks_rocket_print_explosion_star(rocket, 2);
        break;
    case 4:
        fireworks_rocket_print_explosion_block(rocket, 2, ' ');
        fireworks_rocket_print_explosion_block(rocket, 1, '*');
        fireworks_rocket_print_explosion_star(rocket, 3);
        break;
    case 5:
        fireworks_rocket_print_explosion_block(rocket, 3, ' ');
        fireworks_rocket_print_explosion_debris(rocket, 1);
        break;
    case 6:
        fireworks_rocket_print_explosion_block(rocket, 1, ' ');
        fireworks_rocket_print_explosion_debris(rocket, 2);
        break;
    case 7:
        fireworks_rocket_print_explosion_block(rocket, 2, ' ');
        fireworks_rocket_print_explosion_debris(rocket, 3);
        break;
    case 8:
        fireworks_rocket_print_explosion_block(rocket, 3, ' ');
        fireworks_rocket_print_explosion_star(rocket, 4);
        break;
    case 9:
        fireworks_rocket_print_explosion_block(rocket, 4, ' ');
        break;
    default:
        rocket->state = STATE_RECYCLE;
        break;
    }
}

// calculate a starting point, keeping distance to the other rockets
void fireworks_rocket_calc_distance(fireworks_t *fireworks, rocket_t *rocket)
{
    assert(fireworks && rocket);

    int try = 15;
    bool repeat = false;

    do
    {   repeat = false;
        rocket->x = fireworks->x + rand()%fireworks->sx;
        rocket->y = fireworks->y + fireworks->sy - 1;

        int minX = 4; // minimum distance
        int minY = 2; // minimum distance

        for(rocket_t *other=fireworks->rocket; other; other=other->next)
        if(other!=rocket && other->x>=0 && other->y>=0)
        if(DIST(rocket->x, other->x)<minX && DIST(rocket->y, other->y)<minY)
        {
            repeat = true;
            break;
        }
    } while(repeat && try-->0);

    rocket->tail[0].x = rocket->x;
    rocket->tail[0].y = rocket->y;
}

// initialize a rocket
void fireworks_rocket_init(fireworks_t *fireworks, rocket_t *rocket)
{
    assert(fireworks && rocket);

    rocket->state = 0;
    rocket->boom = fireworks->y + fireworks->sy/2 - rand()%(fireworks->sy/2) - 1; // explode on the first half of the screen
    rocket->vx = (float)(rand()%5)/4.0; // 0 - 0.25 - 0.5 - 0.75 - 1 //(rand()%4)*50.0/100.0; // 0-1.5
    rocket->vx = rand()%2 ? rocket->vx : -rocket->vx;
    rocket->vy = -(float)(rand()%4+6)/10.0; // 0.6 - 0.7 - 0.8 - 0.9

    // set tail not visible
    for(int i=0; i<TAIL_SIZE; i++)
        rocket->tail[i].x = rocket->tail[i].y = -1;

    // set a random position that keeps a minimum distance to another rocket
    fireworks_rocket_calc_distance(fireworks, rocket);
}

// increment the state of a rocket
void fireworks_rocket_inc(rocket_t *rocket)
{
    assert(rocket);

    // flying
    if(rocket->state==0)
    {
        rocket->x += rocket->vx;
        rocket->y += rocket->vy;

        // update position
        if((int)rocket->x != rocket->tail[0].x ||
           (int)rocket->y != rocket->tail[0].y)
        {
            fireworks_rocket_clear_tail(rocket); 
            fireworks_rocket_shift_tail(rocket);
            fireworks_rocket_print_tail(rocket);
        }
    
        // hit the explosion altitud
        if((int)rocket->y<=rocket->boom)
            rocket->state = 1;
    }
    else // about to explode
    if(rocket->state>0 && rocket->state<TAIL_SIZE)
    {
        rocket->state++;
        fireworks_rocket_clear_tail(rocket); 
        fireworks_rocket_shift_tail(rocket);
        fireworks_rocket_print_tail(rocket);
    }
    else // exploding
    {
        rocket->state++;
        fireworks_rocket_print_explosion(rocket);
    }
}

/**
 * @brief Initialize the variables for the "fireworks structure"
 * @param fireworks the fireworks sky window
 * @param x position x on screen
 * @param y position y on screen
 * @param sx size x of window
 * @param sy size y of window
 */
void fireworks_init(fireworks_t *fireworks, int x, int y, int sx, int sy)
{
    assert(fireworks);
    assert(sx>0 && sy>0);

    fireworks->x = x;
    fireworks->y = y;
    fireworks->sx = sx;
    fireworks->sy = sy;
    fireworks->count = 0;
    fireworks->rocket = NULL;
}

// remove rocket from the list
void fireworks_rocket_del(fireworks_t *fireworks, rocket_t *rocket)
{
    assert(fireworks && rocket);

    // delete the first one?
    if(fireworks->rocket==rocket)
    {
        fireworks->count--;
        fireworks->rocket = fireworks->rocket->next;
        free(rocket);
    }
    else
    // loop through to find, delete and relink
    for(rocket_t *x=fireworks->rocket; x; x=x->next)
    if(rocket == x->next)
    {
        fireworks->count--;
        x->next = rocket->next;
        free(rocket);
        break;
    }
}

// free memory of the fireworks container
void fireworks_destroy(fireworks_t **fireworks)
{
    assert(fireworks && *fireworks);
    rocket_t *rocket = (*fireworks)->rocket;

    while(rocket)
    {
        rocket_t *x = rocket;
        rocket = rocket->next;
        free(x);
    }

    free(*fireworks);
    *fireworks = NULL;
}

// add one rocket to the list
void fireworks_shoot(fireworks_t *fireworks)
{
    assert(fireworks);
    rocket_t *rocket = (rocket_t*)malloc(sizeof(rocket_t));
    if(!rocket)
        exit(1); // no memory

    if(!fireworks->rocket)
    {
        // first one
        fireworks->rocket = rocket;
        fireworks->rocket->next = NULL;
    }
    else
    {
        // push
        rocket->next = fireworks->rocket;
        fireworks->rocket = rocket;
    }

    fireworks->count++;
    fireworks_rocket_init(fireworks, rocket);
}

// animate the rockets in fireworks
void fireworks_run(fireworks_t *fireworks)
{
    assert(fireworks);

    // print rockets and increment state if needed
    for(rocket_t *rocket=fireworks->rocket; rocket; rocket=rocket->next)
        fireworks_rocket_inc(rocket);

    // recycle rockets after explosion
    rocket_t *rocket = fireworks->rocket;
    while(rocket)
    {
        rocket_t *next = rocket->next;
        if(rocket->state==STATE_RECYCLE)
            fireworks_rocket_del(fireworks, rocket);
        rocket = next;
    }
}

// prepare the color pairs
void init_colors()
{
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
}

int main()
{
    // allocate the rocketry fireworks object
    fireworks_t *fireworks = (fireworks_t*)malloc(sizeof(fireworks_t));
    if(!fireworks)
        exit(1);

    srand(time(0)); // random seed
    initscr();      // initialize ncurses
    start_color();  // start the color handling
    init_colors();  // initialize color pairs
    noecho();       // no echo on getch
    curs_set(0);    // cursor hidden
    nodelay(stdscr, TRUE);  // no wait for key to be pressed

    char status[30];
    int x = getbegx(stdscr);
    int y = getbegy(stdscr);
    int sx = getmaxx(stdscr);
    int sy = getmaxy(stdscr);
    int key = -1, pause = 0;
    
    fireworks_init(fireworks, x, y, sx, sy); // initialize the "fireworks" variables

    do {
        key = getch();

        // shoot
        if(key>='1' && key<='9')
        {
            while(key-->'0' && fireworks->count<200)
                fireworks_shoot(fireworks);    // shoot a rocket
            key = -1;
            pause = 0;
        }
        else
        // pause
        if(key==' ')
        {
            if(fireworks->rocket)
               pause = !pause;
            key = -1;
        }

        if(!pause)
            fireworks_run(fireworks);  // run

        sprintf(status, " Rockets: %d", fireworks->count);
        xy_print_str(x, sy-1, "Press 1-9 to shoot, SPACE to pause/continue, or any other key to exit..");
        xy_print_str(x+sx-strlen(status), sy-1, status);
        wrefresh(stdscr);   // refresh screen
        napms(150);         // make a pause (0.15s)
    } while(key==-1);       // finish if a key is pressed (exception with numbers and SPACE)

    fireworks_destroy(&fireworks); // free memory

    endwin(); 
    return 0;
}