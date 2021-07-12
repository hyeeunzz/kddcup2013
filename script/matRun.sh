#! /bin/bash
# ./matlab.sh "test(10,20)"

exec /home/jmlee/MATLAB/R2014b/bin/matlab -nosplash -nodesktop -r "$*"
