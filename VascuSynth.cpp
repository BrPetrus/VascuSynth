#pragma warning(disable:4786)

#include "direct.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkImageSeriesWriter.h"
#include "itkNumericSeriesFileNames.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <cmath>

#include "OxygenationMap.h"
#include "SupplyMap.h"
#include "TreeDrawer.h"
#include "VascularTree.h"

using namespace std;


/**
	Parameter File Entries:

	SUPPLY_MAP: supply map file
	OXYGENATION_MAP: oxygenation map file
	PERF_POINT: perf_x perf_y perf_z
	PERF_PRESSURE: perfussion pressure
	TERM_PRESSURE: termination pressure
	PERF_FLOW:	perfusion flow
	ROW: row
	GAMMA: gamma
	LAMBDA: lambda
	MEW: mew
	MIN_DISTANCE: minDistance
	NUM_NODES: numNodes
	VOXEL_WIDTH: voxelWidth
	CLOSEST_NEIGHBOURS: closestNeighbours
*/
vector<string> * readFileLines(const char * filename){
	ifstream mapFile;
	mapFile.open(filename);
	string line;
	vector<string> * paramFiles = new vector<string>;
	if(mapFile.is_open()){
		while(!mapFile.eof()){
			getline(mapFile, line);
			string copy = line;
			paramFiles->push_back(copy);
		}
	} else 
		cout<<"Could not open "<<filename<<endl;
	mapFile.close();
	return paramFiles;
}


VascularTree * buildTree(const char * filename){
	SupplyMap * sm = NULL;
	OxygenationMap * om = NULL;
	double* perf = new double[3];
	double pperf;
	double pterm;
	double qperf;
	double row;
	double gamma;
	double lambda;
	double mew;
	double minDistance;
	int numNodes;
	double voxelWidth;
	int closestNeighbours;
	int randomSeed = 0;

	ifstream mapFile;
	mapFile.open(filename);
	string line;

	if(mapFile.is_open()){
		
		while(!mapFile.eof()){
			getline(mapFile, line);
			char* tok = new char[line.size()];
			strcpy(tok, line.c_str());
			
			char * field = strtok(tok, " ");
			
			if(strcmp(field, "SUPPLY_MAP:") == 0){
				sm = new SupplyMap();
				char *s = strtok(NULL, " ");
				sm->loadMap(s);

				if(om != NULL)
					om->supply = sm;
			} else if(strcmp(field, "OXYGENATION_MAP:") == 0){
				om = new OxygenationMap(sm, randomSeed);
				
				char *s = strtok(NULL, " ");
				om->loadMap(s);
			} else if(strcmp(field, "PERF_POINT:") == 0){
				char *s = strtok(NULL, " ");
				perf[0] = atof(s);
				s = strtok(NULL, " ");
				perf[1] = atof(s);
				s = strtok(NULL, " ");
				perf[2] = atof(s);

			} else if(strcmp(field, "PERF_PRESSURE:") == 0){
				char *s = strtok(NULL, " ");
				pperf = atof(s);
			} else if(strcmp(field, "TERM_PRESSURE:") == 0){
				char *s = strtok(NULL, " ");
				pterm = atof(s);
			} else if(strcmp(field, "PERF_FLOW:") == 0){
				char *s = strtok(NULL, " ");
				qperf = atof(s);
			} else if(strcmp(field, "ROW:") == 0){
				char *s = strtok(NULL, " ");
				row = atof(s);
			} else if(strcmp(field, "GAMMA:") == 0){
				char *s = strtok(NULL, " ");
				gamma = atof(s);
			} else if(strcmp(field, "LAMBDA:") == 0){
				char *s = strtok(NULL, " ");
				lambda = atof(s);
			} else if(strcmp(field, "MEW:") == 0){
				char *s = strtok(NULL, " ");
				mew = atof(s);
			} else if(strcmp(field, "MIN_DISTANCE:") == 0){
				char *s = strtok(NULL, " ");
				minDistance = atof(s);
			} else if(strcmp(field, "NUM_NODES:") == 0){
				char *s = strtok(NULL, " ");
				numNodes = atoi(s);
			} else if(strcmp(field, "VOXEL_WIDTH:") == 0){
				char *s = strtok(NULL, " ");
				voxelWidth = atof(s);
			} else if(strcmp(field, "CLOSEST_NEIGHBOURS:") == 0){
				char *s = strtok(NULL, " ");
				closestNeighbours = atoi(s);
			} else if(strcmp(field, "RANDOM_SEED:") == 0){
				char *s = strtok(NULL, " ");
				randomSeed = atoi(s);
			} else{
				;				;
			}
		}
	}
	mapFile.close();
	VascularTree *vt = new VascularTree(om, perf, pperf, pterm, qperf, row, gamma, lambda, mew, minDistance, numNodes, voxelWidth, closestNeighbours);
	vt->buildTree();

	return vt;
}

