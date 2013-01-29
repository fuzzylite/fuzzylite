#!/bin/sh

macdeployqt qtfuzzylite.app

#otool -L executable

#install_name_tool -change PATH/TO/LIB/?.dylib @executable_path/../Frameworks/?.dylib

install_name_tool -change /tmp/fuzzylite/fuzzylite/libfuzzylite-dbg.dylib @executable_path/../Frameworks/libfuzzylite-dbg.dylib qtfuzzylite