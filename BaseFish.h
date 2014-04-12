#ifndef _BASEFISH_H_
#define _BASEFISH_H_

class BaseFish {

private:
	unsigned int survivedChronons;

public:
	BaseFish();
	// Increases the chronon
	void incChronon();
	// Sets the chronon to 0
	void reproduce();
	// Gets the survived chronons
	unsigned int getSurvivedChronons() const;

};
#endif