TreeDrawer *drawTree(VascularTree * vt, double * corner1, double * corner2, double imageVoxelWidth){
	TreeDrawer * td = new TreeDrawer(vt, imageVoxelWidth, corner1, corner2);
	cout<<"Constructed tree drawer."<<endl;
	td->drawImage();

	return td;
}

void drawImage(TreeDrawer * td, const char* rootName){
	typedef unsigned char PixelType;
	const unsigned int Dimension = 3;
	typedef itk::Image< PixelType, Dimension > ImageType;

	ImageType::Pointer image = ImageType::New();

	ImageType::SizeType size;
	size[0] = td->dim[0]; // size along X
	size[1] = td->dim[1]; // size along Y
	size[2] = td->dim[2]; // size along Z

	ImageType::IndexType start;
	start[0] = 0; // first index on X
	start[1] = 0; // first index on Y
	start[2] = 0; // first index on Z

	ImageType::RegionType region;
	region.SetSize( size );
	region.SetIndex( start );

	image->SetRegions( region );
	image->Allocate();
	
	ImageType::IndexType pixelIndex;
	pixelIndex[0] = 0; // x position
	pixelIndex[1] = 0; // y position
	pixelIndex[2] = 0; // z position

	for(int i = 0; i < td->dim[0]; i++){
		for(int j = 0; j < td->dim[1]; j++){
			for(int k = 0 ; k < td->dim[2]; k++){
				pixelIndex[0] = i;
				pixelIndex[1] = j;
				pixelIndex[2] = k;
				
				image->SetPixel(pixelIndex, td->imageAt(i, j, k));
			}
		}
	}


	typedef itk::Image< unsigned char, 2 > Image2DType;
	typedef itk::ImageSeriesWriter< ImageType, Image2DType > WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetInput( image);

	typedef itk::NumericSeriesFileNames NameGeneratorType;
	NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();

	std::string format = rootName;
	format += "%03d";
	format += ".jpg";
	nameGenerator->SetSeriesFormat( format.c_str() );

	const unsigned int firstSlice = start[2];
	const unsigned int lastSlice = start[2] + size[2] - 1;
	nameGenerator->SetStartIndex( firstSlice );
	nameGenerator->SetEndIndex( lastSlice );
	nameGenerator->SetIncrementIndex( 1 );

	writer->SetFileNames( nameGenerator->GetFileNames() );

	try{
		writer->Update();
	}catch( itk::ExceptionObject & excp ){
		std::cerr << "Exception thrown while reading the image" << std::endl;
		std::cerr << excp << std::endl;
	}

	return;
}


/**
	noise.txt format:

	SHADOW: numShadows
	GAUSSIAN: median sigma
	UNIFORM: lb ub
	SALTPEPER: valSalt probsalt valpepper probpepper

	noise will be added in the order specified by the file

	one image for each noise file will be generated

  */
