#include "universe.h"

void Universe_Blits_Selected(SDL_Renderer *renderer, Agent* agent)
{
    char c[80];
    int ix = SCREEN_Y + 50;
    int iy = 50;
    snprintf(c,sizeof c,"origin 0x%x",agent->origin_id);
    draw_text(renderer,ix,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"lineage %d",agent->lineage);
    draw_text(renderer,ix,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"mutation %d",agent->mutation);
    draw_text(renderer,ix,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"connections %d",agent->connections_length+1);
    draw_text(renderer,ix,iy,250,250,250,c);

    iy += 40;
    snprintf(c,sizeof c,"time %d",agent->time);
    draw_text(renderer,ix,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"kills %d",agent->kills);
    draw_text(renderer,ix,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"splits %d",agent->splits);
    draw_text(renderer,ix,iy,250,250,250,c);

    iy += 40;
    snprintf(c,sizeof c,"health %f",agent->health);
    draw_text(renderer,ix,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"stamina %d",agent->stamina);
    draw_text(renderer,ix,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"schield %d",agent->schield);
    draw_text(renderer,ix,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"speedboost %d",agent->speedboost);
    draw_text(renderer,ix,iy,250,250,250,c);

    iy += 40;
    snprintf(c,sizeof c,"v %f",sqrt(agent->vx*agent->vx + agent->vy*agent->vy));
    draw_text(renderer,ix,iy,250,250,250,c);



    int cx = (SCREEN_X - SCREEN_Y) / 2 + SCREEN_Y;
    int cy = SCREEN_Y*3/4;
    float ia = 2.0f*M_PI/NEURONS_AMOUNT;
    float ir = (SCREEN_X - SCREEN_Y) / 3;
    float icx,icy,cor,cog,cob;
    for(int i=0; i<NEURONS_AMOUNT; i++)
    {
        icx = (float)(cx) + ir*cos(ia*i);
        icy = (float)(cy) + ir*sin(ia*i);
        cor = 0.0f;
        cog = 0.0f;
        cob = 0.0f;
        if(agent->potentials[i]<0.5f)
        {
            cor = 255.0f *(agent->potentials[i] + 0.5f);
        }
        if(agent->potentials[i]>0.5f)
        {
            cog = 255.0f * agent->potentials[i];
        }
        if(agent->potentials[i]==0.5f)
        {
            cob = 255.0f;
        }
        draw_circle(renderer,icx,icy,5,(int)cor,(int)cog,(int)cob,255,1);
    }
    float icx1,icx2,icy1,icy2;
    int ccor, ccog, ccob;
    for(int i=0; i<NEURONS_CONNECTIONS; i++)
    {
        if(agent->connections[i][0]!=-13)
        {
            icx1 = (float)(cx) + ir*cos(ia*agent->connections[i][0]);
            icy1 = (float)(cy) + ir*sin(ia*agent->connections[i][0]);
            icx2 = (float)(cx) + ir*cos(ia*agent->connections[i][1]);
            icy2 = (float)(cy) + ir*sin(ia*agent->connections[i][1]);
            ccor = 0;
            ccog = 0;
            ccob = 0;
            if(agent->weights[i]<0.0f)
            {
                ccor = 255;
            }
            if(agent->weights[i]>0.0f)
            {
                ccog = 255;
            }
            if(agent->weights[i]==0.0f)
            {
                ccob = 255;
            }

            draw_line(renderer,icx1,icy1,icx2,icy2,ccor,ccog,ccob,255,0);
        }
    }
}

