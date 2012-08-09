FUZZY_LITE=../fuzzy-lite
CONFIG=Library
all:
	cd ${FUZZY_LITE}/${CONFIG} && /usr/bin/make  -f makefile all
	cp ${FUZZY_LITE}/${CONFIG}/libfuzzylite.dylib dist/
	@qmake --version
	qmake gui.pro -spec macx-g++;
	@make -f QMakefile;

clean: 
	@echo Cleaning
	@if [ -e QMakefile ]; \
	then \
		make distclean -f QMakefile; \
	fi
