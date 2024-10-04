
#include "MazeRoutingFPGA.h"
using namespace std;
/*********************************************************************Tracks method starts********************************************************************* */

void FPGARouting::Track::resetTrack(){
    Tracklabel = -1; // initial: -1, target: -2, rest:0,1,2,3,4..
    srcX = -1;
    srcY = -1;
    srcP = -1;
    usable = true;
}
void FPGARouting::Track::setTracklabelsrc(int l, int x, int y, int p){
    Tracklabel = l;
    srcX = x;
    srcY = y;
    srcP = p;
}
void FPGARouting::Track::setTracklabel(int l) {
    Tracklabel = l;
}

void FPGARouting::Track::setAvailable(bool notUsed){
    usable = notUsed;
}


/*********************************************************************Tracks method ends********************************************************************* */


/********************************************************************** Path test cases method starts **************************************************************** */
FPGARouting::Path::~Path() {}

void FPGARouting::Path::setUsedSinkPin(int pin){
	usedSinkPin = pin;
}
void FPGARouting::Path::resetUsedTracks() {
	usedTracks.clear();
}

void FPGARouting::Path::setState(int state) {
	state = state;
}

void FPGARouting::Path::useTrack(FPGARouting::Track* t) {
	usedTracks.push_back(t);
}

int FPGARouting::Path::getlocationInfo(int index) {
	switch (index) {
	case 0: return loadX; break;
	case 1: return loady; break;
	case 2: return loadP; break;
	case 3: return sinkX; break;
	case 4: return sinkY; break;
	case 5: return sinkP; break;
	default: std::cout << "Invalid input index\n"; return -1;
	}
}

vector<FPGARouting::Track*>* FPGARouting::Path::getTracks() {
	return &usedTracks;
}


/********************************************************************** Path test cases method ends **************************************************************** */




/********************************************************************** Maze routing method starts **************************************************************** */

FPGARouting::MazeRoute::MazeRoute() {
	FPGARouting::MazeRoute::setGridSize(-1);
	FPGARouting::MazeRoute::setnumTracks(-1);  // Combined track count for both vertical and horizontal tracks
}
FPGARouting::MazeRoute::MazeRoute(int numTracks) {
	GridSize = -1;
	this->numTracks = numTracks;  // Set the track count for both vertical and horizontal
}

vector<FPGARouting::Path*>* FPGARouting::MazeRoute::getPaths() {
	return &testCases;
}

FPGARouting::MazeRoute::~MazeRoute(){
	// Free RoutePath
	freeMazeRoute();
	// Free FPGAGrid
	freeFPGAGrid();
}

void FPGARouting::MazeRoute::freeMazeRoute() {
	for (int i = 0; i < FPGARouting::MazeRoute::testCases.size(); i++) {
		delete FPGARouting::MazeRoute::testCases[i];
		FPGARouting::MazeRoute::testCases[i] = nullptr;
	}
}

void FPGARouting::MazeRoute::freeFPGAGrid() {
	for (int i = 0; i < FPGAGrid.size(); i++) {
		for (int j = 0; j < FPGAGrid[i].size(); j++) {
			for (int k = 0; k < FPGAGrid[i][j].size(); k++) {
				delete FPGAGrid[i][j][k];
				FPGAGrid[i][j][k] = nullptr;
			}
		}
	}
}

