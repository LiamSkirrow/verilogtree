# verilogtree

Print out the hierarchy of your Verilog source files in an easy to read, visual format. Like the Unix utility 'tree' but for Verilog.

For example, cloning this repo and running the following:
```
> make
> ./verilogtree -f tests/rtl/simple/*.v --no-inst-name
```
Will give the following output:
```
altTop
└── mod2
    └── mod1
        └── mod3
            └── mod4
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
Which is based on the very simple Verilog code found at the path `tests/rtl/simple`.

### Usage
To run verilogtree, type `verilogtree -f file.v` or `verilogtree --filelist list-of-files.txt` to display the tree output. The supported arguments (all of which are optional, other than one of `-f` and --`filelist`) are...
| Argument | Description |
| -------- | ----------- |
| `-h`/`--help` | Print out help information |
| `-v`/`--version` | Print out version information |
| `-f` | List the Verilog files one by one |
| `--fileilst` | Instead give a list of Verilog files |
| `-L`/`--level` | Only print out x levels of Verilog hierarchy |
| `-m`/`--max-hierarchy` | Specify the maximum hierarchy depth to be recursed down |
| `--no-inst-name` | Don't print out the instance names in output |
| `-n`/`--ignore-modules` | Ignore the child modules of the following modules |
| `-T`/`--top` | List modules that shall be treated as top level modules |
| `--lang` | Select language, either Verilog or VHDL (currently only Verilog is supported in v0.1.x) |
| `--debug` | Verbose mode, print out general information |
| `--super-debug` | Very verbose mode, print out all internal information |

### Examples
Check out the `man-page` for some examples of using verilogtree.

### Tricks
- Sometimes it's usfeul to tell verilogtree to print out the hierarchy of 'all the verilog files in this directory and sub-directories', which is where the `find` command comes in handy. Usually, `find` goes and prints out the filenames with a newline separating them, however if you want to pass the output from a `find` command into verilogtree, it'll have to be separated by spaces. This can be achieved by running `find . -name *.v -printf '%p '`... Which when used in verilogtree, becomes... ` ./verilogtree -f $(find . -name *.v -printf '%p ')`.

### Installation Notes
When building from source, clone this repo and run the following steps:
- run `make` to compile the code. Requires g++ to be installed.
- run `sudo ./install.sh` to install verilogtree. (optional, if this step is skipped just run the executable generated from the previous step)
- try running `verilogtree --version` to test whether the program was installed successfully.

### Unsupported Syntax
#### _verilogtree version: v0.1.x_

Module declarations from a package, for example `module ibex_core import ibex_pkg::*; #(` aren't recognised as a valid module within the design. Therefore, instantiating the module `ibex_core` would cause verilogtree to error out, reporting an unrecognised module. Support for this construct is included in the `v0.2.0` milestone, but is not yet implemented.

Also, instantiating a module with parameters is not supported, for example as shown below.
```
ibex_ex_block #(
    .RV32M          (RV32M),
    .RV32B          (RV32B),
    .BranchTargetALU(BranchTargetALU)
  ) ex_block_i (
    .clk_i (clk_i),
    .rst_ni(rst_ni),
```

Lastly, although module declarations can span multiple lines, module instantiations must be on the same line. For example,
```
module mod0();   // good!

module
    mod0();      // good!

mod0 mod0_inst(); // good!

mod0
    mod0_inst(); // BAD! Not recognised as a valid module instantiation
```

### Version Changelog
| Version | Changes | Status |
| ------- | ------- | ------ |
| [0.2.0](https://github.com/LiamSkirrow/verilogtree/milestone/2)   |  | Planned (see GitHub issues) |
| [0.1.0](https://github.com/LiamSkirrow/verilogtree/milestone/1)   | Initial version | *In Progress* |

### License
This project uses the permissive MIT license. See the LICENSE file in this repo for more information.

### Contributing
Submitting GitHub issues and/or PRs are accepted and welcomed 👍

### Bugs
Refer to the GitHub issues...
