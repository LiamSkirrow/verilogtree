# verilogtree

Print out the hierarchy of your Verilog source files in an easy to read, visual format. Like the Unix utility 'tree' but for Verilog.

For example, running the following command:
```verilogtree -f ./rtl/*.v --no-inst-name```
Will give the following output:
```
top
├── mod0
│   ├── mod3
│   │   └── mod4
│   └── mod1
│       └── mod3
│           └── mod4
├── mod1
│   └── mod3
│       └── mod4
└── mod2
    └── mod1
        └── mod3
            └── mod4
```

**NOTE: this is very much a work in progress and is not yet completely functional, check back for the v0.1.0 release for a basic working prototype.**

### Need to add either virtual environment or container to be able to reliably recompile from any machine... Look into this

### Usage
To run verilogtree, type `verilogtree -f file.v` or `verilogtree --filelist list-of-files.txt` to display the tree output. The supported arguments are...
| Argument | Description |
| -------- | ----------- |
| `-h`/`--help` | print out help information |
| `-v`/`--version` | print out version information |
| `-f` | list the Verilog files one by one |
| `--fileilst` | instead give a list of Verilog files |
| `-L`/`--level` | only print out x levels of Verilog hierarchy |
| `--no-inst-name` | don't print out the instance names in output |
| `-n`/`--ignore-modules` | ignore the child modules of the following modules |
| `--lang` | select language, either Verilog or VHDL (currently only Verilog is supported) |
| `--debug` | verbose mode, print out parsing information |
| `--super-debug` | very verbose mode, print out all internal information |

### Examples
*TODO*

### Tricks
- Sometimes it's usfeul to tell verilogtree to print out the hierarchy of 'all the verilog files in this directory and sub-directories', which is where the `find` command comes in handy. Usually, `find` goes and prints out the filenames with a newline separating them, however if you want to pass the output from a `find` command into verilogtree, it'll have to be separated by spaces. This can be achieved by running `find . -name *.v -printf '%p '`... Which shall become ` ./verilogtree -f $(find . -name *.v -printf '%p ')`.

### Installation Notes
When building from source, clone this repo and run the following steps:
- run the `BUILD` script to compile the code. Requires g++ to be installed.
- **[??? Linux only ???]** run the `INSTALL` script to copy the man page to (??? where ???)

### Unsupported Syntax
*TODO*

### Version Changelog
| Version | Changes | Status |
| ------- | ------- | ------ |
| [0.2.0](https://github.com/LiamSkirrow/verilogtree/milestone/2)   |  | Planned (see GitHub issues) |
| [0.1.0](https://github.com/LiamSkirrow/verilogtree/milestone/1)   | Initial version | *In Progress* |

### License
*TODO*

### Contributing
Submitting GitHub issues and/or PRs are accepted and welcomed 👍

### Bugs
Refer to the GitHub issues...
