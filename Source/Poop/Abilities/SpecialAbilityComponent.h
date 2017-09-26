#pragma once
#include "AbilitiesInterface.h"

class SpecialAbilityComponent : public IAbilitiesComponent
{
public:
	SpecialAbilityComponent();

	virtual void UseAbility() override;
};