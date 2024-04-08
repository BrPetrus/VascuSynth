#ifndef _treedrawer_h
#define _treedrawer_h

#include "VascularTree.h"

#include <vector>

using namespace std;

class TreeDrawer{
public:
	double imageVoxelWidth;
	double mapVoxelWidth;
	VascularTree * vt;
	double *c1, *c2;
	unsigned char *image;
	int dim[3];
	
	TreeDrawer(VascularTree * vt, double width, double *corner1, double *corner2);

	/**
	 * maps a voxel to a point
	 * 8 subsections in a voxel
	 * if a voxel has corners (0,0,0) and (1,1,1)
	 * 	section 0 has corners (0.0, 0.0, 0.0) and (0.5, 0.5, 0.5) => center = voxel center + (-0.25, -0.25, -0.25)
	 * 	section 1 has corners (0.5, 0.0, 0.0) and (1.0, 0.5, 0.5) => center = voxel center + (+0.25, -0.25, -0.25)
	 * 	section 2 has corners (0.0, 0.5, 0.0) and (0.5, 1.0, 0.5) => center = voxel center + (-0.25, +0.25, -0.25)
	 * 	section 3 has corners (0.5, 0.5, 0.0) and (1.0, 1.0, 0.5) => center = voxel center + (+0.25, +0.25, -0.25)
	 * 	section 4 has corners (0.0, 0.0, 0.5) and (0.5, 0.5, 1.0) => center = voxel center + (-0.25, -0.25, +0.25)
	 * 	section 5 has corners (0.5, 0.0, 0.5) and (1.0, 0.5, 1.0) => center = voxel center + (+0.25, -0.25, +0.25)
	 * 	section 6 has corners (0.0, 0.5, 0.5) and (0.5, 1.0, 1.0) => center = voxel center + (-0.25, +0.25, +0.25)
	 * 	section 7 has corners (0.5, 0.5, 0.5) and (1.0, 1.0, 1.0) => center = voxel center + (+0.25, +0.25, +0.25)
	 */
	void voxelToPoint(int* voxel, int subSection, double*ret);	
	//checks if a point is inside a given tube
	bool inTube(double * point, double * p1, double * p2, double radius);	
	//checks if a point is in the end of a tube
	bool inEnd(double *point, double *p1, double radius);	
	//checks if a point is in the same region as a tube (used to improve performance)
	bool checkTube(double *point, double *to, double *from, double radius);	
	//determines the value at a specific voxel
	unsigned char valueAtVoxel(int* voxel);	

	//add some uniform noise to a voxel
	unsigned char addNoise_Uniform(unsigned char c, double lb, double ub);	
	//add some salt&pepper noise to a voxel
	unsigned char addNoise_saltPepper(unsigned char c, unsigned char valSalt, double probSalt, unsigned char valPepper, double probPepper);	
	//add some gaussian noise to a voxel
	unsigned char addNoise_gaussian(unsigned char c, double median, double sigma);

	//draw the tree to a matrix
	void drawImage();
	//add uniform noise to the matrix
	void addNoise_Uniform(double lb, double ub);
	//add salt&pepper noise to the matirx
	void addNoise_saltPepper(unsigned char valSalt, double probSalt, unsigned char valPepper, double probPepper);
	//add gaussian noise to the matrix
	void addNoise_gaussian(double median, double sigma);
	//add a single shadow @ (x,y,z)
	void addShadow(double x, double y, double z, double centerRatio, double r);	
	//add 'numShadows' shadows to the matix
	void addShadows(int numShadows);
	//get the value of the matrix at (x,y,z)
	unsigned char imageAt(int x, int y, int z);
	//copy the matrix
	TreeDrawer copy();
};

#endif