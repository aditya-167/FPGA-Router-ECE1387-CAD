#include <fstream>
#include <iostream>
#include <time.h>
#include <string>
#include "FPGAVisualizeEZGL.h"

int binary_search_minimum_tracks(std::string filePath, bool DEBUG_MODE, bool swappableInputs, int lower_bound, int upper_bound) {
    int low = lower_bound;  // Minimum number of tracks to search
    int high = upper_bound;  // Maximum number of tracks to search
    int minimum_W = high;  // The smallest successful W value

    while (low <= high) {
        int mid = (low + high) / 2;  // Calculate the middle point
        FPGARouting::MazeRoute* router = new FPGARouting::MazeRoute(mid);

        // Parse file and set up the grid
        bool doneReading = router->parseFiles(filePath);
        std::cout << "Testing for tracks per channel: " << mid << std::endl;
        std::cout << "FPGA Gridsize: " << router->getFGPAGridSize() << std::endl;
        std::cout << "FPGA Number of Tracks in each channel: " << router->getNumTracks() << std::endl;
        std::cout << "Total Number of Test Cases: " << router->getPaths()->size() << std::endl;

        router->createFPGAGrid();
        int failed = router->searchRoute(DEBUG_MODE, swappableInputs);  // Test if routing is successful

        if (failed == 0) {  // If routing is successful
            minimum_W = mid;  // Update the minimum successful track count
            high = mid - 1;   // Search in the lower half
            std::cout << "Routing successful with " << mid << " tracks." << std::endl;
			int countS = router->getTotalWires();
			std::cout << "Total number of routing segments used: " << countS << endl;
        } else {  // If routing failed
            low = mid + 1;  // Search in the upper half
            std::cout << "Routing failed with " << mid << " tracks." << std::endl;
        }

        delete router;  // Clean up the router object
    }
						std::cout << "Minimum tracks per channel is: " << minimum_W << endl;

    return minimum_W;
}


int linear_search_minimum_tracks(std::string filePath, bool DEBUG_MODE, bool swappableInputs, int upper_bound){
	int minimum_W = upper_bound;
	for (int i=upper_bound;i>0;i--){
				FPGARouting::MazeRoute* router = new FPGARouting::MazeRoute(i);

					bool doneReading = router->parseFiles(filePath);
					std::cout << "FPGA Gridsize:" << router->getFGPAGridSize() << endl;
					std::cout << "FPGA Number of Tracks in each channel:" << router->getNumTracks() << endl;
					std::cout << "Total Number of Test Cases:" << router->getPaths()->size() << endl;

					router->createFPGAGrid();
					int failed = router->searchRoute(DEBUG_MODE, swappableInputs);
					if (failed){
						std::cout << "Minimum tracks per channel is: " << i+1 << endl;
						minimum_W = i+1;
						break;
					}
					int countS = router->getTotalWires();
					std::cout << "Total number of routing segments used: " << countS << endl;
					//if (VISUALIZE) FPGAVisualizeEZGL::FPGAVisualizeInitVisualization(router, swappableInputs);
					delete router;
			}
		return minimum_W;
}



void runMazeRouter(string& filePath, bool& swappableInputs, bool& DEBUG_MODE, bool& VISUALIZE, bool& MIN_W, int lower_bound, int upper_bound, bool BINARY_SEARCH){

	if(!MIN_W){
				FPGARouting::MazeRoute* router = new FPGARouting::MazeRoute();

		bool doneReading = router->parseFiles(filePath);
	std::cout << "FPGA Gridsize:" << router->getFGPAGridSize() << endl;
	std::cout << "FPGA Number of Tracks in each channel:" << router->getNumTracks() << endl;
	std::cout << "Total Number of Test Cases:" << router->getPaths()->size() << endl;

	router->createFPGAGrid();
	router->searchRoute(DEBUG_MODE, swappableInputs);
	int countS = router->getTotalWires();
	std::cout << "Total number of routing segments used: " << countS << endl;
	if (VISUALIZE) FPGAVisualizeEZGL::FPGAVisualizeInitVisualization(router, swappableInputs);

	delete router;
	}
	else{
		int minimum_W = upper_bound;
		//binary search
		if(BINARY_SEARCH) {minimum_W = binary_search_minimum_tracks(filePath,DEBUG_MODE, swappableInputs, lower_bound, upper_bound );}
		
		// linear search
		else{ minimum_W = linear_search_minimum_tracks(filePath, DEBUG_MODE, swappableInputs, upper_bound); }
			
			
		std::cout<<"********************************MINIMUM WIRE CHANNEL RESULTS *******************************************"<<std::endl;

		FPGARouting::MazeRoute* router = new FPGARouting::MazeRoute(minimum_W);
		bool doneReading = router->parseFiles(filePath);
				std::cout << "FPGA Gridsize:" << router->getFGPAGridSize() << endl;
				std::cout << "FPGA Number of Tracks in each channel:" << router->getNumTracks() << endl;
				std::cout << "Total Number of Test Cases:" << router->getPaths()->size() << endl;
		router->createFPGAGrid();
		router->searchRoute(DEBUG_MODE, swappableInputs);
				int countS = router->getTotalWires();
		if (VISUALIZE) FPGAVisualizeEZGL::FPGAVisualizeInitVisualization(router, swappableInputs);

		std::cout << "Total number of routing segments used: " << countS << endl;
		delete router;		
	}
	
}

int main(int argc, char **argv)
{
	string filePath;
	bool DEBUG_MODE = false;
	bool swappableInputs = false;
	bool VISUALIZE = true;
	bool MIN_W = false;
	bool BINARY_SEARCH = true;
	if (argc == 7 ) {
        filePath = argv[1];
		swappableInputs = std::stoi(argv[2]);
		DEBUG_MODE = std::stoi(argv[3]);
		VISUALIZE = std::stoi(argv[4]);
		MIN_W = std::stoi(argv[5]);
		BINARY_SEARCH = std::stoi(argv[6]);
	} 
	else {
        std::cerr << "Error! first parameter -> routing file 'cct*', second parameter -> swappableInputs = 0 or 1, DEBUG_MODE = 0 or 1 for printing routing test cases on console, VISUALIZE = 0 or 1 to use GUI, MIN_W = 0 or 1 to calculate for total routing segements when W is minimum" << std::endl;
        return -1;
    }	
	//profiling timing
	clock_t start_time = clock();
	int lower_bound = 1; int upper_bound = 50;
	runMazeRouter(filePath, swappableInputs, DEBUG_MODE, VISUALIZE, MIN_W, lower_bound, upper_bound, BINARY_SEARCH);
	clock_t end_time = clock();
	std::cout << "Total running time for routing is (seconds): " << static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC << endl;
	return 0;
}

