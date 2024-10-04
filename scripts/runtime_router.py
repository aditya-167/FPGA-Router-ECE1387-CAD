import subprocess
import re
import time
import glob
import os
def run_routing_and_extract_time(executable, args, num_runs):
    total_runtime = 0.0
    all_outputs = []

    for i in range(num_runs):
        # Run the executable with command-line arguments and capture the output
        result = subprocess.run([f'./{executable}', *args], stdout=subprocess.PIPE, text=True)

        # Extract the running time from the output
        output = result.stdout
        all_outputs.append(f"\nRun {i + 1} Output:\n{output}")

        # Find the line with the total runtime using regex
        match = re.search(r'Total running time for routing is \(seconds\): (\d+\.\d+)', output)

        if match:
            runtime = float(match.group(1))
            all_outputs.append(f"Run {i + 1}: Runtime = {runtime} seconds")
            total_runtime += runtime
        else:
            all_outputs.append(f"Run {i + 1}: Could not extract runtime from output.")
            all_outputs.append(output)

    # Calculate the average runtime
    average_runtime = total_runtime / num_runs if num_runs > 0 else 0.0
    return average_runtime, all_outputs

if __name__ == "__main__":
    # Name of the executable and number of times to run
    executable_name = "../executable/A1Router"  # Replace this with the name of your executable
    num_runs = 25  # Number of runs to average

    test_files = glob.glob("../test_circuits/cct*")  # Adjust the pattern if needed (e.g., "test_cases/cct*")

    input_types = {"Swappable":1, "non-Swappable":0}

    for _,v in input_types.items():
        output_file = None
        for test_file in test_files:
            # Define your command-line arguments (replace with actual arguments, and use the test_file)
            
            command_line_args = [test_file, str(v), "0", "0", "0", "0"] # test file, swappble pins, DEBUG=false, VISUAL = false, MIN_W = false, BinarySearch = false 

            print(f"\nRunning routing for test file: {test_file}")
            start_time = time.time()
            avg_runtime, all_outputs = run_routing_and_extract_time(executable_name, command_line_args, num_runs)
            total_elapsed_time = time.time() - start_time

            # Create a file name based on the test case name
            if v==1:
                output_file = f"../AutoResults/results_swappable_{os.path.basename(test_file)}.txt"
            else:
                output_file = f"../AutoResults/results_non_swappable_{os.path.basename(test_file)}.txt"


            # Write the output to the file
            with open(output_file, "w") as f:
                f.write(f"Test case: {test_file}\n")
                f.write(f"Average runtime over {num_runs} runs: {avg_runtime:.6f} seconds\n")
                f.write(f"Total time taken for the experiment: {total_elapsed_time:.2f} seconds\n\n")
                f.write("\n".join(all_outputs))

            print(f"Results saved to {output_file}")