#!/bin/sh

MAKEOPTS="-j2"

echo "Changing into simulator directory"
cd ./pyrosim/simulator/external

echo "Unpacking ode-0.12.tar.bz2..." &&
tar -xjf ode-0.12.tar.bz2 &&
echo "done" &&

mkdir -p ./tmp

echo "Building ode-0.12..." &&
cd ode-0.12 &&
./configure --enable-double-precision > ../tmp/odeconfigure 2>&1 &&
make $MAKEOPTS > ../tmp/odemake 2>&1 &&
cd ../.. &&
echo "done" &&

echo  "Building simulator..." &&
make $MAKEOPTS
# make $MAKEOPTS > ./external/tmp/pyrosimmake 2>&1 &&
echo "done"
