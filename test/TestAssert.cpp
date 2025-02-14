#include <catch2/catch_all.hpp>
#include <iostream>
#include <vector>

#include "Assert.h"

struct TestTerm {
    std::string name;

    std::string str() const {
        return "TestTerm(" + name + ")";
    }

    explicit TestTerm(const std::string& name) : name(name) {
        std::cout << "TestTerm(const std::string&): " << str() << std::endl;
    }

    ~TestTerm() {
        std::cout << "~" << str() << std::endl;
    }

    TestTerm(const TestTerm& other) : name(other.name) {
        std::cout << "TestTerm(const TestTerm&): " << str() << std::endl;
    }

    TestTerm& operator=(const TestTerm& other) {
        if (this != &other)
            this->name = other.name;
        std::cout << "= (const TestTerm&): " << str() << std::endl;
        return *this;
    }

    TestTerm(TestTerm&& other) noexcept : name(std::move(other.name)) {
        std::cout << "TestTerm(TestTerm&&): " << str() << std::endl;
    }

    TestTerm& operator=(TestTerm&& other) noexcept {
        if (this != &other)
            this->name = std::move(other.name);
        std::cout << "= (TestTerm&&): " << str() << std::endl;
        return *this;
    }
};

struct TestVariable {
    std::string name;
    std::vector<TestTerm*> terms;

    std::string str() const {
        std::ostringstream ss;
        ss << "TestVariable(" << name << "): {";
        for (const auto* term : terms)
            ss << term->str() << ", ";
        ss << "}";
        return ss.str();
    }

    TestVariable() {}

    TestVariable(const std::string& name, const std::vector<TestTerm*>& terms) : name(name), terms(terms) {
        std::cout << "TestVariable(name, terms): " << str() << std::endl;
    }

    ~TestVariable() {
        std::cout << "Destroying: " << str() << std::endl;
        for (const auto* term : terms)
            delete term;
        terms.clear();
        std::cout << "~TestVariable(): " << str() << std::endl;
    }

    TestVariable(const TestVariable& other) : name(other.name) {
        copyFrom(other);
        std::cout << "TestVariable(const TestVariable&): " << str() << std::endl;
    }

    void copyFrom(const TestVariable& other) {
        if (this != &other) {
            name = other.name;
            for (const auto* term : terms)
                delete term;
            terms.clear();

            terms.reserve(other.terms.size());
            for (const auto* term : other.terms) {
                TestTerm* copy = new TestTerm(*term);
                terms.emplace_back(copy);
            }
        }
    }

    TestVariable& operator=(const TestVariable& other) {
        if (this != &other)
            copyFrom(other);
        std::cout << "=(const TestVariable&): " << str() << std::endl;
        return *this;
    }

    TestVariable(TestVariable&& other) noexcept : name(std::move(other.name)), terms(std::move(other.terms)) {
        std::cout << "TestVariable(TestVariable&&): " << str() << std::endl;
    }

    TestVariable& operator=(TestVariable&& other) noexcept {
        if (this != &other) {
            name = std::move(other.name);
            for (auto* term : terms)
                delete term;
            terms = std::move(other.terms);
        }
        std::cout << "=(TestVariable&&): " << str() << std::endl;
        return *this;
    }
};

namespace fuzzylite { namespace test {
    TEST_CASE("Constructors", "[tests][constructor]") {
        std::cout << "================" << std::endl;
        std::cout << "Constructor" << std::endl;
        std::cout << "----------------" << std::endl;
        TestVariable variable{"variable", {new TestTerm{"term 1"}, new TestTerm{"term 2"}}};
    }

    TEST_CASE("Copy Constructors", "[tests][constructor]") {
        std::cout << "================" << std::endl;
        std::cout << "Copy Constructor" << std::endl;
        std::cout << "----------------" << std::endl;
        TestVariable variable{"variable", {new TestTerm{"term 1"}, new TestTerm{"term 2"}}};
        std::cout << "Copying..." << std::endl;
        TestVariable copy(variable);
        copy.name = "copy";
        std::cout << "Copied" << std::endl;

        AssertConstructor().can_copy_construct(variable, &TestVariable::str);
    }

    TEST_CASE("Copy Assignment", "[tests][constructor]") {
        std::cout << "================" << std::endl;
        std::cout << "Copy Assignment" << std::endl;
        std::cout << "----------------" << std::endl;
        TestVariable variable{"variable", {new TestTerm{"term 1"}, new TestTerm{"term 2"}}};
        std::cout << "Copying..." << std::endl;
        TestVariable copy;
        copy = variable;
        copy.name = "copy";
        std::cout << "Copied" << std::endl;
        AssertConstructor().can_copy_assign(variable, &TestVariable::str);
    }

    TEST_CASE("Move Constructors", "[tests][constructor]") {
        std::cout << "================" << std::endl;
        std::cout << "Move Constructor" << std::endl;
        std::cout << "----------------" << std::endl;
        TestVariable variable{"variable", {new TestTerm{"term 1"}, new TestTerm{"term 2"}}};
        std::cout << "Moving..." << std::endl;
        TestVariable copy(std::move(variable));
        copy.name = "move";
        std::cout << "Moved" << std::endl;
        AssertConstructor().can_move_construct(copy, &TestVariable::str);
    }

    TEST_CASE("Move Assignment", "[tests][constructor]") {
        std::cout << "================" << std::endl;
        std::cout << "Move Assignment" << std::endl;
        std::cout << "----------------" << std::endl;
        std::cout << "Copying..." << std::endl;
        TestVariable variable{"variable", {new TestTerm{"term 1"}, new TestTerm{"term 2"}}};
        TestVariable copy;
        copy = std::move(variable);
        copy.name = "move";
        std::cout << "Moved" << std::endl;
        AssertConstructor().can_move_assign(copy, &TestVariable::str);
    }
}}