bool FPGARouting::MazeRoute::parseFiles(std::string& testCasesFile) {
	ifstream infile;
	infile.open(testCasesFile.c_str(), std::ifstream::in);
	if (!infile.is_open()) {
        std::cerr << "Error: Unable to open test circuit file " << testCasesFile << std::endl;
        return false;
    }
	int testCase[6];
	infile >> testCase[0];
	GridSize = testCase[0];
	infile >> testCase[0];
	if (numTracks == -1)  // If numTracks exists, that means we are checking for MIN_W
		numTracks = testCase[0];
	while ((infile >> testCase[0]) && (infile >> testCase[1]) && (infile >> testCase[2]) && (infile >> testCase[3]) && (infile >> testCase[4]) && (infile >> testCase[5])) {
		if (testCase[0] == -1) break;
		else {
			Path* path = new Path(testCase[0], testCase[1], testCase[2], testCase[3], testCase[4], testCase[5]);
			FPGARouting::MazeRoute::testCases.push_back(path);
		}
	}
	infile.close();
	return true;
}
void FPGARouting::MazeRoute::resetPaths() {
	for (int i = 0; i < FPGARouting::MazeRoute::testCases.size(); i++) {
		FPGARouting::MazeRoute::testCases[i]->resetUsedTracks();
		FPGARouting::MazeRoute::testCases[i]->setState(0);
		}
	}
	void FPGARouting::MazeRoute::createFPGAGrid() {
		// Creating the grid for routing
		for (int x = 0; x < (2 * GridSize + 1); x++) {
			bool h = (x % 2 == 1) ? true : false;
			vector< vector<FPGARouting::Track*> > column;
			FPGAGrid.push_back(column);
			for (int y = 0; y < (GridSize + h); y++) {
					vector<FPGARouting::Track*> channel;
				FPGAGrid[x].push_back(channel);
				for (int i = 0; i < numTracks; i++) {  
					FPGARouting::Track* t = new FPGARouting::Track(x, y, i);
					FPGAGrid[x][y].push_back(t);
				}
			}
		}
	}

	void FPGARouting::MazeRoute::resetFPGAGrid() {
		for (int x = 0; x < FPGAGrid.size(); x++) {
			for (int y = 0; y < FPGAGrid[x].size(); y++) {
				for (int i = 0; i < FPGAGrid[x][y].size(); i++) {
					FPGAGrid[x][y][i]->resetTrack();
				}
			}
		}
	}

	void FPGARouting::MazeRoute::labelFPGAGrid() {
		for (int x = 0; x < FPGAGrid.size(); x++) {
			for (int y = 0; y < FPGAGrid[x].size(); y++) {
				for (int i = 0; i < FPGAGrid[x][y].size(); i++) {
					FPGAGrid[x][y][i]->setTracklabel(-1);
				}
		}
	}
}


