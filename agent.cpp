#include "agent.h"
void Agent_Init(Agent* agent)
{
    agent->r = 0.0f;
	agent->x = 0.0f;
	agent->y = 0.0f;
	agent->vx = 0.0f;
	agent->vy = 0.0f;
    agent->ax = 0.0f;
	agent->ay = 0.0f;
	agent->color[0] = 255;
	agent->color[1] = 0;
	agent->color[2] = 255;
	agent->color[3] = 255;
	agent->angle = 0.0f;
	agent->time = 0;

    agent->isselected = 0;

	agent->stamina = 0;
	agent->speedboost = 0;
	agent->schield = 0;

    agent->origin_id = 0;
	agent->lineage = 0;
	agent->mutation = 0;
    agent->kills = 0;
	agent->splits = 0;

    agent->health = 0.0f;
	agent->alive = 0;

    agent->connections_length = -1;
	for (int i=0; i<NEURONS_AMOUNT; i++)
	{
		agent->potentials[i] = 0.0f;
	}
	for (int i=0; i<NEURONS_CONNECTIONS; i++)
	{
		agent->connections[i][0] = -13.0f;
		agent->connections[i][1] = -13.0f;
		agent->weights[i] = 0.0f;
	}
}

float Agent_Calculate_Sigmoid(Agent *agent, int i)
{
    float x = 0.0f;
    int t = 0;
    for (int j=0; j<agent->connections_length+1; j++)
    {
        if (i==agent->connections[j][1])
        {
            t = 1;
            x += agent->weights[j] * Agent_Calculate_Sigmoid(agent, agent->connections[j][0]);
        }
    }
    if (t==1)
    {
    	if (x > - 50)
    	{
        	x = (float)1.0f/(1.0f+exp(-x));
        }
        else
        {
        	x = 0.0f;
        }
        agent->potentials[i] = x;
        return x;
    }
    else
    {
        return agent->potentials[i];
    }
}
void Agent_Calculate_All(Agent *agent)
{
	Agent_Calculate_Sigmoid(agent,NEURON_MOVE_X);
	Agent_Calculate_Sigmoid(agent,NEURON_MOVE_Y);
	Agent_Calculate_Sigmoid(agent,NEURON_MOVE_ANGLE);
	Agent_Calculate_Sigmoid(agent,NEURON_EAT);
	Agent_Calculate_Sigmoid(agent,NEURON_SPLIT);
	Agent_Calculate_Sigmoid(agent,NEURON_OUT_1);
	Agent_Calculate_Sigmoid(agent,NEURON_OUT_2);
	Agent_Calculate_Sigmoid(agent,NEURON_OUT_3);
	Agent_Calculate_Sigmoid(agent,NEURON_SPEAK_1);
	Agent_Calculate_Sigmoid(agent,NEURON_SPEAK_2);
	Agent_Calculate_Sigmoid(agent,NEURON_SPEAK_3);
	Agent_Calculate_Sigmoid(agent,NEURON_SPEEDBOOST);
	Agent_Calculate_Sigmoid(agent,NEURON_SCHIELD);
	Agent_Calculate_Sigmoid(agent,NEURON_SET1_ON);
	Agent_Calculate_Sigmoid(agent,NEURON_SET1_OFF);
	Agent_Calculate_Sigmoid(agent,NEURON_SET2_ON);
	Agent_Calculate_Sigmoid(agent,NEURON_SET2_OFF);
	Agent_Calculate_Sigmoid(agent,NEURON_STOP);
}
void Agent_Calculate_All2(Agent *agent)
{
    float x;
    for (int j=0; j<agent->connections_length+1; j++)
    {
        x = agent->potentials[agent->connections[j][0]] * agent->weights[j];
        if( x < 0.1f && x > -0.1f)
        {
            x = 0.0f;
        }
    	if (x > 1.0f)
    	{
        	x = 1.0f;
        }
        else
        {
            if(x < -1.0f)
            {
                x = -1.0f;
            }
        }
        agent->potentials[agent->connections[j][1]] = x;
    }
}
int Agent_Test_Connection(Agent *agent, int i, int n)
{
    int t = 1;
    n -= 1;
    if (n>0)
    {
        for (int j=0; j<agent->connections_length+1; j++)
        {
            if (i==agent->connections[j][1])
            {
                t = Agent_Test_Connection(agent, agent->connections[j][0], n);
                if (t!=1)
                {
                    break;
                }
            }
        }
    }
    else
    {
        t = 0;
    }
    return t;
}
int Agent_Add_Connection(Agent *agent, int i, int j, float weight, int neuronsin[], int neuronsint, int neuronsout[], int neuronsoutt)
{
    for(int k=0; k<neuronsint;k++)
    {
        if(neuronsin[k]==j)
        {
            return 0;
        }
    }
    for(int k=0; k<neuronsoutt;k++)
    {
        if(neuronsout[k]==i)
        {
            return 0;
        }
    }
	if (i != j && agent->connections_length < NEURONS_CONNECTIONS -1)
	{
		agent->connections_length += 1;
		agent->connections[agent->connections_length][0] = i;
		agent->connections[agent->connections_length][1] = j;
		agent->weights[agent->connections_length] = weight;
		if (!Agent_Test_Connection(agent,j,10))
		{
			agent->connections[agent->connections_length][0] = -13;
			agent->connections[agent->connections_length][1] = -13;
			agent->weights[agent->connections_length] = 0.0f;
			agent->connections_length -= 1;
		return 0;
		}
		return 1;
	}
	else{return 0;}
}

