#pragma once
#include <stdlib.h>
//#include <random>

class CreatureFactory
{
public:

	static CreatureFactory & instance();
	virtual ~CreatureFactory();
	//Creature * generateCreature( int iMaxLevel, std::mt19937 & mt );

private:
	CreatureFactory(void);
	//std::vector<Creature> m_vCreatures;
//	std::mt19937 m_mt;
};