int FPGARouting::MazeRoute::searchRoute(bool DEBUG_MODE, bool swappableInputs) {
	queue<FPGARouting::Track*> q;    // FIFO queue
		int route = 0;
		for (int p = 0; p < testCases.size(); p++) {
			bool foundPath = true;

			bool foundDst = false;

			int wireSegment = 0;
			Track* lastTrack;
			labelFPGAGrid();    
			while (!q.empty()) q.pop();  

			bool isExit = labelTracks(p, q, DEBUG_MODE, swappableInputs);
		if (isExit) {
			route = p++;
			break;
		}
		while (!q.empty()) {  // expansion list
			Track* temp = q.front();
		
			vector<Track*> adjTrack;
			FPGARouting::MazeRoute::lookAround(temp, adjTrack);
			for (int t = 0; t < adjTrack.size(); t++) {   
				Track* adjacentTrack = adjTrack[t];
				if (adjacentTrack->getTracklabel() == -2) { 
					foundDst = true;
					lastTrack = adjacentTrack;
					wireSegment = temp->getTracklabel() + 1; 
					break;
					}
					else if (adjacentTrack->getIsAvailable() && (adjacentTrack->getTracklabel() == -1)) { 
						adjacentTrack->setTracklabelsrc((temp->getTracklabel() + 1), temp->getSourceX(), temp->getSourceY(), temp->getSourceP());
						q.push(adjacentTrack);

					}
					else if ((temp->getSourceX() == adjacentTrack->getSourceX()) &&
						(temp->getSourceY() == adjacentTrack->getSourceY()) &&
						(temp->getSourceP() == adjacentTrack->getSourceP())
						) {  
						if (adjacentTrack->getTracklabel() == -1) { 
							adjacentTrack->setTracklabelsrc((temp->getTracklabel() + 1), temp->getSourceX(), temp->getSourceY(), temp->getSourceP());
							q.push(adjacentTrack);
							//cout << "reuse happened!\n";
						}
						else if ((temp->getTracklabel() + 1) < adjacentTrack->getTracklabel()) { 
							adjacentTrack->setTracklabelsrc((temp->getTracklabel() + 1), temp->getSourceX(), temp->getSourceY(), temp->getSourceP());
							q.push(adjacentTrack);
						}
					}
				} 
				if (foundDst)
					break;
				q.pop();
			}
			if (!foundDst) {
				testCases[p]->setState(2);    
				route = p++;
				break;
			}
			else {
				// backtracking
				testCases[p]->getTracks()->push_back(lastTrack); 
				lastTrack->setAvailable(false); 
				if(swappableInputs){
					int usedSinkPin = getPinFromFinalTrack(p,lastTrack);
				testCases[p]->setUsedSinkPin(usedSinkPin);
				}
				for (int d = wireSegment - 1; d >= 0; d--) {
				vector<Track*> adjTrack;
				lookAround(lastTrack, adjTrack);
					bool foundTrackBack = false;
					for (int t = 0; t < adjTrack.size(); t++) {   
						Track* adjacentTrack = adjTrack[t];
						if (adjacentTrack->getTracklabel() == d) {
							lastTrack = adjacentTrack;
							testCases[p]->getTracks()->push_back(lastTrack); 
							lastTrack->setAvailable(false);  
							foundTrackBack = true;
							break;
						}
					}
					if (!foundTrackBack) {
						foundPath = false;
						break;
					}
				}
			}
			if (foundPath) {
				cout <<"Routing Complete, Total Wiresegments for test case "<< p<<" : " << wireSegment + 1 << endl;

				if (DEBUG_MODE == 1) {
					for (int i = testCases[p]->getTracks()->size() - 1; i >= 0; i--) {
						int x = testCases[p]->getTracks()->at(i)->getlocationTrackX();
						int y = testCases[p]->getTracks()->at(i)->getlocationTrackY();
						int locationTrackID = testCases[p]->getTracks()->at(i)->getTrackId();
						if (i == 0)
							cout << "[" << x << "," << y << "," << locationTrackID << "]" <<std::endl;
						else
							cout << "[" << x << "," << y << "," << locationTrackID << "] =>";
					}
				}			for (int i = testCases[p]->getTracks()->size() - 1; i >= 0; i--) {
					testCases[p]->setState(1); //routed
				}
			}
			else {
				if (DEBUG_MODE == 1) {
					cout << "No path found for test case: "<<p<<std::endl;
				}
				testCases[p]->setState(2);// unroutable
				route = p + 1;
				break;
			}
		} 
		if (DEBUG_MODE == 1) {
			if (!route) {
				cout << "Successfully Routed!!"<<std::endl;
			}		else {
				cout << "unable to route test case:" << route << std::endl;
			}
		}	return route;

}

