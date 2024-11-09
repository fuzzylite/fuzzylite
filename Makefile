# Variables
## BUILD: type of build (eg, `release`, `debug`)
BUILD = RelWithDebInfo
## BUILD_DIR: output of the build
BUILD_DIR = build
## FLOAT: use `fl::scalar` as `float` instead of `double`
FLOAT = OFF
## TESTS: build and run tests
TESTS = ON
## COVERAGE: compute source code coverage
COVERAGE = OFF
## EXPORT_COMPILE_COMMANDS: export compile commands as json to be used in coveralls
EXPORT_COMPILE_COMMANDS = ON
## STRICT: warnings are raised as errors
STRICT = OFF
## INSTALL_PREFIX: path to install fuzzylite libraries and Catch2
INSTALL_PREFIX = .local
## CONTAINER: containerisation tool to build in ubuntu and C++ notebooks
CONTAINER = docker
## ENTRYPOINT: entrypoint for ubuntu
ENTRYPOINT=""
## JOBS: number of jobs to use when building in parallel
JOBS=4

# Tasks
.phonywin:

all: configure build test

clean:
	mv $(BUILD_DIR) /tmp/fuzzylite-build/

configure:
	@cmake --version
	cmake -S . -B $(BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=$(BUILD) \
		-DCMAKE_CXX_STANDARD=$(CXX_STANDARD) \
		-DCMAKE_INSTALL_PREFIX=$(INSTALL_PREFIX) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=$(EXPORT_COMPILE_COMMANDS) \
		-DFL_USE_FLOAT=$(FLOAT) \
		-DFL_WARNINGS_AS_ERRORS=$(STRICT) \
		-DFL_BUILD_TESTS=$(TESTS) \
		-DFL_BUILD_COVERAGE=$(COVERAGE)

.PHONY: build
build: .phonywin
	cmake --build $(BUILD_DIR) --parallel $(JOBS)

.PHONY: test
test: .phonywin
	ctest --test-dir $(BUILD_DIR) --output-on-failure --timeout 120 # --verbose
	@echo
	@echo "alternatively, for debugging information run:"
	@echo "$(BUILD_DIR)/bin/fuzzylite-tests --reporter console"

install:
	cmake --build $(BUILD_DIR) --target install

.PHONY: docs
docs:
	doxygen Doxyfile
	@echo "doxygen: `doxygen --version`"
	@echo "open docs/html/index.html"

python:
	python3 -m venv .venv \
		&& . .venv/bin/activate \
		&& pip install tools/dev

coverage: python
	@. .venv/bin/activate && echo "`gcovr --version`"
	. .venv/bin/activate \
		&& gcovr -r . \
			--filter src/ \
			--filter fuzzylite/ \
			--sort uncovered-percent \
			--txt --txt-summary \
			$(BUILD_DIR)/CMakeFiles/testTarget.dir
