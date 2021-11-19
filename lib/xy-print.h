#ifndef XY_PRINT_H
#define XY_PRINT_H

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <ncurses.h>
//#include <unistd.h>

void clearscr();

void xy_goto(int x, int y);
void xy_attr_set(int x, int y, int flag);
void xy_print_c(int x, int y, char c);
void xy_print_str(int x, int y, char *str);
void xy_print_num(int x, int y, int num);
void xy_print_fmt(int x, int y, const char *fmt, ...);
void xy_line_x(int px, int py, int sizeX, char c);
void xy_line_y(int px, int py, int sizeY, char c);

#endif