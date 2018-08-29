#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <queue>
#include <string>
#include <math.h> 
using namespace std;


//throw exception 
struct BinaryException : std::range_error {
	explicit BinaryException(char const* msg = NULL) : range_error(msg) {}
};


///Binary Node Class
class BinaryNode {
private:
	friend class BinarySearchTree;
	int key;
	int searchCost;
	BinaryNode *left, *right;
public:
	
	//BinaryNode(){int el = 0; int searchCost = 0; BinaryNode *lt = NULL; BinaryNode *rt = NULL;}
	BinaryNode(int el = 0, int searchCost = 1, BinaryNode *lt = NULL, BinaryNode *rt = NULL) : key(el), searchCost(searchCost), left(lt), right(rt) {}
	
	//Get left node
	BinaryNode *getLeft() { return left; }

	//Get Right node
	BinaryNode *getRight() { return right; }

	//Get/set key
	int getKey() { return key; }
	void setKey(int newKey) { key = newKey; return; }

	//Get/set cost
	void setCost(int newCost) { searchCost = newCost; }
	int getCost() { return searchCost; }

	//Change left Node
	void changeLeftNode(BinaryNode *newNode) {
		left = newNode;
		return;
	}

	//Change Right node
	void changeRightNode(BinaryNode *newNode) {
		right = newNode;
		return;
	}

	//Get the size of the binary tree
	int size(BinaryNode *t) {
		if (t == NULL) {
			return  0;
		}
		else {
			return 1 + size(t->left) + size(t->right);
		}
	}


	//In order transversal
	void inOrderTransversal() {
		if (left != NULL) left->inOrderTransversal();
		cout << key << "[" << searchCost <<"]" << " ";
		if (right != NULL) right->inOrderTransversal();
		return;
	}

	//Post order transversal
	void postOrderTransversal() {
		if (left != NULL) left->inOrderTransversal();
		if (right != NULL) right->inOrderTransversal();
		cout << "Key =  " << key << "  searchCost = " << searchCost << endl;
		return;
	}


	//Pre order transversal
	void preOrderTransversal() {
		cout << "Key =  " << key << "  searchCost = " << searchCost << endl;
		if (left != NULL) left->inOrderTransversal();
		if (right != NULL) right->inOrderTransversal();

		return;
	}
};


///Binary Search Tree CLass

class BinarySearchTree {
private:
	//queue to print the nodes
	queue<BinaryNode*> printQ;

	//First (top) Node
	BinaryNode *root;

	//Delete Binary Tree
	void deleteTree(BinaryNode *root) {
		//cout << "deleting a tree." << endl;
		if (root == NULL) return;
		if (root->getLeft() != NULL)
			deleteTree(root->getLeft());
		if (root->getRight() != NULL)
			deleteTree(root->getRight());
		delete root;
	}

	//Update search cost of each node
	void updateCost(BinaryNode *t) {
		if (t == NULL) return;
		if (t->getLeft() != NULL)
			updateCost(t->getLeft());
		if (t->getRight() != NULL)
			updateCost(t->getRight());
		t->setCost(t->searchCost-1);
		return;
	}


	//remove minimum element of binary tree
	BinaryNode *removeMin(BinaryNode *t) {
		if (t == NULL) throw BinaryException("Item Not Found");
		if (t->getLeft() != NULL)
			t->changeLeftNode(removeMin(t->getLeft()));
		else {
			BinaryNode *node = t;
			updateCost(t);
			t = t->getRight();
			delete node;
		}
		return t;
	}

	//find the minimum element of the binary tree
	BinaryNode *findMin(BinaryNode * t) {
		if (t == NULL) throw BinaryException("Item Not Found");
		while (t->left != NULL) t = t->getLeft();
		return t;
	}

	//insert an element in a binary tree
	BinaryNode *insert(int x, BinaryNode *t, int depthNode) {
		if (t == NULL) { t = new BinaryNode(x, depthNode); }
		else if (x < t->getKey()) {
			depthNode++;
			t->changeLeftNode(insert(x, t->getLeft(), depthNode));
		}
		else if (x > t->getKey()) {
			depthNode++;
			t->changeRightNode(insert(x, t->getRight(), depthNode));
		}
		else
			throw BinaryException("Duplicate Item.");
		return t;
	}

	//remove a node in a binary tree
	BinaryNode *remove(int x, BinaryNode *t) {
		if (t == NULL) throw BinaryException("item not found");
		if (x < t->getKey())
			t->changeLeftNode(remove(x, t->getLeft()));
		else if (x > t->getKey())
			t->changeRightNode(remove(x, t->getRight()));
		else if (t->getLeft() != NULL && t->getRight() != NULL) { //x is found, and x has 2 children
			t->setKey(findMin(t->getRight())->getKey());
			t->changeRightNode(removeMin(t->getRight()));
		}
		else{ //x is found, and x has 1 child
			BinaryNode *node = t;
			t = (t->getLeft() != NULL) ? t->getLeft() : t->getRight();
			updateCost(t);
			delete node;
		}
		return t;
	}

	//find a node in the binary tree
	BinaryNode *find(int x, BinaryNode *t) {
		if (t == NULL) throw BinaryException("item not found");
		if (x < t->getKey())
			find(x, t->getLeft());
		else if (x > t->getKey())
			find(x, t->getRight());
		else {
			return t;
		}
		throw BinaryException("Item Not Found");
	}


public:
	BinarySearchTree() { root = NULL; }
	BinarySearchTree(int key) { root = new BinaryNode(key); }
	~BinarySearchTree() { deleteTree(root); root = NULL; }
	BinaryNode *getRoot() { return root; }

