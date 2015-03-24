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

 fuzzylite™ is a trademark of FuzzyLite Limited.

 */

/**
 * \file fl/term/Discrete.tpp
 *
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly.
 */

#ifndef FL_DISCRETE_TPP
#define FL_DISCRETE_TPP

namespace fl {

template <typename T>
Discrete* Discrete::create(const std::string& name, int argc,
		T x1, T y1, ...) { // throw (fl::Exception) {
	std::vector<scalar> xy(argc);
	xy.at(0) = x1;
	xy.at(1) = y1;
	va_list args;
	va_start(args, y1);
	for (int i = 2; i < argc; ++i) {
		xy.at(i) = (scalar) va_arg(args, T);
	}
	va_end(args);

	FL_unique_ptr<Discrete> result(new Discrete(name));
	if (xy.size() % 2 != 0) {
		result->setHeight(xy.back());
		xy.pop_back();
	}
	result->setXY(toPairs(xy));
	return result.release();
}

} // Namespace fl

#endif // FL_DISCRETE_TPP
