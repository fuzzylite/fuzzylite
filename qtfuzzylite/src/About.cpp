/**
    This file is part of qtfuzzylite.

    qtfuzzylite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qtfuzzylite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qtfuzzylite.  If not, see <http://www.gnu.org/licenses/>.

    Juan Rada-Vilela, 01 February 2013
    jcrada@fuzzylite.com
 **/

/*
 * File:   About.cpp
 * Author: jcrada
 *
 * Created on 25 January 2013, 6:00 PM
 */

#include <fl/Headers.h>

#include "fl/qt/About.h"
#include "fl/qt/qtfuzzylite.h"
#include "fl/qt/Window.h"

#include <QDesktopServices>
#include <QUrl>

namespace fl {
    namespace qt {

        About::About(QWidget* parent, Qt::WindowFlags f)
        : QDialog(parent, f), ui(new Ui::About) {
            setWindowFlags(Qt::Dialog
                    | Qt::WindowSystemMenuHint
                    | Qt::WindowCloseButtonHint
                    | Qt::WindowStaysOnTopHint
                    | Qt::CustomizeWindowHint
                    );
        }

        About::~About() {
            disconnect();
            delete ui;
        }

        void About::setup() {
            ui->setupUi(this);

            _donationTerm.push_back("low");
            _donationValue.push_back("$1");
            _donationTerm.push_back("Low");
            _donationValue.push_back("$2");
            _donationTerm.push_back("LOW");
            _donationValue.push_back("$5");

            _donationTerm.push_back("medium");
            _donationValue.push_back("$10");
            _donationTerm.push_back("Medium");
            _donationValue.push_back("$20");
            _donationTerm.push_back("MEDIUM");
            _donationValue.push_back("$50");

            _donationTerm.push_back("high");
            _donationValue.push_back("$100");
            _donationTerm.push_back("High");
            _donationValue.push_back("$200");
            _donationTerm.push_back("HIGH");
            _donationValue.push_back("$500");

            ui->sld_donation->setMinimum(0);
            ui->sld_donation->setMaximum(_donationTerm.size() - 1);
            ui->sld_donation->setValue((_donationTerm.size() - 1) / 2);
            ui->sld_donation->setSingleStep(1);
            ui->sld_donation->setPageStep(1);
            ui->sld_donation->setTickInterval(1);

            for (std::size_t i = 0; i < _donationTerm.size(); ++i) {
                ui->cbx_donation->addItem(
                        QString::fromStdString(_donationValue.at(i)) + " - " +
                        QString::fromStdString(_donationTerm.at(i)));
            }

            ui->lbl_donate->setTextFormat(Qt::RichText);
            ui->lbl_donate->setOpenExternalLinks(true);

            layout()->setSizeConstraint(QLayout::SetFixedSize);
            //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            std::string header =
                    "<html>"
                    "<div align='center'>"
                    "<table>"
                    "<tr >"
                    "<td align='center' valign='middle'>"
                    "<a href='http://www.fuzzylite.com'><img src=':/qtfuzzylite.png'/></a>"
                    "</td>"
                    "<td align='left' valign='top'>"
                    "<b>" + fl::qt::qtfuzzylite::fullname() + "</b> "
                    "<br>"
                    "<b>" + fl::fuzzylite::fullname() + "</b>"
                    "<br>"
                    "</b>"
                    "<a href='http://www.fuzzylite.com'>www.fuzzylite.com</a>"
                    "<br>"
                    "<br>"
                    "Developed by<br>"
                    "Juan Rada-Vilela"
                    "<br>"
                    "<a href='mailto:jcrada@fuzzylite.com'>jcrada@fuzzylite.com</a></td>"
                    "</tr>"
                    "</table>"
                    "</div>"
                    "</html>";
            ui->lbl_header->setTextFormat(Qt::RichText);
            ui->lbl_header->setOpenExternalLinks(true);
            ui->lbl_header->setText(QString::fromStdString(header));
            connect();
            ui->cbx_donation->setCurrentIndex((_donationTerm.size() - 1) / 2);

            adjustSize();
            QRect scr = Window::mainWindow()->geometry();
            move(scr.center().x() - rect().center().x(), scr.top());
        }

        void About::connect() {
            QObject::connect(ui->sld_donation, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeSliderGenerosity(int)));
            QObject::connect(ui->cbx_donation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeComboGenerosity(int)));
        }

        void About::disconnect() {
            QObject::disconnect(ui->sld_donation, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeSliderGenerosity(int)));
            QObject::disconnect(ui->cbx_donation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeComboGenerosity(int)));
        }

        void About::onChangeSliderGenerosity(int) {
            ui->cbx_donation->setCurrentIndex(ui->sld_donation->value());
        }

        void About::onChangeComboGenerosity(int) {
            if (ui->sld_donation->value() != ui->cbx_donation->currentIndex()) {
                ui->sld_donation->setValue(ui->cbx_donation->currentIndex());
            }
            changeDonation();
        }

        void About::changeDonation() {
            std::string href = "http://www.fuzzylite.com/donation.php";
            std::string param = "?value=" + _donationTerm.at(ui->sld_donation->value());
            ui->lbl_donate->setText(QString::fromStdString(
                    "<qt><a href='" + href + param + "'>"
                    "<img src=':/donate.png'/></a></qt>"));
        }

        void About::accept() {
            std::string href = "http://www.fuzzylite.com/donation.php";
            std::string param = "?value=" + _donationTerm.at(ui->sld_donation->value());
            QDesktopServices::openUrl(QUrl(QString::fromStdString(href + param)));
            QDialog::accept();
        }

    }
}
