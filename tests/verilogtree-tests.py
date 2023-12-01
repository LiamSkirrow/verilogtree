# -*- coding: utf-8 -*-
# ^ needed because of the janky tree output characters I'm using ;)

# test script to be run either locally or by GitHub actions test runner

import subprocess
from subprocess import call, check_call, check_output

# TODO:
# - check against the version number?
# - print out specifically which tests failed so it's easier to read from the output
# - 

# run verilogtree as a subprocess
print("Running version...")
check_call(["./verilogtree", "--version"], cwd = "../")

test_vec = []

### >>>>> Test strings and commands <<<<< ###

# ./verilogtree --filelist tests/rtl/simple/simple_filelist
test_str0 = """
top
├── mod0 mod0_inst0
│   ├── mod3 mod3_inst0
│   │   └── mod4 mod4_inst
│   └── mod1 mod1_instAnother
│       └── mod3 mod3_inst1
│           └── mod4 mod4_inst
├── mod1 mod1_inst0
│   └── mod3 mod3_inst1
│       └── mod4 mod4_inst
└── mod2 mod2_inst0
    └── mod1 mod1_inst1
        └── mod3 mod3_inst1
            └── mod4 mod4_inst

altTop
└── mod2 mod2_instX
    └── mod1 mod1_inst1
        └── mod3 mod3_inst1
            └── mod4 mod4_inst
"""

### >>>>> Call commands and check against test_strX <<<<< ###
# received_test_str0 = check_output(["./verilogtree", "--filelist", "tests/rtl/simple/simple_filelist"], cwd = "../")

# TEST 0 | Check basic filelist operation with simple example hierarchy
process = subprocess.Popen(['./verilogtree', '--filelist', 'tests/rtl/simple/simple_filelist'], stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd = '../')
out, err = process.communicate()
if out is not test_str0:
    print('\n----- Expected Output -----\n')
    print(test_str0)
    print('\n----- Actual Output -----\n')
    print(out)
    test_vec.append(False)

print('\n----- Test Summary -----\n')

# loop through the test vector and print the pass fail output
for test_index in range(len(test_vec)):
    if test_vec[test_index] is False:
        print('Test ' + str(test_index) + ' -> FAILED')
        all_tests_fail = True
    else:
        print('Test ' + str(test_index) + ' -> PASSED')
print('\n\n')

# an exit code of anything non-zero shall cause the GitHub actions test runner to report a failure
if(all_tests_fail):
    print('Failing due to previous failed tests')
    exit(1)
