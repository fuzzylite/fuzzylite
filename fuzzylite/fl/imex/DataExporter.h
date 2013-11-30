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
 * File:   DataExporter.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 9:52 AM
 */

#ifndef FL_DATAEXPORTER_H
#define	FL_DATAEXPORTER_H

#include "fl/fuzzylite.h"
#include "fl/imex/Exporter.h"

namespace fl {
    class Engine;

    class FL_EXPORT DataExporter : public Exporter {
    protected:
        std::string _separator;
        int _resolution;

    public:
        DataExporter(const std::string& separator = " ", int resolution = 100);
        virtual ~DataExporter();

        virtual std::string name() const;

        virtual void setSeparator(const std::string& separator);
        virtual std::string getSeparator() const;

        virtual void setResolution(int resolution);
        virtual int getResolution() const;

        //WARNING: The engine will be const_casted, restarted, processed!
        virtual std::string toString(const Engine* mutableEngine) const;

        template<typename T>
        void toWriter(Engine* engine, T& writer,
                const std::string& separator, int resolution) const;


    };
}

#endif	/* FL_DATAEXPORTER_H */

