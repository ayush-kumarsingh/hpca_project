DIFFERENT STEPS FOLLOWED : 
Step 1 : Running the main.cpp 
				>>Make run 
	This command will execute the main.cpp and will generate the executable main file. 


Step 2 : Record the performance using the perf tools  
				>> sudo perf c2c record ./main 
	This will record the performance and will generate perf.data file. 
				>> sudo perf c2c report -i perf.data > report.txt 
	This will convert the perf.data file to readable report.txt (text file). 


Step 3 : Make changes in the main.cpp file 
	Since we found that the program is performing worse because of the false sharing present in the code 
	we made changes in the main.cpp file  to remove false sharing. 
	Changes in the code :  
	3 
		struct SensorReadings { 
		alignas(64) int8_t temperature; 
		alignas(64) int8_t humidity; 
		alignas(64) int8_t pressure; 
		alignas(64) int8_t light; 
		alignas(64) int8_t co2; 
		alignas(64) int8_t aqi; 
		}; 


Step 4: Again run the main.cpp file 
				>>Make run 
	This command will execute the main.cpp and will generate the executable main file. 


Step 5: Again Record the performance using perf tool 
				>>sudo perf c2c record ./main 
	This will record the performance and will generate a perf.data file. 
				>> sudo perf c2c report -i perf.data > report.txt 
	This will convert the perf.data file to readable report.txt (text file). 