	//wrapper functions
	double size() { return (root == NULL) ? 0 : root->size(root); }
	//int height() {return (root == NULL)? 0: root->height(root);}
	void insert(int x) { root = insert(x, root, 1); }
	void remove(int x) { root = remove(x, root); }
	BinaryNode *find(int x) { return find(x, root); }
	bool isEmpty() { return root == NULL; }

	int sumCost(BinaryNode *t){
		if(t == NULL){
			return 0;
		}
		else{
			return t->searchCost + sumCost(t->getLeft()) + sumCost(t->getRight());
		}
	}

	//Output Tree level by level
	void outputTreeLevelByLevel(string fileName){
		int size = this->size();
		if(size <= 16){
			queue<BinaryNode*> q;	//queue to keep track what to print next
			int levelNodes = 0;	//total of nodes in a level
			if(this->getRoot() == NULL) return;
			q.push(this->getRoot());	//start the queue q with the root
			BinaryNode *fakeNode = new BinaryNode(-1);	//create a fakeNode for missing node
			//cout << "fakeNode = " <<fakeNode->getKey()<<endl;
			int treeSize = this->size();	//tree size to know when to stop generating fakeNode
			int currLevel = -1;	//keep track level 
			//cout << "tree size = " <<treeSize <<endl;
			ofstream myfile;
	  		myfile.open(fileName);	//open to output the file

			while(!q.empty()){	//keep iterating till q is empty
				currLevel++;	//
				
				if(treeSize == 1){
					int extraX = pow(2, currLevel) - q.size();
					for(int i = 0; i < extraX; ++i)
						q.push(fakeNode);
					treeSize--;
				}
				levelNodes = q.size();
				while(levelNodes >0){
					BinaryNode * n = q.front();
					q.pop();

					if(n == fakeNode){
						myfile << "x ";
					}
					else{
						myfile << n->getKey() << " ";
					}

					if(n->getLeft() != NULL){
						q.push(n->getLeft());
						treeSize--;
					}
					else if(treeSize > 1){
						//cout << "tree size = " <<treeSize <<endl;
						q.push(fakeNode);
					}
					if(n->getRight() != NULL){
						q.push(n->getRight());
						treeSize--;
					}
					else if(treeSize > 1){
						//cout << "tree size = " <<treeSize <<endl;
						q.push(fakeNode);
					}
					levelNodes--;
				}
				myfile <<endl;
			}
			myfile.close();
		}
	}

	//transverse binary tree in order and push the element into the queue
	void queueInOrder(BinaryNode *t){
		if (t->getLeft() != NULL) queueInOrder(t->getLeft());
		printQ.push(t);
		if (t->getRight() != NULL) queueInOrder(t->getRight());
		return;
	}

	//output to a file
	void printToFile(string fileName){
		ofstream myfile;
		myfile.open(fileName);
		while(!printQ.empty()){
			myfile << printQ.front()->getKey() << "[" << printQ.front()->getCost() <<"]" << " ";
			printQ.pop();
		}
		myfile <<endl;
		myfile << "Total number of nodes is " << size() << endl;
		double averageCost = sumCost(getRoot())/size();
		myfile << "average cost = " << averageCost <<endl;
		myfile.close();
		return;

	}

};



int main() {
	/*cout << "Create a binary search tree." << endl;
	BinarySearchTree bst;
	bst.insert(5);
	bst.insert(3);
	bst.insert(9);
	bst.insert(7);
	bst.insert(10);
	bst.insert(8);


	bst.remove(8);

	//cout << "node after the deleted node = " <<bst.getRoot()->getRight()->getRight()->getKey() <<endl;
	bst.getRoot()->inOrderTransversal();
	cout <<endl;
	bst.outputTreeLevelByLevel("test");
	cout << "Total number of nodes is " << bst.size() << endl;
	double averageCost = bst.sumCost(bst.getRoot())/bst.size();
	cout << "average cost = " << averageCost <<endl;)*/

	cout <<"starting program ..." <<endl;
	BinarySearchTree bst;

	ifstream inFS;

	int readNum = 0;
	inFS.open("3l");	////////////////change the input file you want here/////////////////////////////
	if(!inFS.is_open()){
		cout << "Could not open file." <<endl;
	}
	while(!inFS.eof()){
		inFS >> readNum;
		if(!inFS.eof())
		bst.insert(readNum);
	}
	inFS.close();

	string fileName = "3lOutput.csv";	//////////////change the output file name here for level to level //////////////////////////////////////
	bst.outputTreeLevelByLevel(fileName);

	string fileName2 = "3linOrderTransversal.csv"; //////////////change the output file name here for printing in order //////////////////////////////////////
	//cout << "size = " <<bst.size() <<endl;
	int size = bst.size();

	if(size > 16){
		cout << "input into the file..." <<endl;
		bst.queueInOrder(bst.getRoot());
		bst.printToFile(fileName2);
	}
	else{
		bst.getRoot()->inOrderTransversal();
		cout <<endl;
		cout << "Total number of nodes is " << bst.size() << endl;
		double averageCost = bst.sumCost(bst.getRoot())/bst.size();
		cout << "average cost = " << averageCost <<endl;
	}
	
	return 0;

///////////////////////////////////////
////////////NOTICE: if you are using linux system to test this program, please change fileName at line 202 and 258 to a type the name out like  "test.csv" instead of passing a string in the argument
//////////////////////// 
}