#!/bin/sh

MAKEOPTS="-j2"

#echo -n "Downloading ode-0.12..." &&
#wget https://sourceforge.net/projects/opende/files/ODE/0.12/ode-0.12.tar.bz2 > /tmp/odewget 2>&1 &&
#echo "done" &&

echo "Changing into simulator directory"
cd ./pyrosim/simulator

echo -n "Unpacking ode-0.12.tar.bz2..." &&
tar -xjf ode-0.12.tar.bz2 &&
echo "done" &&

mkdir -p ./tmp

echo -n "Building ode-0.12..." &&
cd ode-0.12 &&
./configure --enable-double-precision # > ../tmp/odeconfigure 2>&1 &&
make $MAKEOPTS # > ../tmp/odemake 2>&1 &&
cd .. &&
echo "done" &&

echo -n "Building simulator..." &&
make $MAKEOPTS # > ./tmp/pyrosimmake 2>&1 &&
echo "done"
