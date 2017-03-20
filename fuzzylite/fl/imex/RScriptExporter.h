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

#ifndef FL_RSCRIPTEXPORTER_H
#define FL_RSCRIPTEXPORTER_H

#include "fl/imex/Exporter.h"
#include "fl/imex/FldExporter.h"

#include <vector>

namespace fl {
    class Engine;
    class InputVariable;
    class OutputVariable;

    /**
      The RScriptExporter class is an Exporter that creates an R script to plot one or
      more surfaces of an engine for two input variables and any number of output
      variables.

      @author Juan Rada-Vilela, Ph.D.
      @see FldExporter
      @see Exporter
      @since 6.0
     */
    class FL_API RScriptExporter : public Exporter {
    private:
        std::string _minimumColor;
        std::string _maximumColor;
        std::string _contourColor;

    public:
        RScriptExporter();
        virtual ~RScriptExporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(RScriptExporter)

        virtual std::string name() const FL_IOVERRIDE;

        /**
          Sets the color to represent the minimum values.
          @param minimumColor is the color to represent the minimum values
         */
        void setMinimumColor(const std::string& minimumColor);
        /**
          Gets the color to represent the minimum values.
          @return the color to represent the minimum values
         */
        std::string getMinimumColor() const;

        /**
         Sets the color to represent the maximum values.
         @param maximumColor is the color to represent the maximum values
         */
        void setMaximumColor(const std::string& maximumColor);
        /**
         Gets the color to represent the maximum values.
         @return maximumColor is the color to represent the maximum values
         */
        std::string getMaximumColor() const;

        /**
         Sets the color to draw the contour lines
         @param contourColor is the color to draw the contour lines
         */
        void setContourColor(const std::string& contourColor);
        /**
         Gets the color to draw the contour lines
         @return the color to draw the contour lines
         */
        std::string getContourColor() const;

        /**
         Returns an R script plotting multiple surfaces based on a data frame
         generated with 1024 values in the scope of FldExporter::AllVariables
         for the first two input variables.
         @param engine is the engine to export
         @return an R script plotting multiple surfaces for the first two input
         variables in the engine.
         */
        virtual std::string toString(const Engine* engine) const FL_IOVERRIDE;

        /**
         Returns an R script plotting multiple surfaces based on a data frame
         generated with the given number of values and scope for the two input
         variables.
         @param engine is the engine to export
         @param a is the first input variable
         @param b is the second input variable
         @param values is the number of values to evaluate the engine
         @param scope is the scope of the number of values to evaluate the engine
         @param outputVariables are the output variables to create the surface for
         @return an R script plotting multiple surfaces for the two input
         variables on the output variables.
         */
        virtual std::string toString(Engine* engine, InputVariable* a, InputVariable* b,
                int values, FldExporter::ScopeOfValues scope,
                const std::vector<OutputVariable*>& outputVariables) const;

        /**
         Returns an R script plotting multiple surfaces based on the input stream
         of values for the two input variables.
         @param engine is the engine to export
         @param a is the first input variable
         @param b is the second input variable
         @param reader is an input stream of data whose lines contain space-separated
         input values
         @param outputVariables are the output variables to create the surface for
         @return an R script plotting multiple surfaces for the two input
         variables on the output variables
         */
        virtual std::string toString(Engine* engine, InputVariable* a, InputVariable* b,
                std::istream& reader, const std::vector<OutputVariable*>& outputVariables) const;

        /**
         Creates an R script file plotting multiple surfaces based on a data frame
         generated with 1024 values in the scope of FldExporter::AllVariables
         for the two input variables
         @param filePath is the full path of the R script file
         @param engine is the engine to export
         */
        virtual void toFile(const std::string& filePath, const Engine* engine) const FL_IOVERRIDE;

        /**
         Creates an R script file plotting multiple surfaces based on a data frame
         generated with the given number of values and scope for the two input
         variables
         @param filePath is the full path of the R script file
         @param engine is the engine to export
         @param a is the first input variable
         @param b is the second input variable
         @param values is the number of values to evaluate the engine
         @param scope is the scope of the number of values to evaluate the engine
         @param outputVariables are the output variables to create the surface for
         */
        virtual void toFile(const std::string& filePath, Engine* engine,
                InputVariable* a, InputVariable* b,
                int values, FldExporter::ScopeOfValues scope,
                const std::vector<OutputVariable*>& outputVariables) const;

        /**
         Creates an R script file plotting multiple surfaces based on the input stream
         of values for the two input variables.
         @param filePath is the full path of the R script file
         @param engine is the engine to export
         @param a is the first input variable
         @param b is the second input variable
         @param reader is an input stream of data whose lines contain space-separated
         input values
         @param outputVariables are the output variables to create the surface for
         */
        virtual void toFile(const std::string& filePath, Engine* engine,
                InputVariable* a, InputVariable* b, std::istream& reader,
                const std::vector<OutputVariable*>& outputVariables) const;


        /**
         Writes an R script plotting multiple surfaces based on a manually
         imported data frame containing the data for the two input variables
         on the output variables.
         @param engine is the engine to export
         @param writer is the output where the engine will be written to
         @param a is the first input variable
         @param b is the second input variable
         @param dataFramePath is the path where the data frame should be located
         (the path will not be accessed, it will only be written to script)
         @param outputVariables are the output variables to create the surface for
         */
        virtual void writeScriptImportingDataFrame(const Engine* engine, std::ostream& writer,
                InputVariable* a, InputVariable* b, const std::string& dataFramePath,
                const std::vector<OutputVariable*>& outputVariables) const;

        /**
         Writes an R script plotting multiple surfaces based on a data frame
         generated with the given number of values and scope for the two input
         variables on the output variables.
         @param engine is the engine to export
         @param writer is the output where the engine will be written to
         @param a is the first input variable
         @param b is the second input variable
         @param values is the number of values to evaluate the engine
         @param scope is the scope of the number of values to evaluate the engine
         @param outputVariables are the output variables to create the surface for
         */
        virtual void writeScriptExportingDataFrame(Engine* engine, std::ostream& writer,
                InputVariable* a, InputVariable* b, int values, FldExporter::ScopeOfValues scope,
                const std::vector<OutputVariable*>& outputVariables) const;

        /**
         Writes an R script plotting multiple surfaces based on a data frame
         generated with the given number of values and scope for the two input
         variables on the output variables.
         @param engine is the engine to export
         @param writer is the output where the engine will be written to
         @param a is the first input variable
         @param b is the second input variable
         @param reader is an input stream of data whose lines contain space-separated
         input values
         @param outputVariables are the output variables to create the surface for
         */
        virtual void writeScriptExportingDataFrame(Engine* engine, std::ostream& writer,
                InputVariable* a, InputVariable* b, std::istream& reader,
                const std::vector<OutputVariable*>& outputVariables) const;

    protected:
        /**
         Writes the header of the R script (e.g., import libraries)
         @param writer is the output where the header will be written to
         @param engine is the engine to export
         */
        virtual void writeScriptHeader(std::ostream& writer, const Engine* engine) const;

        /**
         Writes the code to generate the surface plots for the input variables
         on the output variables.
         @param writer is the output where the engine will be written to
         @param a is the first input variable
         @param b is the second input variable
         @param outputVariables are the output variables to create the surface for
         */
        virtual void writeScriptPlots(std::ostream& writer,
                InputVariable* a, InputVariable* b,
                const std::vector<OutputVariable*>& outputVariables) const;


        virtual RScriptExporter* clone() const FL_IOVERRIDE;

    };

}

#endif /* FL_RSCRIPTEXPORTER_H */