void FPGARouting::MazeRoute::lookAround(Track* temp, vector<Track*> &adjTrack) {
	int locationTrackX = temp->getlocationTrackX();
	int locationTrackY = temp->getlocationTrackY();
	int locationTrackID = temp->getTrackId();
	if (locationTrackX % 2 == 0) { 
		if (locationTrackX > 0) { 
			adjTrack.push_back(FPGAGrid[locationTrackX - 1][locationTrackY][locationTrackID%numTracks]);
			adjTrack.push_back(FPGAGrid[locationTrackX - 1][locationTrackY + 1][locationTrackID%numTracks]);
			
		}
		if (locationTrackX < (FPGAGrid.size() - 1)) { 
			
			  
				adjTrack.push_back(FPGAGrid[locationTrackX + 1][locationTrackY][locationTrackID%numTracks]);
				adjTrack.push_back(FPGAGrid[locationTrackX + 1][locationTrackY + 1][locationTrackID%numTracks]);
			
		}
		if (locationTrackY > 0) {  
			adjTrack.push_back(FPGAGrid[locationTrackX][locationTrackY - 1][locationTrackID]);
		}
		if (locationTrackY < (FPGAGrid[locationTrackX].size() - 1)) {  
			adjTrack.push_back(FPGAGrid[locationTrackX][locationTrackY + 1][locationTrackID]);
		}
	}
	else {  // for horizontal tracks
		if (locationTrackY > 0) {  
				adjTrack.push_back(FPGAGrid[locationTrackX - 1][locationTrackY - 1][locationTrackID%numTracks]);
				adjTrack.push_back(FPGAGrid[locationTrackX + 1][locationTrackY - 1][locationTrackID%numTracks]);
			
		}
		if (locationTrackY < (FPGAGrid[locationTrackX].size() - 1)) { 
			  
				adjTrack.push_back(FPGAGrid[locationTrackX - 1][locationTrackY][locationTrackID%numTracks]);
				adjTrack.push_back(FPGAGrid[locationTrackX + 1][locationTrackY][locationTrackID%numTracks]);
			
		}
		if (locationTrackX > 1) {  
			adjTrack.push_back(FPGAGrid[locationTrackX - 2][locationTrackY][locationTrackID]);
		}
		if (locationTrackX < (FPGAGrid.size() - 2)) {
			adjTrack.push_back(FPGAGrid[locationTrackX + 2][locationTrackY][locationTrackID]);
		}
	}
}
bool FPGARouting::MazeRoute::labelTracks(int p, queue<Track*> &q, bool DEBUG_MODE, bool swappableInputs) {
	int sourceX = testCases[p]->getlocationInfo(0);
	int sourceY = testCases[p]->getlocationInfo(1);
	int sourceP = testCases[p]->getlocationInfo(2);
	int sinkX = testCases[p]->getlocationInfo(3);
	int sinkY = testCases[p]->getlocationInfo(4);
	int sinkP = testCases[p]->getlocationInfo(5);
	bool notfound = true;
	Track* temp;
	
	int source_track_x = 2 * sourceX + 2; //pin for is always source
	int source_track_y = sourceY;
	for (int w = 0; w < FPGAGrid[source_track_x][source_track_y].size(); w++) {
			temp = FPGAGrid[2 * sourceX +2][source_track_y][w];
			if (temp->getIsAvailable()) {
				temp->setTracklabelsrc(0, sourceX, source_track_y, sourceP);
				q.push(FPGAGrid[source_track_x][source_track_y][w]);
			}
			else if ((temp->getSourceX() == sourceX) && (temp->getSourceY() == sourceY) && (temp->getSourceP() == sourceP)) {
				q.push(FPGAGrid[source_track_x][source_track_y][w]);
				temp->setTracklabel(0);
			}
		}
	if (q.empty()) {
		if (DEBUG_MODE) cout <<"can route"<<p<<std::endl;
		testCases[p]->setState(2);
		return true;}
	if(!swappableInputs){
		
		int target_track_x = 0;
		int target_track_y = 0;

		switch (sinkP){
			case (1) :{
				target_track_x =  2 * sinkX + 1;
				target_track_y = sinkY;
				break;

			}
			case (2) :{
				target_track_x =  2 * sinkX;
				target_track_y = sinkY;
				break;

			}
			case (3) :{
				target_track_x =  2 * sinkX + 1;
				target_track_y = sinkY + 1;
				break;

			}
			case (4) :{
				target_track_x =  2 * sinkX + 2;
				target_track_y = sinkY;
				break;

			}
		}

		for (int w = 0; w < FPGAGrid[target_track_x][target_track_y].size(); w++) {
			temp = FPGAGrid[target_track_x][target_track_y][w];
			if ((sinkP == 1 || sinkP == 3) && (w % 2 == 0)) {
				if (temp->getIsAvailable()) {
					notfound = false;
					temp->setTracklabelsrc(-2, -1, -1, -1); 
				} else if ((temp->getSourceX() == sinkX) && (temp->getSourceY() == sinkY) && (temp->getSourceP() == sinkP)) {
					notfound = false;
					temp->setTracklabel(-2);  
					}
				}
			else if (sinkP == 2 && (w % 2 == 1)) {
				if (temp->getIsAvailable()) {
					notfound = false;
					temp->setTracklabelsrc(-2, -1, -1, -1); 
				} else if ((temp->getSourceX() == sinkX) && (temp->getSourceY() == sinkY) && (temp->getSourceP() == sinkP)) {
					notfound = false;
					temp->setTracklabel(-2);  // Mark as target
				}
			}		}
		if (notfound) {
			if (DEBUG_MODE) cout <<"can route"<<p<<std::endl;
			testCases[p]->setState(2);
			return true;
		}		return false;
	}
	//swappable Inputs:
	else{
		for (int possibleSinkPin = 1; possibleSinkPin <= 3; ++possibleSinkPin) {
			int target_track_x = 0;
			int target_track_y = 0;

			switch (possibleSinkPin) {
				case 1: {
					target_track_x = 2 * sinkX + 1;
					target_track_y = sinkY;
					break;
				}
				case 2: {
					target_track_x = 2 * sinkX;
					target_track_y = sinkY;
					break;
				}
				case 3: {
					target_track_x = 2 * sinkX + 1;
					target_track_y = sinkY + 1;
					break;
				}
			}

			for (int w = 0; w < FPGAGrid[target_track_x][target_track_y].size(); w++) {
				temp = FPGAGrid[target_track_x][target_track_y][w];
				if ((possibleSinkPin == 1 || possibleSinkPin == 3) && (w % 2 == 0)) {
					if (temp->getIsAvailable()) {
						notfound = false;
						temp->setTracklabelsrc(-2, -1, -1, -1); 
					} else if ((temp->getSourceX() == sinkX) && (temp->getSourceY() == sinkY)) {
						notfound = false;
						temp->setTracklabel(-2); 
					}
				}				else if (possibleSinkPin == 2 && (w % 2 == 1)) {
					if (temp->getIsAvailable()) {
						notfound = false;
						temp->setTracklabelsrc(-2, -1, -1, -1); 

					} else if ((temp->getSourceX() == sinkX) && (temp->getSourceY() == sinkY)) {
						notfound = false;
						temp->setTracklabel(-2);  
						
						
					}
				}
			}
		}
    }

			if (notfound) {
			if (DEBUG_MODE) cout <<"can route"<<p<<std::endl;
			testCases[p]->setState(2);
			return true;
		}

		return false;
}

