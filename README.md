# verilogtree

Print out the hierarchy of your Verilog source files in an easy to read, visual format. Like the Unix utility 'tree' but for Verilog!

### Usage
*TODO*

### Installation Notes
*TODO*

### Bugs
Refer to the GitHub issues... I've tried to keep them up to date. If you find a bug in this project, filing an issue and/or PR is encouraged.

### Implementation Plan
- Parse through a list of Verilog/SystemVerilog files (passed in on the command line, or provided in a text file), searching for the first module declaration (use a regex string search), once found now look for any module instantiations. These are the children of the first module.
- Repeat this until all the modules are parsed and all files are read through.
- You should have stored all module declarations (as parent nodes) and all module instantiations (as children nodes). Now loop through (efficiently!) and replace the pointers to children nodes, with pointers to the parent nodes. This will complete the tree data structure.
- Return a pointer to the top-level parent node, and recurse down the tree, printing to stdout as you go. 
- Include command line arguments similar to that of Unix 'tree' -> for example "-L" to specify the level to recurse down and print. 
    - Include another argument to avoid printing out a specific module/instance name (remove from the printing stage) in case it clutters up the terminal.
    - Include a module search feature, where a specific module/instance name is highlighted in a special colour or something.
- the ultimate test is to run like so: ' ./verilogtree -f $(find . -name \*.v)', this will feed ALL verilog files into the tool

- NOTE: it would be useful to track the addition of specific features. To this end, when adding a specific feature, it would be useful to do this in its own branch, which would then eventually get merged into main. This way, the group of commits that implement this feature can be isolated out and maybe even reverted.

### TODO/Features
- Convert the notes in this README into GitHub issues 
- Should (eventually) support VHDL syntax, since that's just a matter of allowing for different regex search patterns. Include this functionality as a command line argument to switch between Verilog/VHDL.
- include all the usual arguments like 
    - -h/--help: (display all arguments). Look into creating a man page that is slightly more verbose than just the --help arg.
    - --version: when debugging, makes it easier to verify that I've checked out the correct version of the code and built the right version.
    - -L: specify how many levels of hierarchy to print out
    - --debug: print out lots of internal information that would be useful for debugging code
    - --find: print out the hpath(s) to one/many specific modules as they're instantiated in the hierarchy
              just print out the hpath to each module, this mode shouldn't require fancy formatted output.
- include a changelog once official versioning is started (v0.1.0 for example), it'd be useful to track which features get added in in which versions.

### Testing
- Include some Python test infrastructure, reading from the --debug output. Run on Jenkins server for nightly regressions.
    - Also include in Makefile to be able to run test(s) manually

### Stretch Features 
- Extend this project to be a slightly more general command line Verilog parser:
    - Display top level IO ports in a given module/list of modules.
    - How many hardware registers are inferred? This is super useful to know.
