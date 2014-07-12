/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright (C) 2010-2014 FuzzyLite Limited
 All rights reserved

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 fuzzylite is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * File:   FldExporter.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 9:52 AM
 */

#ifndef FL_FLDEXPORTER_H
#define FL_FLDEXPORTER_H

#include "fl/imex/Exporter.h"

#include <vector>

namespace fl {
    class Engine;
    class InputVariable;
    class OutputVariable;

    class FL_EXPORT FldExporter : public Exporter {
    protected:
        std::string _separator;
        bool _exportHeaders;
        bool _exportInputValues;
        bool _exportOutputValues;
    public:
        FldExporter(const std::string& separator = " ");
        virtual ~FldExporter();

        virtual std::string name() const;

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
        virtual std::string toString(const Engine* engine) const;
        virtual std::string toString(Engine* engine, int maximumNumberOfResults) const;
        virtual std::string toString(Engine* engine, const std::string& inputData) const;

        virtual void toFile(const std::string& path, Engine* engine, int maximumNumberOfResults) const;
        virtual void toFile(const std::string& path, Engine* engine, const std::string& inputData) const;

        virtual std::vector<scalar> parse(const std::string& x) const;

        void write(Engine* engine, std::ostream& writer, int maximumNumberOfResults) const;
        void write(Engine* engine, std::ostream& writer, std::istream& reader) const;
        void write(Engine* engine, std::ostream& writer, const std::vector<scalar>& inputValues) const;

        virtual FldExporter* clone() const;
    };
}

#endif  /* FL_FLDEXPORTER_H */

