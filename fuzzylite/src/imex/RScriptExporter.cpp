/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/imex/RScriptExporter.h"

#include "fl/Engine.h"
#include "fl/imex/FllExporter.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"

#include <fstream>
namespace fl {

    RScriptExporter::RScriptExporter() : Exporter(),
    _minimumColor("yellow"), _maximumColor("red"), _contourColor("black") { }

    RScriptExporter::~RScriptExporter() { }

    std::string RScriptExporter::name() const {
        return "RScriptExporter";
    }

    void RScriptExporter::setMinimumColor(const std::string& minimumColor) {
        this->_minimumColor = minimumColor;
    }

    std::string RScriptExporter::getMinimumColor() const {
        return this->_minimumColor;
    }

    void RScriptExporter::setMaximumColor(const std::string& maximumColor) {
        this->_maximumColor = maximumColor;
    }

    std::string RScriptExporter::getMaximumColor() const {
        return _maximumColor;
    }

    void RScriptExporter::setContourColor(const std::string& contourColor) {
        this->_contourColor = contourColor;
    }

    std::string RScriptExporter::getContourColor() const {
        return this->_contourColor;
    }

    RScriptExporter* RScriptExporter::clone() const {
        return new RScriptExporter(*this);
    }

    std::string RScriptExporter::toString(const Engine* engine) const {
        if (engine->inputVariables().empty()) {
            throw Exception("[exporter error] engine has no input variables to export the surface", FL_AT);
        }
        if (engine->outputVariables().empty()) {
            throw Exception("[exporter error] engine has no output variables to export the surface", FL_AT);
        }
        InputVariable* a = engine->inputVariables().at(0);
        InputVariable* b = engine->inputVariables().at(1 % engine->numberOfInputVariables());
        return toString(const_cast<Engine*> (engine), a, b,
                1024, FldExporter::AllVariables, engine->outputVariables());
    }

    std::string RScriptExporter::toString(Engine* engine, InputVariable* a, InputVariable* b,
            int values, FldExporter::ScopeOfValues scope,
            const std::vector<OutputVariable*>& outputVariables) const {
        std::ostringstream writer;
        writeScriptExportingDataFrame(engine, writer, a, b, values, scope, outputVariables);
        return writer.str();
    }

    std::string RScriptExporter::toString(Engine* engine, InputVariable* a, InputVariable* b,
            std::istream& reader, const std::vector<OutputVariable*>& outputVariables) const {
        std::ostringstream writer;
        writeScriptExportingDataFrame(engine, writer, a, b, reader, outputVariables);
        return writer.str();
    }

    void RScriptExporter::toFile(const std::string& filePath, const Engine* engine) const {
        if (engine->inputVariables().empty()) {
            throw Exception("[exporter error] engine has no input variables to export the surface", FL_AT);
        }
        if (engine->outputVariables().empty()) {
            throw Exception("[exporter error] engine has no output variables to export the surface", FL_AT);
        }
        InputVariable* a = engine->inputVariables().at(0);
        InputVariable* b = engine->inputVariables().at(1 % engine->numberOfInputVariables());

        toFile(filePath, const_cast<Engine*> (engine), a, b,
                1024, FldExporter::AllVariables, engine->outputVariables());
    }

    void RScriptExporter::toFile(const std::string& filePath, Engine* engine,
            InputVariable* a, InputVariable* b, int values, FldExporter::ScopeOfValues scope,
            const std::vector<OutputVariable*>& outputVariables) const {
        std::ofstream writer(filePath.c_str());
        if (not writer.is_open()) {
            throw Exception("[file error] file <" + filePath + "> could not be created", FL_AT);
        }
        writeScriptExportingDataFrame(engine, writer, a, b, values, scope, outputVariables);
        writer.close();
    }

    void RScriptExporter::toFile(const std::string& filePath, Engine* engine,
            InputVariable* a, InputVariable* b, std::istream& reader,
            const std::vector<OutputVariable*>& outputVariables) const {
        std::ofstream writer(filePath.c_str());
        if (not writer.is_open()) {
            throw Exception("[file error] file <" + filePath + "> could not be created", FL_AT);
        }
        writeScriptExportingDataFrame(engine, writer, a, b, reader, outputVariables);
        writer.close();
    }

    void RScriptExporter::writeScriptImportingDataFrame(const Engine* engine, std::ostream& writer,
            InputVariable* a, InputVariable* b, const std::string& dfPath,
            const std::vector<OutputVariable*>& outputVariables) const {
        writeScriptHeader(writer, engine);

        writer << "engine.fldFile = \"" << dfPath << "\"\n";
        writer << "if (require(data.table)) {\n"
                << "    engine.df = data.table::fread(engine.fldFile, sep=\"auto\", header=\"auto\")\n"
                << "} else {\n"
                << "    engine.df = read.table(engine.fldFile, header=TRUE)\n"
                << "}\n";
        writer << "\n";

        writeScriptPlots(writer, a, b, outputVariables);
    }

