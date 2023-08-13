#!/bin/bash

make cp		FILE=$1
make chmod 	FILE=$1
make 		FILE=$1
make clean
make nfs
