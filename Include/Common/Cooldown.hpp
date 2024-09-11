#ifndef __TIMER_H__
#define __TIMER_H__

struct Cooldown {

	float target;
	float value;

	Cooldown();
	Cooldown(float t);
	Cooldown(float t, float v);

	void update(float dt);
	bool updateAutoReset(float dt);

	bool ready() const;
	bool readyNext(float dt) const;

	float getRatio() const;

	void reset();
};

#endif