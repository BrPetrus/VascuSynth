#pragma warning(disable:4786)

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

#include "MersenneTwister.h"
#include "OxygenationMap.h"
#include "SupplyMap.h"

using namespace std;

// A parametric macro to allow the use of dynamic multi-dimensional arrays
#define arr(arr,x,y,z,dim) *(arr + (z + dim[2]*(y + dim[1]*(x))))

OxygenationMap::OxygenationMap(SupplyMap *sMap, int randSeed):rand((MTRand::uint32)randSeed){
	supply = sMap;
}

//calculate the sum of the current effective map
double OxygenationMap::sum(){
	double acc = 0;
	
	for(int i = 0; i < dim[0]; i++){
		for(int j = 0; j < dim[1]; j++){
			for(int k = 0; k < dim[2]; k++)
				acc += arr(effectiveMap_d, i, j, k, dim);
		}
	}
	
	return acc;
}

//selecte a candidate terminal node
void OxygenationMap::candidate(double sum, int *cand){
	//MTRand rand;
	double r = rand.rand()*sum;
	
	double acc = 0;
	for(int i = 0; i < dim[0]; i++){
		for(int j = 0; j < dim[1]; j++){
			for(int k = 0; k < dim[2]; k++){
				acc += arr(effectiveMap_d, i, j, k, dim);
				if(acc >= r){
					cand[0] = i;
					cand[1] = j;
					cand[2] = k;
					return;
				}
			}
		}
	}
	
	return;
}

//update the effective map based on using cand as a new terminal node
void OxygenationMap::applyCandidate(int cand[]){
	int temp[3];
	for(int i = 0; i < dim[0]; i++){
		for(int j = 0; j < dim[1]; j++){
			for(int k = 0; k < dim[2]; k++){
				temp[0] = i; temp[1] = j; temp[2] = k;
				arr(effectiveMap_d, i, j, k, dim) *= supply->reduction(cand, temp);
			}
		}
	}
}

//determine if source is visible from target with respect to the oxygenation map (uses original map - not effective map)
bool OxygenationMap::visible(double source[], double target[]){
	double vect[3];
	vect[0] = target[0] - source[0];
	vect[1] = target[1] - source[1];
	vect[2] = target[2] - source[2];
	
	double pos[3];
	pos[0] = source[0]; pos[1] = source[1]; pos[2] =  source[2];
	
	int voxel[3];
	voxel[0] = (int)(source[0]+0.5); voxel[1] = (int)(source[1]+0.5); voxel[2] = (int)(source[2]+0.5);
	
	int targetVoxel[3];
	targetVoxel[0] =(int)(target[0]+0.5); targetVoxel[1] =(int)(target[1]+0.5); targetVoxel[2] =(int)(target[2]+0.5);

	int i;	
	while( !(voxel[0] == targetVoxel[0] && voxel[1] == targetVoxel[1] && voxel[2] == targetVoxel[2]) &&
			!((fabs(pos[0] - target[0]) < 1e-10) && (fabs(pos[1] - target[1]) < 1e-10) && (fabs(pos[2] - target[2]) < 1e-10))){
		
		double mult = 1e50;
		
		double dir = 0.5;
		for(i = 0; i < 3; i++){
			if(vect[i] < 0)
				dir = -0.5;
			else
				dir = 0.5;
			
			double singleMult = fabs((voxel[i]-pos[i] + dir)/vect[i]);
			while(singleMult == 0){
				//singleMult should always be > 0, this will only present itself as a problem 
				// when vect[i] < 0 - where pos[i] moves to x.5 - when it should move to x.5 - c 
				// where c is some small positive number
				dir *= 1.000000001;
				singleMult = fabs((voxel[i]-pos[i] + dir)/vect[i]);
			}
			if(singleMult < mult)
				mult = singleMult;
		}
		
		for(i = 0; i < 3; i++){
			pos[i] += mult*vect[i];
			voxel[i] = (int)(pos[i]+0.5);
		}

		if(arr(map_d, voxel[0], voxel[1], voxel[2], dim) == 0)
			return false;
	}
	
	return true;
}

//load a file containing the description of an oxygenation map
void OxygenationMap::loadMap(char* filename){
	ifstream mapFile;
	mapFile.open(filename);
	string line;
	
	if(mapFile.is_open()){
		//defining i,j,k,l outside of the for stament so this is compatable with both VC++ 6.0 and later versions
		int i,j,k; 

		getline(mapFile, line);
		char* tok = new char[line.size()];
		strcpy(tok, line.c_str());
		
		for(i = 0; i < 3; i++){
			char * temp = strtok((i == 0? tok : NULL), " ");
			if(temp == NULL)
				break;
			dim[i] = atoi(temp);
		}
		
		map_d = new double[dim[0]*dim[1]*dim[2]];
		effectiveMap_d = new double[dim[0]*dim[1]*dim[2]];

		for(i = 0; i < dim[0]; i++)
			for(j = 0; j < dim[1]; j++)
				for(k = 0; k < dim[2]; k++){
					arr(map_d, i, j, k, dim) = 0;
					arr(effectiveMap_d, i, j, k, dim) = 0;
				}

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

			double value = atof(line.c_str());
			
			for(i = region[0]; i < region[3]; i++)
				for(j = region[1]; j< region[4]; j++)
					for(k = region[2]; k < region[5]; k++){
						arr(map_d, i, j,k, dim) = value;
						arr(effectiveMap_d, i, j, k, dim) = value;
					}
		}
	}
}