void applyNoise(TreeDrawer *td, const char* noiseFile){
	ifstream mapFile;
	mapFile.open(noiseFile);
	string line;

	double lb, ub, median, sigma, probSalt, probPepper;
	int numShadows;
	char valSalt, valPepper;

	if(mapFile.is_open()){		
		while(!mapFile.eof()){
			getline(mapFile, line);
			char* tok = new char[line.size()];
			strcpy(tok, line.c_str());
			
			if(line.length() == 0)
				continue;

			char * field = strtok(tok, ":");
					
			if(strcmp(field, "SHADOW") == 0){
				numShadows = atoi(strtok(NULL, " "));
				
				cout<<"Applying shadow noise."<<endl;
				td->addShadows(numShadows);
			} else if(strcmp(field, "GAUSSIAN") == 0){
				median = atof(strtok(NULL, " "));
				sigma = atof(strtok(NULL, " "));

				cout<<"Applying gaussian noise."<<endl;
				td->addNoise_gaussian(median, sigma);
			} else if(strcmp(field, "UNIFORM") == 0){
				lb = atof(strtok(NULL, " "));
				ub = atof(strtok(NULL, " "));

				cout<<"Applying uniform noise."<<endl;
				td->addNoise_Uniform(lb, ub);
			} else if(strcmp(field, "SALTPEPPER") == 0){
				valSalt = (char)atoi(strtok(NULL, " "));
				probSalt = atof(strtok(NULL, " "));
				valPepper = (char)atoi(strtok(NULL, " "));
				probPepper = atof(strtok(NULL, " "));
				
				valPepper = (char)valPepper;
				
				cout<<"Applying salt & pepper noise."<<endl;
				td->addNoise_saltPepper(valSalt, probSalt, valPepper, probPepper);
			} else{
				;				;
			}
		}
	}
}



void subPrint_node(NodeTable *nt, int segment, ofstream &os){
	os<<"  <node id=\"n"<<segment<<"\">"<<endl;
	os<<"    <attr name=\" nodeType\">"<<endl;
	if(nt->getType(segment) == NodeTable::ROOT){
		os<<"      <string> root node </string>"<<endl;
	} else if(nt->getType(segment) == NodeTable::TERM){
		os<<"      <string> terminal node </string>"<<endl;
	} else if(nt->getType(segment) == NodeTable::BIF){
		os<<"      <string> bifurication </string>"<<endl;
	} else {
		os<<"      <string> unknown type </string>"<<endl;
	}
	os<<"    </attr>"<<endl;

	os<<"    <attr name=\" position\">"<<endl;
	os<<"      <tup>"<<endl;
	double *pos = nt->getPos(segment);
	os<<"        <float>"<<pos[0]<<"</float>"<<endl;
	os<<"        <float>"<<pos[1]<<"</float>"<<endl;
	os<<"        <float>"<<pos[2]<<"</float>"<<endl;
	os<<"      </tup>"<<endl;
	os<<"    </attr>"<<endl;	
	os<<"  </node>"<<endl;

	if(nt->getType(segment) != NodeTable::TERM){
		subPrint_node(nt, nt->getLeftChild(segment), os);
		if(nt->getType(segment) != NodeTable::ROOT)
			subPrint_node(nt, nt->getRightChild(segment), os);
	}
}

void subPrint_edge(NodeTable *nt, int segment, ofstream &os){

	if(nt->getType(segment) != NodeTable::ROOT){
		os<<"  <edge id=\"e"<<segment<<"\" to=\"n"<<segment<<"\" from=\"n"<<nt->getParent(segment)<<"\">"<<endl;
		os<<"    <attr name=\" flow\">"<<endl;
		os<<"      <float>"<<nt->getFlow(segment)<<"</float>"<<endl;
		os<<"    </attr>"<<endl;

		os<<"    <attr name=\" radius\">"<<endl;
		os<<"      <float>"<<nt->getRadius(segment)<<"</float>"<<endl;
		os<<"    </attr>"<<endl;

		os<<"  </edge>"<<endl;
	}

	if(nt->getType(segment) != NodeTable::TERM){
		subPrint_edge(nt, nt->getLeftChild(segment), os);
		if(nt->getType(segment) != NodeTable::ROOT)
			subPrint_edge(nt, nt->getRightChild(segment), os);
	}
}

