#include "Shark.h"

unsigned int Shark::reproducesIn = 4;

Shark::Shark(unsigned int energy):BaseFish() {
	this->energy = energy;
}

void Shark::decreaseEnergy() {
	this->energy--;
}

unsigned int Shark::getEnergy() const {
	return this->energy;
}
