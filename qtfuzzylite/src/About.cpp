/* 
 * File:   About.cpp
 * Author: jcrada
 * 
 * Created on 25 January 2013, 6:00 PM
 */

#include "fl/qt/About.h"
#include <fl/Headers.h>
namespace fl {
    namespace qt {

        About::About(QWidget* parent, Qt::WindowFlags f)
        : QDialog(parent, f), ui(new Ui::About) {
            setWindowFlags(Qt::Dialog);
        }

        About::~About() {
            disconnect();
            delete ui;
        }

        void About::setup() {
            ui->setupUi(this);

            std::vector<int> generosity;
            generosity.push_back(10);
            generosity.push_back(20);
            generosity.push_back(50);

            generosity.push_back(100);
            generosity.push_back(200);
            generosity.push_back(500);

            generosity.push_back(1000);
            generosity.push_back(2000);
            generosity.push_back(5000);

            ui->sld_generosity->setMinimum(0);
            ui->sld_generosity->setMaximum(generosity.size() - 1);
            ui->sld_generosity->setValue((generosity.size() - 1) / 2);
            ui->sld_generosity->setSingleStep(1);
            ui->sld_generosity->setPageStep(1);
            ui->sld_generosity->setTickInterval(1);

            for (std::size_t i = 0; i < generosity.size(); ++i) {
                ui->cbx_generosity->addItem("$" + QString::number(generosity.at(i)));
            }

            ui->lbl_donate->setTextFormat(Qt::RichText);
            ui->lbl_donate->setOpenExternalLinks(true);

            layout()->setSizeConstraint(QLayout::SetFixedSize);
            this->adjustSize();
            connect();
            ui->cbx_generosity->setCurrentIndex((generosity.size() - 1) / 2);
        }

        void About::connect() {
            QObject::connect(ui->sld_generosity, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeSliderGenerosity(int)));
            QObject::connect(ui->cbx_generosity, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeComboGenerosity(int)));
        }

        void About::disconnect() {
            QObject::disconnect(ui->sld_generosity, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeSliderGenerosity(int)));
            QObject::disconnect(ui->cbx_generosity, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeComboGenerosity(int)));
        }

        void About::onChangeSliderGenerosity(int) {
            ui->cbx_generosity->setCurrentIndex(ui->sld_generosity->value());
        }

        void About::onChangeComboGenerosity(int) {
            if (ui->sld_generosity->value() != ui->cbx_generosity->currentIndex()) {
                ui->sld_generosity->setValue(ui->cbx_generosity->currentIndex());
            }
            changeDonation();
        }

        void About::changeDonation() {
            std::vector<std::string> generosity;
            generosity.push_back("low");
            generosity.push_back("Low");
            generosity.push_back("LOW");

            generosity.push_back("medium");
            generosity.push_back("Medium");
            generosity.push_back("MEDIUM");

            generosity.push_back("high");
            generosity.push_back("High");
            generosity.push_back("HIGH");

            std::string href = "https://www.paypal.com/cgi-bin/webscr?"
                    "cmd=_s-xclick&hosted_button_id=NEP8FHAW8FJ8S&on0=Generosity";
            std::string param = "&os0=" + generosity.at(ui->sld_generosity->value());
            ui->lbl_donate->setText(QString::fromStdString(
                    "<qt><a href='" + href + param + "'>"
                    "<img src=':/icons/btn_donateCC_LG.gif'/></a></qt>"));
        }

    }
}