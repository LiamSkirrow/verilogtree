# verilogtree

Print out the hierarchy of your Verilog in an easy to read visual format. Like the command line utility 'tree' but for Verilog instead of files!

### Usage


### Installation Notes
TODO

### Bugs
For a better picture of the bugginess of this project, check out the GitHub issues... I've tried to keep them up to date. If you find a bug, filing an issue and/or PR is appreciated!

### Implementation Plan
- Parse through a list of Verilog/SystemVerilog files (passed in on the command line, or provided in a text file), searching for the first module declaration (use a regex string search), once found now look for any module instantiations. These are the children of the first module.
- Repeat this until all the modules are parsed and all files are read through.
- You should have stored all module declarations (as parent nodes) and all module instantiations (as children nodes). Now loop through (efficiently!) and replace the pointers to children nodes, with pointers to the parent nodes. This will complete the tree data structure.
- Return a pointer to the top-level parent node, and recurse down the tree, printing to stdout as you go. 
- Include command line arguments similar to that of Unix 'tree' -> for example "-L" to specify the level to recurse down and print. 
    - Include another argument to avoid printing out a specific module/instance name (remove from the printing stage) in case it clutters up the terminal.
    - Include a module search feature, where a specific module/instance name is highlighted in a special colour or something.

### TODO/Features
- Convert the notes in this README into GitHub issues 
- Read in LEF/DEF and show the shape of the design in the main window, include the IO pins in the correct locations. Pins should be clickable, and a little popup shows the pin-name and associated constraints (input/output delay etc, pin type [clock, data etc])
- Should (eventually) support VHDL syntax, since that's just a matter of allowing for different regex search patterns. Include this functionality as a command line argument to switch between Verilog/VHDL.
- include all the usual arguments like 
    - -h/--help: (display all arguments). Look into creating a man page that is slightly more verbose than just the --help arg.
    - --version: when debugging, makes it easier to verify that I've checked out the correct version of the code and built the right version.
    - -L: specify how many levels of hierarchy to print out

### Stretch Features 
- Extend this project to be a slightly more general command line Verilog parser:
    - Display top level IO ports in a given module/list of modules.
    - How many hardware registers are inferred? This is super useful to know.
