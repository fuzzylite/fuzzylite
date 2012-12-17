/*
 * Wizard.cpp
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Wizard.h"

#include <QtGui/QMessageBox>

namespace fl {
    namespace qt {

        Wizard::Wizard(QWidget* parent, Qt::WindowFlags f)
                : QDialog(parent, f), ui(new Ui::Wizard) {
            setWindowFlags(Qt::Tool);
        }

        Wizard::~Wizard() {
            disconnect();
            delete ui;
        }

        void Wizard::setup(const std::string& prefix) {
            this->prefix = prefix;
            ui->setupUi(this);
            QRect scr = parentWidget()->geometry();
            move(scr.center() - rect().center());
            setFixedSize(size());
            connect();
        }

        void Wizard::connect() {
            QObject::connect(ui->btn_create_labels, SIGNAL(clicked()),
                    this, SLOT(onClickGenerate()));
            QObject::connect(ui->cbx_term, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onCurrentTermChanged(int)));
        }

        void Wizard::disconnect() {
            QObject::disconnect(ui->btn_create_labels, SIGNAL(clicked()),
                    this, SLOT(onClickGenerate()));
            QObject::disconnect(ui->cbx_term, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onCurrentTermChanged(int)));
        }

        void Wizard::onClickGenerate() {
            ui->ptx_terms->clear();
            char name = 'A';
            std::string separator = prefix.empty() ? "" : ".";
            for (int i = 0; i < ui->spx_terms->value(); ++i) {
                ui->ptx_terms->appendPlainText(
                        QString::fromStdString(prefix + separator) + name++);
            }
        }

        void Wizard::onCurrentTermChanged(int index) {
            scalar overlap = 0.0;
            switch (index) {
                case 0:
                    overlap = 0.5;
                    break; //triangle
                case 1:
                    overlap = 4.0 / 5.0;
                    break; //trapezoid
                default:
                    overlap = 0.5;
                    break;
            }
            ui->sbx_overlap->setValue(overlap);
        }

        void Wizard::accept() {
            if (fl::Op::IsGE(ui->dsx_min->value(), ui->dsx_max->value())) {
                QMessageBox::critical(this, "Error",
                        "The minimum value must be less than the maximum value",
                        QMessageBox::Ok);
                return;
            }

            if (ui->ptx_terms->document()-> toPlainText().isEmpty()) {
                onClickGenerate();
            }

            std::vector<std::string> names;
            std::istringstream tokenizer(
                    ui->ptx_terms->document()->toPlainText().toStdString());
            std::string token;
            while (tokenizer >> token) {
                names.push_back(token);
            }

            if ((int) names.size() != ui->spx_terms->value()) {
                QMessageBox::critical(this, "Error",
                        "Number of terms and names do not match",
                        QMessageBox::Ok);
                return;
            }
            createTerms(ui->spx_terms->value(),
                    ui->dsx_min->value(), ui->dsx_max->value(),
                    ui->cbx_term->currentText().toStdString(),
                    false);
            for (std::size_t i = 0; i < names.size(); ++i) {
                terms[i]->setName(names[i]);
            }
            QDialog::accept();
        }

        void Wizard::reject() {
            for (std::size_t i = 0; i < terms.size(); ++i) {
                delete terms[i];
            }
            QDialog::reject();
        }

        std::vector<std::pair<scalar, scalar> >
        Wizard::Overlap(int number_of_sections,
                scalar min, scalar max, scalar factor) {
            std::vector<std::pair<scalar, scalar> > limits;
//            scalar range = (max - min) / (factor * (number_of_sections + 1));
            scalar range = (max - min) / (number_of_sections);

            scalar nextOverlap = factor * range;
            scalar previousOverlap = (1- factor) * range;
            scalar step = min +previousOverlap;
            for (int i = 0; i < number_of_sections; ++i) {
                limits.push_back(std::make_pair(step - previousOverlap,
                        step + range + nextOverlap));
                step += range + nextOverlap;
            }
            return limits;
        }

        void Wizard::createTerms(int number_of_terms, scalar min, scalar max,
                const std::string& classname, bool hasShoulders) {
            (void) hasShoulders;
            scalar factor = ui->sbx_overlap->value();
            std::vector<std::pair<scalar, scalar> > limits =
                    Overlap(number_of_terms, min, max, factor);

            for (std::size_t i = 0; i < limits.size(); ++i) {
//                if (hasShoulders and i == 0) {
//                    terms.push_back(new LeftShoulder("",
//                            limits[i].first, limits[i].second));
//                } else if (hasShoulders and (int) i == number_of_terms - 1) {
//                    terms.push_back(new RightShoulder("",
//                            limits[i].first, limits[i].second));
//                } else
                if (classname == Triangle().className()) {
                    scalar a = limits[i].first;
                    scalar c = limits[i].second;
                    terms.push_back(new fl::Triangle("", a, (a + c) / 2, c));
                } else if (classname == Trapezoid().className()) {
                    scalar a = limits[i].first;
                    scalar d = limits[i].second;
                    terms.push_back(new Trapezoid("", a,
                            a + (d - a) * 1 / 5, a + (d - a) * 4 / 5,
                            d));
                    FL_LOG("a=" << a << ", d=" << d);
                    FL_LOG(terms[terms.size() - 1]->toString());
                } else {
                    std::ostringstream ex;
                    ex << "[unexpected error]";
                    throw fl::Exception(ex.str());
                }
            }
        }
    } /* namespace qt */
} /* namespace fl */
