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

#ifndef FL_FLDEXPORTER_H
#define FL_FLDEXPORTER_H

#include "fl/imex/Exporter.h"

#include <vector>

namespace fl {
    class Engine;
    class InputVariable;
    class OutputVariable;

    /**
      The FldExporter class is an Exporter that evaluates an Engine and exports
      its input values and output values to the FuzzyLite Dataset (FLD) format,
      see [http://www.fuzzylite.com/fll-fld](http://www.fuzzylite.com/fll-fld)
      for more information.

      @author Juan Rada-Vilela, Ph.D.
      @see FllExporter
      @see Exporter
      @since 4.0
     */
    class FL_API FldExporter : public Exporter {
    private:
        std::string _separator;
        bool _exportHeaders;
        bool _exportInputValues;
        bool _exportOutputValues;
    public:

        /**
         The ScopeOfValues refers to the scope of the equally-distributed values
         to generate.
         */
        enum ScopeOfValues {
            /**Generates @f$n@f$ values for each variable*/
            EachVariable,
            /**Generates @f$n@f$ values for all variables*/
            AllVariables
        };
        explicit FldExporter(const std::string& separator = " ");
        virtual ~FldExporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(FldExporter)

        virtual std::string name() const FL_IOVERRIDE;

        /**
          Sets the separator of the dataset columns
          @param separator is the separator of the dataset columns
         */
        virtual void setSeparator(const std::string& separator);
        /**
          Gets the separator of the dataset columns
          @return the separator of the dataset columns
         */
        virtual std::string getSeparator() const;

        /**
          Sets whether the header of the dataset is to be exported
          @param exportHeaders indicates whether the header of the dataset is
          to be exported
         */
        virtual void setExportHeader(bool exportHeaders);
        /**
          Gets whether the header of the dataset is to be exported
          @return whether the header of the dataset is to be exported
         */
        virtual bool exportsHeader() const;

        /**
          Sets whether the values of the input variables are to be exported
          @param exportInputValues indicates whether the values of the input
          variables are to be exported
         */
        virtual void setExportInputValues(bool exportInputValues);
        /**
          Gets whether the values of the input variables are to be exported
          @return whether the values of the input variables are to be exported
         */
        virtual bool exportsInputValues() const;

        /**
          Sets whether the values of the output variables are to be exported
          @param exportOutputValues indicates whether the values of the output
          variables are to be exported
         */
        virtual void setExportOutputValues(bool exportOutputValues);
        /**
          Gets whether the values of the output variables are to be exported
          @return whether the values of the output variables are to be exported
         */
        virtual bool exportsOutputValues() const;

        /**
          Gets the header of the dataset for the given engine
          @param engine is the engine to be exported
          @return the header of the dataset for the given engine
         */
        virtual std::string header(const Engine* engine) const;

        /**
          Returns a FuzzyLite Dataset from the engine. Please consider that the
          engine will be `const_cast`ed to achieve so; that is, despite being
          marked as `const`, the engine will be modified in order to compute
          the output values based on the input values.
          @param engine is the engine to export
          @return a FuzzyLite Dataset from the engine
         */
        virtual std::string toString(const Engine* engine) const FL_IOVERRIDE;
        /**
          Returns a FuzzyLite Dataset from the engine.
          @param engine is the engine to export
          @param values is the number of values to export
          @param scope indicates the scope of the values
          @return a FuzzyLite Dataset from the engine
         */
        virtual std::string toString(Engine* engine, int values, ScopeOfValues scope = AllVariables) const;

        /**
          Returns a FuzzyLite Dataset from the engine.
          @param engine is the engine to export
          @param values is the number of values to export
          @param scope indicates the scope of the values
          @param activeVariables contains the input variables to generate values for.
          The input variables must be in the same order as in the engine. A value of
          fl::null indicates the variable is not active.
          @return a FuzzyLite Dataset from the engine
         */
        virtual std::string toString(Engine* engine, int values, ScopeOfValues scope,
                const std::vector<InputVariable*>& activeVariables) const;
        /**
          Returns a FuzzyLite Dataset from the engine.
          @param engine is the engine to export
          @param reader is the reader of a set of lines containing space-separated
          input values
          @return a FuzzyLite Dataset from the engine
         */
        virtual std::string toString(Engine* engine, std::istream& reader) const;


        using Exporter::toFile;
        /**
          Saves the engine as a FuzzyLite Dataset into the specified file
          @param path is the full path of the file
          @param engine is the engine to export
          @param values is the number of values to export
          @param scope indicates the scope of the values
         */
        virtual void toFile(const std::string& path, Engine* engine,
                int values, ScopeOfValues scope = AllVariables) const;
        /**
          Saves the engine as a FuzzyLite Dataset into the specified file
          @param path is the full path of the file
          @param engine is the engine to export
          @param values is the number of values to export
          @param scope indicates the scope of the values
          @param activeVariables contains the input variables to generate values for.
          The input variables must be in the same order as in the engine. A value of
          fl::null indicates the variable is not active.
         */
        virtual void toFile(const std::string& path, Engine* engine,
                int values, ScopeOfValues scope,
                const std::vector<InputVariable*>& activeVariables) const;
        /**
          Saves the engine as a FuzzyLite Dataset into the specified file
          @param path is the full path of the file
          @param engine is the engine to export
          @param reader is the reader of a set of lines containing space-separated input values
         */
        virtual void toFile(const std::string& path, Engine* engine, std::istream& reader) const;

        /**
          Parses the string into a vector of values unless the string starts with `#`
          @param values is a space-separated set of values
          @return a vector of values
         */
        virtual std::vector<scalar> parse(const std::string& values) const;

        /**
          Writes the engine into the given writer
          @param engine is the engine to export
          @param writer is the output where the engine will be written to
          @param values is the number of values to export
          @param scope indicates the scope of the values
         */
        virtual void write(Engine* engine, std::ostream& writer, int values,
                ScopeOfValues scope = AllVariables) const;
        /**
          Writes the engine into the given writer
          @param engine is the engine to export
          @param writer is the output where the engine will be written to
          @param values is the number of values to export
          @param scope indicates the scope of the values
          @param activeVariables contains the input variables to generate values for.
          The input variables must be in the same order as in the engine. A value of
          fl::null indicates the variable is not active.
         */
        virtual void write(Engine* engine, std::ostream& writer, int values, ScopeOfValues scope,
                const std::vector<InputVariable*>& activeVariables) const;
        /**
          Writes the engine into the given writer
          @param engine is the engine to export
          @param writer is the output where the engine will be written to
          @param reader is the reader of a set of lines containing space-separated input values
         */
        virtual void write(Engine* engine, std::ostream& writer, std::istream& reader) const;
        /**
          Writes the engine into the given writer
          @param engine is the engine to export
          @param writer is the output where the engine will be written to
          @param inputValues is the vector of input values
         */
        virtual void write(Engine* engine, std::ostream& writer, const std::vector<scalar>& inputValues) const;
        /**
          Writes the engine into the given writer
          @param engine is the engine to export
          @param writer is the output where the engine will be written to
          @param inputValues is the vector of input values
          @param activeVariables contains the input variables to generate values for.
          The input variables must be in the same order as in the engine. A value of
          fl::null indicates the variable is not active.
         */
        virtual void write(Engine* engine, std::ostream& writer, const std::vector<scalar>& inputValues,
                const std::vector<InputVariable*>& activeVariables) const;

        virtual FldExporter* clone() const FL_IOVERRIDE;
    };
}

#endif  /* FL_FLDEXPORTER_H */

