FUZZY_LITE=../fuzzy-lite
all:
	cd ${FUZZY_LITE}/Library && /usr/bin/make  -f makefile all
	cp ${FUZZY_LITE}/Library/libfuzzylite.dylib dist/
	@qmake --version
	qmake gui.pro -spec macx-g++;
	@make -f QMakefile;

clean: 
	@echo Cleaning
	@if [ -e QMakefile ]; \
	then \
		make distclean -f QMakefile; \
	fi
