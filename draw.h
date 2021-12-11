
#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED


#include "includes.h"

void draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int r,int g,int b,int a, int filled);

void draw_rect(SDL_Renderer *renderer, int x, int y, int b, int l, int cr,int cg,int cb,int ca, int filled);


void draw_circle(SDL_Renderer *renderer, int x, int y, int r, int color_r, int color_g, int color_b, int color_a, int filled);

void draw_text(SDL_Renderer* renderer,int x, int y, int r,int g,int b, const char* txt);

void draw_float(SDL_Renderer* renderer,int x, int y, int r,int g,int b, float f);

void draw_int(SDL_Renderer* renderer,int x, int y, int r,int g,int b, int f);

#endif // DRAW_H_INCLUDED
