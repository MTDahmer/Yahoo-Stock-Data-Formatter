#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "time.h"
#include "dirent.h"
#include "tickerList.h"
#include <algorithm>

using namespace std;

tickerList tL;

//function to grab every ticker from a given file. Used to grab to first and last list for comparison
vector <string> tickerScrape(string file) {
	ifstream infoScrape;
	infoScrape.open(file); //opens requested file
	string tempString1;
	vector <string>  tempVector1;
	vector <string> tempVector2; //opens the strings and vectors needed for the process
	while (getline(infoScrape, tempString1)) { //iterates through every line in the file
		stringstream ss1(tempString1); //turns line into stringstream object for vectorization
		while (ss1.good()) { //loop for turning the string into a vector split by comma as long as there are elements to split
			string temp3;
			getline(ss1, temp3, ',');
			tempVector1.push_back(temp3); //pushes each element into temproary holding vector while it iterates through the string
		}
		tempVector2.push_back(tempVector1[0]); //grabs only the first eelement, the ticker and adds to a list of tickers
		tempVector1.clear(); //clears the temp vector to avoid overwriting
	}
	return tempVector2; //returns the tickers
}

vector <string> directoryPull() { //Code courtesy of Cyril Harris, changed to pass a vector of file names rather than display them
	const char* m1_input_prefix_char = "C:\\3304 Input Files\\";
	vector <string> input_filenames;//stores the filenames that exist in a directory
	unsigned int num_of_files;
	DIR* dir;
	struct dirent* ent;
	string intput_buffer;
	if ((dir = opendir(m1_input_prefix_char)) != NULL) { //If you can open that directory
		//ifstream in_file;
		while ((ent = readdir(dir)) != NULL)//while you can read in filenames from a directory
		{
			intput_buffer = m1_input_prefix_char;//assign(re-assign) the directory to the input_buffer
			intput_buffer += ent->d_name;//append the filename to the input_buffer
			input_filenames.push_back(intput_buffer);//push the filename with its directory onto the vector
		}
		closedir(dir);
		//Erase the "." and the ".." as filenames.  For some reason these will always be listed as files at the beginning.
		//This probably has something to do with the UNIX commands for 
		//	accessing the current directory (cd .)
		//	accessing the parent directory (cd ..)
		input_filenames.erase(input_filenames.begin(), input_filenames.begin() + 2);//erase the first 2 elements.
		cout << endl << endl;
		//***************************************************************************************************
		num_of_files = input_filenames.size();
		return input_filenames;
	}
	else {
		cout << "The directory " << m1_input_prefix_char << " cannot be opened" << endl;
	}
}

//function to pass the important bits of the newly made vectors to the needed node functions, but only if their tickers exist in the list of tickers that exist for every file
void informationFill(vector <string> line, vector <string>& tickers, int i, int lineCount) {
	if (lineCount == 0 && binary_search(tickers.begin(), tickers.end(), line[0])) { //checks the first element of the grabbed line of data (the ticker name) and checks it agains the final ticker list made earlier using a built in binary search function but only if this is the first line in the file
		tL.appendNode(i, line[1], line[2], line[5]); //passed the i (for identification of node), the date (only for the first time creation) and the open and close price and passes it to the function for append new nodes for the start of a file
	}
	else if (binary_search(tickers.begin(), tickers.end(), line[0])) { //same as above, but runs for every other line besdies the first
		tL.addPrice(i, line[2], line[5]);// same as above, but doesn't pass the date as the date only needs to be recorded once
	}
	
}

//function to strip lines from the current file and prepare them to be passed to the function respobile
void listFiller(string file, int i, vector <string>& tickerNames) {
	ifstream data;
	data.open(file); //opens current file
	int lineCount = 0;
	string tempString;
	vector <string> tempVector; //sets up needed variables, line count for tracking iterations, and tempstring/vector for the string stream
	while (getline(data, tempString)) { //iterates through every line in the file
		stringstream ss(tempString); //turns line into string stream object
		while (ss.good()) { //moves through string, splitting by comma as long as there are elements to get
			string temp;
			getline(ss, temp, ',');
			tempVector.push_back(temp); //ushes elements to vector
		}
		informationFill(tempVector, tickerNames, i, lineCount);
		tempVector.clear();//passes the line, the names, the current file iteration and the current line iteration to the function responsilbe for deciding what goes and what stays
		lineCount++; //ups line count
	}
	data.close();
}
//function to iterate through file list and pass needed info. This was made as a seperate file to avoid passing as many vectors around as possible for speed.
void tickerFactory(vector <string> names, int Fcount, vector <string>& tickerNames) {
	for (int i = 0; i < Fcount; i++) { //runs for as long as there are file names to give it
		listFiller(names[i], i, tickerNames); //passes current file, the iterator and the ticker vector to the next function
	}
}

int main() {
	cout << "CS 3304" << std::endl; //1st line in the main function
	cout << "Isolate Prices Assignment" << std::endl;
	cout << "Mitchell Dahmer" << std::endl;
	clock_t begin, end; //for timing
	begin = clock();
	vector <string> fileNames = directoryPull(); //vector of file names
	int fileCount = fileNames.size(); //finds number of files for later use
	string outPathOpen = "C:\\3304 Output Files\\Open Prices.csv"; //sets up paths for output
	string outPathClose = "C:\\3304 Output Files\\Close Prices.csv";
	vector <string> tickerNames = tickerScrape(fileNames[0]); //grabs list of intitial tickers, a list that given the parameters will only shrink, never grow
	vector <string> finalTickers = tickerScrape(fileNames[fileCount -1]); //grabs the list of tickers from the last final
	tL.neDeleter(tickerNames, finalTickers); //takes both lists and weeds out any from the first that dont appear in the second. doing this allows for the amount of iterations needed for comparing tickers to be as low as possible and speed things up
	tickerFactory(fileNames, fileCount, tickerNames); //begins strip loop
	tL.outputOpen(outPathOpen, tickerNames); //output for open
	tL.outputClose(outPathClose, tickerNames); // output for close
	tL.~tickerList(); //destructor for entire list
	end = clock();
	cout << (double(end) - double(begin)) / CLOCKS_PER_SEC; //end timer and output
	cin.get();
}
