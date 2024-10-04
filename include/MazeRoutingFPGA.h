#include <iostream>

#include <stdio.h>
#include <string>
#include <fstream>
#include <queue>
#include <cstdlib>
#include "graphics.h"

#include <vector>
#include <iostream>

using namespace std;

namespace FPGARouting{
	class Track{
		private:
			int Tracklabel; // {starting -> -1, dst -> -2, rest: 0 to n}

			int locationTrackX;
			int locationTrackY;
			int TrackId; // index of tracks to pin down even and odd pins of clbs
			int srcX;
			int srcY;
			int srcP;
			bool usable; 
		public:
			Track(int x, int y, int i){
				 locationTrackX = x;
					locationTrackY = y;
					TrackId = i;
					Tracklabel = -1; 
					srcX = -1;
					srcY = -1;
					srcP = -1;
					usable = true;
			};
			void setTracklabelsrc(int l, int x, int y, int p);
			void resetTrack();
			void setTracklabel(int l);
			void setAvailable(bool notUsed);
					int getlocationTrackY(){ return locationTrackY; }
			int getTrackId(){ return TrackId; }
			int getlocationTrackX(){ return locationTrackX; }


		bool getIsAvailable(){ return usable; }
			int getSourceP(){ return srcP; }
			int getTracklabel(){ return Tracklabel; }
			int getSourceX(){ return srcX; }
			int getSourceY(){ return srcY; }
	};

	class Path{
		private:
					int state;  
			vector<Track*> usedTracks;

			int loadX;
			int loady;
			int loadP;
			int sinkX;
			int sinkY;
			int sinkP;
			int usedSinkPin;

		public:
			Path(int srcx, int srcy, int pinsrc, int dstx, int dsty, int pindst) {
				loadX = srcx;
				loady = srcy;
				loadP = pinsrc;
				sinkX = dstx;
				sinkY = dsty;
				sinkP = pindst;
			} ;
			~Path();
			void setState(int state);
			void useTrack(Track* thisTrack);
			int getlocationInfo(int index);  // 0-5
			void setUsedSinkPin(int pin);
			int getUsedSinkPin() { return usedSinkPin; }
			int getState(){ return state; }
			vector<Track*>* getTracks();
			void resetUsedTracks();
			
	};


	class MazeRoute {
		private:
			int GridSize;
			int numTracks;
			vector<Path*> testCases;
			vector<vector<vector<Track*>>>FPGAGrid; 
		public:
			MazeRoute();
			MazeRoute(int numTracks);
			~MazeRoute();
			void freeMazeRoute();
			void freeFPGAGrid();
			int getFGPAGridSize() { 
				return GridSize; 
			}
			int getNumTracks(){
				return numTracks;
				}
			vector< vector< vector<Track*>>>*getFPGAGrid();
			vector<Path*>*getPaths();
			bool parseFiles(std::string& testCasesFile);
			void resetPaths();
			void createFPGAGrid();
			void resetFPGAGrid();
			void labelFPGAGrid();
					int getTotalWires();
			void setGridSize(int gridSize){GridSize = gridSize;}

			void setnumTracks(int numTracks){this->numTracks = numTracks;};
			int searchRoute(bool DEBUG_MODE, bool swappableInputs);
			bool labelTracks(int p, queue<Track*> &q, bool DEBUG_MODE, bool swappableInputs);
			void lookAround(Track* tempTrack, vector<Track*> &adjacentTracks);
			int getPinFromFinalTrack(int p, Track* finalTrack);	};


}