int FPGARouting::MazeRoute::getTotalWires() {
	int wires = 0;
	for (int x = 0; x < FPGAGrid.size(); x++) {
		for (int y = 0; y < FPGAGrid[x].size(); y++) {
			for (int i = 0; i < FPGAGrid[x][y].size(); i++) {
				if (!FPGAGrid[x][y][i]->getIsAvailable()) wires++;
			}
		}
	}	return wires;
}

vector<vector<vector<FPGARouting::Track*>>>* FPGARouting::MazeRoute::getFPGAGrid() {return & FPGAGrid;}

int FPGARouting::MazeRoute::getPinFromFinalTrack(int p,Track* finalTrack) {
    // Logic to determine the sink pin based on the final track location and locationTrackID
    // Assuming the track coordinates or locationTrackID can help identify the pin

    int x = finalTrack->getlocationTrackX();
    int y = finalTrack->getlocationTrackY();
    int tracklocationTrackID = finalTrack->getTrackId();
	int sinkX = testCases[p]->getlocationInfo(3);
    int sinkY = testCases[p]->getlocationInfo(4);

    // Use the grid and track information to determine the actual pin
    // check if track is closer to pin 1, 2, or 3
    if (tracklocationTrackID % 2 == 0) {
        // Track is on an even locationTrackID, more likely associated with Pin 1 or Pin 3
        if ((x == (2 * sinkX + 1)) && (y == sinkY)) {
            return 1;  // Pin 1 (South)
        } else if(((x == (2 * sinkX + 1)) && (y == sinkY+1))) {
            return 3;  // Pin 3 (North)
        }
    } else if (tracklocationTrackID % 2 == 1) {
        // Track is on an odd locationTrackID, more likely associated with Pin 2
        if ((x == (2 * sinkX ) && (y == sinkY))) {
            return 2;  // Pin 2 (West)
        }
    }

    return -1;  // Return invalid if no pin is found (should not happen if routing succeeded)
}
/********************************************************************** Maze routing method ends **************************************************************** */
