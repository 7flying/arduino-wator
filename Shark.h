#ifndef _SHARK_H_
#define _SHARK_H_
#include "BaseFish.h"

class Shark: public BaseFish {

private:
	unsigned int energy;

public:
	static unsigned int reproducesIn;
	Shark(unsigned int energy);
	// Decreases the shark's energy
	void decreaseEnergy();
	// Gets the shark's energy
	unsigned int getEnergy() const;

};

#endif