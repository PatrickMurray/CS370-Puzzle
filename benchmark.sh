#! /usr/bin/env bash

make

./puzzle testcase.txt
gprof ./puzzle
