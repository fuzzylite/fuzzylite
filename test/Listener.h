#ifndef FL_TEST_LISTENER_H
#define FL_TEST_LISTENER_H

#include <catch2/catch_all.hpp>
#include <iostream>

/**
 * Adjusted from:
 * https://github.com/catchorg/Catch2/blob/fa43b77429ba76c462b1898d6cd2f2d7a9416b14/examples/210-Evt-EventListeners.cpp#L314
 */
namespace fuzzylite { namespace test {
    std::string ws(const int level) {
        return std::string(2 * level, ' ');
    }

    std::ostream& operator<<(std::ostream& out, Catch::Tag t) {
        return out << "original: " << t.original;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
        os << "{ ";
        for (const auto& x : v)
            os << x << ", ";
        return os << "}";
    }

    // struct SourceLineInfo {
    //     char const* file;
    //     std::size_t line;
    // };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::SourceLineInfo& info) {
        os << ws(level) << title << ":\n"
           << ws(level + 1) << "- file: " << info.file << "\n"
           << ws(level + 1) << "- line: " << info.line << "\n";
    }

    // struct MessageInfo {
    //     std::string macroName;
    //     std::string message;
    //     SourceLineInfo lineInfo;
    //     ResultWas::OfType type;
    //     unsigned int sequence;
    // };

    void print(std::ostream& os, const int level, const Catch::MessageInfo& info) {
        os << ws(level + 1) << "- macroName: '" << info.macroName << "'\n"
           << ws(level + 1) << "- message '" << info.message << "'\n";
        print(os, level + 1, "- lineInfo", info.lineInfo);
        os << ws(level + 1) << "- sequence " << info.sequence << "\n";
    }

    void print(std::ostream& os, const int level, const std::string& title, const std::vector<Catch::MessageInfo>& v) {
        os << ws(level) << title << ":\n";
        for (const auto& x : v) {
            os << ws(level + 1) << "{\n";
            print(os, level + 2, x);
            os << ws(level + 1) << "}\n";
        }
        //    os << ws(level+1) << "\n";
    }