void Agent_Remove_Connection(Agent *agent, int i)
{
	if (i <= agent->connections_length && i != -1)
	{
		agent->connections[i][0] = agent->connections[agent->connections_length][0];
		agent->connections[i][1] = agent->connections[agent->connections_length][1];
		agent->connections[agent->connections_length][0] = -13;
		agent->connections[agent->connections_length][1] = -13;
        agent->weights[agent->connections_length] = 0.0f;
		agent->weights[i] = agent->weights[agent->connections_length];
		agent->connections_length -= 1;
	}
}
void Agent_Change_Connection(Agent *agent, int i, float weight)
{
	if (i <= agent->connections_length && i != -1)
	{
		agent->weights[i] = weight;
	}
}
void Agent_Create_Connections(Agent *agent, int wile, int neuronsin[], int neuronsint, int neuronsout[], int neuronsoutt)
{
	int amount = ((int)rand()%(AGENT_START_CONNECTIONS));
	int i,j,t,tt,test,yy,i1,i2;
	float weight;
	for (i=0; i<amount; i++)
	{
		t = 0;
		test = 0;
		while (t == 0 && (test < 200 && wile==1))
		{
			test += 1;
			tt = 1;
			i1 = ((int)rand()%NEURONS_AMOUNT);
			i2 = ((int)rand()%NEURONS_AMOUNT);
			if (i1==i2)
			{
				tt = 0;
			}
			for (j=0; j< agent->connections_length;j++)
			{
				if (i1==agent->connections[j][0]&&i2==agent->connections[j][1])
				{
					tt = 0;
				}
			}
			weight = ((float)(rand()%(2*NEURONS_WEIGHT_SIZE) - NEURONS_WEIGHT_SIZE)/(float)NEURONS_WEIGHT_SCALE) * ((float)(rand()%NEURONS_WEIGHT_MULTIPLIER));
			if (tt==1)
			{
				t = Agent_Add_Connection(agent,i1,i2,weight, neuronsin, neuronsint, neuronsout, neuronsoutt);
			}
		}

	}
}

