#include "FPGAVisualizeEZGL.h"
FPGARouting::MazeRoute* Router;   // set as global variable so other functions can access easily
int nextTestcase = 0;
bool swappable = false;
void FPGAVisualizeEZGL::FPGAVisualizeInitVisualization(FPGARouting::MazeRoute* router, bool swappableInputs) {
    	init_graphics("ECE1387 Assignment 1 (Maze router - By Aditya Srichandan)", WHITE);               

	int i;
	Router = router;
	swappable = swappableInputs;
	init_world(0., 0., 1000.0, 1000.0);
	create_button("Window", "Next Path", FPGAVisualizeEZGL::FPGAVisualizeNextPath);    
	create_button("Window", "PreviousPath", FPGAVisualizeEZGL::FPGAVisualizePreviousPath);         
	FPGAVisualizeEZGL::FPGAVisualizedrawGrid();
	event_loop(FPGAVisualizeEZGL::FPGAVisualizebutton_click, NULL, NULL, FPGAVisualizeEZGL::FPGAVisualizedrawGrid);
	close_graphics();                                             
}
void FPGAVisualizeEZGL::FPGAVisualizebutton_click(float x, float y){std::cout<<x<<std::endl<<y<<std::endl;}
void FPGAVisualizeEZGL::FPGAVisualizedrawGrid(void) {  
    	float area = 75;
                                       
	float gridSize = Router->getFGPAGridSize();
	float numtrck = Router->getNumTracks();
    	float switchSize = (1000.0 - 2 * area) / (gridSize * 2 + 1) * 0.5;
	float blockSize = (1000.0 - 2 * area) / (gridSize * 2 + 1);

    	float hTrackSize = switchSize / (numtrck + 1);                               

	float vTrackSize = switchSize / (numtrck + 1);                               
	float spacebwpin = switchSize / 4;                                   
    float pin_offset = spacebwpin * 1.35;      
	set_draw_mode(DRAW_NORMAL);                                   
	clearscreen();                                                 
	for (int i = 0; i < 2 * gridSize + 1; i++) {
		for (int j = 0; j < 2 * gridSize + 1; j++) {
			if ((i + j) % 2 == 0) {   
				setcolor(DARKGREY); 
                fillrect((area + j * blockSize + 0.5 * blockSize - 0.5 * switchSize),
                         (area + i * blockSize + 0.5 * blockSize - 0.5 * switchSize),
                         (area + j * blockSize + 0.5 * blockSize + 0.5 * switchSize),
                         (area + i * blockSize + 0.5 * blockSize + 0.5 * switchSize));
                char coord_label[10];
                snprintf(coord_label, sizeof(coord_label), "SWB");
                setcolor(WHITE);  
                setfontsize(12);  
                drawtext(area + j * blockSize + 0.5 * blockSize,
                         area + i * blockSize + 0.5 * blockSize,
                         coord_label, 100.0); 	
			}
			else { 
				if (i % 2 == 1) { 
					for (int k = 0; k < numtrck; k++) {
						setlinewidth(1);
						setcolor(LIGHTGREY);
						drawline((area + j * blockSize + 0.5*blockSize - 0.5*switchSize + vTrackSize * (k + 1)),
							(area + i * blockSize + 0.5*blockSize + 0.5*blockSize*1.5),
							(area + j * blockSize + 0.5*blockSize - 0.5*switchSize + vTrackSize * (k + 1)),
							(area + i * blockSize + 0.5*blockSize - 0.5*blockSize*1.5));
					}
				}
				else {  
					for (int k = 0; k < numtrck; k++) {
						setlinewidth(1);
						setcolor(LIGHTGREY);
						drawline((area + j * blockSize + 0.5*blockSize - 0.5*blockSize*1.5),
							(area + i * blockSize + 0.5*blockSize - 0.5*switchSize + hTrackSize * (k + 1)),
							(area + j * blockSize + 0.5*blockSize + 0.5*blockSize*1.5),
							(area + i * blockSize + 0.5*blockSize - 0.5*switchSize + hTrackSize * (k + 1)));
					}

				}
			}
			if ((i % 2 == 1) && (j % 2 == 1)) {
				setcolor(CYAN);
				fillrect((area + j * blockSize + 0.5*blockSize + 0.5*switchSize),
					(area + i * blockSize + 0.5*blockSize + 0.5*switchSize),
					(area + j * blockSize + 0.5*blockSize - 0.5*switchSize),
					(area + i * blockSize + 0.5*blockSize - 0.5*switchSize));


                char coord_label[10];
                snprintf(coord_label, sizeof(coord_label), "(%d,%d)", (int)j / 2,(int)(2 * gridSize - i) / 2); 
                setcolor(BLACK);  
                setfontsize(12);  
                drawtext(area + j * blockSize + 0.5 * blockSize,
                         area + i * blockSize + 0.5 * blockSize,
                         coord_label, 100.0);  
				for (int p = 0; p < 4; p++) {
                    switch (p) {
                        case 0:  
						setlinewidth(1);
                    	setcolor(BLACK);
                            drawline(area + j * blockSize + 0.5 * blockSize + pin_offset,
                                     area + i * blockSize + 0.5 * blockSize + 0.5 * switchSize,
                                     area + j * blockSize + 0.5 * blockSize + pin_offset,
                                     area + i * blockSize + 0.5 * blockSize + 2.5 * switchSize + pin_offset);
                            break;

                        case 1:  
						setlinewidth(1);
                    setcolor(BLACK);
                            drawline(area + j * blockSize + 0.5 * blockSize - 0.5 * switchSize,
                                     area + i * blockSize + 0.5 * blockSize + pin_offset,
                                     area + j * blockSize + 0.5 * blockSize - 2.5 * switchSize - pin_offset,
                                     area + i * blockSize + 0.5 * blockSize + pin_offset);
                            break;

                        case 2:  
						setlinewidth(1);
                    setcolor(BLACK);
                            drawline(area + j * blockSize + 0.5 * blockSize - pin_offset,
                                     area + i * blockSize + 0.5 * blockSize - 0.5 * switchSize,
                                     area + j * blockSize + 0.5 * blockSize - pin_offset,
                                     area + i * blockSize + 0.5 * blockSize - 2.5* switchSize - pin_offset);
                            break;

                        case 3: 
						setlinewidth(1);
                    setcolor(BLACK);
                            drawline(area + j * blockSize + 0.5 * blockSize + 0.5 * switchSize,
                                     area + i * blockSize + 0.5 * blockSize - pin_offset,
                                     area + j * blockSize + 0.5 * blockSize + 2.5 * switchSize + pin_offset,
                                     area + i * blockSize + 0.5 * blockSize - pin_offset);
                            break;
                    }

                    // Pin number labels
                    char pin_label[2];
                    snprintf(pin_label, sizeof(pin_label), "%d", p + 1);
                    setfontsize(10);
                    setcolor(BLACK);

                    switch (p) {
                        case 0:
                            drawtext(area + j * blockSize + 0.5 * blockSize+pin_offset,
                                     area + i * blockSize + 0.5 * blockSize + 0.5 * switchSize, pin_label, 100.0);
                            break;

                        case 1:
                            drawtext(area + j * blockSize + 0.5 * blockSize - 0.5 * switchSize,
                                     area + i * blockSize + 0.5 * blockSize, pin_label, 100.0);
                            break;

                        case 2:
                            drawtext(area + j * blockSize + 0.5 * blockSize- pin_offset,
                                     area + i * blockSize + 0.5 * blockSize - 0.5 * switchSize, pin_label, 100.0);
                            break;

                        case 3:
                            drawtext(area + j * blockSize + 0.5 * blockSize + 0.5 * switchSize,
                                     area + i * blockSize + 0.5 * blockSize, pin_label, 100.0);
                            break;
                    }
				}
				
			}

		}
	}
	FPGAVisualizeEZGL::FPGAVisualizedrawIncrementaltestCases();
}
void FPGAVisualizeEZGL::FPGAVisualizeNextPath(void(*drawscreen_ptr) (void)) {
	nextTestcase++;
	if (nextTestcase > Router->getPaths()->size()) {
        nextTestcase = Router->getPaths()->size();
    }
	FPGAVisualizedrawGrid();
	printf("Test Case : %d\n", nextTestcase);
}
void FPGAVisualizeEZGL::FPGAVisualizedrawIncrementaltestCases() {
            float area = 75;
    float gridSize = Router->getFGPAGridSize();
        float blockSize = (1000.0 - 2 * area) / (gridSize * 2 + 1);        
    float numtrck = Router->getNumTracks();
    float switchSize = (1000.0 - 2 * area) / (gridSize * 2 + 1) * 0.5;
    float spacebwpin = switchSize / 4;          
    float vTrackSize = switchSize / (numtrck + 1);  
    float hTrackSize = switchSize / (numtrck + 1);  
    float pin_offset = spacebwpin * 1.35;  

    if (nextTestcase >= Router->getPaths()->size() + 1) {
		printf("All test cases Done!!\n");
		update_message("Finished all test cases!");
	}
    else if (nextTestcase > 0) {
		if (nextTestcase == Router->getPaths()->size()) {
			printf("All test cases Done!!\n");
		update_message("Finished all test cases!");
		}
    for (int n = 0; n < nextTestcase; n++) {
        int src_x = 2 * Router->getPaths()->at(n)->getlocationInfo(0) + 1;
        int src_y = 2 * (gridSize - 1 - Router->getPaths()->at(n)->getlocationInfo(1)) + 1;
        int dst_x = 2 * Router->getPaths()->at(n)->getlocationInfo(3) + 1;
        int dst_y = 2 * (gridSize - 1 - Router->getPaths()->at(n)->getlocationInfo(4)) + 1;
        int j = 2 * Router->getPaths()->at(n)->getlocationInfo(0) + 1;
        int i = 2 * (gridSize - 1 - Router->getPaths()->at(n)->getlocationInfo(1)) + 1;
        int pinS = Router->getPaths()->at(n)->getlocationInfo(2);  
        setlinestyle(SOLID);
        setlinewidth(3);
        setcolor(BLUE);  
        switch (pinS) {
            case 1:  // South pin
                drawline(area + j * blockSize + 0.5 * blockSize + pin_offset,
                         area + i * blockSize + 0.5 * blockSize + 0.5 * switchSize,
                         area + j * blockSize + 0.5 * blockSize + pin_offset,
                         area + i * blockSize + 0.5 * blockSize + 2.5 * switchSize + pin_offset);
                break;
            case 2:  // West pin
                drawline(area + j * blockSize + 0.5 * blockSize - 0.5 * switchSize,
                      area + i * blockSize + 0.5 * blockSize + pin_offset,
                         area + j * blockSize + 0.5 * blockSize - 2.5 * switchSize - pin_offset,
                         area + i * blockSize + 0.5 * blockSize + pin_offset);
                break;
            case 3:  // North pin
                drawline(area + j * blockSize + 0.5 * blockSize - pin_offset,
                         area + i * blockSize + 0.5 * blockSize - 0.5 * switchSize,
                     area + j * blockSize + 0.5 * blockSize - pin_offset,
                         area + i * blockSize + 0.5 * blockSize - 2.5 * switchSize - pin_offset);
                break;
            case 4:  // East pin
                drawline(area + j * blockSize + 0.5 *blockSize +0.5 * switchSize,
                         area + i * blockSize + 0.5 *blockSize -pin_offset,
                         area + j * blockSize + 0.5 *blockSize +2.5 * switchSize + pin_offset,
                         area + i * blockSize + 0.5 *blockSize -pin_offset);
                break;
        }
        setcolor(RED);
        fillrect((area + src_x * blockSize + 0.5 * blockSize + 0.5 * switchSize),
                 (area + src_y * blockSize + 0.5 * blockSize + 0.5 * switchSize),
                 (area + src_x * blockSize + 0.5 * blockSize - 0.5 * switchSize),
                 (area + src_y * blockSize + 0.5 * blockSize - 0.5 * switchSize));
                 char coord_label[10];
        setcolor(GREEN);
        fillrect((area + dst_x * blockSize + 0.5 * blockSize + 0.5 * switchSize),
                 (area + dst_y * blockSize + 0.5 * blockSize + 0.5 * switchSize),
                 (area + dst_x * blockSize + 0.5 * blockSize - 0.5 * switchSize),
                 (area + dst_y * blockSize + 0.5 * blockSize - 0.5 * switchSize));
        vector<FPGARouting::Track*> *paths = Router->getPaths()->at(n)->getTracks();
        setcolor(BLUE);
        for (int ti = 0; ti < paths->size(); ti++) {
            FPGARouting::Track*track = paths->at(ti);
            int gx = track->getlocationTrackX();
            int gy = 2 * (gridSize - track->getlocationTrackY());
            int ix = track->getTrackId();
            if (gx % 2 == 0) { 
                setlinestyle(SOLID);
              setlinewidth(3);
                drawline((area + gx * blockSize + 0.5 * blockSize - 0.5 * switchSize + vTrackSize * (ix + 1)),
                         (area + (gy - 1) * blockSize + 0.5 * blockSize + 0.5 * 1.5 *blockSize),
                   (area + gx * blockSize + 0.5 * blockSize - 0.5 * switchSize + vTrackSize * (ix + 1)),
                         (area + (gy - 1) * blockSize + 0.5 * blockSize - 0.5 * 1.5*blockSize));
            } else {  // horizontal tracks
                setlinestyle(SOLID);
                setlinewidth(3);
                drawline((area + gx * blockSize + 0.5 * blockSize - 0.5 * 1.5 * blockSize),
                         (area + gy * blockSize + 0.5 * blockSize - 0.5 * switchSize + hTrackSize * (ix + 1)),
                  (area + gx * blockSize + 0.5 * blockSize + 0.5 * 1.5 *blockSize),
                  (area + gy * blockSize + 0.5 * blockSize - 0.5 * switchSize + hTrackSize * (ix + 1)));
            }
        }

        // Draw load pin wire
        j = 2 * Router->getPaths()->at(n)->getlocationInfo(3) + 1;
        i = 2 * (gridSize - 1 - Router->getPaths()->at(n)->getlocationInfo(4)) + 1;
        int pinL = 0;
        if (!swappable) {
            pinL = Router->getPaths()->at(n)->getlocationInfo(5);  // get load pin in swappable
        } else {
            pinL = Router->getPaths()->at(n)->getUsedSinkPin();
        }
        setlinestyle(SOLID);
        setlinewidth(3);
        setcolor(BLUE);  
        switch (pinL) {
        case 1:  // South pin
                drawline(area + j * blockSize + 0.5 * blockSize + pin_offset,
                         area + i * blockSize + 0.5 * blockSize + 0.5 * switchSize,
                         area + j * blockSize + 0.5 * blockSize + pin_offset,
                         area + i * blockSize + 0.5 * blockSize + 2.5 * switchSize + pin_offset);
                break;
            case 2:  // West pin
                drawline(area + j * blockSize + 0.5 * blockSize - 0.5 * switchSize,
                         area + i * blockSize + 0.5 * blockSize + pin_offset,
                         area + j * blockSize + 0.5 * blockSize - 2.5 * switchSize - pin_offset,
                         area + i * blockSize + 0.5 * blockSize + pin_offset);
              break;
            case 3:  // North pin
                drawline(area + j * blockSize + 0.5 * blockSize - pin_offset,
                         area + i * blockSize + 0.5 * blockSize - 0.5 * switchSize,
                         area + j * blockSize + 0.5 * blockSize - pin_offset,
                         area + i * blockSize + 0.5 * blockSize - 2.5 * switchSize - pin_offset);
                break;
            case 4:  // East pin
                drawline(area + j * blockSize + 0.5 * blockSize + 0.5 * switchSize,
                         area + i * blockSize + 0.5 * blockSize - pin_offset,
                         area + j * blockSize + 0.5 * blockSize + 2.5 * switchSize + pin_offset,
                         area + i * blockSize + 0.5 * blockSize - pin_offset);
                break;
        }
    }
}
}

void FPGAVisualizeEZGL::FPGAVisualizePreviousPath(void(*drawscreen_ptr) (void)) {
	if(nextTestcase<=0) nextTestcase = 0;
	else if (nextTestcase > 0) {
		nextTestcase--;
	}
	FPGAVisualizeEZGL::FPGAVisualizedrawGrid();}
