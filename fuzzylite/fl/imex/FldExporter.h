/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/* 
 * File:   FldExporter.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 9:52 AM
 */

#ifndef FL_FLDEXPORTER_H
#define	FL_FLDEXPORTER_H

#include "fl/fuzzylite.h"
#include "fl/imex/Exporter.h"

#include <vector>

namespace fl {
    class Engine;

    class FL_EXPORT FldExporter : public Exporter {
    protected:
        std::string _separator;
    public:
        FldExporter(const std::string& separator = " ");
        virtual ~FldExporter();

        virtual std::string name() const;

        virtual void setSeparator(const std::string& separator);
        virtual std::string getSeparator() const;

        //WARNING: The engine will be const_casted in order to be processed!
        virtual std::string toString(const Engine* mutableEngine) const;
        virtual std::string toString(const Engine* mutableEngine, int maximumNumberOfResults) const;
        virtual std::string toString(const Engine* mutableEngine, const std::string& inputData) const;


        template<typename T>
        void toWriter(Engine* engine, T& writer,
                const std::string& separator, int maximumNumberOfResults) const;

        virtual void parse(const std::string& x, std::vector<scalar>& inputValues) const;
        
        template<typename T>
        void toWriter(Engine* engine, T& writer, const std::string& separator,
                const std::vector<scalar>& inputValues) const;


    };
}

#endif	/* FL_FLDEXPORTER_H */

