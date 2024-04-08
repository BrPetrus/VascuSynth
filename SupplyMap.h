#ifndef _supplymap_h
#define _supplymap_h

#include <vector>
#include <string>


using namespace std;

class SupplyMap{
public:
	double *map_d;
	int dim[4];
	//load a file describing a supply map
	void loadMap(char* filename);
	//calculate the reduction to target if supplied becomes a terminal node
	double reduction(int supplied[], int target[]);
};

#endif