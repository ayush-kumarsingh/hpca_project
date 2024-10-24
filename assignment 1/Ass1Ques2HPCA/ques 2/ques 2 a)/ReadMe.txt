Steps to run ChampSim simulator-

1. Clone the ChampSim repository from the github
	-git clone https://github.com/ChampSim/ChampSim.git
	
2. Move to the ChampSim directory
	-cd ChampSim
	
3. Download and Install the dependencies
	-git clone https://github.com/microsoft/vcpkg
	-git init
	-git submodule update --init
	-vcpkg/bootstrap-vcpkg.sh
	-vcpkg/vcpkg install
	
4. Make the folder 'traces' and keep the traces provided in the assignment 
	-ChampSim/traces/{trace files}
	
    Trace files - cassandra_phase1_core3.trace.xz     
		cloud9_phase2_core3.trace.xz  
		classification_phase2_core3.trace.xz

5. Change the following key value in the champsim_config.json file
	-"branch_predictor" : "bimodal"           //change it with perceptron,ghare,tage
		
6. Compile the configuration file 
	-./config.sh champsim_config.json
	-make

7. bin/champsim --warmup-instructions 200000000 --simulation-instructions 500000000 ~/traces/cassandra_phase1_core3.trace.xz

NOTE : replace the trace names with different names to run different traces.
