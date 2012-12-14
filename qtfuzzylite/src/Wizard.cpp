/*
 * Wizard.cpp
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#include "../fl/qt/Wizard.h"

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

            connect();
        }

        void Wizard::connect() {
            QObject::connect(ui->btn_create_labels, SIGNAL(clicked()),
                    this, SLOT(onClickGenerate()));
        }

        void Wizard::disconnect() {
            QObject::disconnect(ui->btn_create_labels, SIGNAL(clicked()),
                    this, SLOT(onClickGenerate()));
        }

        void Wizard::onClickGenerate() {
            int numberOfTerms = ui->spx_terms->value();

        }

        void Wizard::reject() {
            for (std::size_t i = 0; i < terms.size(); ++i) {
                delete terms[i];
            }
            QDialog::reject();
        }

        void  Wizard::createTerms(int number_of_terms, scalar min, scalar max,
                const std::string& classname, bool hasShoulders)  {

            scalar intersection; //Proportion of intersection between terms
            if (classname == Trapezoid().className()) {
                intersection = 4.0 / 5.0;
            } else {
                intersection = 0.5;
            }
            //TODO: What is the intersection in other terms?

            scalar term_range = (max - min) / (number_of_terms - number_of_terms / 2);
            scalar current_step = min + (1 - intersection) * term_range;
            for (int i = 0; i < number_of_terms; ++i) {
                if (hasShoulders and i == 0) {
                    terms.push_back(new LeftShoulder("",
                            current_step - (1 - intersection) * term_range,
                            current_step + intersection * term_range));
                } else if (hasShoulders and i == number_of_terms - 1) {
                    terms.push_back(new RightShoulder("",
                            current_step - (1 - intersection) * term_range,
                            current_step + intersection * term_range));
                }else if (classname == Triangle().className()) {
                    scalar a = current_step - (1 - intersection) * term_range;
                    scalar c = current_step + intersection * term_range;
                    terms.push_back(new fl::Triangle("", a, (a + c) / 2, c));
                } else if (classname == Trapezoid().className()) {
                    scalar a = current_step - (1 - intersection) * term_range;
                    scalar d = current_step + intersection * term_range;
                    terms.push_back(new Trapezoid("", a, .25 * (a + d), .75 * (a + d), d));
                }else{
                    FL_LOG("unexpected error");
                    throw std::exception();
                }
                current_step += intersection * term_range;
            }
        }
    } /* namespace qt */
} /* namespace fl */
