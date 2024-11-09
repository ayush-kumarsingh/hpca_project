import os
import re

def parse_perf_file(file_path):
    data = {
        "cycles": 0,
        "instructions": 0,
        "branches": 0,
        "branch_misses": 0,
        "topdown_retiring": 0,
        "topdown_bad_spec": 0,
        "task_clock": 0,
    }

    try:
        with open(file_path, 'r') as f:
            for line in f:
                if "cycles" in line and "task-clock" not in line:
                    data["cycles"] = int(re.search(r'([\d,]+)', line).group(1).replace(',', ''))
                elif "instructions" in line:
                    data["instructions"] = int(re.search(r'([\d,]+)', line).group(1).replace(',', ''))
                elif "branches" in line:
                    data["branches"] = int(re.search(r'([\d,]+)', line).group(1).replace(',', ''))
                elif "branch-misses" in line:
                    data["branch_misses"] = int(re.search(r'([\d,]+)', line).group(1).replace(',', ''))
                elif "topdown-retiring" in line:
                    data["topdown_retiring"] = int(re.search(r'([\d,]+)', line).group(1).replace(',', ''))
                elif "topdown-bad-spec" in line:
                    data["topdown_bad_spec"] = int(re.search(r'([\d,]+)', line).group(1).replace(',', ''))
                elif "task-clock" in line:
                    data["task_clock"] = float(re.search(r'([\d,\.]+)', line).group(1).replace(',', ''))

        # Calculate IPC 
        if data["cycles"] > 0:
            data["ipc"] = data["instructions"] / data["cycles"]
        else:
            data["ipc"] = 0  # Set IPC to 0 if there are no cycles
    except Exception as e:
        print(f"Error parsing {file_path}: {e}")

    return data

# Function to rank threads based on performance metrics
def rank_threads(perf_data):
    ranked_threads = sorted(
        perf_data.items(), 
        key=lambda x: (
            -x[1]['ipc'],                # Higher IPC is better
            x[1]['branch_misses'],       # Lower branch misses is better
            -x[1]['topdown_retiring'],   # Higher topdown-retiring is better
            -x[1]['topdown_bad_spec'],   # Higher topdown-bad-spec is better
            x[1]['task_clock']           # Lower task clock is better
        )
    )
    return ranked_threads

# Function to determine if a thread needs to be pinned to a core
def needs_pinning(metrics):
    if metrics['ipc'] > 1.0 and metrics['branch_misses'] < 500000000:
        return True  # Suggest pinning
    return False  # Suggest letting the OS handle it

def analyze_threads_perf(log_dir='.'):
    perf_data = {}

    for thread_idx in range(3):  
        file_name = f"perf_report_thread_{thread_idx}.txt"
        file_path = os.path.join(log_dir, file_name)
        
        if os.path.exists(file_path):
            perf_data[thread_idx] = parse_perf_file(file_path)
        else:
            print(f"Error: {file_name} not found.")
            return None

    ranked_threads = rank_threads(perf_data)

    # Write the core assignment to a file
    with open('core_assignment.txt', 'w') as f:
        core_assignment = {0: -1, 1: -1, 2: -1, 3: -1}  

        for thread_idx, metrics in ranked_threads:
            if needs_pinning(metrics):
                assigned = False
                for core in [0, 1, 2, 3]:
                    if core_assignment[core] == -1:
                        core_assignment[core] = thread_idx
                        f.write(f"Thread {thread_idx}: Core {core}\n")
                        assigned = True
                        break
                if not assigned:
                    f.write(f"Thread {thread_idx}: Core -1 (no available cores)\n")
            else:
                f.write(f"Thread {thread_idx}: Core -1 (let OS manage)\n")

    return ranked_threads


if __name__ == "__main__":
    analyze_threads_perf('.')
