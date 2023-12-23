# run with sudo to move the executable and the man page to the right location
echo "*** Make sure to run this script with sudo!"

# recompile from scratch
echo "*** Recompiling just in case!"
make clean && make

# move executable to somewhere in $PATH
echo "*** Copying executable to /usr/bin"
cp verilogtree /usr/bin

# move man page to right location, type 'man verilogtree'
echo "*** Copying man page to /usr/share/man/man1"
cp man-page/verilogtree.1 /usr/share/man/man1
