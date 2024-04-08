#pragma warning(disable:4786)

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

#include "SupplyMap.h"

using namespace std;

// A parametric macro to allow the use of dynamic multi-dimensional arrays
#define arr(arr,w,x,y,z,dim) *(arr + (z + dim[3]*(y + dim[2]*(x + dim[1]*(w)))))


//calculate the reduction to target if supplied becomes a terminal node
double SupplyMap::reduction(int supplied[], int target[]){
	if(supplied[0] == target[0] && supplied[1] == target[1] && supplied[2] == target[2])
		return 0;

	double dist = sqrt(pow((double)(supplied[0] - target[0]), 2) + pow((double)(supplied[1] - target[1]),2 )+ pow((double)(supplied[2] - target[2]), 2));
	double acc = 0;

	if(arr(map_d, target[0], target[1], target[2], dim[3]-1, dim) <= dist)
		return 1;
	
	for(int i = 0; i < dim[3]; i++){
		acc += pow(dist, i)*arr(map_d, target[0], target[1], target[2], i, dim);
	}
	
	acc = 1.0 - (1.0/acc);
	acc = (acc < 0 ? 0 : acc);
	
	
	return acc;
}

void SupplyMap::loadMap(char* filename){
	ifstream mapFile;
	mapFile.open(filename);
	string line;
	
	if(mapFile.is_open()){
		//defining i,j,k,l outside of the for stament so this is compatable with both VC++ 6.0 and later versions
		int i,j,k,l; 

		getline(mapFile, line);
		char* tok = new char[line.size()];
		strcpy(tok, line.c_str());
		
		for(i = 0; i < 4; i++){
			char * temp = strtok((i == 0? tok : NULL), " ");
			if(temp == NULL)
				break;
			dim[i] = atoi(temp);
		}
		
		//get the dimensions
		map_d = new double[dim[0]*dim[1]*dim[2]*dim[3]];

		for(i = 0; i < dim[0]; i++)
			for(j = 0; j < dim[1]; j++)
				for(k = 0; k < dim[2]; k++)
					arr(map_d,i,j,k,dim[3]-1,dim) = 0;

		//get the regions
		while(!mapFile.eof()){
			getline(mapFile, line);
						
			tok = new char[line.size()];
			strcpy(tok, line.c_str());

			int region[6];
			for(i = 0; i < 6; i++){
				char * temp = strtok((i == 0? tok : NULL), " ");
				if(temp == NULL)
					break;
				region[i] = atoi(temp);
			}
				
			if(mapFile.eof())
				break;

			getline(mapFile, line);
			tok = new char[line.size()];
			strcpy(tok, line.c_str());

			double *values = new double[dim[3]];
			for(i = 0; i < dim[3]; i++)
				values[i] = atof(strtok((i == 0? tok : NULL), " "));

			for(i = region[0]; i < region[3]; i++)
				for(j = region[1]; j< region[4]; j++)
					for(k = region[2]; k < region[5]; k++)
						for(l = 0; l < dim[3]; l++)
							arr(map_d, i, j, k, l, dim) = values[l];
		}
	}
}
