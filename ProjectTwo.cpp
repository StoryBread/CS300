/*
Dan Story
SNHU CS300 - project two
20APR2025
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>

using namespace std;
//Making a struct to hold all the course info
struct Course {
	string courseId;
	string courseName;
	vector<string> reqList;

};



class BinarySearchTree {

private:
	//defining structure
	struct Node {
		Course course;
		Node* right;
		Node* left;

		//constructor

		Node() {
			left = nullptr;
			right = nullptr;
		}
		//Initialize with a course

		Node(Course aCourse) {
			course = aCourse;
			left = nullptr;
			right = nullptr;
		}
	};
	
	Node* root;
	void inOrder(Node* node);
	int size = 0;

public:
	BinarySearchTree();
	void inOrder();
	void Insert(Course aCourse);
	Course Search(string courseId);
	int Size();
};

//default constructor, sets root to null

BinarySearchTree::BinarySearchTree() {
	this->root = nullptr;
}

//A way to traverse the tree in order starting from root

void BinarySearchTree::inOrder() {
	inOrder(root);
}

//Inserting a new course/node

void BinarySearchTree::Insert(Course aCourse) {
	Node* currentNode = root;
	//Checks if tree is empty, and inserts new node as root if so
	if (root == NULL) {
		root = new Node(aCourse);
	}
	//logic to traverse tree checking left for right and looking for a space for the node to insert
	else {
		while (currentNode != NULL) {
			if (aCourse.courseId < currentNode->course.courseId) {
				if (currentNode->left == nullptr) {
					currentNode->left = new Node(aCourse);
					currentNode = NULL;
				}
				else {
					currentNode = currentNode->left;
				}

			}
		
			else {
				if (currentNode->right == nullptr) {
					currentNode->right = new Node(aCourse);
					currentNode = NULL;
				}
				else {
					currentNode = currentNode->right;
				}
			}
		}
	}
	size++;
}
//Way to search for a course for print function
Course BinarySearchTree::Search(string courseId){
	Course aCourse;
	Node* currentNode = root;

	//while the tree is not empty 
	while (currentNode != NULL) {
		//Checks if the search term has been found and returns if it has
		if (currentNode->course.courseId == courseId) {
			return currentNode->course;
		}
		//logic to traverse left and right
		else if (courseId < currentNode->course.courseId) {
			currentNode = currentNode->left;
		}
		else {
			currentNode = currentNode->right;
		}
	}
	//If it's not found

	cout << "Course not found." << endl;
	return aCourse;
}


//A way to print the courses in order recursively
void BinarySearchTree::inOrder(Node* node) {

	if (node == NULL) {
		return;
	}
	inOrder(node->left);
	//Only prints info if the course ID begins with CS or MA. This should only print the Computer science and Math courses.
	if(((node->course.courseId[0] == 'C') && (node->course.courseId[1] == 'S')) || ((node->course.courseId[0]=='M') && (node->course.courseId[1]=='A'))) {
		cout << node->course.courseId << "," << node->course.courseName<< endl;
			}
		inOrder(node->right);
	
}
int BinarySearchTree::Size() {
	return size;
}
//parsing the CSV and loading courses
void parseText(string csvPath, BinarySearchTree* courseList) {
	ifstream inputFile;
	Course aCourse;
	string line;
	inputFile.open(csvPath); 

	//Error message if file wasn't opened
	if (!inputFile.is_open()) {
		cout << "Could not open file." << endl;
		return;
	}
	else{
		string line = "";
		while (getline(inputFile, line)) {
			//Variables for the course
			string courseId = "";
			string courseName = "";
			string temp;
			vector <string> reqlist;

			stringstream inputString(line);
			getline(inputString, courseId, ',');
			aCourse.courseId = courseId;
			getline(inputString, courseName, ',');
			aCourse.courseName = courseName;
			//Checking if string/line is empty
			if ((courseId.compare("") == 0) || (courseName.compare("") == 0)) {
				cout << "Invalid format, line skipped" << endl << endl;
				continue;
			}
			//Looking at the rest of the line and checking for pre-reqs will only loop until there isn't whitespace.
			while (getline(inputString, temp, '\n')) {
				stringstream ss(temp);
				while (getline(ss, temp, ',')) {
					//Adds the pre-req to the list if there's a comma
					aCourse.reqList.push_back(temp);

				}
			}
			//Inserts course into end of tree
			courseList->Insert(aCourse); 

			//reset everything
			line = "";
			aCourse.reqList.clear();

		}

	

	}
	inputFile.close();
}
//Searching and displaying course info
void courseSearch(Course aCourse) {
	cout << aCourse.courseId << ", " << aCourse.courseName << endl;
	cout << "Prerequisites: ";
	//checks for pre-reqs
	if (aCourse.reqList.empty())
		cout << "None" << endl;
	else {
		for (unsigned int i = 0; i < aCourse.reqList.size(); i++) {
			cout << aCourse.reqList.at(i);
			if (aCourse.reqList.size() > 1 && i < aCourse.reqList.size() - 1) {
				cout << ",";
			}
		}
	}
	cout << endl;
}

int main(int argc, char* argv[]) {

	string csvPath, aCourseKey;

	//For command line loading
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;

	default:
		csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
	}
	//Table to hold courses
	BinarySearchTree* courseList = new BinarySearchTree();

	Course course;
	bool validChoice;
	int choice = 0;

	while (choice != 9) {
		cout << "Welcome to the course planner." << endl << endl;
		cout << "1. Load Data Structure." << endl;
		cout << "2. Print Computer Science Program Course List." << endl; //I changed this to indicate to the user this is not the full course list
		cout << "3. Print a Specific Course." << endl;
		cout << "9. Exit." << endl;
		cout << "Enter Choice: ";

		//reset all variables
		aCourseKey = "";
		string anyKey = " ";
		choice = 0;

		try {
			cin >> choice;
			if ((choice > 0 && choice < 4) || (choice == 9)) {
				validChoice = true;
			}
			else {
				validChoice = false;
				throw 1;
			}
			switch (choice) {
			case 1:
				parseText(csvPath, courseList);
				cout << courseList->Size() << " courses loaded" << endl<<endl;
				break;

			case 2:
				courseList->inOrder();
				cout << endl;
				break;

			case 3:
				cout << "Enter course ID (EX:'CSCI100'): " << endl;
				cin >> aCourseKey;
				course = courseList->Search(aCourseKey);
				if (!course.courseId.empty()) {
					courseSearch(course);
				}
				else {
					cout << "Course not found" << endl<<endl;
				}
				break;
			case 9:
				exit;
				break;

			default:

				throw 2;


			}
		}
		//catching throws
		catch (int err) {
			cout << "Invalid input" << endl;
		}
		//clearing bad inputs
		cin.clear();
		cin.ignore();


	}
	return 0;
}


