BUILD = release
CPP98 = OFF
FLOAT = OFF
COVERAGE = ON

.PHONY: configure make test install format lint

all: configure make test

configure:
	cmake -B build/ -DCMAKE_BUILD_TYPE=$(BUILD) -DFL_CPP98=$(CPP98) -DFL_USE_FLOAT=$(FLOAT) -DFL_BUILD_COVERAGE=$(COVERAGE) .

make:
	cmake --build build/ --parallel

test:
	ctest --test-dir build/

coverage:
	gcovr -r src/ build/CMakeFiles/fl-coverage.dir/

coverage_coveralls:
	gcovr -r src/ build/CMakeFiles/fl-coverage.dir/ --coveralls build/coveralls.json --verbose

install:
	cmake --build build/ --target install

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

clean:
	rm -rf build
