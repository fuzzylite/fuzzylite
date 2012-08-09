#include "FuzzyExceptions.h"

namespace fuzzy_lite {

    OutOfRangeException::OutOfRangeException(const std::string& file, int line,
            const std::string& function, const std::string& message)
    : FuzzyException(file, line, function, message) {

    }

    OutOfRangeException::OutOfRangeException(const std::string& file, int line,
            const std::string& function, const std::string& message,
            const FuzzyException& previous_exception)
    : FuzzyException(file, line, function, message, previous_exception) {

    }

    OutOfRangeException::~OutOfRangeException() throw () {

    }

    std::string OutOfRangeException::getName() const throw () {
        return "Out Of Range";
    }

    void OutOfRangeException::CheckArray(const std::string& file, int line, const std::string& function,
            int index, int max) {
        if (index < 0 || index >= max) {
            std::stringstream ss;
            ss << "index=" << index << " and maximum value is " << max - 1;
            throw OutOfRangeException(file, line, function, ss.str());
        }
    }
}

namespace fuzzy_lite {

    InvalidArgumentException::InvalidArgumentException(const std::string& file,
            int line, const std::string& function, const std::string& message)
    : FuzzyException(file, line, function, message) {

    }

    InvalidArgumentException::InvalidArgumentException(const std::string& file,
            int line, const std::string& function, const std::string& message,
            const FuzzyException& previous_exception)
    : FuzzyException(file, line, function, message, previous_exception) {

    }

    InvalidArgumentException::~InvalidArgumentException() throw () {

    }

    std::string InvalidArgumentException::getName() const throw () {
        return "Invalid Argument";
    }

}

namespace fuzzy_lite {

    NullPointerException::NullPointerException(const std::string& file, int line,
            const std::string& function, const std::string& message)
    : FuzzyException(file, line, function, message) {

    }

    NullPointerException::NullPointerException(const std::string& file, int line,
            const std::string& function, const std::string& message,
            const FuzzyException& previous_exception)
    : FuzzyException(file, line, function, message, previous_exception) {

    }

    NullPointerException::~NullPointerException() throw () {

    }

    std::string NullPointerException::getName() const throw () {
        return "Null Pointer";
    }
}


namespace fuzzy_lite {

    RuleParsingException::RuleParsingException(const std::string& file, int line,
            const std::string& function, const std::string& message)
    : FuzzyException(file, line, function, message) {

    }

    RuleParsingException::RuleParsingException(const std::string& file, int line,
            const std::string& function, const std::string& message,
            const FuzzyException& previous_exception)
    : FuzzyException(file, line, function, message, previous_exception) {

    }

    RuleParsingException::~RuleParsingException() throw () {

    }

    std::string RuleParsingException::getName() const throw () {
        return "Rule Error";
    }

}

