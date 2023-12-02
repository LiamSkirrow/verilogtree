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

### >>>>> Call commands and check against test_strX <<<<< ###
# received_test_str0 = check_output(["./verilogtree", "--filelist", "tests/rtl/simple/simple_filelist"], cwd = "../")

# TEST 0 | Check basic filelist operation with simple example hierarchy
process = subprocess.Popen(['./verilogtree', '--filelist', 'tests/rtl/simple/simple_filelist'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
out, err = process.communicate()
out_string = out.decode('utf-8')
if out_string != test_str0:
    print('\n----- Expected Output -----\n')
    print(test_str0)
    print('\n----- Actual Output -----\n')
    print(out_string)
    test_vec.append(False)
else:
    print('Test 0: Expected and Actual strings match')
    test_vec.append(True)

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
