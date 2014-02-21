#!/bin/bash

scons
mv *.o ./linux
mv *.os ./linux
cp libELFModule_OpenAL.so ./linux
strip --strip-unneeded ../../../bin/linux/ELFModule_OpenAL.edm
cp libELFModule_OpenAL.so ../../../bin/playon/ELFModule_OpenAL.edm
mv libELFModule_OpenAL.so ../../../bin/linux/ELFModule_OpenAL.edm

#documentation generation
../../../tools/ELFDocGen/elfdocgen ELFModuleDSO.cpp
cp *.html *.wiki ../../../\[Docs\]/
mv *.html *.wiki ./docs
rm documentation.xml