void Universe_Blits(SDL_Renderer *renderer, Universe *universe, float scale, float x, float y, int mx, int my)
{
    int s = -1;
    for (int i=0; i<(int)universe->amount; i++)
	{
	    if(mx != NULL)
        {
            float dx = scale*(universe->agents[i].x+x)-(float)mx;
            float dy = scale*(universe->agents[i].y+y)-(float)my;
            float dr = scale*(universe->agents[i].r + 2.0f) * 2.0f;
            if(dx*dx+dy*dy<dr*dr)
            {
                universe->agents[i].isselected = 1;
            }
            else
            {
                universe->agents[i].isselected = 0;
            }
        }
        if(universe->agents[i].schield>0)
        {
            draw_circle(renderer, (int)((universe->agents[i].x+x) * scale),(int)((universe->agents[i].y+y) * scale),
            (int)((universe->agents[i].r +3) * scale), 100,100,255,255, 1);
        }
        if(universe->agents[i].speedboost>0)
        {
            draw_circle(renderer, (int)((universe->agents[i].x+x) * scale),(int)((universe->agents[i].y+y) * scale),
            (int)((universe->agents[i].r +3) * scale), 250,100,100,255, 1);
        }
        if(universe->agents[i].isselected==1)
        {
            draw_circle(renderer, (int)((universe->agents[i].x+x) * scale),(int)((universe->agents[i].y+y) * scale),AGENT_VISUAL_RANGE*scale, 255,0,255,255, 0);
            Universe_Blits_Selected(renderer,&universe->agents[i]);
        }
        if(universe->agents[i].time > AGENT_TIME)
        {
            draw_circle(renderer, (int)((universe->agents[i].x+x) * scale),(int)((universe->agents[i].y+y) * scale),
            (int)((universe->agents[i].r +1) * scale), (int)(universe->agents[i].color[0]),
            (int)(universe->agents[i].color[1]),
            (int)(universe->agents[i].color[2]), (int)(universe->agents[i].color[3]), 1);
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderDrawLine(renderer, (int)((universe->agents[i].x+x) * scale), (int)((universe->agents[i].y+y) * scale),
            (int)(((universe->agents[i].x+x) + universe->agents[i].r * cos(universe->agents[i].angle)) * scale),
            (int)(((universe->agents[i].y+y) + universe->agents[i].r * sin(universe->agents[i].angle)) * scale));
        }
        else
        {
            draw_circle(renderer, (int)((universe->agents[i].x+x) * scale),(int)((universe->agents[i].y+y) * scale),
            (int)((universe->agents[i].r +1) * scale), 255,255,255,255, 1);
        }
	}
}

