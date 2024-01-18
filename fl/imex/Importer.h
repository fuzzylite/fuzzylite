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

#ifndef FL_IMPORTER_H
#define FL_IMPORTER_H

#include "fl/fuzzylite.h"

#include <string>

namespace fl {
    class Engine;

    /**
      The Importer class is the abstract class for importers to configure an
      Engine and its components from different text formats.

      @todo declare methods to import specific components

      @author Juan Rada-Vilela, Ph.D.
      @see Exporter
      @since 4.0
     */
    class FL_API Importer {
    public:

        Importer();
        virtual ~Importer();
        FL_DEFAULT_COPY_AND_MOVE(Importer)

        /**
          Imports the engine from the given text
          @param text is the string representation of the engine to import from
          @return the engine represented by the text
         */
        virtual Engine* fromString(const std::string& text) const = 0;
        /**
          Imports the engine from the given file
          @param path is the full path of the file containing the engine to import from
          @return the engine represented by the file
         */
        virtual Engine* fromFile(const std::string& path) const;

        /**
          Returns the name of the importer
          @return the name of the importer
         */
        virtual std::string name() const = 0;
        /**
          Creates a clone of the importer
          @return a clone of the importer
         */
        virtual Importer* clone() const = 0;
    };
}

#endif  /* IMPORTER_H */

