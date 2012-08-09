/*   Copyright 2010 Juan Rada-Vilela

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
#include <fuzzylite/FuzzyOperator.h>
#include <fuzzylite/StringOperator.h>

#include <limits>
#include <math.h>
namespace fl {

	FuzzyOperator::FuzzyOperator(TNORM tnorm, SNORM snorm, CMODULATION cmod,
		DEFUZZIFY defuzzify, SNORM accumulate, int samples) :
		_tnorm(tnorm), _snorm(snorm), _cmod(cmod), _defuzzify(defuzzify), _accumulate(
			accumulate), _samples(samples) {

	}

	FuzzyOperator::~FuzzyOperator() {

	}

	flScalar FuzzyOperator::fuzzyAnd(flScalar mu1, flScalar mu2) const {
		return FuzzyOperator::FuzzyAnd(mu1, mu2, _tnorm);
	}

	flScalar FuzzyOperator::fuzzyOr(flScalar mu1, flScalar mu2) const {
		return FuzzyOperator::FuzzyOr(mu1, mu2, _snorm);
	}

	flScalar FuzzyOperator::modulate(flScalar modulation, flScalar mu) const {
		switch (_cmod) {
		case CM_CLIP:
			return FuzzyOperator::Min(modulation, mu);
		case CM_SCALE:
			return FuzzyOperator::AlgebraicProduct(modulation, mu);
		default:
			throw InvalidArgumentException(FL_AT,StringOperator::IntToString(_cmod) + " is not an option for modulation") ;
		}
	}

	flScalar FuzzyOperator::defuzzify(const LinguisticTerm& term) const {
		switch (_defuzzify) {
		case D_COG: {
			flScalar x, y;
			FuzzyOperator::Centroid(term, x, y, samples());
			return x;
		}
		default:
			throw InvalidArgumentException(FL_AT,StringOperator::IntToString(_defuzzify) + " is not an option for defuzzification") ;
		}
		return flScalar(-1.0);
	}

	flScalar FuzzyOperator::accumulate(flScalar mu1, flScalar mu2) const {
		return FuzzyOperator::FuzzyOr(mu1, mu2, _accumulate);
	}

	//static

	flScalar FuzzyOperator::FuzzyAnd(flScalar mu1, flScalar mu2, TNORM tnorm) {
		switch (tnorm) {
		case TN_MIN:
			return FuzzyOperator::Min(mu1, mu2);
		case TN_PROD:
			return FuzzyOperator::AlgebraicProduct(mu1, mu2);
		case TN_BDIFF:
			return FuzzyOperator::BoundedDiff(mu1, mu2);
		default:
			throw InvalidArgumentException(FL_AT,StringOperator::IntToString(tnorm) + " is not an option for T-Norm") ;
		}
	}

	flScalar FuzzyOperator::FuzzyOr(flScalar mu1, flScalar mu2, SNORM snorm) {
		switch (snorm) {
		case SN_MAX:
			return FuzzyOperator::Max(mu1, mu2);
		case SN_SUM:
			return FuzzyOperator::AlgebraicSum(mu1, mu2);
		case SN_BSUM:
			return FuzzyOperator::BoundedSum(mu1, mu2);
		default:
			throw InvalidArgumentException(FL_AT,StringOperator::IntToString(snorm) + " is not an option for S-Norm") ;
		}
	}

	//SETTERS

	void FuzzyOperator::setTnorm(TNORM tnorm) {
		this->_tnorm = tnorm;
	}

	void FuzzyOperator::setSnorm(SNORM snorm) {
		this->_snorm = snorm;
	}

	void FuzzyOperator::setConsequentModulation(CMODULATION cmod) {
		this->_cmod = cmod;
	}

	void FuzzyOperator::setDefuzzify(DEFUZZIFY defuzz) {
		this->_defuzzify = defuzz;
	}

	void FuzzyOperator::setAggregate(SNORM aggregate) {
		this->_accumulate = aggregate;
	}

	void FuzzyOperator::setSamples(int samples) {
		this->_samples = samples;
	}

	//GETTERS

	FuzzyOperator::TNORM FuzzyOperator::getTnorm() const {
		return this->_tnorm;
	}

	FuzzyOperator::SNORM FuzzyOperator::getSnorm() const {
		return this->_snorm;
	}

	FuzzyOperator::CMODULATION FuzzyOperator::getConsequentModulation() const {
		return this->_cmod;
	}

	FuzzyOperator::DEFUZZIFY FuzzyOperator::getDefuzzification() const {
		return this->_defuzzify;
	}

	FuzzyOperator::SNORM FuzzyOperator::getAggregation() const {
		return this->_accumulate;
	}

	int FuzzyOperator::samples() const {
		return this->_samples;
	}

	//STATIC

	FuzzyOperator& FuzzyOperator::DefaultFuzzyOperator() {
		static FuzzyOperator* FUZZY_OP = new fl::FuzzyOperator;
		return *FUZZY_OP;
	}

	flScalar FuzzyOperator::Min(flScalar mu1, flScalar mu2) {
		return mu1 < mu2 ? mu1 : mu2;
	}

	flScalar FuzzyOperator::AlgebraicProduct(flScalar mu1, flScalar mu2) {
		return mu1 * mu2;
	}

	flScalar FuzzyOperator::BoundedDiff(flScalar mu1, flScalar mu2) {
		flScalar result = mu1 + mu2;
		return result > flScalar(0.0) ? result : flScalar(0.0);
	}

	//SNORMS

	flScalar FuzzyOperator::Max(flScalar mu1, flScalar mu2) {
		return mu1 > mu2 ? mu1 : mu2;
	}

	flScalar FuzzyOperator::AlgebraicSum(flScalar mu1, flScalar mu2) {
		return mu1 + mu2 - (mu1 * mu2);
	}

	flScalar FuzzyOperator::BoundedSum(flScalar mu1, flScalar mu2) {
		flScalar result = mu1 + mu2;
		return result < flScalar(1.0) ? result : flScalar(1.0);
	}

	//Other operators

	flScalar FuzzyOperator::Absolute(flScalar value) {
		return value < flScalar(0.0) ? value * flScalar(-1.0) : value;
	}

	bool FuzzyOperator::IsEq(flScalar x1, flScalar x2) {
		//http://stackoverflow.com/questions/17333/most-effective-way-for-float-and-double-comparison
		//        return fabs(x1 - x2) < std::numeric_limits<flScalar>::epsilon() ;
		return fabs(x1 - x2) < FL_EPSILON;
	}

	bool FuzzyOperator::IsLEq(flScalar x1, flScalar x2) {
		return IsEq(x1, x2) ? true : x1 < x2;
	}

	bool FuzzyOperator::IsGEq(flScalar x1, flScalar x2) {
		return IsEq(x1, x2) ? true : x1 > x2;
	}

	flScalar FuzzyOperator::Scale(flScalar src_min, flScalar src_max, flScalar value,
		flScalar target_min, flScalar target_max) {
		//Courtesy of Rubén Parma :)
		return (target_max - target_min) / (src_max - src_min) * (value - src_min)
			+ target_min;
	}

	flScalar FuzzyOperator::Area(const LinguisticTerm& term, int samples) {
		flScalar result = flScalar(0.0);
		flScalar step_size = (term.maximum() - term.minimum()) / samples;
		flScalar step = term.minimum();
		for (int i = 0; i < samples; ++i) {
			result += step_size * term.membership(step); //Area under the curve.
			step += step_size;
		}
		return result;
	}

	void FuzzyOperator::Centroid(const LinguisticTerm& term, flScalar& centroid_x,
		flScalar& centroid_y, int samples) {
		centroid_x = flScalar(0);
		centroid_y = flScalar(0);

		flScalar sum_area = flScalar(0.0);
		flScalar sum_a_x = flScalar(0.0);
		flScalar sum_a_y = flScalar(0.0);
		flScalar tmp_area = flScalar(0.0);

		flScalar range = term.maximum() - term.minimum();
		flScalar step_size = range / samples;

		int pi = 0;
		flScalar x[3];
		flScalar y[3];
		bool y_is_0 = true;
		x[0] = term.minimum();
		y[0] = term.membership(x[0]);
		x[1] = term.minimum() + step_size;
		y[1] = term.membership(x[1]);

		pi = 2;
		int i = 1;
		//Triangulation:
		while (pi < samples) {
			++i;
			if (y_is_0) {
				x[i % 3] = x[(i - 1) % 3];
				y[i % 3] = 0;
			} else {
				x[i % 3] = term.minimum() + pi * step_size;
				y[i % 3] = term.membership(x[i % 3]);
				++pi;
			}

			y_is_0 = !y_is_0;

			//Area of triangle:
			// |Ax(By - Cy) + Bx(Cy - Ay) + Cx(Ay - By)| / 2
			// |x0(y1 - y2) + x1(y2 - y0) + x2(y0 - y1)| / 2
			tmp_area = FuzzyOperator::Absolute(x[0] * (y[1] - y[2]) + x[1]
				* (y[2] - y[0]) + x[2] * (y[0] - y[1])) / 2;
			//Centroid of a triangle:
			// x = (x0 + x1 + x2)/3; y = (y0 + y1 + y2)/3;
			sum_a_x += tmp_area * (x[0] + x[1] + x[2]) / 3;
			sum_a_y += tmp_area * (y[0] + y[1] + y[2]) / 3;

			sum_area += tmp_area;
		}
		centroid_x = sum_a_x / sum_area;
		centroid_y = sum_a_y / sum_area;
	}

	flScalar FuzzyOperator::AreaAndCentroid(const LinguisticTerm& term, flScalar& centroid_x,
		flScalar& centroid_y, int samples) {
		centroid_x = flScalar(0);
		centroid_y = flScalar(0);

		flScalar sum_area = flScalar(0.0);
		flScalar sum_a_x = flScalar(0.0);
		flScalar sum_a_y = flScalar(0.0);
		flScalar tmp_area = flScalar(0.0);

		flScalar range = term.maximum() - term.minimum();
		flScalar step_size = range / samples;

		int pi = 0;
		flScalar x[3];
		flScalar y[3];
		bool y_is_0 = true;
		x[0] = term.minimum();
		y[0] = term.membership(x[0]);
		x[1] = term.minimum() + step_size;
		y[1] = term.membership(x[1]);

		pi = 2;
		int i = 1;
		//Triangulation:
		while (pi < samples) {
			++i;
			if (y_is_0) {
				x[i % 3] = x[(i - 1) % 3];
				y[i % 3] = 0;
			} else {
				x[i % 3] = term.minimum() + pi * step_size;
				y[i % 3] = term.membership(x[i % 3]);
				++pi;
			}

			y_is_0 = !y_is_0;

			//Area of triangle:
			// |Ax(By - Cy) + Bx(Cy - Ay) + Cx(Ay - By)| / 2
			// |x0(y1 - y2) + x1(y2 - y0) + x2(y0 - y1)| / 2
			tmp_area = FuzzyOperator::Absolute(x[0] * (y[1] - y[2]) + x[1]
				* (y[2] - y[0]) + x[2] * (y[0] - y[1])) / 2;
			//Centroid of a triangle:
			// x = (x0 + x1 + x2)/3; y = (y0 + y1 + y2)/3;
			sum_a_x += tmp_area * (x[0] + x[1] + x[2]) / 3;
			sum_a_y += tmp_area * (y[0] + y[1] + y[2]) / 3;

			sum_area += tmp_area;
		}
		centroid_x = sum_a_x / sum_area;
		centroid_y = sum_a_y / sum_area;
		return sum_area;
	}

	void FuzzyOperator::ColorGradientF(flScalar from_r, flScalar from_g, flScalar from_b,
		flScalar from_a, flScalar to_r, flScalar to_g, flScalar to_b, flScalar to_a,
		flScalar degree, flScalar& red, flScalar& green, flScalar& blue, flScalar& alpha) {
		red = fl::FuzzyOperator::Scale(0, 1, degree, from_r, to_r);
		green = fl::FuzzyOperator::Scale(0, 1, degree, from_g, to_g);
		blue = fl::FuzzyOperator::Scale(0, 1, degree, from_b, to_b);
		alpha = fl::FuzzyOperator::Scale(0, 1, degree, from_a, to_a);
	}

	void FuzzyOperator::ColorGradient(int from_r, int from_g, int from_b, int from_a,
		int to_r, int to_g, int to_b, int to_a, flScalar degree, int& red, int& green,
		int& blue, int& alpha) {
		red = fl::FuzzyOperator::Scale(0, 1, degree, from_r, to_r);
		green = fl::FuzzyOperator::Scale(0, 1, degree, from_g, to_g);
		blue = fl::FuzzyOperator::Scale(0, 1, degree, from_b, to_b);
		alpha = fl::FuzzyOperator::Scale(0, 1, degree, from_a, to_a);
	}

	void FuzzyOperator::main(int argc, char** argv) {
		//		TriangularTerm t;
		//		t.setA(0);
		//		t.setB(1);
		//		t.setC(2);
		////		t.setD(1);
		//		for (flScalar i = 0; FuzzyOperator::IsLEq(i, 1); i += 0.1) {
		////			FL_LOG(t.membership(i));
		//		}
		//		flScalar x, y;
		//		FL_LOG(AreaAndCentroid(t,x,y,0.01));
		//		FL_LOG("x:" << x <<", y:" << y);
	}
}
