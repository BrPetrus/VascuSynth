#pragma warning(disable:4786)

#include "NodeTable.h"

#include <vector>
#include <map>
#include <iostream>

using namespace std;

int NodeTable::TERM = 0;
int NodeTable::ROOT = 1;
int NodeTable::BIF = 2;
int NodeTable::FIELDS = 12;


NodeTable::NodeTable(){
	prepareUndo = false;
}

//start recording entries for undos
void NodeTable::startUndo(){
	prepareUndo = true;
	length = nodes.size();
}

//stop recording entries for undo
void NodeTable::stopUndo(){
	prepareUndo = false;
}

//clear the undo table
void NodeTable::clearUndo(){
	for(int i = 0; i < originalEntries.size(); i++)
		delete originalEntries[i];

	originalEntries.clear();
}

//apply all accumulate undos
void NodeTable::applyUndo(){
	stopUndo();
	map<int, double*>::iterator itr;
	for(itr = originalEntries.begin(); itr != originalEntries.end(); itr++){
		double *newNode = new double[FIELDS];
		for(int i = 0; i < FIELDS; i++)
			newNode[i] = itr->second[i];
		setNode(itr->first, newNode);
	}

	if(nodes.size() > length)
		nodes.resize(length);
	clearUndo();
	startUndo();
}

int NodeTable::getType(int index){
	if(index > nodes.size())
		return -1;
	return (int)nodes[index][0];
}

void NodeTable::setType(int index, int type){
	if(index >= nodes.size())
		return;

	//if this is the first time this entry has been modified - adds its original state to the undo table
	if(prepareUndo == true && originalEntries[index] == NULL){
		double * d = new double[FIELDS];
		double * ent = nodes[index];
		for(int i = 0 ; i < FIELDS; i++)
			d[i] = ent[i];
		originalEntries[index] = d;
	}

	nodes[index][0] = type;
}


double* NodeTable::getPos(int index){
	if(index > nodes.size())
		return NULL;
	
	return nodes[index]+1;
}


void NodeTable::setPos(int index, double* source){
	if(index >= nodes.size())
		return;

	//if this is the first time this entry has been modified - adds its original state to the undo table
	if(prepareUndo == true && originalEntries[index] == NULL){
		double * d = new double[FIELDS];
		double * ent = nodes[index];
		for(int i = 0 ; i < FIELDS; i++)
			d[i] = ent[i];
		originalEntries[index] = d;
	}

	nodes[index][1] = source[0];
	nodes[index][2] = source[1];
	nodes[index][3] = source[2];
}

int NodeTable::getParent(int index){
	if(index > nodes.size())
		return NULL;
	
	return (int) nodes[index][4];
}

void NodeTable::setParent(int index, int parent){
	if(index >= nodes.size())
		return;

	//if this is the first time this entry has been modified - adds its original state to the undo table
	if(prepareUndo == true && originalEntries[index] == NULL){
		double * d = new double[FIELDS];
		double * ent = nodes[index];
		for(int i = 0 ; i < FIELDS; i++)
			d[i] = ent[i];
		originalEntries[index] = d;
	}

	nodes[index][4] = parent;
}

double NodeTable::getLeftRatio(int index){
	if(index > nodes.size())
		return NULL;
	return nodes[index][5];
}

void NodeTable::setLeftRatio(int index, double ratio){
	if(index >= nodes.size())
		return;

	//if this is the first time this entry has been modified - adds its original state to the undo table
	if(prepareUndo == true && originalEntries[index] == NULL){
		double * d = new double[FIELDS];
		double * ent = nodes[index];
		for(int i = 0 ; i < FIELDS; i++)
			d[i] = ent[i];
		originalEntries[index] = d;
	}
	nodes[index][5] = ratio;
}

double NodeTable::getRightRatio(int index){
	if(index > nodes.size())
		return NULL;
	return nodes[index][6];
}

void NodeTable::setRightRatio(int index, double ratio){
	if(index >= nodes.size())
		return;

	//if this is the first time this entry has been modified - adds its original state to the undo table
	if(prepareUndo == true && originalEntries[index] == NULL){
		double * d = new double[FIELDS];
		double * ent = nodes[index];
		for(int i = 0 ; i < FIELDS; i++)
			d[i] = ent[i];
		originalEntries[index] = d;
	}
	nodes[index][6] = ratio;
}

