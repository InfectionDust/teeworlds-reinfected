#include "human.h"

#include <engine/shared/config.h>

CInfClassHuman::CInfClassHuman()
	: CInfClassPlayerClass()
{
}

void CInfClassHuman::OnSlimeEffect(int Owner)
{
	int Count = Config()->m_InfSlimePoisonDuration;
	Poison(Count, Owner);
}
