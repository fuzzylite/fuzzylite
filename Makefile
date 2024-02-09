BUILD = release
CPP98 = OFF
FLOAT = OFF

.PHONY: configure make test install format lint

all: configure make test

configure:
	cmake -B build/ -DCMAKE_BUILD_TYPE=$(BUILD) -DFL_CPP98=$(CPP98) -DFL_USE_FLOAT=$(FLOAT) .

make:
	cmake --build build/ --parallel

test:
	ctest --test-dir build/

coverage:
	LLVM_PROFILE_FILE="build/default.profraw" ./build/bin/fuzzylite-tests
	xcrun llvm-profdata merge -sparse build/default.profraw -o build/fuzzylite.profdata
	xcrun llvm-cov report build/bin/fuzzylite-tests -instr-profile=build/fuzzylite.profdata --ignore-filename-regex="test/.*"
	xcrun llvm-cov show -format=html build/bin/fuzzylite-tests -instr-profile=build/fuzzylite.profdata -output-dir=/tmp --ignore-filename-regex="test/.*"

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
