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
/* 
 * File:   FuzzyAction.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 8:54 PM
 */

#ifndef _FUZZYACTION_H
#define	_FUZZYACTION_H

#include <fuzzylite/OutputLVar.h>
#include <fuzzylite/Hedge.h>
#include <fuzzylite/FuzzyOperator.h>
#include <fuzzylite/LinguisticTerm.h>

namespace fl {

	class FuzzyAction {
	private:
		OutputLVar* _output_lvar;
		std::vector<Hedge*> _hedges;
		const LinguisticTerm* _term;
		flScalar _weight;
	public:
		FuzzyAction();
		virtual ~FuzzyAction();

		virtual void execute(flScalar degree);

		virtual void setOutputLVar(OutputLVar* output_lvar);
		virtual OutputLVar* outputLVar() const;

		virtual void setTerm(const LinguisticTerm& term);
		virtual const LinguisticTerm* term() const;

		virtual void setWeight(flScalar prob);
		virtual flScalar weight() const;

		virtual void addHedge(Hedge& hedge);
		virtual Hedge* hedge(int index) const throw (OutOfRangeException);
		virtual int numberOfHedges() const;

		virtual std::string toString() const;

	};
}

#endif	/* _FUZZYACTION_H */

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
/* 
 * File:   FuzzyAction.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 8:54 PM
 */

#ifndef _FUZZYACTION_H
#define	_FUZZYACTION_H

#include <OutputLVar.h>
#include <Hedge.h>
#include <FuzzyOperator.h>
#include <LinguisticTerm.h>

namespace fl {

	class FuzzyAction {
	private:
		OutputLVar* _output_lvar;
		std::vector<Hedge*> _hedges;
		const LinguisticTerm* _term;
		flScalar _weight;
	public:
		FuzzyAction();
		virtual ~FuzzyAction();

		virtual void execute(flScalar degree);

		virtual void setOutputLVar(OutputLVar* output_lvar);
		virtual OutputLVar* outputLVar() const;

		virtual void setTerm(const LinguisticTerm& term);
		virtual const LinguisticTerm* term() const;

		virtual void setWeight(flScalar prob);
		virtual flScalar weight() const;

		virtual void addHedge(Hedge& hedge);
		virtual Hedge* hedge(int index) const throw (OutOfRangeException);
		virtual int numberOfHedges() const;

		virtual std::string toString() const;

	};
}

#endif	/* _FUZZYACTION_H */

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
/* 
 * File:   FuzzyAction.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 8:54 PM
 */

#ifndef _FUZZYACTION_H
#define	_FUZZYACTION_H

#include <OutputLVar.h>
#include <Hedge.h>
#include <FuzzyOperator.h>
#include <LinguisticTerm.h>

namespace fl {

	class FuzzyAction {
	private:
		OutputLVar* _output_lvar;
		std::vector<Hedge*> _hedges;
		const LinguisticTerm* _term;
		flScalar _weight;
	public:
		FuzzyAction();
		virtual ~FuzzyAction();

		virtual void execute(flScalar degree);

		virtual void setOutputLVar(OutputLVar* output_lvar);
		virtual OutputLVar* outputLVar() const;

		virtual void setTerm(const LinguisticTerm& term);
		virtual const LinguisticTerm* term() const;

		virtual void setWeight(flScalar prob);
		virtual flScalar weight() const;

		virtual void addHedge(Hedge& hedge);
		virtual Hedge* hedge(int index) const throw (OutOfRangeException);
		virtual int numberOfHedges() const;

		virtual std::string toString() const;

	};
}

#endif	/* _FUZZYACTION_H */

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
/* 
 * File:   FuzzyAction.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 8:54 PM
 */

#ifndef _FUZZYACTION_H
#define	_FUZZYACTION_H

#include <OutputLVar.h>
#include <Hedge.h>
#include <FuzzyOperator.h>
#include <LinguisticTerm.h>

namespace fl {

	class FuzzyAction {
	private:
		OutputLVar* _output_lvar;
		std::vector<Hedge*> _hedges;
		const LinguisticTerm* _term;
		flScalar _weight;
	public:
		FuzzyAction();
		virtual ~FuzzyAction();

		virtual void execute(flScalar degree);

		virtual void setOutputLVar(OutputLVar* output_lvar);
		virtual OutputLVar* outputLVar() const;

		virtual void setTerm(const LinguisticTerm& term);
		virtual const LinguisticTerm* term() const;

		virtual void setWeight(flScalar prob);
		virtual flScalar weight() const;

		virtual void addHedge(Hedge& hedge);
		virtual Hedge* hedge(int index) const throw (OutOfRangeException);
		virtual int numberOfHedges() const;

		virtual std::string toString() const;

	};
}

#endif	/* _FUZZYACTION_H */

