# Hardware Implementation

### Open Project
* Download the github project in the local repository.
* Open the project. The project name ends with (.prj). 
 ![open_project](./Hw_Implementation_Docs/open_project.png)
* The project has different solutions. Each solution has unique target device, clock period, cosimultation settings, and configuration settings.
 ![solution_names](./Hw_Implementation_Docs/solution_names.png)
* For all the solutions, the top module is same. Change or view the top module from, project -> project settings -> synthesis.
 ![project_settings](./Hw_Implementation_Docs/project_settings.png)


### C-Simulation 
* The C-simulation is common for all the solution. Run the C-simulation: Project -> Run C Simulation.
 ![simulation](./Hw_Implementation_Docs/simulation.png)
* We run the C-simulation with debug mode. Simulation does not work on normal mode. Hence, make sure "Launch Debugger" is ticked.
* In the debug mode, run the "Resume(F8)" to start the simulation. If the program works successfully, console shows "Known Answer test PASSED."
 ![debug_mode](./Hw_Implementation_Docs/debug_mode.png)
 ![debug_mode_2](./Hw_Implementation_Docs/debug_mode_2.png)


### Paper Citation
If you are using this software/methodology/concept in any research paper, kindly cite our work.
* https://eprint.iacr.org/2019/047.pdf
* https://csrc.nist.gov/CSRC/media/Events/Second-PQC-Standardization-Conference/documents/accepted-papers/soni-hardware-evaluation.pdf