void Universe_Initiate(Universe* universe)
{
    universe->gravity = 0;
    universe->amount = 0;
    universe->agents = (Agent*)malloc(UNIVERSE_MAX_AGENTS*sizeof(Agent));
    universe->neurons_in[0] = NEURON_SEE;
    universe->neurons_in[1] = NEURON_ALIVE_1;
    universe->neurons_in[2] = NEURON_ALIVE_2;
    universe->neurons_in[3] = NEURON_SEE_ANGLE;
    universe->neurons_in[4] = NEURON_SEE_HEALTH;
    universe->neurons_in[5] = NEURON_SEE_GRAVITY;
    universe->neurons_in[6] = NEURON_SEE_DISTANCE_1;
    universe->neurons_in[7] = NEURON_SEE_DISTANCE_2;
    universe->neurons_in[8] = NEURON_SENSE_GRAVITY;
    universe->neurons_in[9] = NEURON_SENSE_GRAVITY_ANGLE;
    universe->neurons_in[10] = NEURON_SENSE_ANGLE;
    universe->neurons_in[11] = NEURON_SENSE_HEALTH;
    universe->neurons_in[12] = NEURON_HEAR_1;
    universe->neurons_in[13] = NEURON_HEAR_2;
    universe->neurons_in[14] = NEURON_HEAR_3;
    universe->neurons_in[15] = NEURON_SINE_1;
    universe->neurons_in[16] = NEURON_SINE_2;
    universe->neurons_in[17] = NEURON_SINE_3;
    universe->neurons_in[18] = NEURON_SEE_R;
    universe->neurons_in[19] = NEURON_SEE_G;
    universe->neurons_in[20] = NEURON_SEE_B;
    universe->neurons_in[21] = NEURON_SEE_ANGLE2;
    universe->neurons_in[22] = NEURON_SEE_AL;
    universe->neurons_in[23] = NEURON_SEE_AR;
    universe->neurons_in[24] = NEURON_SET1;
    universe->neurons_in[25] = NEURON_SET2;
    universe->neurons_in[26] = NEURON_SEE_AB;
    universe->neurons_in[27] = NEURON_SEE_ALL;
    universe->neurons_in[28] = NEURON_SEE_ARR;
    universe->neurons_in[29] = NEURON_SEE_ALLL;
    universe->neurons_in[30] = NEURON_SEE_ARRR;
    universe->neurons_in[31] = NEURON_SEE_TIME;
    universe->neurons_in[32] = NEURON_IN_1;
    universe->neurons_in[33] = NEURON_IN_2;
    universe->neurons_in[34] = NEURON_IN_3;
    universe->neurons_int = 35;
    universe->neurons_out[0] = NEURON_MOVE_X;
    universe->neurons_out[1] = NEURON_MOVE_Y;
    universe->neurons_out[2] = NEURON_MOVE_ANGLE;
    universe->neurons_out[3] = NEURON_ATTACK;
    universe->neurons_out[4] = NEURON_EAT;
    universe->neurons_out[5] = NEURON_SPLIT;
    universe->neurons_out[6] = NEURON_SPEAK_1;
    universe->neurons_out[7] = NEURON_SPEAK_2;
    universe->neurons_out[8] = NEURON_SPEAK_3;
    universe->neurons_out[9] = NEURON_SPEEDBOOST;
    universe->neurons_out[10] = NEURON_SCHIELD;
    universe->neurons_out[11] = NEURON_SET1_ON;
    universe->neurons_out[12] = NEURON_SET2_ON;
    universe->neurons_out[13] = NEURON_SET1_OFF;
    universe->neurons_out[14] = NEURON_SET2_OFF;
    universe->neurons_out[15] = NEURON_STOP;
    universe->neurons_out[16] = NEURON_OUT_1;
    universe->neurons_out[17] = NEURON_OUT_2;
    universe->neurons_out[18] = NEURON_OUT_3;
    universe->neurons_outt = 19;
	for (int i = 0; i < UNIVERSE_START_AGENTS; i++)
	{
		Agent p;
		Agent_Init(&p);
		p.r = (float)AGENT_SIZE;
		float r;
		float a;
		r = (float)rand()*(UNIVERSE_SIZE/6.0f)/RAND_MAX;
		a = (float)rand()*(M_PI*2.0f)/RAND_MAX;
		p.x = UNIVERSE_SIZE/2.0f + r*cos(a);
		p.y = UNIVERSE_SIZE/2.0f + r*sin(a);
		p.color[0] = rand()%230 + 25;
		p.color[1] = rand()%230 + 25;
		p.color[2] = rand()%230 + 25;
		p.color[3] = 255;
		p.angle = (float)(rand()%200 - 100) / 100.0f * M_PI;
		p.time = -200;


		p.origin_id = i;
		p.lineage = 1;

		p.health = AGENT_MAX_HEALTH*1.1f;
		p.alive = 1;

        p.connections_length = -1;
		for (int i=0; i<NEURONS_AMOUNT; i++)
		{
			p.potentials[i] = (float)NEURONS_START_POTENTIAL + (float)POTENTIALSHIFT ;//+ ((float)(rand()) / (float)(RAND_MAX)- 0.5f) / 10.0f;
		}

        //Agent_Create_Connections(&p,1,universe->neurons_in,universe->neurons_int,universe->neurons_out,universe->neurons_outt);
        Agent_Add_Connection(&p,NEURON_ALIVE_1,NEURON_SPLIT,2.0f,universe->neurons_in,universe->neurons_int,universe->neurons_out,universe->neurons_outt);
        Agent_Add_Connection(&p,NEURON_ALIVE_1,NEURON_MOVE_X,2.0f,universe->neurons_in,universe->neurons_int,universe->neurons_out,universe->neurons_outt);
        Agent_Add_Connection(&p,NEURON_ALIVE_1,NEURON_EAT,2.0f,universe->neurons_in,universe->neurons_int,universe->neurons_out,universe->neurons_outt);
        Agent_Heridity(&p,1,universe->neurons_in,universe->neurons_int,universe->neurons_out,universe->neurons_outt);

		universe->agents[universe->amount] = p;
		universe->amount += 1;
	}
}

