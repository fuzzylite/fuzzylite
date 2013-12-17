#!/bin/bash
make -f Makefile clean
rm -rf Makefile CMakeFiles CMakeScripts Debug qtfuzzylite.build qtfuzzylite.xcodeproj tmp cmake_install.cmake CMakeCache.txt qtfuzzylite.app moc_*
rm *.h *.cxx Info.plist 
