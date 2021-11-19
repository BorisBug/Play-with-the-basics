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
#define TAIL_SIZE sizeof(tail_chars)

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
    }
}

// initialize a rocket
void fireworks_rocket_init(fireworks_t *fw, rocket_t *rocket)
{
    assert(fw && rocket);
    int x, y, try = 15;
    float vx = (rand()%4)*50.0/100.0;
    bool repeat = false;

    rocket->state = 0;
    rocket->boom = fw->y + fw->sy/2 - rand()%(fw->sy/2) - 1;
    rocket->vx = rand()%2 ? vx : -vx;
    rocket->vy = -(float)(rand()%4+6)/10.0;

    // set tail not visible
    for(int i=0; i<TAIL_SIZE; i++)
        rocket->tail[i].x = rocket->tail[i].y = -1;

    // set a random position that keeps a minimum distance to another rocket

    do
    {   repeat = false;
        x = fw->x + rand()%fw->sx;
        y = fw->y + fw->sy - 1;

        int minX = 4; // minimum distance
        int minY = 2; // minimum distance

        for(rocket_t *r=fw->rocket; r; r=r->next)
        if(r != rocket && r->x>=0 && r->y>=0)
        if(DIST(x, r->x) < minX && DIST(y, r->y) < minY)
        {
            repeat = true;
            break;
        }
    } while(repeat && try-->0);

    rocket->x = x;
    rocket->y = y;
    rocket->tail[0].x = x;
    rocket->tail[0].y = y;
}

// increment the state of a rocket
void fireworks_rocket_inc(fireworks_t *fw, rocket_t *rocket)
{
    assert(fw && rocket);

    // flying
    if(rocket->state==0)
    {
        rocket->x += rocket->vx;
        rocket->y += rocket->vy;

        if((int)rocket->x != rocket->tail[0].x || (int)rocket->y != rocket->tail[0].y)
        {
            fireworks_rocket_clear_tail(rocket); 
            fireworks_rocket_shift_tail(rocket);
            fireworks_rocket_print_tail(rocket);
        }
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

    // hit the explosion altitud
    if(rocket->state==0 && (int)rocket->y==rocket->boom)
        rocket->state = 1;
}

/**
 * @brief Initialize the variables for the "fireworks structure"
 * @param fw the fireworks sky window
 * @param x position x on screen
 * @param y position y on screen
 * @param sx size x of window
 * @param sy size y of window
 */
void fireworks_init(fireworks_t *fw, int x, int y, int sx, int sy)
{
    assert(fw);
    assert(sx>0 && sy>0);

    fw->x = x;
    fw->y = y;
    fw->sx = sx;
    fw->sy = sy;
    fw->count = 0;
    fw->rocket = NULL;
}

// remove rocket from the list
void fireworks_rocket_del(fireworks_t *fw, rocket_t *rocket)
{
    assert(fw && rocket);

    // delete the first one?
    if(fw->rocket==rocket)
    {
        fw->count--;
        fw->rocket = fw->rocket->next;
        free(rocket);
    }
    else
    // loop through to find, delete and relink
    for(rocket_t *x=fw->rocket; x; x=x->next)
    if(rocket == x->next)
    {
        fw->count--;
        x->next = rocket->next;
        free(rocket);
        break;
    }
}

// free memory of the fireworks container
void fireworks_destroy(fireworks_t **fw)
{
    assert(fw && *fw);
    rocket_t *rocket = (*fw)->rocket;

    while(rocket)
    {
        rocket_t *x = rocket;
        rocket = rocket->next;
        free(x);
    }

    free(*fw);
    *fw = NULL;
}

// add one rocket to the list
void fireworks_shoot(fireworks_t *fw)
{
    assert(fw);
    rocket_t *rocket = (rocket_t*)malloc(sizeof(rocket_t));
    if(!rocket)
        exit(1); // no memory

    if(!fw->rocket)
    {
        fw->rocket = rocket;
        fw->rocket->next = NULL;
    }
    else
    {
        // push
        rocket->next = fw->rocket;
        fw->rocket = rocket;
    }

    fw->count++;
    fireworks_rocket_init(fw, rocket);
}

// animate the rockets in fireworks
void fireworks_run(fireworks_t *fw)
{
    assert(fw);

    // print frame
    // xy_print_c(fw->x, fw->y,'@');
    // xy_print_c(fw->x+fw->sx-1, fw->y, '@');
    // xy_print_c(fw->x, fw->y+fw->sy-1,'@');
    // xy_print_c(fw->x+fw->sx-1, fw->y+fw->sy-1, '@');

    // print rockets and increment state if needed
    for(rocket_t *rocket=fw->rocket; rocket; rocket=rocket->next)
        fireworks_rocket_inc(fw, rocket);

    rocket_t *rocket = fw->rocket;
    while(rocket)
    {
        rocket_t *next = rocket->next;
        if(rocket->state>20) // recycle ?
            fireworks_rocket_del(fw, rocket);
        rocket = next;
    }
}

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
    fireworks_t *fw = (fireworks_t*)malloc(sizeof(fireworks_t));
    if(!fw)
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
    
    fireworks_init(fw, x, y, sx, sy); // initialize the "fireworks" variables

    do {
        key = getch();

        // shoot ?
        if(key>='1' && key<='9')
        {
            while(key-->'0')
                fireworks_shoot(fw);    // shoot a rocket
            key = -1;
            pause = 0;
        }
        else
        // pause ?
        if(key==' ')
        {
            if(fw->rocket)
               pause = !pause;
            key = -1;
        }

        if(!pause)
            fireworks_run(fw);  // run

        sprintf(status, " Rockets: %d", fw->count);
        xy_print_str(x, sy-1, "Press 1-9 to shoot, SPACE to pause/continue, or any other key to exit..");
        xy_print_str(x+sx-strlen(status), sy-1, status);
        wrefresh(stdscr);   // refresh screen
        napms(150);         // make a pause (0.15s)
    } while(key==-1);       // finish if a key is pressed (exception with numbers and SPACE)

    fireworks_destroy(&fw); // free memory

    endwin(); 
    return 0;
}