    // struct TestRunInfo {
    //     std::string name;
    // };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::TestRunInfo& info) {
        os << ws(level) << title << ":\n" << ws(level + 1) << "- name: " << info.name << "\n";
    }

    // struct Counts {
    //     std::size_t total() const;
    //     bool allPassed() const;
    //     bool allOk() const;
    //
    //     std::size_t passed = 0;
    //     std::size_t failed = 0;
    //     std::size_t failedButOk = 0;
    // };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::Counts& info) {
        os << ws(level) << title << ":\n"
           << ws(level + 1) << "- total(): " << info.total() << "\n"
           << ws(level + 1) << "- allPassed(): " << info.allPassed() << "\n"
           << ws(level + 1) << "- allOk(): " << info.allOk() << "\n"
           << ws(level + 1) << "- passed: " << info.passed << "\n"
           << ws(level + 1) << "- failed: " << info.failed << "\n"
           << ws(level + 1) << "- failedButOk: " << info.failedButOk << "\n";
    }

    // struct Totals {
    //     Counts assertions;
    //     Counts testCases;
    // };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::Totals& info) {
        os << ws(level) << title << ":\n";
        print(os, level + 1, "- assertions", info.assertions);
        print(os, level + 1, "- testCases", info.testCases);
    }

    // struct TestRunStats {
    //     TestRunInfo runInfo;
    //     Totals totals;
    //     bool aborting;
    // };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::TestRunStats& info) {
        os << ws(level) << title << ":\n";
        print(os, level + 1, "- runInfo", info.runInfo);
        print(os, level + 1, "- totals", info.totals);
        os << ws(level + 1) << "- aborting: " << info.aborting << "\n";
    }

    //    struct Tag {
    //        StringRef original, lowerCased;
    //    };
    //
    //
    //    enum class TestCaseProperties : uint8_t {
    //        None = 0,
    //        IsHidden = 1 << 1,
    //        ShouldFail = 1 << 2,
    //        MayFail = 1 << 3,
    //        Throws = 1 << 4,
    //        NonPortable = 1 << 5,
    //        Benchmark = 1 << 6
    //    };
    //
    //
    //    struct TestCaseInfo : NonCopyable {
    //
    //        bool isHidden() const;
    //        bool throws() const;
    //        bool okToFail() const;
    //        bool expectedToFail() const;
    //
    //
    //        std::string name;
    //        std::string className;
    //        std::vector<Tag> tags;
    //        SourceLineInfo lineInfo;
    //        TestCaseProperties properties = TestCaseProperties::None;
    //    };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::TestCaseInfo& info) {
        os << ws(level) << title << ":\n"
           << ws(level + 1) << "- isHidden(): " << info.isHidden() << "\n"
           << ws(level + 1) << "- throws(): " << info.throws() << "\n"
           << ws(level + 1) << "- okToFail(): " << info.okToFail() << "\n"
           << ws(level + 1) << "- expectedToFail(): " << info.expectedToFail() << "\n"
           << ws(level + 1) << "- tagsAsString(): '" << info.tagsAsString() << "'\n"
           << ws(level + 1) << "- name: '" << info.name << "'\n"
           << ws(level + 1) << "- className: '" << info.className << "'\n"
           << ws(level + 1) << "- tags: " << info.tags << "\n";
        print(os, level + 1, "- lineInfo", info.lineInfo);
        os << ws(level + 1) << "- properties (flags): 0x" << std::hex << static_cast<uint32_t>(info.properties)
           << std::dec << "\n";
    }

    // struct TestCaseStats {
    //     TestCaseInfo testInfo;
    //     Totals totals;
    //     std::string stdOut;
    //     std::string stdErr;
    //     bool aborting;
    // };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::TestCaseStats& info) {
        os << ws(level) << title << ":\n";
        // print( os, level+1 , "- testInfo", *info.testInfo );
        print(os, level + 1, "- totals", info.totals);
        os << ws(level + 1) << "- stdOut: " << info.stdOut << "\n"
           << ws(level + 1) << "- stdErr: " << info.stdErr << "\n"
           << ws(level + 1) << "- aborting: " << info.aborting << "\n";
    }

    // struct SectionInfo {
    //     std::string name;
    //     std::string description;
    //     SourceLineInfo lineInfo;
    // };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::SectionInfo& info) {
        os << ws(level) << title << ":\n" << ws(level + 1) << "- name: " << info.name << "\n";
        print(os, level + 1, "- lineInfo", info.lineInfo);
    }

    // struct SectionStats {
    //     SectionInfo sectionInfo;
    //     Counts assertions;
    //     double durationInSeconds;
    //     bool missingAssertions;
    // };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::SectionStats& info) {
        os << ws(level) << title << ":\n";
        print(os, level + 1, "- sectionInfo", info.sectionInfo);
        print(os, level + 1, "- assertions", info.assertions);
        os << ws(level + 1) << "- durationInSeconds: " << info.durationInSeconds << "\n"
           << ws(level + 1) << "- missingAssertions: " << info.missingAssertions << "\n";
    }

    // struct AssertionInfo
    // {
    //     StringRef macroName;
    //     SourceLineInfo lineInfo;
    //     StringRef capturedExpression;
    //     ResultDisposition::Flags resultDisposition;
    // };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::AssertionInfo& info) {
        os << ws(level) << title << ":\n" << ws(level + 1) << "- macroName: '" << info.macroName << "'\n";
        print(os, level + 1, "- lineInfo", info.lineInfo);
        os << ws(level + 1) << "- capturedExpression: '" << info.capturedExpression << "'\n"
           << ws(level + 1) << "- resultDisposition (flags): 0x" << std::hex << info.resultDisposition << std::dec
           << "\n";
    }

    // struct AssertionResultData
    //{
    //     std::string reconstructExpression() const;
    //
    //     std::string message;
    //     mutable std::string reconstructedExpression;
    //     LazyExpression lazyExpression;
    //     ResultWas::OfType resultType;
    // };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::AssertionResultData& info) {
        os << ws(level) << title << ":\n"
           << ws(level + 1) << "- reconstructExpression(): '" << info.reconstructExpression() << "'\n"
           << ws(level + 1) << "- message: '" << info.message << "'\n"
           << ws(level + 1) << "- lazyExpression: '"
           << "(info.lazyExpression)"
           << "'\n"
           << ws(level + 1) << "- resultType: '" << info.resultType << "'\n";
    }

    // class AssertionResult {
    //     bool isOk() const;
    //     bool succeeded() const;
    //     ResultWas::OfType getResultType() const;
    //     bool hasExpression() const;
    //     bool hasMessage() const;
    //     std::string getExpression() const;
    //     std::string getExpressionInMacro() const;
    //     bool hasExpandedExpression() const;
    //     std::string getExpandedExpression() const;
    //     std::string getMessage() const;
    //     SourceLineInfo getSourceInfo() const;
    //     std::string getTestMacroName() const;
    //
    //     AssertionInfo m_info;
    //     AssertionResultData m_resultData;
    // };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::AssertionResult& info) {
        os << ws(level) << title << ":\n"
           << ws(level + 1) << "- isOk(): " << info.isOk() << "\n"
           << ws(level + 1) << "- succeeded(): " << info.succeeded() << "\n"
           << ws(level + 1) << "- getResultType(): " << info.getResultType() << "\n"
           << ws(level + 1) << "- hasExpression(): " << info.hasExpression() << "\n"
           << ws(level + 1) << "- hasMessage(): " << info.hasMessage() << "\n"
           << ws(level + 1) << "- getExpression(): '" << info.getExpression() << "'\n"
           << ws(level + 1) << "- getExpressionInMacro(): '" << info.getExpressionInMacro() << "'\n"
           << ws(level + 1) << "- hasExpandedExpression(): " << info.hasExpandedExpression() << "\n"
           << ws(level + 1) << "- getExpandedExpression(): " << info.getExpandedExpression() << "'\n"
           << ws(level + 1) << "- getMessage(): '" << info.getMessage() << "'\n";
        print(os, level + 1, "- getSourceInfo(): ", info.getSourceInfo());
        os << ws(level + 1) << "- getTestMacroName(): '" << info.getTestMacroName() << "'\n";

        print(os, level + 1, "- *** m_info (AssertionInfo)", info.m_info);
        print(os, level + 1, "- *** m_resultData (AssertionResultData)", info.m_resultData);
    }

    // struct AssertionStats {
    //     AssertionResult assertionResult;
    //     std::vector<MessageInfo> infoMessages;
    //     Totals totals;
    // };

    void print(std::ostream& os, const int level, const std::string& title, const Catch::AssertionStats& info) {
        os << ws(level) << title << ":\n";
        print(os, level + 1, "- assertionResult", info.assertionResult);
        print(os, level + 1, "- infoMessages", info.infoMessages);
        print(os, level + 1, "- totals", info.totals);
    }

    const char* dashed_line = "--------------------------------------------------------------------------";

    struct Listener : Catch::EventListenerBase {
        using EventListenerBase::EventListenerBase;  // inherit constructor

        // Get rid of Wweak-tables
        ~Listener() override = default;

        // The whole test run starting
        void testRunStarting(const Catch::TestRunInfo& testRunInfo) override {
            std::cout << std::boolalpha << "\nEvent: testRunStarting:\n";
            print(std::cout, 1, "- testRunInfo", testRunInfo);
        }

        // The whole test run ending
        void testRunEnded(const Catch::TestRunStats& testRunStats) override {
            std::cout << dashed_line << "\nEvent: testRunEnded:\n";
            print(std::cout, 1, "- testRunStats", testRunStats);
        }

        // A test is being skipped (because it is "hidden")
        void skipTest(const Catch::TestCaseInfo& testInfo) override {
            std::cout << dashed_line << "\nEvent: skipTest:\n";
            print(std::cout, 1, "- testInfo", testInfo);
        }

        // Test cases starting
        void testCaseStarting(const Catch::TestCaseInfo& testInfo) override {
            std::cout << dashed_line << "\nEvent: testCaseStarting:\n";
            print(std::cout, 1, "- testInfo", testInfo);
        }

        // Test cases ending
        void testCaseEnded(const Catch::TestCaseStats& testCaseStats) override {
            std::cout << "\nEvent: testCaseEnded:\n";
            print(std::cout, 1, "testCaseStats", testCaseStats);
        }

        // Sections starting
        void sectionStarting(const Catch::SectionInfo& sectionInfo) override {
            std::cout << "\nEvent: sectionStarting:\n";
            print(std::cout, 1, "- sectionInfo", sectionInfo);
        }

        // Sections ending
        void sectionEnded(const Catch::SectionStats& sectionStats) override {
            std::cout << "\nEvent: sectionEnded:\n";
            print(std::cout, 1, "- sectionStats", sectionStats);
        }

        // Assertions before/ after
        void assertionStarting(const Catch::AssertionInfo& assertionInfo) override {
            (void)assertionInfo;
            // std::cout << "\nEvent: assertionStarting:\n";
            // print( std::cout, 1, "- assertionInfo", assertionInfo );
        }

        void assertionEnded(const Catch::AssertionStats& assertionStats) override {
            (void)assertionStats;
            // std::cout << "\nEvent: assertionEnded:\n";
            // print( std::cout, 1, "- assertionStats", assertionStats );
        }
    };

}}
#endif