void Agent_Heridity(Agent *agent, int mutant, int neuronsin[], int neuronsint, int neuronsout[], int neuronsoutt)
{

    int r = rand() % 105;
    Agent_CM(agent);
    if(r > 20)
    {
        if (r < 50 && r > 20)
        {
            int ri = rand()%NEURONS_AMOUNT;
            int rj = rand()%NEURONS_AMOUNT;
            float rweight = ((float)(rand()%(2*(int)NEURONS_WEIGHT_SIZE) - (float)NEURONS_WEIGHT_SIZE)
            /((float)NEURONS_WEIGHT_SCALE));
            Agent_Add_Connection(agent, ri, rj, rweight, neuronsin, neuronsint, neuronsout, neuronsoutt);
        }
        else
        {
            if (r < 50 + (int)( 30.0f*((float)agent->connections_length + 1.0f) / (float)NEURONS_CONNECTIONS))
            {
                if (agent->connections_length > 20)
                {
                    int ri = rand()%agent->connections_length;
                    Agent_Remove_Connection(agent,ri);
                }
            }
            else
            {
                if (r < 99)
                {
                    if (agent->connections_length > 0)
                    {
                        int ri = rand()%agent->connections_length;
                        if((r+ri+mutant)%3==0)
                        {
                            float rweight = ((float)(rand()%(2*(int)NEURONS_WEIGHT_SIZE) -
                            (float)NEURONS_WEIGHT_SIZE)/((float)NEURONS_WEIGHT_SCALE))  * ((float)(rand()%NEURONS_WEIGHT_MULTIPLIER));
                            Agent_Change_Connection(agent,ri,rweight);
                        }
                        else
                        {
                            float rweight = ((float)(rand()%(3) - 1)/((float)NEURONS_WEIGHT_SCALE));
                            Agent_Change_Connection(agent,ri,agent->weights[ri] + rweight);
                        }
                    }
                }
                else
                {
                    if(mutant==1 && r < 105)
                    {
                        Agent_Create_Connections(agent,0, neuronsin, neuronsint, neuronsout, neuronsoutt);
                        for (int i=0; i<NEURONS_AMOUNT; i++)
                        {
                            agent->potentials[i] = (float)NEURONS_START_POTENTIAL + (float)POTENTIALSHIFT;// + ((float)(rand()) / (float)(RAND_MAX)- 0.5f) / 10.0f;
                        }
                        agent->color[0] = rand()%230 + 25;
                        agent->color[1] = rand()%230 + 25;
                        agent->color[2] = rand()%230 + 25;
                        agent->mutation = agent->lineage;
                    }
                }
            }
        }
    }
}
void Agent_CM(Agent* agent)
{
    int margin = 8;
    int rr = rand()%3-1;
    int rg = rand()%3-1;
    int rb = rand()%3-1;
    agent->color[0] += rr*margin;
    agent->color[1] += rg*margin;
    agent->color[2] += rb*margin;
    if(agent->color[0] > 255)
    {
        agent->color[0] = 255;
    }
    if(agent->color[1] > 255)
    {
        agent->color[1] = 255;
    }
    if(agent->color[2] > 255)
    {
        agent->color[2] = 255;
    }
    if(agent->color[0] < 20)
    {
        agent->color[0] = 20;
    }
    if(agent->color[1] < 20)
    {
        agent->color[1] = 20;
    }
    if(agent->color[2] < 20)
    {
        agent->color[2] = 20;
    }
}
void Agent_Color(Agent* agent)
{
    static int ii = NEURONS_CONNECTIONS / 3;
    static int ir = 0;
    static int ig = 0;
    static int ib = 0;

    for(int i=0; i<NEURONS_CONNECTIONS; i++)
    {
        if(i%3==0)
        {
            ir += (int)agent->weights[i]*(int)agent->weights[i];
        }
        if(i%3==1)
        {
            ig += (int)agent->weights[i]*(int)agent->weights[i];
        }
        if(i%3==2)
        {
            ib += (int)agent->weights[i]*(int)agent->weights[i];
        }
    }

    int im = ir;
    if(ig>im)
    {
        im = ig;
    }
    if(ib>im)
    {
        im = ib;
    }
    if(im==0)
    {
        im = 1;
    }
    int imm = 200/im;

    agent->color[0] = ir*imm + 50;
    agent->color[1] = ig*imm + 50;
    agent->color[2] = ib*imm + 50;
    printf("%d %d %d\n",agent->color[0],agent->color[1],agent->color[2]);
}

