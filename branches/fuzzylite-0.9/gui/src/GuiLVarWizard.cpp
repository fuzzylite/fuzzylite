#include <QtGui/qmessagebox.h>

#include "GuiLVarWizard.h"

namespace fl_gui {

	GuiLVarWizard::GuiLVarWizard(fl::LinguisticVariable& model, QWidget* parent,
		Qt::WindowFlags f) :
		QDialog(parent, f), _model(&model), _ui(new Ui::LinguisticVariableWizard) {

	}

	GuiLVarWizard::~GuiLVarWizard() {
		disconnect();
		delete _ui;
	}

	fl::LinguisticVariable& GuiLVarWizard::getModel() const {
		return *this->_model;
	}

	Ui::LinguisticVariableWizard& GuiLVarWizard::getUi() const {
		return *this->_ui;
	}

	void GuiLVarWizard::setup() {
		getUi().setupUi(this);
		connect();
	}

	void GuiLVarWizard::connect() {
		QObject::connect(getUi().dsx_min, SIGNAL(valueChanged(double)), this,
			SLOT(onChangeMinimum(double)));
		QObject::connect(getUi().dsx_max, SIGNAL(valueChanged(double)), this,
			SLOT(onChangeMaximum(double)));
		QObject::connect(getUi().btn_create_labels, SIGNAL(clicked()), this,
			SLOT(onClickCreateLabels()));
	}

	void GuiLVarWizard::disconnect() {
		QObject::disconnect(getUi().dsx_min, SIGNAL(valueChanged(double)), this,
			SLOT(onChangeMinimum(double)));
		QObject::disconnect(getUi().dsx_max, SIGNAL(valueChanged(double)), this,
			SLOT(onChangeMaximum(double)));
		QObject::disconnect(getUi().btn_create_labels, SIGNAL(clicked()), this,
			SLOT(onClickCreateLabels()));
	}

	void GuiLVarWizard::onChangeMinimum(double min) {
		if (getUi().dsx_max->value() < min) {
			getUi().dsx_max->setValue(min);
		}
	}

	void GuiLVarWizard::onChangeMaximum(double max) {
		if (getUi().dsx_min->value() > max) {
			getUi().dsx_min->setValue(max);
		}
	}

	void GuiLVarWizard::onClickCreateLabels() {
		getUi().ptx_term_labels->clear();
		for (int i = 1; i <= getUi().spx_terms->value(); ++i) {
			getUi().ptx_term_labels->appendPlainText(QString::fromStdString(
				getModel().name()) + "." + QString::number(i));
		}
	}

	void GuiLVarWizard::accept() {
		fl::flScalar min = getUi().dsx_min->value();
		fl::flScalar max = getUi().dsx_max->value();
		if (fl::FuzzyOperator::IsEq(max - min, 0)) {
			QMessageBox::critical(this, "Error",
				"Minimum and maximum values must be different", QMessageBox::Ok,
				QMessageBox::Ok);
			return;
		}

		int terms = getUi().spx_terms->value();

		QStringList labels = getUi().ptx_term_labels->toPlainText().split("\n",
			QString::SkipEmptyParts);
		if (labels.count() != terms) {
			QMessageBox::critical(this, "Error",
				"The amount of terms does not match the amount of labels",
				QMessageBox::Ok, QMessageBox::Ok);
			return;
		}

		fl::flScalar intersection = NAN;
		switch (getUi().cbx_lterm->currentIndex()) {
		case 0:
			intersection = 0.5;
			break;
		case 1:
			intersection = 4.0 / 5.0;
			break;
		default:
			intersection = NAN;
		}

		fl::flScalar term_range = (max - min) / (terms - terms / 2);
		fl::flScalar current_step = min + (1 - intersection) * term_range;
		FL_LOG("current_step=" << current_step);

		for (int i = 0; i < terms; ++i) {
			fl::LinguisticTerm* term = NULL;
			switch (getUi().cbx_lterm->currentIndex()) {
			case 0:
				term = new fl::TriangularTerm(labels.at(i).toStdString(), current_step
					- (1 - intersection) * term_range, current_step + intersection
					* term_range);
				break;
			case 1:
				if (i == 0 || i == terms - 1) {
					term = new fl::ShoulderTerm(labels.at(i).toStdString(), current_step
						- term_range, current_step + term_range, i == 0);
				} else {
					term = new fl::TriangularTerm(labels.at(i).toStdString(),
						current_step - (1 - intersection) * term_range, current_step
							+ intersection * term_range);
				}
				break;
			case 2:
				term = new fl::TrapezoidalTerm(labels.at(i).toStdString(), current_step
					- (1 - intersection) * term_range, current_step + intersection
					* term_range);
				break;
			default:
				throw fl::InvalidArgumentException(FL_AT,QString::number(getUi().cbx_lterm->currentIndex()).toStdString() +
				                            " is not an option for membership function") ;
			}
			current_step += intersection * term_range;
			FL_LOG("current_step=" << current_step);
			getModel().addTerm(*term);
		}
		done(QDialog::Accepted);
	}
}
