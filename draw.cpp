
#include "draw.h"

void draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int r,int g,int b,int a, int filled)
{
    SDL_SetRenderDrawColor(renderer,r,g,b,a);
    if(filled==1)
    {
        if(x2-x1==0)
        {
            if(y1<y2)
            {
                 for(int i=y1;i < y2; i++)
                {
                    SDL_RenderDrawPoint(renderer,x1,i);
                }
            }
            if(y1>y2)
            {
                 for(int i=y2;i < y1; i++)
                {
                    SDL_RenderDrawPoint(renderer,x1,i);
                }
            }
        }
        float m = (float)(y2 - y1)/(float)(x2 - x1);
        if(x2-x1>0)
        {
            for(int i=0;i < x2-x1; i++)
            {
                SDL_RenderDrawPoint(renderer,x1 + i, y1 + i*m);
            }
        }
        if(x2-x1<0)
        {
            for(int i=0;i < x1-x2; i++)
            {
                SDL_RenderDrawPoint(renderer,x2 + i, y2 + i*m);
            }
        }
    }
    else
    {
        SDL_RenderDrawLine(renderer,x1,y1,x2,y2);
    }

}

void draw_rect(SDL_Renderer *renderer, int x, int y, int b, int l, int cr,int cg,int cb,int ca, int filled)
{
    SDL_SetRenderDrawColor(renderer,cr,cg,cb,ca);
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = b;
    r.h = l;
    SDL_RenderDrawRect(renderer, &r);
    SDL_RenderFillRect(renderer, &r);
}

void draw_circle(SDL_Renderer *renderer, int x, int y, int r, int color_r, int color_g, int color_b, int color_a, int filled)
{
    SDL_SetRenderDrawColor(renderer,color_r,color_g,color_b,color_a);
    if (r < 1){r = 1;}
    if(filled == 1)
    {
        for (int i = -r; i <= r; i++)
        {
            for (int j = 0; j*j<=r*r-i*i;j++)
            {
                SDL_RenderDrawPoint(renderer,x + i, y + j);
                SDL_RenderDrawPoint(renderer,x + i, y - j);
            }
        }
    }
    else
    {
        for (int i = -r; i <= r; i++)
        {
            SDL_RenderDrawPoint(renderer,x + i, y + (int)sqrt(r*r - i*i));
            SDL_RenderDrawPoint(renderer,x + i, y - (int)sqrt(r*r - i*i));
        }
    }

}

void draw_text(SDL_Renderer* renderer,int x, int y, int r,int g,int b, const char* txt)
{
    TTF_Font * font = TTF_OpenFont("arial.ttf", 11);
    SDL_Color color = { (uint8_t)r, (uint8_t)g, (uint8_t)b };
    SDL_Surface * surface = TTF_RenderText_Solid(font,txt, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { x, y, texW, texH };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}


void draw_float(SDL_Renderer* renderer,int x, int y, int r,int g,int b, float f)
{
    char c[10];
    snprintf(c,sizeof c,"%f",f);
    draw_text(renderer,x,y,255,255,255,c);
}

void draw_int(SDL_Renderer* renderer,int x, int y, int r,int g,int b, int f)
{
    char c[10];
    snprintf(c,sizeof c,"%d",f);
    draw_text(renderer,x,y,255,255,255,c);
}

