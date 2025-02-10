#include <catch2/catch_all.hpp>
#include <iostream>
#include <vector>

#include "Assert.h"

struct Term {
    std::string name;

    std::string str() const {
        return "Term(" + name + ")";
    }

    explicit Term(const std::string& name) : name(name) {
        std::cout << "Term(const std::string&): " << str() << std::endl;
    }

    ~Term() {
        std::cout << "~" << str() << std::endl;
    }

    Term(const Term& other) : name(other.name) {
        std::cout << "Term(const Term&): " << str() << std::endl;
    }

    Term& operator=(const Term& other) {
        if (this != &other)
            this->name = other.name;
        std::cout << "= (const Term&): " << str() << std::endl;
        return *this;
    }

    Term(Term&& other) noexcept : name(std::move(other.name)) {
        std::cout << "Term(Term&&): " << str() << std::endl;
    }

    Term& operator=(Term&& other) noexcept {
        if (this != &other)
            this->name = std::move(other.name);
        std::cout << "= (Term&&): " << str() << std::endl;
        return *this;
    }
};

struct Variable {
    std::string name;
    std::vector<Term*> terms;

    std::string str() const {
        std::ostringstream ss;
        ss << "Variable(" << name << "): {";
        for (const auto* term : terms)
            ss << term->str() << ", ";
        ss << "}";
        return ss.str();
    }

    Variable() {}

    Variable(const std::string& name, const std::vector<Term*>& terms) : name(name), terms(terms) {
        std::cout << "Variable(name, terms): " << str() << std::endl;
    }

    ~Variable() {
        std::cout << "Destroying: " << str() << std::endl;
        for (const auto* term : terms)
            delete term;
        terms.clear();
        std::cout << "~Variable(): " << str() << std::endl;
    }

    Variable(const Variable& other) : name(other.name) {
        copyFrom(other);
        std::cout << "Variable(const Variable&): " << str() << std::endl;
    }

    void copyFrom(const Variable& other) {
        if (this != &other) {
            name = other.name;
            for (const auto* term : terms)
                delete term;
            terms.clear();

            terms.reserve(other.terms.size());
            for (const auto* term : other.terms) {
                Term* copy = new Term(*term);
                terms.emplace_back(copy);
            }
        }
    }

    Variable& operator=(const Variable& other) {
        if (this != &other)
            copyFrom(other);
        std::cout << "=(const Variable&): " << str() << std::endl;
        return *this;
    }

    Variable(Variable&& other) noexcept : name(std::move(other.name)), terms(std::move(other.terms)) {
        std::cout << "Variable(Variable&&): " << str() << std::endl;
    }

    Variable& operator=(Variable&& other) noexcept {
        if (this != &other) {
            name = std::move(other.name);
            for (auto* term : terms)
                delete term;
            terms = std::move(other.terms);
        }
        std::cout << "=(Variable&&): " << str() << std::endl;
        return *this;
    }
};

namespace fuzzylite { namespace test {
    TEST_CASE("Constructors", "[tests][constructor]") {
        std::cout << "================" << std::endl;
        std::cout << "Constructor" << std::endl;
        std::cout << "----------------" << std::endl;
        Variable variable{"variable", {new Term{"term 1"}, new Term{"term 2"}}};
    }

    TEST_CASE("Copy Constructors", "[tests][constructor]") {
        std::cout << "================" << std::endl;
        std::cout << "Copy Constructor" << std::endl;
        std::cout << "----------------" << std::endl;
        Variable variable{"variable", {new Term{"term 1"}, new Term{"term 2"}}};
        std::cout << "Copying..." << std::endl;
        Variable copy(variable);
        copy.name = "copy";
        std::cout << "Copied" << std::endl;

        AssertConstructor().can_copy_construct(variable, &Variable::str);
    }

    TEST_CASE("Copy Assignment", "[tests][constructor]") {
        std::cout << "================" << std::endl;
        std::cout << "Copy Assignment" << std::endl;
        std::cout << "----------------" << std::endl;
        Variable variable{"variable", {new Term{"term 1"}, new Term{"term 2"}}};
        std::cout << "Copying..." << std::endl;
        Variable copy;
        copy = variable;
        copy.name = "copy";
        std::cout << "Copied" << std::endl;
        AssertConstructor().can_copy_assign(variable, &Variable::str);
    }

    TEST_CASE("Move Constructors", "[tests][constructor]") {
        std::cout << "================" << std::endl;
        std::cout << "Move Constructor" << std::endl;
        std::cout << "----------------" << std::endl;
        Variable variable{"variable", {new Term{"term 1"}, new Term{"term 2"}}};
        std::cout << "Moving..." << std::endl;
        Variable copy(std::move(variable));
        copy.name = "move";
        std::cout << "Moved" << std::endl;
        AssertConstructor().can_move_construct(copy, &Variable::str);
    }

    TEST_CASE("Move Assignment", "[tests][constructor]") {
        std::cout << "================" << std::endl;
        std::cout << "Move Assignment" << std::endl;
        std::cout << "----------------" << std::endl;
        std::cout << "Copying..." << std::endl;
        Variable variable{"variable", {new Term{"term 1"}, new Term{"term 2"}}};
        Variable copy;
        copy = std::move(variable);
        copy.name = "move";
        std::cout << "Moved" << std::endl;
        AssertConstructor().can_move_assign(copy, &Variable::str);
    }
}}
