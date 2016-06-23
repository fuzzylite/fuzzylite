
#define CATCH_CONFIG_RUNNER

#include "test/catch.hpp"

#include "fl/Headers.h"

int main(int argc, char** argv) {

    // global setup...
    fl::fuzzylite::setDebugging(false);
    fl::fuzzylite::setLogging(true);

    int result = Catch::Session().run(argc, argv);

    // global clean-up...

    return result;
}
