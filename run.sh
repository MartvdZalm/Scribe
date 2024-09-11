#!/bin/bash
# run.sh

if [ -n $1 ]
then
	full_path=$(realpath $1)
	echo $full_path

	cd build/src/main || exit

	./scribe $full_path
else
	cd build/src/main || exit

	./scribe
fi