void Agent_Gravitate(Agent *agent,int gravitate)
{
    float dx = (float)(agent->x - (float)UNIVERSE_SIZE / 2.0f);
	float dy = (float)(agent->y - (float)UNIVERSE_SIZE / 2.0f);
	float fx,fy;
	if (dx*dx + dy*dy > (float)(UNIVERSE_SIZE*UNIVERSE_SIZE)/4.0f)
	{
		if (dx == 0.0f)
		{
			dx = 0.1f;
		}
		if (dy == 0.0f)
		{
			dy = 0.1f;
		}
		fx = (float)(-dx / ((float)UNIVERSE_FORCE) * 1.0f * (dx * dx + dy * dy));
		fy = (float)(-dy / ((float)UNIVERSE_FORCE) * 1.0f * (dx * dx + dy * dy));
		agent->ax = fx;
		agent->ay = fy;
	}
	else
	{

        agent->ax = 0.0f;
        agent->ay = 0.0f;
	}
	if(gravitate==1)
    {
        if (dx == 0.0f)
        {
            dx = 0.1f;
        }
        if (dy == 0.0f)
        {
            dy = 0.1f;
        }
        fx = (float)(-dx / ((float)UNIVERSE_FORCE2) * (dx * dx + dy * dy));
        fy = (float)(-dy / ((float)UNIVERSE_FORCE2) * (dx * dx + dy * dy));
		agent->ax = fx;
		agent->ay = fy;
    }
}
void Agent_Collide(Agent *agent1, Agent *agent2)
{
    float m1 = (float)(agent1->r);
    float m2 = (float)(agent2->r);
    float cosY = (float)((agent1->x - agent2->x) / (agent1->r + agent2->r));
    float sinY = (float)((agent1->y - agent2->y) / (agent1->r + agent2->r));
    float v1x = (float)(agent1->vx);
    float v1y = (float)(agent1->vy);
    float v2x = (float)(agent2->vx);
    float v2y = (float)(agent2->vy);
    float dc = 1.0f;
	agent1->vx = (float)(UNIVERSE_COLISSION_MULTIPLYER*((v1x * cosY + v1y * sinY) * (m1 - m2) + 2.0f * m2 * (v2x * cosY + v2y * sinY)) * cosY / (m1 + m2) - (v1y * cosY - v1x * sinY) * sinY);
	agent1->vy = (float)(UNIVERSE_COLISSION_MULTIPLYER*((v1x * cosY + v1y * sinY) * (m1 - m2) + 2.0f * m2 * (v2x * cosY + v2y * sinY)) * sinY / (m1 + m2) + (v1y * cosY - v1x * sinY) * cosY);
	agent2->vx = (float)(UNIVERSE_COLISSION_MULTIPLYER*((v2x * cosY + v2y * sinY) * (m2 - m1) + 2.0f * m1 * (v1x * cosY + v1y * sinY)) * cosY / (m1 + m2) - (v2y * cosY - v2x * sinY) * sinY);
	agent2->vy = (float)(UNIVERSE_COLISSION_MULTIPLYER*((v2x * cosY + v2y * sinY) * (m2 - m1) + 2.0f * m1 * (v1x * cosY + v1y * sinY)) * sinY / (m1 + m2) + (v2y * cosY - v2x * sinY) * cosY);
	int tttt1 = 0;
	int tttt2 = 0;
	float tt1 = atan2(agent2->y - agent1->y,agent2->x - agent1->x) - agent1->angle ;
	float tt2 = atan2(agent1->y - agent2->y,agent1->x - agent2->x) - agent2->angle ;
    if(- AGENT_FOV < tt1&& tt1< AGENT_FOV)
    {
        tttt1 = 1;
    }
    if(- AGENT_FOV < tt2&& tt2 < AGENT_FOV)
    {
        tttt2 = 1;
    }

	if (agent1->x >= agent2->x)
	{
		agent1->x += dc;
		agent2->x -= dc;
	}
	else
	{
		agent1->x -= dc;
		agent2->x += dc;
	}
	if (agent1->y >= agent2->y)
	{
		agent1->y += dc;
		agent2->y -= dc;
	}
	else
	{
		agent1->y -= dc;
		agent2->y += dc;
	}
	if(agent1->time > AGENT_TIME && agent1->potentials[NEURON_EAT] > (float)POTENTIALSHIFT && tttt1 && agent2->schield == 0)
	{
        if(!(agent2->time > AGENT_TIME && agent2->potentials[NEURON_EAT] > (float)POTENTIALSHIFT && tttt2))
        {
            if (agent2->health>(float)AGENT_EAT_DAMAGE)
            {
                agent2->health -= (float)AGENT_EAT_DAMAGE;
                agent2->stamina -= (int)AGENT_EAT_DAMAGE;
                agent1->health += (float)AGENT_EAT_PERCENTAGE*AGENT_EAT_DAMAGE;
            }
            else
            {
                agent1->health += (float)AGENT_EAT_PERCENTAGE*agent2->health;
                agent1->kills += 1;
                agent2->health = -10.0f;
                agent2->alive = 0;
            }
        }
	}


	if(agent2->time > AGENT_TIME && agent2->potentials[NEURON_EAT] > (float)POTENTIALSHIFT && tttt2 && agent1->schield == 0)
	{
	    if(!(agent1->time > AGENT_TIME && agent1->potentials[NEURON_EAT] > (float)POTENTIALSHIFT && tttt1))
        {
            if (agent1->health>(float)AGENT_EAT_DAMAGE)
            {
                agent1->health -= (float)AGENT_EAT_DAMAGE;
                agent1->stamina -= (int)AGENT_EAT_DAMAGE;
                agent2->health += (float)AGENT_EAT_PERCENTAGE*AGENT_EAT_DAMAGE;
            }
            else
            {
                agent2->health += (float)AGENT_EAT_PERCENTAGE*agent1->health;
                agent2->kills += 1;
                agent1->health = -10.0f;
                agent1->alive = 0;
            }
        }
	}
}

