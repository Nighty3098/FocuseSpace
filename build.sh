#!/bin/bash

cd src/FocuseSpace && qmake FocuseSpace.pro
make
cd ../..
bash FocuseSpace.sh

