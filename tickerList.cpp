//Adaptationof  Implementation file for the NumberList class
// Taken from Gaddis Definiton provided by Cyril Harris

//Trie data structures (good for information retreival)
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "tickerList.h"
using namespace std;


void tickerList::appendNode(int ident, string date, string open, string close) //Append node from Gaddis Definition, changed to allow for added data types
{
    ListNode* newNode;  // To point to a new node
    ListNode* nodePtr;  // To move through the list

    // Allocate a new node and store num there.
    newNode = new ListNode;
    newNode->ident = ident;
    newNode->openLine = {date, open };
    newNode->closeLine = {date, close };
    newNode->next = nullptr;

    // If there are no nodes in the list
    // make newNode the first node.
    if (!head)
        head = newNode;
    else  // Otherwise, insert newNode at end.
    {
        // Initialize nodePtr to head of list.
        nodePtr = head;

        // Find the last node in the list.
        while (nodePtr->next)
            nodePtr = nodePtr->next;

        // Insert newNode as the last node.
        nodePtr->next = newNode;
    }
}

void tickerList::addPrice(int ident, string open, string close) { //function to push line info the needed nodes
    ListNode* nodePtr = head;			// To traverse the list
    ListNode* previousNode = nullptr;	// The previous node
    while (nodePtr->next != nullptr && nodePtr->ident != ident) //runs through the nodes until the indentifier is found
    {
        previousNode = nodePtr; //list traversal
        nodePtr = nodePtr->next;

    }
    nodePtr->openLine.push_back(open); //pushs the open price
    nodePtr->closeLine.push_back(close);// pushs the closed price
}


void tickerList::neDeleter(vector <string>& tickerNames, vector <string>& final) //function for removing all nonexistant tickers
{   
    vector <string> tempVec; //sets up temp holder for the inner joined tickers
    for (int i = 0; i < tickerNames.size(); i++) { //loops trough every intial ticker
        if (binary_search(final.begin(), final.end(), tickerNames[i])) {//checks final ticker list for first tickers, only pushes if found, otherwise ignored
            tempVec.push_back(tickerNames[i]);
        }
   }
    tickerNames = tempVec; //overwrites the intial tickers with the difference to save on declared vectors
}

//function for outputting the open prices to a csv, along with the ticker and dates. The close version is identical, except for 'nodePtr->openLine.size' is changed to nodePtr->closeLine.size()'
// I was unable to find a succint way to get the list refrenced to change with a variable so I decided it was best to just copy the function and change one thing 
void tickerList::outputOpen(string path, vector <string> ticker) {
    ofstream outFile;
    ListNode* nodePtr = head; //forces nodePTr to the start of the list
    outFile.open(path);
    outFile << "Dates" << ","; //dates is a static feature of the csv that never changes
    list <string>::iterator oIT; //iterator for traversing lists
    for (int i = 0; i < ticker.size(); i++) { //pushes all tickers to the first line of the csv
        outFile << ticker[i] << ",";
    }
    outFile << "\n"; //new line before
    while (nodePtr->next != nullptr) { //runs until there are no nodes left
        oIT = nodePtr->openLine.begin(); //sets the pointer to the beginning of the list everytime it iterators
        for (int i = 0; i < nodePtr->openLine.size(); i++) { //iterates through every element in the list
            outFile << *oIT << ","; //pushes the current value and a comma
            oIT++; //iterates forward
        }
        outFile << "\n"; //pushes new line for new node
        nodePtr = nodePtr->next; // sets ptr to the next node
    }

}

void tickerList::outputClose(string path, vector <string> ticker) {
    ofstream outFile;
    ListNode* nodePtr = head;
    outFile.open(path);
    outFile << "Dates" << ",";
    list <string>::iterator oIT;
    for (int i = 0; i < ticker.size(); i++) {
        outFile << ticker[i] << ",";
    }
    outFile << "\n";
    while (nodePtr->next != nullptr) {
        oIT = nodePtr->closeLine.begin();
        for (int i = 0; i < nodePtr->openLine.size(); i++) {
            outFile << *oIT << ",";
            oIT++;
        }
        outFile << "\n";
        nodePtr = nodePtr->next;
    }
}



tickerList::~tickerList()
{
    ListNode* nodePtr;   // To traverse the list
    ListNode* nextNode;  // To point to the next node

    // Position nodePtr at the head of the list.
    nodePtr = head;

    // While nodePtr is not at the end of the list...
    while (nodePtr != nullptr)
    {
        // Save a pointer to the next node.
        nextNode = nodePtr->next;

        // Delete the current node.
        delete nodePtr;

        // Position nodePtr at the next node.
        nodePtr = nextNode;
    }
}

