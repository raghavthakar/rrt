#! /bin/bash

cd src
g++ -o rrt rrt.cpp && ./rrt < output.txt
cd ..
cd scripts
python3 visualiser.py