# Variables
## BUILD: type of build (eg, `release`, `debug`)
BUILD = release
## BUILD_DIR: output of the build
BUILD_DIR = build/$(BUILD)
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
## DOWNLOAD_PREFIX: path to download Catch2 sources
DOWNLOAD_PREFIX = .local/src
## CONTAINER: containerisation tool to build in ubuntu and C++ notebooks
CONTAINER = docker



# Tasks
.phonywin:

all: configure build test

clean:
	rm -rf $(BUILD_DIR)

configure:
	cmake --version
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
build: .phonywin configure
	cmake --build $(BUILD_DIR) --parallel

.PHONY: test
test: .phonywin build
	ctest --test-dir $(BUILD_DIR) --output-on-failure --timeout 120 # --verbose
	# alternatively, for debugging information run:
	# $(BUILD_DIR)/bin/fuzzylite-tests --reporter console

install: build
	cmake --build $(BUILD_DIR) --target install

install-catch2:
	test -d $(DOWNLOAD_PREFIX)/Catch2 || git clone --single-branch -b v3.7.1 https://github.com/catchorg/Catch2.git $(DOWNLOAD_PREFIX)/Catch2
	cmake -B $(DOWNLOAD_PREFIX)/Catch2/build -S $(DOWNLOAD_PREFIX)/Catch2 -DCMAKE_INSTALL_PREFIX=$(INSTALL_PREFIX) -DCATCH_ENABLE_WERROR=OFF
	cmake --build $(DOWNLOAD_PREFIX)/Catch2/build --parallel --target install

python:
	python3 --version
	python3 -m venv .venv
	. .venv/bin/activate \
		&& python3 -m pip install -e .

coverage: python
	. .venv/bin/activate \
		&& gcovr -r . \
			--filter src/ \
			--filter fuzzylite/ \
			--coveralls $(BUILD_DIR)/coveralls.json \
			--html $(BUILD_DIR)/coverage.html \
			--html-details \
			--html-single-page \
			--sort uncovered-percent \
			--html-theme github.dark-blue \
			--txt --txt-summary \
			$(BUILD_DIR)/CMakeFiles/testTarget.dir
	# open build/coverage.html

clean-coverage:
	find $(BUILD_DIR)/CMakeFiles/testTarget.dir -type f -name '*.gcda' -print0 | xargs -0 rm

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

format: python
	. .venv/bin/activate \
		&& echo `clang-format --version` \
		&& find fuzzylite -type f -name '*.h' -print0 | xargs -0 $(CLANG_FORMAT) \
		&& find src -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT) \
		&& find test -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT)

	. .venv/bin/activate \
		echo "pymarkdown: `pymarkdown version`" \
		&& pymarkdown --config pyproject.toml fix README.md

lint: python
	. .venv/bin/activate \
		&& echo `clang-format --version` \
		&& find fuzzylite -type f -name '*.h' -print0 | xargs -0 $(CLANG_FORMAT) --dry-run --Werror \
		&& find src -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT) --dry-run --Werror \
		&& find test -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT) --dry-run --Werror

	. .venv/bin/activate \
		&& echo "pymarkdown: `pymarkdown version`" \
		&& pymarkdown --config pyproject.toml scan README.md

	. .venv/bin/activate \
		&& echo `cmakelint --version` \
		&& cmakelint CMakeLists.txt
