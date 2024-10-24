Steps to Run the c files for various varients of the matrix multiplication (ijk, kij, jik) and different sizes of 2048 and 8192.

1. Install the perf tool in ubuntu: 
	-sudo apt-get update
	-sudo apt-get install linux-tools-common linux-tools-generic linux-tools-$(uname -r)
	
2. Provide sufficient access to the perf :
	-edit the sysctl.conf file
		-sudo nano /etc/sysctl.conf
	-add the following line:
		-kernel.perf_event_paranoid = 1
	     after adding the line, save the file 
	-in order to apply the changes, run the following command
		-sudo sysctl -p
		
3. Create the matrix_multiplication file :
	-touch mat_mul.c
	-write the program for matrix multiplication
	
4. Compile the program :
	-gcc mat_mul.c
	it will generate the output file named a.out
	
5. In order to check performance status :
	-general status :
		- perf stat ./a.out
	-particular status :
		-perf stat -e L1-dcache-loads,L1-dcache-load-misses,L1-icache-loads,L1-icache-load-misses,L2_rqsts.all_demand_data_rd,L2_rqsts.miss,dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,iTLB-loads,iTLB-load-misses,page-faults,cache-misses ./a.out
		
		
NOTE : follow the same instructions for all the varients of the matrix multiplication: 
		ijk - mat_mul.c
		jik - mat_mul_jik.c
		kij - mat_mul_kij.c
	for changing the size of the matrix, directly change from the C file.
	change the line 
		- #define N 2048 //change this to 8192
