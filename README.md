
# FPGA Maze Routing Visualization and Analysis

 FPGA maze routing algorithm, provides visualization using EZGL, and compares two routing styles: swappable input pins and non-swappable input pins. The software also allows users to explore different search strategies for optimizing the number of tracks per channel. The routing and visualization are tested on various FPGA test circuits (`cct1`, `cct2`, `cct3`, and `cct4`).

## Project Structure

```
.
├── A1-MazeECE1387_Report.pdf         # Project report with test results and analysis
├── AutoResults                       # Contains results for swappable and non-swappable routing tests
│   ├── results_non_swappable_cct1.txt
│   ├── results_non_swappable_cct2.txt
│   ├── results_non_swappable_cct3.txt
│   ├── results_non_swappable_cct4.txt
│   ├── results_swappable_cct1.txt
│   ├── results_swappable_cct2.txt
│   ├── results_swappable_cct3.txt
│   ├── results_swappable_cct4.txt
├── executable                        # Executable folder
│   └── A1Router                      # Compiled executable for the router
├── include                           # Header files for the routing and visualization
│   ├── FPGAVisualizeEZGL.h
│   ├── graphics.h
│   └── MazeRoutingFPGA.h
├── lib                               # Implementation files for the router and visualization
│   ├── FPGAVisualizeEZGL.cpp
│   ├── FPGAVisualizeEZGL.o
│   ├── graphics.cpp
│   ├── graphics.o
│   ├── MazeRoutingFPGA.cpp
│   └── MazeRoutingFPGA.o
├── makefile                          # Makefile to compile the project
├── README.md                         # This file
├── scripts                           # Python script to run the routing algorithm multiple times
│   └── runtime_router.py
├── src                               # Source files
│   ├── main.cpp                      # Entry point for the routing algorithm
│   └── main.o
└── test_circuits                     # Test circuit files to run the router
    ├── cct1
    ├── cct2
    ├── cct3
    └── cct4
```

## Compilation and Execution

### Step 1: Compile the Project
To compile the project, use the provided `makefile`. Simply run the following command in the root directory:

```bash
make
```

This will generate the `A1Router` executable in the `executable` directory.

### Step 2: Running the Maze Router
The executable takes multiple command-line arguments to control routing parameters. Here is an example of how to run the routing algorithm:

```bash
./executable/A1Router <test_circuit_file> <swappable_inputs> <DEBUG_MODE> <VISUALIZE> <MIN_W> <BINARY_SEARCH>
```

#### Command-line Arguments:

- **test_circuit_file**: Path to the test circuit file (e.g., `../test_circuits/cct1`).
- **swappable_inputs**: Set to `1` for swappable inputs or `0` for non-swappable inputs.
- **DEBUG_MODE**: Set to `1` to print routing paths to the console for debugging, or `0` for no debug output.
- **VISUALIZE**: Set to `1` to enable GUI visualization of the routing process, or `0` to disable it.
- **MIN_W**: Set to `1` to calculate the minimum number of tracks per channel, or `0` for normal routing.
- **BINARY_SEARCH**: Set to `1` to use binary search to find the minimum tracks per channel, or `0` to use linear search.

#### Example:
Run the router with swappable inputs, no debug mode, visualization enabled, and binary search for the minimum tracks:

```bash
./executable/A1Router ../test_circuits/cct1 1 0 1 1 1
```

#### Arguments Breakdown:
- The first argument is the path to the test circuit file (`../test_circuits/cct1`).
- The second argument (`1`) enables swappable inputs.
- The third argument (`0`) disables debug mode.
- The fourth argument (`1`) enables GUI visualization.
- The fifth argument (`1`) enables calculation of the minimum number of tracks.
- The sixth argument (`1`) enables binary search for optimizing the track count.

## Binary and Linear Search Algorithms

The software includes two methods for finding the minimum number of tracks per channel that successfully routes all test cases:

- **Binary Search**: Efficiently narrows down the track count by halving the search space.
- **Linear Search**: Tests each possible track count starting from the upper bound.

These search methods help in identifying the minimum number of tracks for successful routing.

## Output and Results

The output includes the runtime of the routing process, the total number of routing segments used, and whether routing was successful for the given test case. Results are saved in the `AutoResults` directory, with separate files for swappable and non-swappable tests.

Each result file contains:
- The test case name
- The average runtime over multiple runs
- The total number of routing segments used

### Example Output:
For a non-swappable routing test, the output might look like:

```
Test case: ../test_circuits/cct1
Average runtime over 5 runs: 0.010008 seconds
Total time taken for the experiment: 0.06 seconds

Total number of routing segments used: 46
```

## Visualization

The visualization feature, powered by EZGL, dynamically displays the FPGA grid and the routes being generated. Source and destination logic blocks (CLBs) are highlighted in different colors. You can visualize each routing path individually using the "Next Path" and "Previous Path" buttons.

## Python Script for Runtime Analysis

The `scripts/runtime_router.py` automates the process of running the router multiple times and collecting average runtime results. It runs the executable with different test circuits and saves the results in `AutoResults`.

To run the script:

```bash
python3 scripts/runtime_router.py
```

This script allows you to quickly gather runtime statistics for both swappable and non-swappable configurations across all test circuits.

## Test Circuits

The `test_circuits` directory contains 4 test circuits: `cct1`, `cct2`, `cct3`, and `cct4`. Each circuit defines a grid size, number of tracks per channel, and a list of pin-to-pin connections. These circuits serve as inputs to the router.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