int Agent_See(float x, float y, float jx, float jy, float a, float d)
{
    float m,b,D1,D2,D,sinn,coss;
    int tttt;
    sinn = sin(a);
    coss = cos(a);
    if (coss != 0.0f)
    {
        m = sinn/coss;
    }
    else
    {
        if (sinn < 0.0f)
        {
            m = -1000000.0f;
        }
        else
        {
            m = 1000000.0f;
        }
    }
    b = -m*x + y;
    D1 = (2.0f*m*b-2.0f*m*jy-2.0f*jx);
    D2 = -4.0f*(m*m+1.0f)*(b*b - 2.0f*b*jy + jx*jx + jy*jy - d * tan(AGENT_FOV) * d * tan(AGENT_FOV));
    D = D1*D1 + D2;
    tttt = 0;
    if (D > 0.0f)
    {
        if(sinn > 0.0f && jy>y)
        {
            if(coss > 0.0f && jx>x)
            {
                tttt = 1;
            }
            if(coss <= 0.0f && jx<=x)
            {
                tttt = 1;
            }
        }
        if(sinn <= 0.0f && jy<=y)
        {
            if(coss > 0.0f && jx>x)
            {
                tttt = 1;
            }
            if(coss <= 0.0f && jx<=x)
            {
                tttt = 1;
            }
        }
    }
    return tttt;

}
