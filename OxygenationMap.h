#ifndef _oxygenationmap_h
#define _oxygenationmap_h

#include <string>
#include <vector>

#include "SupplyMap.h"
#include "MersenneTwister.h"

using namespace std;

class OxygenationMap{
public:
	SupplyMap *supply;

	double * map_d;
	double * effectiveMap_d;
	MTRand rand;

	int dim[3];

	OxygenationMap(SupplyMap *sMap, int randSeed);

	//load a file containing the description of an oxygenation map
	void loadMap(char* filename);
	//calculate the sum of the current effective map
	double sum();
	//selecte a candidate terminal node
	void candidate(double sum, int *cand);
	//update the effective map based on using cand as a new terminal node
	void applyCandidate(int cand[]);
	//determine if source is visible from target with respect to the oxygenation map (uses original map - not effective map)
	bool visible(double source[], double target[]);
};

#endif