void printTreeStructure(VascularTree * vt, const char * filePath){
	ofstream output;
	cout<<"Trying to write to \""<<filePath<<"\""<<endl;
	output.open(filePath);
	output<<"<gxl><graph id=\""<<filePath<<"\" edgeids=\" true\" edgemode=\" directed\" hypergraph=\" false\">"<<endl;
	for(int i = 0; i < vt->nt.length; i++){
		if(vt->nt.getType(i) == NodeTable::ROOT){
			subPrint_node(&vt->nt, i, output);
			subPrint_edge(&vt->nt, i, output);
			output<<"</graph></gxl>"<<endl;
			output.close();
			return;
		}
	}

	cout<<"Unable to find root node."<<endl;
	output.close();
}

/**

  - param file
  - image root name
  - voxel width
  - noise set 1
  - noise set 2
  - ...


  */
//#define DEBUG_VERSION

#ifdef DEBUG_VERSION
void main(int argc, char** argv){
//	VascuSynth testParams.txt image 0.02 noise.txt

	cout<<"Building tree."<<endl;
	VascularTree * vt = buildTree("testParams.txt");
	cout<<"Tree Built."<<endl;
	double corner1[] = {0,0,0};
	double corner2[] = {vt->oxMap->dim[0], vt->oxMap->dim[1], vt->oxMap->dim[2]};
	cout<<"Drawing tree to matrix."<<endl;
	TreeDrawer * td = drawTree(vt, corner1, corner2, atof("0.02"));
	
	cout<<"Drawing matrix to image."<<endl;
	drawImage(td, "image");
	cout<<"Original image drawn to: "<<"image"<<endl;

	char *buff = new char[20];
		TreeDrawer * td_c = &td->copy();
		applyNoise(td_c, "noise.txt");
		
		string rootName = "image";
		rootName += "noise_";
		rootName += itoa(0, buff, 10);
		rootName += "_";
		drawImage(td_c, rootName.c_str());
		cout<<"Noise image "<<0<<" drawn to: "<<rootName<<endl;
	
}
#else

void main(int argc, char** argv){
	cout<<"Building trees."<<endl;

	cout<<"getting param files"<<endl;
	vector<string> *paramFiles = readFileLines(argv[1]);
	cout<<"getting image names"<<endl;
	vector<string> *imageNameFiles = readFileLines(argv[2]);
	
	string voxelWidth = argv[3];
	string *noiseFiles = new string[argc-4];


	for(int m = 0; m < paramFiles->size(); m++){
		cout<<"building tree "<<m<<endl;
		string paramFile = paramFiles->operator [](m);
		string rootDirectory = imageNameFiles->operator [](m);
		cout<<"new param file = "<<paramFile<<endl;
		cout<<"new root dir = "<<rootDirectory<<endl;

		for(int i = 4; i < argc; i++)
			noiseFiles[i-4] = argv[i];
		int numNoise = argc-4;


		VascularTree * vt = buildTree(paramFile.c_str());
		
		rootDirectory = "./"+rootDirectory+"/";

		mkdir(rootDirectory.c_str());

		string treeStructure = rootDirectory+"tree_structure.xml";
		printTreeStructure(vt, treeStructure.c_str());

		cout<<"Tree Built."<<endl;
		double corner1[] = {0,0,0};
		double corner2[] = {vt->oxMap->dim[0], vt->oxMap->dim[1], vt->oxMap->dim[2]};
		cout<<"Drawing tree to matrix."<<endl;
		TreeDrawer * td = drawTree(vt, corner1, corner2, atof(voxelWidth.c_str()));
		
		cout<<"Drawing matrix to image."<<endl;
		string imageName = rootDirectory+"original_image";
		mkdir(imageName.c_str());
		imageName = imageName + "/image";
		drawImage(td, imageName.c_str());
		
		cout<<"Original image drawn to: "<<rootDirectory<<endl;

		char *buff = new char[20];
		for(int i = 0; i < numNoise; i++){
			TreeDrawer * td_c = &td->copy();
			applyNoise(td_c, noiseFiles[i].c_str());
			
			string noiseImage = rootDirectory+"noise_image_"+itoa(i, buff, 10);
			mkdir(noiseImage.c_str());
			noiseImage = noiseImage+"/image";
			drawImage(td_c, noiseImage.c_str());
			cout<<"Noise image "<<(i)<<" drawn to: "<<noiseImage<<endl;
		}

		delete buff;
		delete td;
		delete vt;
	}
}
#endif
