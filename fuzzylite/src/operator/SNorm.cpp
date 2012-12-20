/* 
 * File:   SNorm.cpp
 * Author: jcrada
 * 
 * Created on 21 December 2012, 9:34 AM
 */

#include "fl/operator/SNorm.h"

namespace fl {

    //class Maximum : public SNorm {

    std::string Maximum::name() const {
        return "Maximum";
    }

    scalar Maximum::compute(scalar a, scalar b) const {
        return Op::Max(a, b);
    }


    //class Sum : public SNorm {

    std::string Sum::name() const {
        return "Sum";
    }

    scalar Sum::compute(scalar a, scalar b) const {
        return a + b - (a * b);
    }


    //class BoundedSum : public SNorm {

    std::string BoundedSum::name() const {
        return "BoundedSum";
    }

    scalar BoundedSum::compute(scalar a, scalar b) const {
        return Op::Min(1, a + b);
    }


    //class NormalizedSum : public SNorm {

    std::string NormalizedSum::name() const {
        return "NormalizedSum";
    }

    scalar NormalizedSum::compute(scalar a, scalar b) const {
        return a + b / Op::Max(1, Op::Max(a, b));
    }


    //class DrasticSum : public SNorm {

    std::string DrasticSum::name() const {
        return "DrasticSum";
    }

    scalar DrasticSum::compute(scalar a, scalar b) const {
        if (Op::IsEq(Op::Min(a, b), 0.0)) {
            return Op::Max(a, b);
        }
        return 1.0;
    }


    //class EinsteinSum : public SNorm {

    std::string EinsteinSum::name() const {
        return "EinsteinSum";
    }

    scalar EinsteinSum::compute(scalar a, scalar b) const {
        return (a + b) / (1 + a * b);
    }


    //class HamacherSum : public SNorm {

    std::string HamacherSum::name() const {
        return "HamacherSum";
    }

    scalar HamacherSum::compute(scalar a, scalar b) const {
        return (a + b - 2 * a * b) / (1 - a * b);
    }


}