double NodeTable::getFlow(int index){
	if(index > nodes.size())
		return NULL;
	return nodes[index][7];
}

void NodeTable::setFlow(int index, double flow){
	if(index >= nodes.size())
		return;

	//if this is the first time this entry has been modified - adds its original state to the undo table
	if(prepareUndo == true && originalEntries[index] == NULL){
		double * d = new double[FIELDS];
		double * ent = nodes[index];
		for(int i = 0 ; i < FIELDS; i++)
			d[i] = ent[i];
		originalEntries[index] = d;
	}
	nodes[index][7] = flow;
}

int NodeTable::getLeftChild(int index){
	if(index > nodes.size())
		return NULL;
	return (int) nodes[index][8];
}

void NodeTable::setLeftChild(int index, int id){
	if(index >= nodes.size())
		return;

	//if this is the first time this entry has been modified - adds its original state to the undo table
	if(prepareUndo == true && originalEntries[index] == NULL){
		double * d = new double[FIELDS];
		double * ent = nodes[index];
		for(int i = 0 ; i < FIELDS; i++)
			d[i] = ent[i];
		originalEntries[index] = d;
	}
	nodes[index][8] = id;
}

int NodeTable::getRightChild(int index){
	if(index > nodes.size())
		return NULL;
	return (int) nodes[index][9];
}

void NodeTable::setRightChild(int index, int id){
	if(index >= nodes.size())
		return;

	//if this is the first time this entry has been modified - adds its original state to the undo table
	if(prepareUndo == true && originalEntries[index] == NULL){
		double * d = new double[FIELDS];
		double * ent = nodes[index];
		for(int i = 0 ; i < FIELDS; i++)
			d[i] = ent[i];
		originalEntries[index] = d;
	}
	nodes[index][9] = id;
}

double NodeTable::getRadius(int index){
	if(index > nodes.size())
		return NULL;
	return nodes[index][11];
}

void NodeTable::setRadius(int index, double radius){
	if(index >= nodes.size())
		return;

	//if this is the first time this entry has been modified - adds its original state to the undo table
	if(prepareUndo == true && originalEntries[index] == NULL){
		double * d = new double[FIELDS];
		double * ent = nodes[index];
		for(int i = 0 ; i < FIELDS; i++)
			d[i] = ent[i];
		originalEntries[index] = d;
	}
	nodes[index][11] = radius;
}

double NodeTable::getReducedResistance(int index){
	if(index > nodes.size())
		return NULL;
	return nodes[index][10];
}

void NodeTable::setReducedResistence(int index, double resistance){
	if(index >= nodes.size())
		return;

	//if this is the first time this entry has been modified - adds its original state to the undo table
	if(prepareUndo == true && originalEntries[index] == NULL){
		double * d = new double[FIELDS];
		double * ent = nodes[index];
		for(int i = 0 ; i < FIELDS; i++)
			d[i] = ent[i];
		originalEntries[index] = d;
	}
	nodes[index][10] = resistance;
}
	
void NodeTable::addNode(double* node){
	double *row = new double[FIELDS];
	for(int i = 0; i < FIELDS; i++)
		row[i] = node[i];

	nodes.push_back(row);
}

void NodeTable::setNode(int index, double* node){
	//if this is the first time this entry has been modified - adds its original state to the undo table
	if(prepareUndo == true && originalEntries[index] == NULL){
		double * d = new double[FIELDS];
		double * ent = nodes[index];
		for(int i = 0 ; i < FIELDS; i++)
			d[i] = ent[i];
		originalEntries[index] = d;
	}
	
	if(index >= nodes.size())
		nodes.resize(index+1);

	delete nodes[index];
	nodes[index] = node;
}

void NodeTable::addNode(int type, double* pos, int parent, double leftRatio, double rightRatio, double flow, int leftChild, int rightChild){
	double * row = new double[FIELDS];
	row[0] = type;
	row[1] = pos[0];
	row[2] = pos[1];
	row[3] = pos[2];
	row[4] = parent;
	row[5] = leftRatio;
	row[6] = rightRatio;
	row[7] = flow;
	row[8] = leftChild;
	row[9] = rightChild;
	
	nodes.push_back(row);
}

NodeTable NodeTable::copy(){
	NodeTable * nt = new NodeTable();
	vector<double*>::iterator itr;

	for(itr = nodes.begin(); itr != nodes.end(); itr++)
		nt->addNode(*itr);
	
	return *nt;
}
