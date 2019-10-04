#include <unistd.h>
#include <term.h>
#include <cstdio>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <istream>
#include <fstream>

int countNeighbors(bool *gameboard, int, int, int, int);

int main(int argc, char* argv[]) {

	if(argc != 2) {
		std::cout << "USAGE: ./conway input_file" << std::endl;
		exit(1);
	}

	//Get initial state from file
	std::ifstream infile;
	infile.open(argv[1]);
	if(!infile.is_open()) {
		std::cout << "Error opening file: " << argv[1] << std::endl;
		exit(2);
	}
	std::string tmp;
	infile >> tmp; // Get height and width from input file  TODO: Error check values
	int height = std::stoi(tmp);
	infile >> tmp;
	int width = std::stoi(tmp);

	bool gameboard[height][width];

	for(int i = 0; i < height; i++) {
		std::getline(infile, tmp);
		for(int j = 0; j < width; j++) {
			gameboard[i][j] = (tmp[j] == '1' ? true : false);	//Check if each character in file is a 1, set cell to alive if it is
										//TODO: Check for extra garbage data and give an error if found						
		}
	}		
	//Run game loop
	while(true) {
		std::string nextScreen;
		for(int i = 0; i < height; i++) {
			for(int j = 0; j < width; j++) {
				if(gameboard[i][j]) {
					nextScreen +="\u25A0"; //Square character
				} else {
					nextScreen +=" ";
				}
			}
			nextScreen += "\n";
		}
		std::cout << nextScreen << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		
		//Count number of neighbors and determine whether the cell will be alive or dead in the next iteration
		int neighborCount[height][width];
		for(int i = 0; i < height; i++) {
			for(int j = 0; j < width; j++) {
				neighborCount[i][j] = countNeighbors( (bool *)gameboard, i, j, height, width);
				//std::cout << neighborCount[i][j] << " ";
			}
			//std::cout << std::endl;
		}



		for(int i = 0; i < height; i++) {
			for(int j = 0; j < width; j++) {
				if(neighborCount[i][j] < 2) {
					gameboard[i][j] = false;
				} else if(neighborCount[i][j] == 3) {
					gameboard[i][j] = true;
				} else if(neighborCount[i][j] > 3) {
					gameboard[i][j] = false;
				}
			}
		}
		
		//std::cin >> tmp;

		for(int i = 0; i < height; i++) {
			std::cout << "\x1b[A"; //ANSI go up one line
			for(int j = 0; j <= width; j++) {
				std::cout << " "; //Override line with spaces to clear for next display tick
			}
			std::cout << "\r" << std::flush; // return cursor to beginning of line
		}
	}
}


int countNeighbors(bool *gameboard, int i, int j, int height, int width) {
	/*
	[i-1, j-1]* [i-1, j ]* [i-1, j+1]*
	[i, j-1  ]*            [i, j+1  ]*
	[i+1, j-1]* [i+1, j ]* [i+1, j+1]*
	*/
	int total = 0;
	if( !(i+1 >= height) ) {
		if(gameboard[(i+1)*width + j]) {	//[i+1, j]
			total += 1;
		}

		if( !(j+1 >= width) ) {
			if(gameboard[(i+1)*width + (j+1)]) { //[i+1, j+1]
				total += 1;
			}
		}
		if( !(j-1 < 0) ) {
			if(gameboard[(i+1)*width + (j-1)]) { //[i+1, j-1]
				total += 1;
			}
		}
	}

	if( !(j+1 >= width) ) {
		if(gameboard[i*width + (j+1)]) { //[i, j+1]
			total += 1;
		}
	}

	if( !(j-1 < 0) ) {
		if(gameboard[i*width + (j-1)]) { //[i, j-1]/
			total += 1;
		}
	}

	if( !(i-1 < 0) ) {
		if( !(j-1 < 0) ) {
			if(gameboard[(i-1)*width + (j-1)]) { //[i-1, j-1]
				total += 1;
			}
		}

		if(gameboard[(i-1)*width + j]) { 	//[i-1, j]
			total += 1;
		}

		if( !(j+1 >= width) ) {
			if(gameboard[(i-1)*width + (j+1)]) { //[i-1, j+1]
				total += 1;
			}
		}
	}
	return total;
	
}
