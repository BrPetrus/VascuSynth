#ifndef _vasculartree_h
#define _vasculartree_h

#include "NodeTable.h"
#include "OxygenationMap.h"

class VascularTree{
public:
	NodeTable nt;
	OxygenationMap *oxMap;

	double Pperf;
	double Pterm;
	double Qperf;
	double Qterm;
	
	double row;
	double gamma;
	double lambda;
	double mew;
	
	double minDistance;
	int numNodes;
	
	double* perf;
	
	int closestNeighbours;
	
	double mapVoxelWidth; //in cm
	
	VascularTree(OxygenationMap * oxMap, double* perf, double Pperf, double Pterm, double Qperf, double row, double gamma, double lambda, double mew, double minDistance, int numNodes, double voxelWidth, int closestNeighbours);
		
	//calculate the distance between to nodes in the node table
	double distance(int from, int to);
	//calculate the reduced resistence of segment @ id
	void calculateReducedResistence(int id);	
	//calculates the ratio of radii of the segment @ id
	void calculateRatios(int id);	
	//update the tree @ the bifurication point @ id
	void updateAtBifurication(int id, int newChild);
	
	//calculate the radii throughout the tree
	void calculateRadius();
	void calculateRadius(int id);

	//calculate the fitness function
	double calculateFitness();

	/**
	 * When used by local optimization, ignored is the segment to connect to
	 * otherwise it sould be -1;
	 */
	bool validateCandidate(double* x0, int ignored);
	
	//connect point to segment through bifPoint
	void connectPoint(double* point, int segment, double* bifPoint);	
	//update the flow throughout the tree
	void incrementFlow(int parent, double Qterm);	
	//the distance between a point and a segment
	double pointSegmentDistance(double* x0, int segment);
	//optimize the location of a bifurication point for terminal node point and segment 'segment'
	double* localOptimization(double * point, int segment, int steps);
	//determine if point is in the volume
	bool inVolume(double * point);
	//try to attach point to the tree
	bool connectCandidate(double * point, int steps);
	//build the tree
	void buildTree();
};

#endif