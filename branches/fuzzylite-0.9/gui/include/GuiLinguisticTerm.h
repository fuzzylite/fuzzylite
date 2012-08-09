/*
 * GuiLinguisticTerm.h
 *
 *  Created on: Dec 13, 2009
 *      Author: jcrada
 */

#ifndef _GUILINGUISTICTERM_H_
#define _GUILINGUISTICTERM_H_

#include <QtGui/QDialog>
#include <fuzzylite/FuzzyLite.h>

#include "ui_LinguisticTerm.h"

namespace fl_gui {

	class GuiLinguisticTerm: public QDialog {
	Q_OBJECT
	protected slots:
		virtual void updateUi();

		virtual void onChangeTriangularA(double a);
		virtual void onChangeTriangularB(double b);
		virtual void onChangeTriangularC(double c);

		virtual void onChangeRectangularMin(double min);
		virtual void onChangeRectangularMax(double max);

		virtual void onChangeTrapezoidalA(double a);
		virtual void onChangeTrapezoidalB(double b);
		virtual void onChangeTrapezoidalC(double c);
		virtual void onChangeTrapezoidalD(double d);

		virtual void onChangeShoulderMin(double min);
		virtual void onChangeShoulderMax(double max);
		virtual void onChangeShoulderSide();

		virtual void onChangeSingletonValue(double value);

	private:
		Ui::LinguisticTerm* _ui;
		fl::TriangularTerm _triangular;
		fl::RectangularTerm _rectangular;
		fl::TrapezoidalTerm _trapezoidal;
		fl::ShoulderTerm _shoulder;
		fl::SingletonTerm _singleton;

	protected:
		enum eMembershipFunction {
			MF_TRIANGULAR = 0, MF_RECTANGULAR, MF_TRAPEZOIDAL, MF_SHOULDER, MF_SINGLETON
		};
		virtual void connect();
		virtual void disconnect();

	public:
		GuiLinguisticTerm(QWidget* parent = NULL, Qt::WindowFlags f = 0);
		virtual ~GuiLinguisticTerm();

		virtual Ui::LinguisticTerm& getUi() const;

		virtual void setup();

		virtual void showTerm(const fl::LinguisticTerm& term);

		virtual fl::LinguisticTerm* selectedTerm() const;

	};

} // namespace fuzzy_lite
#endif /* _GUILINGUISTICTERM_H_ */
