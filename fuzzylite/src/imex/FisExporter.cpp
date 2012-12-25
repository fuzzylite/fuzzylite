/* 
 * File:   FisExporter.cpp
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:53 PM
 */

#include "fl/imex/FisExporter.h"

#include "fl/Headers.h"

namespace fl {

    FisExporter::FisExporter() {
    }

    FisExporter::~FisExporter() {
    }

    std::string FisExporter::name() const {
        return "FisExporter";
    }

    std::string FisExporter::toString(const Engine* engine) {
        Configuration* configuration = engine->getConfiguration();
        if (not configuration) {
            throw fl::Exception("[engine error] a configuration object is required within "
                    "the engine to export to FIS format");
        }
        std::ostringstream fcl;
        fcl << "[System]" << std::endl;
        fcl << "Name='" << engine->getName() << "'" << std::endl;
        fcl << "Type='mamdani'" << std::endl;
        fcl << "Version=" << FL_VERSION << "" << std::endl;
        fcl << "NumInputs=" << engine->numberOfInputVariables() << std::endl;
        fcl << "NumOutputs=" << engine->numberOfOutputVariables() << std::endl;
        fcl << "NumRules=" << engine->getRuleBlock(0)->numberOfRules() << std::endl;

        fcl << "AndMethod='" << toFis(configuration->getTnorm()) << std::endl;
        fcl << "OrMethod='" << toFis(configuration->getSnorm()) << std::endl;
        fcl << "ImpMethod='" << toFis(configuration->getActivation()) << std::endl;
        fcl << "AggMethod='" << toFis(configuration->getAccumulation()) << std::endl;
        fcl << "DefuzzMethod='" << toFis(configuration->getDefuzzifier()) << std::endl;

        fcl << std::endl;

        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            InputVariable* ivar = engine->getInputVariable(i);
            fcl << "[Input" << (i + 1) << "]" << std::endl;
            fcl << "Name='" << ivar->getName() << "'" << std::endl;
            fcl << "Range=[" << ivar->getMinimum() << " " << ivar->getMaximum() << "]" << std::endl;
            fcl << "NumMFs=" << ivar->numberOfTerms() << std::endl;
            for (int t = 0; t < ivar->numberOfTerms(); ++t) {
                fcl << "MF" << (t + 1) << "=" << toFis(ivar->getTerm(i)) << std::endl;
            }
        }
        fcl << std::endl;

        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* ivar = engine->getOutputVariable(i);
            fcl << "[Output" << (i + 1) << "]" << std::endl;
            fcl << "Name='" << ivar->getName() << "'" << std::endl;
            fcl << "Range=[" << ivar->getMinimum() << " " << ivar->getMaximum() << "]" << std::endl;
            fcl << "NumMFs=" << ivar->numberOfTerms() << std::endl;
            for (int t = 0; t < ivar->numberOfTerms(); ++t) {
                fcl << "MF" << (t + 1) << "=" << toFis(ivar->getTerm(i)) << std::endl;
            }
        }
        fcl << std::endl;
        
        fcl << "[Rules]" << std::endl;
        
        return fcl.str();
    }
    
    
    std::string FisExporter::toFis(const TNorm* tnorm) const {
        if (not tnorm) return "";
        std::string name = tnorm->name();
        if (name == Minimum().name()) return "min";
        //        if (name == AlgebraicProduct().name()) return "prod";
        //        if (name == BoundedDifference().name()) return "bdif";
        //        if (name == DrasticProduct().name()) return "DPROD";
        //        if (name == EinsteinProduct().name()) return "EPROD";
        //        if (name == HamacherProduct().name()) return "HPROD";
        return tnorm->name();
    }

    std::string FisExporter::toFis(const SNorm * snorm) const {
        if (not snorm) return "";
        std::string name = snorm->name();
        if (name == Maximum().name()) return "max";
        //        if (name == AlgebraicSum().name()) return "ASUM";
        //        if (name == BoundedSum().name()) return "BSUM";
        //        if (name == DrasticSum().name()) return "DSUM";
        //        if (name == EinsteinSum().name()) return "ESUM";
        //        if (name == HamacherSum().name()) return "HSUM";
        return snorm->name();
    }

    std::string FisExporter::toFis(const Defuzzifier* defuzzifier) const {
        if (not defuzzifier) return "";
        if (defuzzifier->name() == CenterOfGravity().name()) return "centroid";
        //        if (defuzzifier->name() == SmallestOfMaximum().name()) return "SOM";
        //        if (defuzzifier->name() == LargestOfMaximum().name()) return "LOM";
        //        if (defuzzifier->name() == MeanOfMaximum().name()) return "MOM";
        return defuzzifier->name();
    }

    std::string FisExporter::toFis(const Term* term) const{
        return term->toString();
    }
}