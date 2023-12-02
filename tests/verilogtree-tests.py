# test script to be run either locally or by GitHub actions test runner

import subprocess
from subprocess import check_call

# TODO:
# - check against the version number?
# - print out specifically which tests failed so it's easier to read from the output
# - refactor this file to make it more scalable
#   - put the test strings into another Python file and import it here
#   - split up below generic code into functions

# run verilogtree as a subprocess
print("Running version...")
check_call(["./tests/verilogtree", "--version"])

test_vec = []
all_tests_fail = False

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

# ./verilogtree --filelist tests/rtl/simple/simple_filelist --no-inst-name
test_str1 = """
top
├── mod0
│   ├── mod3
│   │   └── mod4
│   └── mod1
│       └── mod3
│           └── mod
├── mod1
│   └── mod3
│       └── mod4
└── mod2
    └── mod1
        └── mod3
            └── mod4

altTop
└── mod2
    └── mod1
        └── mod3
            └── mod4

"""

# ./verilogtree --filelist tests/rtl/simple/simple_filelist --level 2
test_str2 = """
top
├── mod0 mod0_inst0
├── mod1 mod1_inst0
└── mod2 mod2_inst0

altTop
└── mod2 mod2_instX

"""

# ./verilogtree --filelist tests/rtl/simple/simple_filelist --ignore-modules mod3
test_str3 = """
top
├── mod0 mod0_inst0
│   ├── mod3 mod3_inst0
│   └── mod1 mod1_instAnother
│       └── mod3 mod3_inst1
├── mod1 mod1_inst0
│   └── mod3 mod3_inst1
└── mod2 mod2_inst0
    └── mod1 mod1_inst1
        └── mod3 mod3_inst1

altTop
└── mod2 mod2_instX
    └── mod1 mod1_inst1
        └── mod3 mod3_inst1

Ignoring childs of following module(s): mod3 
"""

### >>>>> Call commands and check against test_strX <<<<< ###

# generic template for tests -> compare strings and threshold pass/fail
def test_function(exp_str, actual_str, test_vec, test_num):
    if actual_str != exp_str:
        print('Test ' + str(test_num) + ' Failed...')
        print('\n----- Expected Output -----\n')
        print(exp_str)
        print('\n----- Actual Output -----\n')
        print(actual_str)
        test_vec.append(False)
    else:
        print('Test ' + str(test_num) + ' Passed: Expected and Actual strings match')
        test_vec.append(True)
    
    return test_vec

# TEST 0 | Check basic filelist operation with simple example hierarchy
process = subprocess.Popen(['./verilogtree', '--filelist', 'tests/rtl/simple/simple_filelist'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
out, err = process.communicate()
out_string = out.decode('utf-8')
test_vec = test_function(test_str0, out_string, test_vec, 0)

# TEST 1 | Check basic filelist operation with simple example hierarchy without instance names
process = subprocess.Popen(['./verilogtree', '--filelist', 'tests/rtl/simple/simple_filelist', '--no-inst-name'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
out, err = process.communicate()
out_string = out.decode('utf-8')
test_vec = test_function(test_str1, out_string, test_vec, 1)

# TEST 2 | Check the --level argument, set to a level depth of 2
process = subprocess.Popen(['./verilogtree', '--filelist', 'tests/rtl/simple/simple_filelist', '--level', '2'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
out, err = process.communicate()
out_string = out.decode('utf-8')
test_vec = test_function(test_str2, out_string, test_vec, 2)

# TEST 3 | Ignore modules
process = subprocess.Popen(['./verilogtree', '--filelist', 'tests/rtl/simple/simple_filelist', '--ignore-modules', 'mod3'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
out, err = process.communicate()
out_string = out.decode('utf-8')
test_vec = test_function(test_str3, out_string, test_vec, 3)


print('\n----- Test Summary -----\n')

# loop through the test vector and print the pass fail output
for test_index in range(len(test_vec)):
    if test_vec[test_index] is False:
        print('Test ' + str(test_index) + ' -> FAILED')
        all_tests_fail = True
    else:
        print('Test ' + str(test_index) + ' -> PASSED')

# an exit code of anything non-zero shall cause the GitHub actions test runner to report a failure
if(all_tests_fail):
    print('Failing due to previous failed tests')
    print('*** Final Test Status: TEST FAILED ***')
    # Test runner shall fail the GH Action
    exit(1)
else:
    print('All Expected and Actual strings match!')
    print('*** Final Test Status: TEST PASSED ***')
    # Test runner shall fail the GH Action
    exit(0) 
