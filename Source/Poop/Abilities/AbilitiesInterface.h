#pragma once

class IAbilitiesComponent
{
public:
	IAbilitiesComponent() {}
	virtual ~IAbilitiesComponent() {}
	virtual void UseAbility() = 0;
};