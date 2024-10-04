#include "graphics.h"
#include <iostream>
#include "MazeRoutingFPGA.h"

namespace FPGAVisualizeEZGL{
    void FPGAVisualizeInitVisualization(FPGARouting::MazeRoute* r, bool swappableInputs);
void FPGAVisualizedrawGrid(void);
void FPGAVisualizeNextPath (void (*drawscreen_ptr) (void));
void FPGAVisualizePreviousPath(void(*drawscreen_ptr)(void));
void FPGAVisualizedrawIncrementaltestCases();
void FPGAVisualizebutton_click(float x, float y);}