void Universe_Run(Universe *universe)
{
    float neuronshift = (float)POTENTIALSHIFT;
    Agent* agent;
    Agent* agentj;
    Agent lsee;
    float a,x,y,sinn,coss;
    float dx,dy,dr,d;
    float feel_d,feel_ar,distance,hear1,hear2,hear3;
    float jx,jy;
    float dv,dvv,sdv;
    int r,insight;
    int i,j,k;
    float dangle,tt2;
    float d_l,d_ll,d_lll,d_r,d_rr,d_rrr,d_b;
	for (i=0; i<(int)universe->amount; i++)
	{
	    agent = &universe->agents[i];
		if(agent->health > 0.0f)
		{
			agent->time += 1;

			Agent_Gravitate(&universe->agents[i],universe->gravity);
			x = agent->x;
			y = agent->y;

			a = agent->angle;



			agent->x += agent->ax;
			agent->y += agent->ay;


			if (agent->time > AGENT_TIME)
			{
                if(agent->time%3000==0)
                {
                    Agent_Heridity(&universe->agents[i],0,universe->neurons_in,universe->neurons_int,universe->neurons_out, universe->neurons_outt);
                }

                if(agent->stamina < AGENT_STAMINA)
                {
                    agent->stamina += 1;
                }
                if(agent->stamina < 0)
                {
                    agent->stamina = 1;
                }
				agent->r = AGENT_SIZE + agent->health / AGENT_MAX_HEALTH * AGENT_SIZE_SCALE;
                if(agent->r>2.0f*AGENT_SIZE)
                {
                    agent->r = 2.0f*AGENT_SIZE;
                }



				agent->potentials[NEURON_ALIVE_1] = 1.0f;
				agent->potentials[NEURON_ALIVE_2] = -1.0f;
				agent->potentials[NEURON_SINE_1] = sin((float)agent->time / 10.0f);
				agent->potentials[NEURON_SINE_2] = sin((float)agent->time / 300.0f);
				agent->potentials[NEURON_SINE_3] = sin((float)agent->time / 1000.0f);
				agent->potentials[NEURON_IN_1] = agent->potentials[NEURON_OUT_1];
				agent->potentials[NEURON_IN_2] = agent->potentials[NEURON_OUT_2];
				agent->potentials[NEURON_IN_3] = agent->potentials[NEURON_OUT_3];
				agent->potentials[NEURON_SENSE_ANGLE] = a / M_PI;
				agent->potentials[NEURON_SENSE_HEALTH] = (float)agent->health /
				((float)AGENT_MAX_HEALTH);

				Agent_Calculate_All(&universe->agents[i]);

				agent->potentials[NEURON_SEE_AL] = 0.0f;
				agent->potentials[NEURON_SEE_AR] = 0.0f;
				agent->potentials[NEURON_SEE_ALL] = 0.0f;
				agent->potentials[NEURON_SEE_ARR] = 0.0f;
				agent->potentials[NEURON_SEE_AB] = 0.0f;
				if(agent->potentials[NEURON_SET1_OFF] > neuronshift)
                {
                    agent->potentials[NEURON_SET1] = 0.0f;
                }
                if(agent->potentials[NEURON_SET1_ON] > neuronshift)
                {
                    agent->potentials[NEURON_SET1] = 1.0f;
                }
                if(agent->potentials[NEURON_SET2_OFF] > neuronshift)
                {
                    agent->potentials[NEURON_SET2] = 0.0f;
                }
                if(agent->potentials[NEURON_SET2_ON] > neuronshift)
                {
                    agent->potentials[NEURON_SET2] = 1.0f;
                }

                float aa = 1.0f;
                if(agent->stamina > SPEEDBOOSTCOST && agent->potentials[NEURON_SPEEDBOOST] > neuronshift)
                {
                    agent->speedboost = SPEEDBOOST;
                    agent->stamina -= SPEEDBOOSTCOST;
                    aa = 3.0f;
                }

                if(agent->stamina > SCHIELDCOST && agent->potentials[NEURON_SCHIELD] > neuronshift)
                {
                    agent->schield = SCHIELD;
                    agent->stamina -= SCHIELDCOST;
                }

                agent->angle += aa * (1.0f + (float)agent->stamina/(float)AGENT_STAMINA) *
                                                (agent->potentials[NEURON_MOVE_ANGLE] - neuronshift) / 2.0f *
                                                (float)(M_PI)/4.0f;
                if (a > (float)M_PI)
                {
                    agent->angle = -(float)M_PI;
                }
                if (agent->angle < -(float)M_PI)
                {
                    agent->angle = (float)M_PI;
                }
                a = agent->angle;

                sinn = sin(a);
                coss = cos(a);


				if(agent->potentials[NEURON_STOP] > neuronshift)
                {
                    agent->vx = 0.0f;
                    agent->vy = 0.0f;
                }
                else
                {
                    agent->vx += (1.0f + (float)agent->stamina/(float)AGENT_STAMINA) * ((float)AGENT_SPEED)*(coss*
                    (agent->potentials[NEURON_MOVE_X] - neuronshift) - sinn*
                    (agent->potentials[NEURON_MOVE_Y] - neuronshift));

                    agent->vy += (1.0f + (float)agent->stamina/(float)AGENT_STAMINA) * ((float)AGENT_SPEED)*(coss*
                    (agent->potentials[NEURON_MOVE_Y] - neuronshift) + sinn*
                    (agent->potentials[NEURON_MOVE_X] - neuronshift));
                }




				if ((agent->potentials[NEURON_SPLIT] > neuronshift &&
				agent->health > (float)AGENT_SPLIT_HEALTH*1.1f) || (agent->health > AGENT_MAX_HEALTH))
				{
				    if(universe->amount < UNIVERSE_MAX_AGENTS)
                    {
                        agent->health -= (float)AGENT_SPLIT_HEALTH;
                        agent->splits += 1;
                        agent->stamina -= AGENT_STAMINA / 2;
                        Agent p;
                        Agent_Init(&p);
                        p.r = (float)AGENT_SIZE;
                        p.x = x - 2.1f*(float)AGENT_SIZE*coss;
                        p.y = y - 2.1f*(float)AGENT_SIZE*sinn;
                        p.vx = agent->vx;
                        p.vy = agent->vy;
                        p.color[0] = agent->color[0];
                        p.color[1] = agent->color[1];
                        p.color[2] = agent->color[2];
                        p.color[3] = 255;
                        p.angle = (float)(rand()%200 - 100) / 100.0f * M_PI;


                        p.origin_id = agent->origin_id;
                        p.lineage = agent->lineage + 1;
                        p.mutation = agent->mutation;


                        p.health = (float)AGENT_SPLIT_HEALTH;
                        p.alive = 1;

                        p.connections_length = agent->connections_length;
                        for (k=0; k<NEURONS_AMOUNT; k++)
                        {
                            p.potentials[k] = (float)NEURONS_START_POTENTIAL + (float)POTENTIALSHIFT;
                        }
                        for (k=0; k<NEURONS_CONNECTIONS; k++)
                        {
                            p.connections[k][0] = agent->connections[k][0];
                            p.connections[k][1] = agent->connections[k][1];
                            p.weights[k] = agent->weights[k];
                        }
                        r = rand()%NEURON_MUTATIONRATE;
                        Agent_Heridity(&p, 1,universe->neurons_in,universe->neurons_int,universe->neurons_out,universe->neurons_outt);
                        for(k=0; k<r; k++)
                        {
                            Agent_Heridity(&p, 0,universe->neurons_in,universe->neurons_int,universe->neurons_out,universe->neurons_outt);
                        }
                        Agent_CM(&p);
                        universe->agents[universe->amount] = p;
                        universe->amount += 1;
                    }
				}



                feel_d = 0.0f;
                feel_ar = 0.0f;
                insight = 0;
                distance = (float)AGENT_VISUAL_RANGE;
                hear1 = 0.0f;
                hear2 = 0.0f;
                hear3 = 0.0f;
                d_l = 0.0f;
                d_ll = 0.0f;
                d_lll = 0.0f;
                d_r = 0.0f;
                d_rr = 0.0f;
                d_rrr = 0.0f;
                d_b = 0.0f;
                for (j=0; j<(int)universe->amount; j++)
                {
                    if (i!=j)
                    {
                        if(universe->agents[j].health>0.0f)
                        {
                            jx = universe->agents[j].x;
                            jy = universe->agents[j].y;
                            d = (jx - x)*(jx - x) + (jy - y)*(jy - y);
                            if (d==0.0f)
                            {
                                d = 0.01f;
                            }
                            if (d < (2.0f*((float) AGENT_SIZE))*(2.0f*((float) AGENT_SIZE)))
                            {
                                Agent_Collide(&universe->agents[i],&universe->agents[j]);
                            }

                            if (d < AGENT_VISUAL_RANGE * AGENT_VISUAL_RANGE)
                            {
                                feel_d += 2.0f/(d + 1.0f);
                                feel_ar += (atan2(jx-x,jx-x) - a) / (d + 1.0f);


                                hear1 += universe->agents[j].potentials[NEURON_SPEAK_1]/(d + 1.0f);
                                hear2 += universe->agents[j].potentials[NEURON_SPEAK_2]/(d + 1.0f);
                                hear3 += universe->agents[j].potentials[NEURON_SPEAK_3]/(d + 1.0f);

                                if(Agent_See(x,y,jx,jy,a,d)==1)
                                {
                                    insight = 1;
                                    if(d<distance)
                                    {
                                        lsee = universe->agents[i];
                                        distance = d + 1.0f;
                                    }
                                }
                                if(Agent_See(x,y,jx,jy,a+AGENT_FOV,d)==1)
                                {
                                    if(1.0f/d > d_l)
                                    {
                                        d_l = 1.0f/d;
                                    }
                                }
                                if(Agent_See(x,y,jx,jy,a-AGENT_FOV,d)==1)
                                {
                                    if(1.0f/d > d_r)
                                    {
                                        d_r = 1.0f/d;
                                    }
                                }
                                if(Agent_See(x,y,jx,jy,a + 2 * AGENT_FOV,d)==1)
                                {
                                    if(1.0f/d > d_ll)
                                    {
                                        d_ll = 1.0f/d;
                                    }
                                }
                                if(Agent_See(x,y,jx,jy,a - 2 * AGENT_FOV,d)==1)
                                {
                                    if(1.0f/d > d_rr)
                                    {
                                        d_rr = 1.0f/d;
                                    }
                                }
                                if(Agent_See(x,y,jx,jy,a + 3 * AGENT_FOV,d)==1)
                                {
                                    if(1.0f/d > d_lll)
                                    {
                                        d_lll = 1.0f/d;
                                    }
                                }
                                if(Agent_See(x,y,jx,jy,a - 3 * AGENT_FOV,d)==1)
                                {
                                    if(1.0f/d > d_rrr)
                                    {
                                        d_rrr = 1.0f/d;
                                    }
                                }
                                if(Agent_See(x,y,jx,jy,a-M_PI,d)==1)
                                {
                                    if(1.0f/d > d_b)
                                    {
                                        d_b = 1.0f/d;
                                    }
                                }

                            }


                        }

                    }
                }

                agent->potentials[NEURON_HEAR_1] = hear1;
                agent->potentials[NEURON_HEAR_2] = hear2;
                agent->potentials[NEURON_HEAR_3] = hear3;

                agent->potentials[NEURON_SEE_AL] = d_l;
                agent->potentials[NEURON_SEE_ALL] = d_ll;
                agent->potentials[NEURON_SEE_ALLL] = d_lll;
                agent->potentials[NEURON_SEE_AR] = d_r;
                agent->potentials[NEURON_SEE_ARR] = d_rr;
                agent->potentials[NEURON_SEE_ARRR] = d_rrr;
                agent->potentials[NEURON_SEE_AB] = d_b;

                if(insight == 1)
                {
                    dangle = atan2(y - lsee.y, x - lsee.x);
                    agent->potentials[NEURON_SEE_DISTANCE_1] = distance /
                    ((float)UNIVERSE_SIZE*(float)UNIVERSE_SIZE);
                    agent->potentials[NEURON_SEE_DISTANCE_2] = 1.0f / distance;
                    agent->potentials[NEURON_SEE_ANGLE] = (dangle - a) * M_PI;
                    tt2 = dangle - lsee.angle;
                    agent->potentials[NEURON_SEE_ANGLE2] = tt2 * M_PI;
                    agent->potentials[NEURON_SEE_HEALTH] = lsee.health/(agent->health + 1.0f);
                    agent->potentials[NEURON_SEE] = 1.0f;
                    agent->potentials[NEURON_SEE_R] = (float)lsee.color[0] / 255.0f;
                    agent->potentials[NEURON_SEE_G] = (float)lsee.color[1] / 255.0f;
                    agent->potentials[NEURON_SEE_B] = (float)lsee.color[2] / 255.0f;
                    if(lsee.time>AGENT_TIME)
                    {
                        agent->potentials[NEURON_SEE_TIME] = 1.0f;
                    }
                    else
                    {
                        agent->potentials[NEURON_SEE_TIME] = 0.0f;
                    }
                }
                else
                {
                    agent->potentials[NEURON_SEE_DISTANCE_1] = 1.0f;
                    agent->potentials[NEURON_SEE_DISTANCE_2] = 0.0f;
                    agent->potentials[NEURON_SEE_ANGLE] = 0.0f;
                    agent->potentials[NEURON_SEE_ANGLE2] = M_PI/2.0f;
                    agent->potentials[NEURON_SEE_HEALTH] = 0.0f;
                    agent->potentials[NEURON_SEE] = 0.0f;
                    agent->potentials[NEURON_SEE_R] = 0.0f;
                    agent->potentials[NEURON_SEE_G] = 0.0f;
                    agent->potentials[NEURON_SEE_B] = 0.0f;
                    agent->potentials[NEURON_SEE_TIME] = 0.0f;
                }

                agent->potentials[NEURON_FIELD_DISTANCE] = feel_d;
                agent->potentials[NEURON_FIELD_ANGLE_R] = feel_ar;
			}
			else
			{
				for (int j=0; j<(int)universe->amount; j++)
                {
                    if(universe->agents[j].health>0.0f)
                    {
                        if (i!=j)
                        {
                            jx = universe->agents[j].x;
                            jy = universe->agents[j].y;
                            dx = jx - x;
                            dy = jy - y;
                            d = dx*dx + dy*dy;
                            if (d==0.0f)
                            {
                                d = 0.01f;
                            }
                            if (d < (agent->r + universe->agents[j].r)*(agent->r + universe->agents[j].r))
                            {
                                Agent_Collide(&universe->agents[i],&universe->agents[j]);
                            }
                        }
                    }
                }
			}


			//agent->anglespeed *= (1.0f - 20.0f * UNIVERSE_FRICTION);




			float dv = agent->vx*agent->vx + agent->vy*agent->vy;
			float dvv = (float)AGENT_MAX_SPEED *
                        ((1.0f + (float)agent->stamina/(float)AGENT_STAMINA));
            float sdv = sqrt(dv);
            if (agent->speedboost > 0)
            {
                dvv *= 2.0f;
                agent->speedboost -= 1;
            }
			if (sdv > dvv)
			{
				float sdvv = sqrt(dvv);
				agent->vx = dvv * agent->vx / sdv;
				agent->vy = dvv * agent->vy / sdv;
			}
            if(agent->schield > 0)
            {
                agent->schield -= 1;
            }
			agent->vx *= (1.0f - (float)UNIVERSE_FRICTION);
			agent->vy *= (1.0f - (float)UNIVERSE_FRICTION);
			agent->x += agent->vx;
			agent->y += agent->vy;

		}
		else
		{
            universe->agents[i] = universe->agents[universe->amount-1];
            universe->amount -= 1;
		}
	}
}
