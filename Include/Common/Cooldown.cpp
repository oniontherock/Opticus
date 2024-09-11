#include "Cooldown.hpp"

Cooldown::Cooldown()
	: target(0.0f)
	, value(0.0f)
{}

Cooldown::Cooldown(float t)
	: target(t)
	, value(0.0f)
{}

Cooldown::Cooldown(float t, float v)
	: target(t)
	, value(v)
{}

void Cooldown::update(float dt)
{
	value += dt;
}

bool Cooldown::updateAutoReset(float dt)
{
	update(dt);
	bool res = ready();
	if (res) {
		reset();
	}
	return res;
}

bool Cooldown::ready() const
{
	return value >= target;
}

bool Cooldown::readyNext(float dt) const
{
	return value < target && value + dt >= target;
}

float Cooldown::getRatio() const
{
	return value / target;
}

void Cooldown::reset()
{
	value = 0.0f;
}