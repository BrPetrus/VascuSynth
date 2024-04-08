#ifndef _nodetable_h
#define _nodetable_h

#include <vector>
#include <map>

using namespace std;

class NodeTable{
public:
	//constants used for the type field of a node
	static int TERM;
	static int ROOT;
	static int BIF;
	static int FIELDS;

	//copy of entries in the node table before they were modified (used for undos)
	map<int, double*> originalEntries;
	int length;
	bool prepareUndo;

	//current set of nodes
	vector<double *> nodes;
	
	NodeTable();

	//undo utilities
	void startUndo();
	void stopUndo();
	void clearUndo();
	void applyUndo();
	
	
	//setters/getters
	int getType(int index);
	void setType(int index, int type);
	double* getPos(int index);
	void setPos(int index, double* source);
	int getParent(int index);
	void setParent(int index, int parent);
	double getLeftRatio(int index);
	void setLeftRatio(int index, double ratio);
	double getRightRatio(int index);
	void setRightRatio(int index, double ratio);
	double getFlow(int index);
	void setFlow(int index, double flow);
	int getLeftChild(int index);
	void setLeftChild(int index, int id);
	int getRightChild(int index);
	void setRightChild(int index, int id);
	double getRadius(int index);
	void setRadius(int index, double radius);
	double getReducedResistance(int index);
	void setReducedResistence(int index, double resistance);
	
	//adds a new node to the node table (makes a copy of *node
	void addNode(double* node);
	//sets the node specified by index to *node
	void setNode(int index, double* node);
	//adds a new node to the node table
	void addNode(int type, double* pos, int parent, double leftRatio, double rightRatio, double flow, int leftChild, int rightChild);

	//makes a copy of the current node table
	NodeTable copy();
};
#endif