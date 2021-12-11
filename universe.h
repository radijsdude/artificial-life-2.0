#ifndef UNIVERSE_H_INCLUDED
#define UNIVERSE_H_INCLUDED
#ifndef AGENT_H_INCLUDED
#include "agent.h"
#endif
#include "draw.h"

struct Universe
{
    Agent* agents;
    int amount;
    int gravity;
    int neurons_in[NEURONS_AMOUNT];
    int neurons_out[NEURONS_AMOUNT];
    int neurons_int;
    int neurons_outt;
};

void Universe_Blits(SDL_Renderer *renderer, Universe *universe, float scale, float x, float y, int mx, int my);
void Universe_Initiate(Universe *universe);
void Universe_Run(Universe* universe);
#endif // UNIVERSE_H_INCLUDED
