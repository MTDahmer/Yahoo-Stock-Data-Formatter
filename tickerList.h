// Adapted from Specification file for the NumberList class
// Taken from Gaddis Definiton provided by Cyril Harriss
#ifndef TICKERLIST_H
#define TICKERLIST_H

#include <list>
#include <string>
#include <vector>

using namespace std;

class tickerList
{
private:
    // Declare a structure for the list
    struct ListNode
    {
        //Each node will hold all the open and close prices for all extant tickers for every file. when the process is done and the list made, the nodes will simply be pushed to the files one by one with no need for any post processing
        int ident; //used to identify the nodes for later access
        list <string> openLine; //open values foe each line
        list <string> closeLine;//close values for each line
        struct ListNode* next;  // To point to the next node
    };

    ListNode* head;            // List head pointer

public:
    // Constructor
    tickerList()
    {
        head = nullptr;
    }

    // Destructor
    ~tickerList();

    // Linked list operations
    void appendNode(int, string, string, string);
    void addPrice(int, string, string);
    void neDeleter(vector <string>&, vector <string>&);
    void outputOpen(string, vector <string>);
    void outputClose(string, vector <string>);

};
#endif