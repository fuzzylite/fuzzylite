BUILD = debug
CPP98 = OFF
FLOAT = OFF
COVERAGE = OFF

G = ""

ifeq ($(G),"")
	ifeq ($(OS),Windows_NT) # OS is a preexisting environment variable on Windows
		G = "NMake Makefiles"
	else
		UNAME := $(shell uname -s)
		ifeq ($(UNAME),Darwin)
			G = "Unix Makefiles"
		else ifeq ($(UNAME),Linux)
			G = "Unix Makefiles"
		else
			G = "Unix Makefiles"
		endif
	endif
endif

.phonywin:

all: configure build test

clean:
	rm -rf build/

configure:
	cmake -B build/ -G $(G) -DCMAKE_BUILD_TYPE=$(BUILD) -DFL_CPP98=$(CPP98) -DFL_USE_FLOAT=$(FLOAT) -DFL_BUILD_COVERAGE=$(COVERAGE) .

.PHONY: build
build: .phonywin
	cmake --build build/ --parallel

.PHONY: test
test: .phonywin
	ctest --test-dir build/

test-only:
	$(MAKE) configure && \
		cmake --build build/ --parallel --target fl-test && \
		 $(MAKE) test

install:
	cmake --build build/ --target install

coverage:
	# pip install gcovr
	gcovr -r src/ build/CMakeFiles/fl-test.dir/ --coveralls build/coveralls.json --html build/coverage.html --html-details --sort uncovered-percent --html-theme github.blue --txt --txt-summary
	# open build/coverage.html

clean-coverage:
	find build/CMakeFiles/fl-test.dir -type f -name '*.gc' -print0 | xargs -0 rm

jupyter:
	docker build -f examples/notebook/Dockerfile -t xeus . && docker run --rm -p 8888:8888 -v.:/mnt/fuzzylite -it xeus jupyter notebook --allow-root --ip 0.0.0.0


CLANG_FORMAT=clang-format --style=file -i

format:
	# headers
	find fuzzylite -type f -name '*.h' -print0 | xargs -0 $(CLANG_FORMAT)

	# sources
	find src -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT)

	# tests
	find test -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT)

lint:
	# headers
	find fuzzylite -type f -name '*.h' -print0 | xargs -0 $(CLANG_FORMAT) --dry-run --Werror

	# sources
	find src -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT) --dry-run --Werror

	# tests
	find test -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT) --dry-run --Werror


