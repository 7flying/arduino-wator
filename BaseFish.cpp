#include "BaseFish.h"

BaseFish::BaseFish() {
	this->survivedChronons = 0;
}

void BaseFish::incChronon() {
	this->survivedChronons++;
}

void BaseFish::reproduce() {
	this->survivedChronons = 0;
}

unsigned int BaseFish::getSurvivedChronons() const {
	return this->survivedChronons;
}
