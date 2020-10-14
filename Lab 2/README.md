To compile: run make
To run: 

Run sim.exe with the desired program text file as an argument
Once running, you can enter these commands into the 'shell'. You must enter 'q' before the commands will execute.
Shell Commands:
    g | G | go: simulate the program until it indicates that the simulator should halt.
    r | R | run <n>: simulate the execution of the machine for n instructions.
    mdump <low> <high>: dump the contents of memory, from location low to location high to the screen <low> and <high> address should be provided as hexadecimal numbers.
    rdump: dump the current instruction count, the contents of $0- $31, FLAG N, Z, C, V, and the PC to the screen.
    i | I | input reg_num reg_val: set register (denoted reg_num) to value (reg_val).
    ? | help: print out a list of all shell commands.
    q | Q| quit: quit the shell.
