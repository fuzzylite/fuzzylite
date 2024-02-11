BUILD = release
CPP98 = OFF
FLOAT = OFF
COVERAGE = ON


all: configure make test

.PHONY: configure
configure:
	cmake -B build/ -DCMAKE_BUILD_TYPE=$(BUILD) -DFL_CPP98=$(CPP98) -DFL_USE_FLOAT=$(FLOAT) -DFL_BUILD_COVERAGE=$(COVERAGE) .

.PHONY: make
make:
	cmake --build build/ --parallel

.PHONY: test
test:
	ctest --test-dir build/

.PHONY: coverage
coverage:
	# pip install gcovr
	gcovr -r src/ build/CMakeFiles/fl-test.dir/

.PHONY: coverage_coveralls
coverage_coveralls:
	# pip install gcovr
	gcovr -r src/ build/CMakeFiles/fl-test.dir/ --coveralls build/coveralls.json

.PHONY: install
install:
	cmake --build build/ --target install

.PHONY: jupyter
jupyter:
	docker build -f examples/notebook/Dockerfile -t xeus . && docker run --rm -p 8888:8888 -v.:/mnt/fuzzylite -it xeus jupyter notebook --allow-root --ip 0.0.0.0


CLANG_FORMAT=clang-format --style=file -i

.PHONY: format
format:
	# headers
	find fuzzylite -type f -name '*.h' -print0 | xargs -0 $(CLANG_FORMAT)

	# sources
	find src -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT)

	# tests
	find test -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT)

.PHONY: lint
lint:
	# headers
	find fuzzylite -type f -name '*.h' -print0 | xargs -0 $(CLANG_FORMAT) --dry-run --Werror

	# sources
	find src -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT) --dry-run --Werror

	# tests
	find test -type f -name '*.cpp' -print0 | xargs -0 $(CLANG_FORMAT) --dry-run --Werror

.PHONY: clean
clean:
	rm -rf build
