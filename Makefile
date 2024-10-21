BUILD = release
FLOAT = OFF
TESTS = ON
COVERAGE = OFF
EXPORT_COMPILE_COMMANDS = ON
STRICT = OFF

CONTAINER = docker

.phonywin:

all: configure build test

clean:
	rm -rf build/

configure:
	cmake --version
	cmake -B build/ \
		-DCMAKE_BUILD_TYPE=$(BUILD) \
		-DCMAKE_CXX_STANDARD=$(CXX_STANDARD) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=$(EXPORT_COMPILE_COMMANDS) \
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
	cmake -B build/
	cmake --build build/ --parallel --target testTarget
	$(MAKE) test

install:
	cmake --build build/ --target install

python:
	python3 --version
	python3 -m venv .venv \
		&& . .venv/bin/activate \
		&& python3 -m pip install -e .

coverage: python
	. .venv/bin/activate \
		&& gcovr -r . \
			--filter src/ \
			--filter fuzzylite/ \
			--coveralls build/coveralls.json \
			--html build/coverage.html \
			--html-details \
			--html-single-page \
			--sort uncovered-percent \
			--html-theme github.dark-blue \
			--txt --txt-summary \
			build/CMakeFiles/testTarget.dir
	# open build/coverage.html

clean-coverage:
	find build/CMakeFiles/testTarget.dir -type f -name '*.gcda' -print0 | xargs -0 rm

.PHONY: docs
docs:
	# requires doxygen
	doxygen Doxyfile
	# open docs/html/index.html

jupyter:
	$(CONTAINER) --version
	$(CONTAINER) build -f tools/notebook/Dockerfile -t fl-xeus .
	$(CONTAINER) run --rm -p 8888:8888 -v.:/mnt/fuzzylite -it fl-xeus jupyter notebook --allow-root --ip 0.0.0.0

ubuntu:
	$(CONTAINER) --version
	$(CONTAINER) build -f tools/docker/ubuntu-2404.Dockerfile -t fl-ubuntu .
	$(CONTAINER) run --rm -p 8888:8888 -v.:/mnt/fuzzylite -it fl-ubuntu

CLANG_FORMAT=clang-format --style=file:.clang-format -i

format: python
	echo `$(CLANG_FORMAT) --version`
	. .venv/bin/activate \
		&& find fuzzylite -type f -name '*.h' -print0 | xargs -0 $(CLANG_FORMAT) \
		&& find src -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT) \
		&& find test -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT)

	echo "pymarkdown: `pymarkdown version`"
	. .venv/bin/activate && pymarkdown --config pyproject.toml fix README.md

lint: python
	echo `$(CLANG_FORMAT) --version`
	. .venv/bin/activate \
		&& find fuzzylite -type f -name '*.h' -print0 | xargs -0 $(CLANG_FORMAT) --dry-run --Werror \
		&& find src -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT) --dry-run --Werror \
		&& find test -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT) --dry-run --Werror

	echo "pymarkdown: `pymarkdown version`"
	. .venv/bin/activate && pymarkdown --config pyproject.toml scan README.md

	echo `cmakelint --version`
	. .venv/bin/activate && cmakelint CMakeLists.txt

	echo "clang-tidy: `clang-tidy --version`"
	. .venv/bin/activate && clang-tidy -format-style=file -header-filter=. -p build/ fuzzylite/Headers.h

	#TODO: https://github.com/llvm/llvm-project/blob/main/clang-tools-extra/clang-tidy/tool/run-clang-tidy.py
	#echo "clang-tidy: `clang-tidy --version`"
	#. .venv/bin/activate && clang-tidy -format-style=file -p build/ fuzzylite/Headers.h
