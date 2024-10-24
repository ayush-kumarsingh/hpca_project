import sys
import re
from collections import defaultdict

def analyze_memory_access(file_path, mx_large_pages):
    # we are creating a Dictionary to hold TLB misses for each 2MB region
    TLB_misses = defaultdict(int)
    # In order to extract the addresses we are matching the pattern
    add_pattern = re.compile(r'([0-9a-f]{16})')  # Matching 16-digit hex addresses

    # we are opening the file to Read the memory access trace file
    with open(file_path, 'r') as f:
        for line in f:
            # Look for memory load/store lines with TLB misses
            if 'L2 miss' in line: 
                match = add_pattern.search(line)
                if match:
                    # hex to decimal conversion
                    address = int(match.group(1), 16)
                    # Base address calculation for 2MB region
                    base_add = (address // (2 * 1024 * 1024)) * (2 * 1024 * 1024)
                    # Incrementing the TLB miss count 
                    TLB_misses[base_add] += 1

    # Sort the regions in decreasing order of their TLB misses
    sorted_regions = sorted(TLB_misses.items(), key=lambda x: x[1], reverse=True)

    # Get the top 'n' regions optimal regions
    opt_reg = sorted_regions[:mx_large_pages]

    # Conversion of base addresses to decimal
    dec_reg = [base for base, _ in opt_reg]        #here dec_reg is the decimal regions

    return dec_reg

def save_large_pages(opt_reg, out_file):
    #Save the optimal large page addresses to a text file.
    with open(out_file, 'w') as f:
        for region in opt_reg:
            f.write("{}\n".format(region))  

def main():
    if len(sys.argv) != 2:
        print("Usage: python analyze.py <n>")
        sys.exit(1)

    try:
        mx_large_pages = int(sys.argv[1])
    except ValueError:
        print("Error: n must be an integer.")
        sys.exit(1)

    # mem_access_file contains the path to the data file
    mem_access_file = 'report.txt'
    
    # for the Analysis of the memory access data
    opt_reg = analyze_memory_access(mem_access_file, mx_large_pages)
    
    if not opt_reg:
        print("No optimal regions found.")
        sys.exit(1)
    
    # Saving the results to largepages.txt
    save_large_pages(opt_reg, 'largepages.txt')
    print("Optimal large page regions saved to 'largepages.txt'.")

if __name__ == "__main__":
    main()

