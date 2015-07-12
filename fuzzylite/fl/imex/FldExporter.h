/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#ifndef FL_FLDEXPORTER_H
#define FL_FLDEXPORTER_H

#include "fl/imex/Exporter.h"

#include <vector>

namespace fl {
    class Engine;
    class InputVariable;
    class OutputVariable;

    class FL_API FldExporter : public Exporter {
    private:
        std::string _separator;
        bool _exportHeaders;
        bool _exportInputValues;
        bool _exportOutputValues;
    public:
        explicit FldExporter(const std::string& separator = " ");
        virtual ~FldExporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(FldExporter)

        virtual std::string name() const FL_IOVERRIDE;

        virtual void setSeparator(const std::string& separator);
        virtual std::string getSeparator() const;

        virtual void setExportHeader(bool exportHeaders);
        virtual bool exportsHeader() const;

        virtual void setExportInputValues(bool exportInputValues);
        virtual bool exportsInputValues() const;

        virtual void setExportOutputValues(bool exportOutputValues);
        virtual bool exportsOutputValues() const;

        virtual std::string header(const Engine* engine) const;

        //WARNING: The engine will be const_casted in order to be processed!
        virtual std::string toString(const Engine* engine) const FL_IOVERRIDE;
        virtual std::string toString(Engine* engine, int maximumNumberOfResults) const;
        virtual std::string toString(Engine* engine, const std::string& inputData) const;

        using Exporter::toFile;
        virtual void toFile(const std::string& path, Engine* engine, int maximumNumberOfResults) const;
        virtual void toFile(const std::string& path, Engine* engine, const std::string& inputData) const;

        virtual std::vector<scalar> parse(const std::string& x) const;

        void write(Engine* engine, std::ostream& writer, int maximumNumberOfResults) const;
        void write(Engine* engine, std::ostream& writer, std::istream& reader) const;
        void write(Engine* engine, std::ostream& writer, const std::vector<scalar>& inputValues) const;

        virtual FldExporter* clone() const FL_IOVERRIDE;
    };
}

#endif  /* FL_FLDEXPORTER_H */

