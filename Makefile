BUILD = debug
CPP98 = OFF
FLOAT = OFF
TESTS = ON
COVERAGE = OFF
EXPORT_COMPILE_COMMANDS = OFF
STRICT = OFF

CONTAINER = docker

.phonywin:

all: configure build test

clean:
	rm -rf build/

configure:
	cmake -B build/ \
		-DCMAKE_BUILD_TYPE=$(BUILD) \
		-DCMAKE_CXX_STANDARD=$(CXX_STANDARD) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=$(EXPORT_COMPILE_COMMANDS) \
		-DFL_CPP98=$(CPP98) \
		-DFL_USE_FLOAT=$(FLOAT) \
		-DFL_WARNINGS_AS_ERRORS=$(STRICT) \
		-DFL_BUILD_TESTS=$(TESTS) \
		-DFL_BUILD_COVERAGE=$(COVERAGE)


.PHONY: build
build: .phonywin
	cmake --build build/ --parallel

.PHONY: test
test: .phonywin
	ctest --test-dir build/ --output-on-failure --timeout 120 # --verbose

test-only:
	cmake -B build/ && \
		cmake --build build/ --parallel --target fl-test && \
		 $(MAKE) test

install:
	cmake --build build/ --target install

coverage:
	python3 -m venv .venv && . .venv/bin/activate && \
	python3 -m pip install gcovr && \
	gcovr -r . \
	  	--filter src/ \
	  	--filter fuzzylite/ \
		--coveralls build/coveralls.json \
		--html build/coverage.html \
		--html-details \
		--sort uncovered-percent \
		--html-theme github.dark-blue \
		--txt --txt-summary \
		build/CMakeFiles/fl-test.dir && \
	deactivate
	# open build/coverage.html

clean-coverage:
	find build/CMakeFiles/fl-test.dir -type f -name '*.gc' -print0 | xargs -0 rm

jupyter:
	$(CONTAINER) build -f tools/notebook/Dockerfile -t fl-xeus . && \
 	$(CONTAINER) run --rm -p 8888:8888 -v.:/mnt/fuzzylite -it fl-xeus jupyter notebook --allow-root --ip 0.0.0.0

ubuntu:
	$(CONTAINER) build -f tools/docker/ubuntu-2404.Dockerfile -t fl-ubuntu . && \
 	$(CONTAINER) run --rm -p 8888:8888 -v.:/mnt/fuzzylite -it fl-ubuntu

CLANG_FORMAT=clang-format --style=file:.clang-format -i

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


