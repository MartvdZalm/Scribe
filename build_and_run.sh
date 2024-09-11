#!/bin/bash
# build_and_run.sh

cd build || exit

cmake --build .

# Check if build was successful
if [ $? -eq 0 ]; then
  echo "Build successful, running tests..."

  cd src/test
  ./unit_tests
else
  echo "Build failed."
  exit 1
fi
