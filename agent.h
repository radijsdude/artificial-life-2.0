#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED
#ifndef CONSTANTS_H_INCLUDED
#include "constants.h"
#endif
struct Agent
{
    float r;
	float x;
	float y;
	float vx;
	float vy;
	float ax;
	float ay;
	float angle;

	float health;
	int time;

	int origin_id;
	int lineage;
	int mutation;
	int isselected;

	int kills;
	int splits;

	int stamina;
	int speedboost;
	int schield;

	int alive;

	int color[4];

	int connections_length;
	float potentials[NEURONS_AMOUNT];
	float errors[NEURONS_AMOUNT];
	int connections[NEURONS_CONNECTIONS][2];
	float weights[NEURONS_CONNECTIONS];
};


void Agent_Init(Agent* agent);
float Agent_Calculate_Sigmoid(Agent *agent, int i);
void Agent_Calculate_All(Agent *agent);
void Agent_Calculate_All2(Agent *agent);
int Agent_Test_Connection(Agent *agent, int i, int n);
int Agent_Add_Connection(Agent *agent, int i, int j, float weight, int neuronsin[], int neuronsint, int neuronsout[], int neuronsoutt);
void Agent_Remove_Connection(Agent *agent, int i);
void Agent_Change_Connection(Agent *agent, int i, float weight);
void Agent_Heridity(Agent *agent2, int mutant, int neuronsin[], int neuronsint, int neuronsout[], int neuronsoutt);
void Agent_Create_Connections(Agent *agent,int wile, int neuronsin[], int neuronsint, int neuronsout[], int neuronsoutt);

void Agent_CM(Agent* agent);
void Agent_Color(Agent* agent);

void Agent_Gravitate(Agent *agent, int gravitate);
void Agent_Collide(Agent *agent1, Agent *agent2);
int Agent_See(float x, float y, float jx, float jy, float a, float d);

#endif // LARTICLE_H_INCLUDED
