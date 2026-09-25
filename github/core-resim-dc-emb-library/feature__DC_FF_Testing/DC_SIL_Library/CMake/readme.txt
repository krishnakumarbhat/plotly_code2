Step for Generating CMake Solution for RECU
============================================

1.Go to build folder, open command prompt.
 
 a. For Windows- Type below command in command prompt
		cmake -G "Visual Studio 16 2019" -DCUST=BMWSP25 -DTRACKER_TYPE=SRR -DVARIANT=BMW_SP25_L3 ..
        cmake -G "Visual Studio 16 2019" -DCUST=BMWSP25 -DTRACKER_TYPE=MRR -DVARIANT=BMW_SP25_L3 ..
		
		Vehicle Logs
		cmake -G "Visual Studio 16 2019" -DCUST=BMWSP25 -DTRACKER_TYPE=SRR -DVARIANT=BMW_SP25_L2 ..
        cmake -G "Visual Studio 16 2019" -DCUST=BMWSP25 -DTRACKER_TYPE=MRR -DVARIANT=BMW_SP25_L2 ..
	
 b. For Linux CMake configuartion
	In Debug mode- 
 		cmake -DCMAKE_BUILD_TYPE=DEBUG -DCUST=BMWSP25 -DTRACKER_TYPE=SRR -DVARIANT=BMW_SP25_L3 ..
		cmake -DCMAKE_BUILD_TYPE=DEBUG -DCUST=BMWSP25 -DTRACKER_TYPE=MRR -DVARIANT=BMW_SP25_L3 ..
	In Releame mode-
		cmake -DCMAKE_BUILD_TYPE=Release -DCUST=BMWSP25 -DTRACKER_TYPE=SRR -DVARIANT=BMW_SP25_L3 ..
		cmake -DCMAKE_BUILD_TYPE=Release -DCUST=BMWSP25 -DTRACKER_TYPE=MRR -DVARIANT=BMW_SP25_L3 ..
 c. For Linux compiling just enter- make 
 
	  