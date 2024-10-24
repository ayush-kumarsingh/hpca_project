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
	
    Trace files - cloud9_phase2_core3.trace.xz  

5. Change the following key value in the champsim_config.json file
	-"branch_predictor" : "tage"          
		
6. Compile the configuration file 
	-./config.sh champsim_config.json
	-make

7. bin/champsim --warmup-instructions 200000000 --simulation-instructions 500000000 ~/traces/cloud9_phase2_core3.trace.xz 

NOTE : replace the trace names with different names to run different traces.

some changes in configurations by making changes in tage.cc

tage normal : 
	NUM_BANKS: 2
	BIMODAL_SIZE: 65536 entries
	LEN_GLOBAL: 13 bits (this means 213=81922^{13} = 8192213=8192 entries per TAGE table)
	LEN_TAG: 10 bits (tag length in each TAGE entry)
	MIN_HISTORY_LEN: 5
	MAX_HISTORY_LEN: 48

tage balanced configuration :
	NUM_BANKS = 4
	BIMODAL_SIZE = 40960
	LEN_GLOBAL = 9
	LEN_TAG = 9
	MAX_HISTORY_LEN = 32
	
tage increased history length : 
	NUM_BANKS = 3
	BIMODAL_SIZE = 32768
	LEN_GLOBAL = 10
	LEN_TAG = 9
	MAX_HISTORY_LEN = 48
	
tage shorter history : 
	NUM_BANKS = 6
	BIMODAL_SIZE = 20480
	LEN_GLOBAL = 8
	LEN_TAG = 10
	MAX_HISTORY_LEN = 24