    void RScriptExporter::writeScriptExportingDataFrame(Engine* engine, std::ostream& writer,
            InputVariable* a, InputVariable* b, int values, FldExporter::ScopeOfValues scope,
            const std::vector<OutputVariable*>& outputVariables) const {
        writeScriptHeader(writer, engine);

        std::vector<InputVariable*> activeVariables = engine->inputVariables();
        for (std::size_t i = 0; i < activeVariables.size(); ++i) {
            if (activeVariables.at(i) != a and activeVariables.at(i) != b) {
                activeVariables.at(i) = fl::null;
            }
        }
        writer << "engine.fld = \"";
        FldExporter().write(engine, writer, values, scope, activeVariables);
        writer << "\"\n\n";
        writer << "engine.df = read.delim(textConnection(engine.fld), header=TRUE, "
                "sep=\" \", strip.white=TRUE)\n\n";

        writeScriptPlots(writer, a, b, outputVariables);
    }

    void RScriptExporter::writeScriptExportingDataFrame(Engine* engine, std::ostream& writer,
            InputVariable* a, InputVariable* b, std::istream& reader,
            const std::vector<OutputVariable*>& outputVariables) const {
        writeScriptHeader(writer, engine);

        writer << "engine.fld = \"";
        FldExporter().write(engine, writer, reader);
        writer << "\"\n\n";

        writer << "engine.df = read.delim(textConnection(engine.fld), header=TRUE, "
                "sep=\" \", strip.white=TRUE)\n\n";

        writeScriptPlots(writer, a, b, outputVariables);
    }

    void RScriptExporter::writeScriptHeader(std::ostream& writer, const Engine* engine) const {
        writer << "#Code automatically generated with " << fuzzylite::library() << ".\n\n"
                << "library(ggplot2);\n"
                << "\n";
        writer << "engine.name = \"" << engine->getName() << "\"\n";
        if (not engine->getDescription().empty())
            writer << "engine.description = \"" << engine->getDescription() << "\"\n";
        writer << "engine.fll = \"" << FllExporter().toString(engine) << "\"\n\n";
    }

    void RScriptExporter::writeScriptPlots(std::ostream& writer,
            InputVariable* a, InputVariable* b,
            const std::vector<OutputVariable*>& outputVariables) const {
        std::ostringstream arrangeGrob;
        arrangeGrob << "arrangeGrob(";
        for (std::size_t i = 0; i < outputVariables.size(); ++i) {
            OutputVariable* z = outputVariables.at(i);
            if (a != b) {
                writer << "engine.plot.i1i2_o" << (i + 1) << " = ggplot(engine.df, aes(" << a->getName() << ", " << b->getName() << ")) + \n"
                        << "    geom_tile(aes(fill=" << z->getName() << ")) + \n"
                        << "    scale_fill_gradient(low=\"" << _minimumColor << "\", high=\"" << _maximumColor << "\") + \n"
                        << "    stat_contour(aes(x=" << a->getName() << ", y=" << b->getName() << ", z=" << z->getName() << "), color=\"" << _contourColor << "\") + \n"
                        << "    ggtitle(\"(" << a->getName() << ", " << b->getName() << ") = " << z->getName() << "\")\n\n";

                writer << "engine.plot.i2i1_o" << (i + 1) << " = ggplot(engine.df, aes(" << b->getName() << ", " << a->getName() << ")) + \n"
                        << "    geom_tile(aes(fill=" << z->getName() << ")) + \n"
                        << "    scale_fill_gradient(low=\"" << _minimumColor << "\", high=\"" << _maximumColor << "\") + \n"
                        << "    stat_contour(aes(x=" << b->getName() << ", y=" << a->getName() << ", z=" << z->getName() << "), color=\"" << _contourColor << "\") + \n"
                        << "    ggtitle(\"(" << b->getName() << ", " << a->getName() << ") = " << z->getName() << "\")\n\n";
                arrangeGrob << "engine.plot.i1i2_o" << (i + 1) << ", " << "engine.plot.i2i1_o" << (i + 1) << ", ";
            } else {
                writer << "engine.plot.i1_o" << (i + 1) << " = ggplot(engine.df, aes(" << a->getName() << ", " << z->getName() << ")) + \n"
                        << "    geom_line(aes(color=" << z->getName() << "), size=3, lineend=\"round\", linejoin=\"mitre\") + \n"
                        << "    scale_color_gradient(low=\"" << _minimumColor << "\", high=\"" << _maximumColor << "\") + \n"
                        << "    ggtitle(\"" << a->getName() << " vs " << z->getName() << "\")\n\n";

                writer << "engine.plot.o" << (i + 1) << "_i1 = ggplot(engine.df, aes(" << a->getName() << ", " << z->getName() << ")) + \n"
                        << "    geom_line(aes(color=" << z->getName() << "), size=3, lineend=\"round\", linejoin=\"mitre\") + \n"
                        << "    scale_color_gradient(low=\"" << _minimumColor << "\", high=\"" << _maximumColor << "\") + \n"
                        << "    coord_flip() + \n"
                        << "    ggtitle(\"" << z->getName() << " vs " << a->getName() << "\")\n\n";
                arrangeGrob << "engine.plot.i1_o" << (i + 1) << ", " << "engine.plot.o" << (i + 1) << "_i1, ";
            }

        }
        arrangeGrob << "ncol=2, top=engine.name)";
        writer << "if (require(gridExtra)) {\n"
                << "    engine.plots = " << arrangeGrob.str() << "\n"
                << "    ggsave(paste0(engine.name, \".pdf\"), engine.plots)\n"
                << "    if (require(grid)) {\n"
                << "        grid.newpage()\n"
                << "        grid.draw(engine.plots)\n"
                << "    }\n"
                << "}\n";
    }
}
