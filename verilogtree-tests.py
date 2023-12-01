# test script to be run either locally or by GitHub actions test runner

from subprocess import call, check_call

check_call(["../verilogtree", "-h"])
