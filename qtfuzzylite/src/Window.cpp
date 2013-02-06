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
 * Window.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Window.h"
#include "fl/qt/About.h"
#include "fl/qt/Preferences.h"
#include "fl/qt/Term.h"
#include "fl/qt/Variable.h"
#include "fl/qt/Model.h"
#include "fl/qt/Control.h"

#include "fl/qt/qtfuzzylite.h"
#include "ui_ImEx.h"
#include "ui_About.h"


#include <QtGui/QListWidgetItem>
#include <QtGui/QScrollBar>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopWidget>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>
#include <QtCore/QTextStream>
#include <QtGui/QMenu>
#include <QtCore/QDateTime>
#include <QtGui/QMenuBar>
namespace fl {
    namespace qt {

        Window* Window::instance = NULL;

        Window* Window::mainWindow() {
            if (not instance) {
                instance = new Window;
            }
            return instance;
        }

        Window::Window(QWidget* parent, Qt::WindowFlags flags) :
        QMainWindow(parent, flags), _lastOpenedFilePath("."),
        ui(new Ui::Window), preferences(NULL) { }

        Window::~Window() {
            disconnect();
            if (preferences) delete preferences;
            if (_inputViewer) delete _inputViewer;
            if (_outputViewer) delete _outputViewer;
            delete ui;
        }

        void Window::setup() {
            ui->setupUi(this);
            ui->ptx_rules->setFont(typeWriterFont());
            ui->lsw_test_rules->setFont(typeWriterFont());
            ui->lsw_test_rules_activation->setFont(typeWriterFont());

            setUnifiedTitleAndToolBarOnMac(true);
            setGeometry(0, 0, 800, 600);
            preferences = new fl::qt::Preferences(this);
            preferences->setup();
            ui->tab_container->setCurrentIndex(0);

#ifdef Q_WS_MAC
            ui->menuTools->addAction("About qtfuzzylite", this, SLOT(onMenuAbout()));
            ui->menuTools->addSeparator();
            ui->menuTools->addAction("Preferences...", this, SLOT(onMenuPreferences()));
#endif
            ui->menuTools->addAction(ui->actionNew);
            ui->menuTools->addSeparator();
            ui->menuTools->addAction(ui->actionTerms);
            ui->menuTools->addSeparator();

            QMenu* importMenu = new QMenu("&Import");
            importMenu->setIcon(QIcon(":/icons/bottom.png"));
            importMenu->addAction("from &file...", this, SLOT(onMenuImportFromFile()));
            importMenu->addSeparator();
            importMenu->addAction("Fuzzy Controller &Language (FCL)", this, SLOT(onMenuImportFromFCL()));
            importMenu->addAction("Fuzzy Inference &System (FIS)", this, SLOT(onMenuImportFromFIS()));
            ui->menuTools->addMenu(importMenu);

            QMenu* exportMenu = new QMenu("&Export");
            exportMenu->setIcon(QIcon(":/icons/top.png"));
            exportMenu->addAction("fuzzylite (&C++)", this, SLOT(onMenuExportToCpp()));
            exportMenu->addSeparator();
            exportMenu->addAction("Fuzzy Controller Language (FC&L)", this, SLOT(onMenuExportToFCL()));
            exportMenu->addAction("Fuzzy Inference System (FI&S)", this, SLOT(onMenuExportToFIS()));
            ui->menuTools->addMenu(exportMenu);

#ifndef Q_WS_MAC
            ui->menuTools->addSeparator();
            ui->menuTools->addAction(ui->actionPreferences);
            ui->menuTools->addSeparator();
            ui->menuTools->addAction(ui->actionAbout);
            ui->menuTools->addSeparator();
            ui->menuTools->addAction(ui->actionQuit);
#endif

            QList<int> sizes;
            sizes << .75 * size().width() << .25 * size().width();
            ui->spl_control_inout_rules->setSizes(sizes);
            sizes.clear();
            sizes << .90 * size().width() << .10 * size().width();
            ui->spl_control_rule_strength->setSizes(sizes);

            QRect scr = QApplication::desktop()->screenGeometry();
            move(scr.center() - rect().center());

            setContextMenuPolicy(Qt::NoContextMenu);

            QWidget* widget = new QWidget;
            QHBoxLayout* spacerLayout = new QHBoxLayout;
            QSpacerItem* spacer = new QSpacerItem(1, 1,
                    QSizePolicy::Expanding, QSizePolicy::Minimum);
            spacerLayout->addSpacerItem(spacer);
            widget->setLayout(spacerLayout);
            ui->toolBar->insertWidget(ui->toolBar->actions().last(), widget);

            connect();
        }

        void Window::connect() {
            QObject::connect(ui->actionPreferences, SIGNAL(triggered()),
                    this, SLOT(onMenuPreferences()));
            QObject::connect(ui->actionTerms, SIGNAL(triggered()),
                    this, SLOT(onMenuTerms()));
            QObject::connect(ui->actionNew, SIGNAL(triggered()),
                    this, SLOT(onMenuNew()));
            QObject::connect(ui->actionAbout, SIGNAL(triggered()),
                    this, SLOT(onMenuAbout()));
            QObject::connect(ui->actionQuit, SIGNAL(triggered()),
                    this, SLOT(onMenuQuit()));

            QObject::connect(ui->actionImport, SIGNAL(triggered()),
                    this, SLOT(onMenuImport()));
            QObject::connect(ui->actionExport, SIGNAL(triggered()),
                    this, SLOT(onMenuExport()));

            QObject::connect(ui->lvw_inputs, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onChangeInputSelection()));
            QObject::connect(ui->lvw_outputs, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onChangeOutputSelection()));

            QObject::connect(ui->lvw_inputs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onDoubleClickInputItem(QListWidgetItem*)));
            QObject::connect(ui->lvw_outputs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onDoubleClickOutputItem(QListWidgetItem*)));

            QObject::connect(ui->btn_add_input, SIGNAL(clicked()), this,
                    SLOT(onClickAddInputVariable()));
            QObject::connect(ui->btn_remove_input, SIGNAL(clicked()), this,
                    SLOT(onClickRemoveInputVariable()));
            QObject::connect(ui->btn_edit_input, SIGNAL(clicked()), this,
                    SLOT(onClickEditInputVariable()));

            QObject::connect(ui->btn_add_output, SIGNAL(clicked()), this,
                    SLOT(onClickAddOutputVariable()));
            QObject::connect(ui->btn_remove_output, SIGNAL(clicked()), this,
                    SLOT(onClickRemoveOutputVariable()));
            QObject::connect(ui->btn_edit_output, SIGNAL(clicked()), this,
                    SLOT(onClickEditOutputVariable()));

            QObject::connect(ui->btn_generate_rules, SIGNAL(clicked()),
                    this, SLOT(onClickGenerateAllRules()));
            QObject::connect(ui->btn_parse_rules, SIGNAL(clicked()),
                    this, SLOT(onClickParseAllRules()));

            QObject::connect(ui->tab_container, SIGNAL(currentChanged(int)),
                    this, SLOT(onTabChange(int)));

            QObject::connect(ui->lsw_test_rules->verticalScrollBar(), SIGNAL(valueChanged(int)),
                    ui->lsw_test_rules_activation->verticalScrollBar(), SLOT(setValue(int)));
            QObject::connect(ui->lsw_test_rules_activation->verticalScrollBar(), SIGNAL(valueChanged(int)),
                    ui->lsw_test_rules->verticalScrollBar(), SLOT(setValue(int)));

        }

        void Window::disconnect() {
            QObject::disconnect(ui->actionPreferences, SIGNAL(triggered()),
                    this, SLOT(onMenuPreferences()));
            QObject::disconnect(ui->actionTerms, SIGNAL(triggered()),
                    this, SLOT(onMenuTerms()));
            QObject::disconnect(ui->actionNew, SIGNAL(triggered()),
                    this, SLOT(onMenuNew()));
            QObject::disconnect(ui->actionAbout, SIGNAL(triggered()),
                    this, SLOT(onMenuAbout()));
            QObject::disconnect(ui->actionQuit, SIGNAL(triggered()),
                    this, SLOT(onMenuQuit()));

            QObject::disconnect(ui->actionImport, SIGNAL(triggered()),
                    this, SLOT(onMenuImport()));
            QObject::disconnect(ui->actionExport, SIGNAL(triggered()),
                    this, SLOT(onMenuExport()));

            QObject::disconnect(ui->lvw_inputs, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onChangeInputSelection()));
            QObject::disconnect(ui->lvw_outputs, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onChangeOutputSelection()));

            QObject::disconnect(ui->lvw_inputs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onDoubleClickInputItem(QListWidgetItem*)));
            QObject::disconnect(ui->lvw_outputs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onDoubleClickOutputItem(QListWidgetItem*)));

            QObject::disconnect(ui->btn_add_input, SIGNAL(clicked()),
                    this, SLOT(onClickAddInputVariable()));
            QObject::disconnect(ui->btn_remove_input, SIGNAL(clicked()),
                    this, SLOT(onClickRemoveInputVariable()));
            QObject::disconnect(ui->btn_edit_input, SIGNAL(clicked()),
                    this, SLOT(onClickEditInputVariable()));

            QObject::disconnect(ui->btn_add_output, SIGNAL(clicked()),
                    this, SLOT(onClickAddOutputVariable()));
            QObject::disconnect(ui->btn_remove_output, SIGNAL(clicked()),
                    this, SLOT(onClickRemoveOutputVariable()));
            QObject::disconnect(ui->btn_edit_output, SIGNAL(clicked()),
                    this, SLOT(onClickEditOutputVariable()));

            QObject::disconnect(ui->btn_generate_rules, SIGNAL(clicked()),
                    this, SLOT(onClickGenerateAllRules()));
            QObject::disconnect(ui->btn_parse_rules, SIGNAL(clicked()),
                    this, SLOT(onClickParseAllRules()));

            QObject::disconnect(ui->tab_container, SIGNAL(currentChanged(int)),
                    this, SLOT(onTabChange(int)));

            QObject::disconnect(ui->lsw_test_rules->verticalScrollBar(), SIGNAL(valueChanged(int)),
                    ui->lsw_test_rules_activation->verticalScrollBar(), SLOT(setValue(int)));
            QObject::disconnect(ui->lsw_test_rules_activation->verticalScrollBar(), SIGNAL(valueChanged(int)),
                    ui->lsw_test_rules->verticalScrollBar(), SLOT(setValue(int)));
        }

        void Window::reloadModel() {
            Engine* engine = Model::Default()->engine();

            preferences->applyDefaults();
            if (preferences->isVisible()) preferences->loadFromModel();

            ui->lvw_inputs->clear();
            ui->lvw_outputs->clear();
            for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
                ui->lvw_inputs->addItem(QString::fromStdString(
                        engine->getInputVariable(i)->getName()));

            }
            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                ui->lvw_outputs->addItem(QString::fromStdString(
                        engine->getOutputVariable(i)->getName()));
            }

            if (engine->numberOfRuleBlocks() != 1) {
                std::ostringstream ex;
                ex << "[ruleblock error] expected one ruleblock, but found <"
                        << engine->numberOfRuleBlocks() << "> in the current engine";
                throw fl::Exception(ex.str(), FL_AT);
            }
            ui->ptx_rules->clear();
            RuleBlock* ruleblock = engine->getRuleBlock(0);
            for (int i = 0; i < ruleblock->numberOfRules(); ++i) {
                ui->ptx_rules->appendPlainText(
                        QString::fromStdString(ruleblock->getRule(i)->getUnparsedRule()));
            }
            reloadTest();
        }

        void Window::resetTest() {
            //Inputs
            QLayout* layout = ui->grx_test_inputs->layout();

            for (int i = layout->count() - 1; i >= 0; --i) {
                QLayoutItem* item = layout->itemAt(i);
                Control* control = dynamic_cast<Control*> (item->widget());
                if (control) {
                    QObject::disconnect(control, SIGNAL(valueChanged(double)),
                            this, SLOT(onInputValueChanged()));
                }
                layout->removeItem(item);
                delete item->widget();
                delete item;
            }

            //Rules
            ui->lsw_test_rules->clear();
            ui->lsw_test_rules_activation->clear();

            //Outputs
            layout = ui->grx_test_outputs->layout();
            for (int i = layout->count() - 1; i >= 0; --i) {
                QLayoutItem* item = layout->itemAt(i);
                Control* control = dynamic_cast<Control*> (item->widget());
                if (control) {
                    QObject::disconnect(this, SIGNAL(processOutput()),
                            control, SLOT(updateOutput()));
                }
                layout->removeItem(item);
                delete item->widget();
                delete item;
            }
        }

        void Window::reloadTest() {
            resetTest();
            Engine* engine = Model::Default()->engine();
            QLayout* layout = ui->grx_test_inputs->layout();
            for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
                Control* control = new Control;
                control->setup(engine->getInputVariable(i));
                control->ui->bottom_line->setVisible(i != engine->numberOfInputVariables() - 1);
                layout->addWidget(control);

                QObject::connect(control, SIGNAL(valueChanged(double)),
                        this, SLOT(onInputValueChanged()));

            }

            //Rules
            for (int i = 0; i < engine->getRuleBlock(0)->numberOfRules(); ++i) {
                QString number;
                if (i + 1 < 10 and engine->getRuleBlock(0)->numberOfRules() >= 10)
                    number = "0";
                number += QString::number(i + 1);

                QString rule = number + ": " +
                        QString::fromStdString(engine->getRuleBlock(0)->getRule(i)->getUnparsedRule());
                QListWidgetItem* item = new QListWidgetItem(rule);
                item->setToolTip(rule);
                ui->lsw_test_rules->addItem(item);

                item = new QListWidgetItem;
                item->setText("-");
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->lsw_test_rules_activation->addItem(item);
            }

            layout = ui->grx_test_outputs->layout();
            //Outputs
            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                Control* control = new Control;
                control->setup(engine->getOutputVariable(i));
                control->ui->bottom_line->setVisible(i != engine->numberOfOutputVariables() - 1);
                layout->addWidget(control);

                QObject::connect(this, SIGNAL(processOutput()),
                        control, SLOT(updateOutput()), Qt::QueuedConnection);
            }
        }

        void Window::removeRules() {
            ui->ptx_rules->clear();
            RuleBlock* ruleblock = Model::Default()->engine()->getRuleBlock(0);
            for (int i = ruleblock->numberOfRules() - 1; i >= 0; --i) {
                delete ruleblock->removeRule(i);
            }
            reloadModel();
        }

        void Window::fixDependencies() {
            QString rules = ui->ptx_rules->toPlainText();
            if (rules.isEmpty()) return;
            removeRules();
            ui->ptx_rules->setPlainText(rules);
            onClickParseAllRules();
        }

        void Window::resizeEvent(QResizeEvent*) {
            //            FL_LOG("resizing Window");
        }

        void Window::showEvent(QShowEvent*) {
            //            FL_LOG("showing Window");
        }

        /**
         * Events
         */
        void Window::onChangeInputSelection() {
            ui->btn_remove_input->setEnabled(
                    ui->lvw_inputs->selectedItems().size() > 0);
            ui->btn_edit_input->setEnabled(
                    ui->lvw_inputs->selectedItems().size() > 0);
        }

        void Window::onChangeOutputSelection() {
            ui->btn_remove_output->setEnabled(
                    ui->lvw_outputs->selectedItems().size() > 0);
            ui->btn_edit_output->setEnabled(
                    ui->lvw_outputs->selectedItems().size() > 0);
        }

        void Window::onDoubleClickInputItem(QListWidgetItem* item) {
            (void) item;
            ui->btn_edit_input->click();
        }

        void Window::onDoubleClickOutputItem(QListWidgetItem* item) {
            (void) item;
            ui->btn_edit_output->click();
        }

        void Window::onSelectTestRule(int selected) {
            ui->lsw_test_rules_activation->item(selected)->setSelected(true);
        }

        void Window::onSelectTestActivation(int selected) {
            ui->lsw_test_rules->item(selected)->setSelected(true);
        }

        void Window::onInputValueChanged() {


            QColor from_color(Qt::white);
            QColor to_color(0, 200, 0);
            fl::RuleBlock* ruleblock = Model::Default()->engine()->getRuleBlock(0);
            for (int i = 0; i < ruleblock->numberOfRules(); ++i) {
                fl::Rule* rule = ruleblock->getRule(i);
                scalar degree = rule->firingStrength(ruleblock->getTnorm(),
                        ruleblock->getSnorm());
                if (not fl::Op::isInf(degree) and not fl::Op::isNan(degree)) {
                    int red, green, blue, alpha;
                    Viewer::ColorGradient((int) (degree * 255), red, green, blue, alpha,
                            from_color.red(), from_color.green(), from_color.blue(), from_color.alpha(),
                            to_color.red(), to_color.green(), to_color.blue(), to_color.alpha());

                    QColor color = QColor(red, green, blue, alpha);

                    ui->lsw_test_rules->item(i)->setBackground(QBrush(color));
                    ui->lsw_test_rules_activation->item(i)->setBackground(QBrush(
                            color));
                    ui->lsw_test_rules_activation->item(i)->setText(
                            QString::number(degree, 'f', 3));
                }
            }

            Model::Default()->engine()->process();
            emit(processOutput());
        }

        /**
         * Button actions
         */

        void Window::onClickAddInputVariable() {
            Variable* window = new Variable(this);
            window->setup(Variable::INPUT_VARIABLE);
            if (window->exec()) {
                Model::Default()->engine()->addInputVariable(
                        dynamic_cast<InputVariable*> (window->variable));
                reloadModel();
            }
            delete window;
        }

        void Window::onClickRemoveInputVariable() {
            std::ostringstream message;
            message << "Do you want to delete the following input variable";
            std::string title;
            if (ui->lvw_inputs->selectedItems().size() > 1) {
                title = "Delete multiple input variables";
                message << "s";
            } else {
                title = "Delete input variable";
            }
            message << "?\n\n";


            Engine* engine = Model::Default()->engine();
            for (int i = 0; i < ui->lvw_inputs->count(); ++i) {
                if (ui->lvw_inputs->item(i)->isSelected()) {
                    message << "<" << engine->getInputVariable(i)->getName() << ">: "
                            << engine->getInputVariable(i)->toString();
                    if (i < ui->lvw_inputs->selectedItems().size() - 1) message << "\n\n";
                }
            }



            QMessageBox::StandardButton clicked = QMessageBox::warning(this,
                    QString::fromStdString(title),
                    QString::fromStdString(message.str()),
                    QMessageBox::Yes | QMessageBox::No);
            if (clicked == QMessageBox::Yes) {
                for (int i = ui->lvw_inputs->count() - 1; i >= 0; --i) {
                    if (ui->lvw_inputs->item(i)->isSelected()) {
                        delete engine->removeInputVariable(i);
                        fixDependencies();
                    }
                }
                reloadModel();
            }
        }

        void Window::onClickEditInputVariable() {
            Engine* engine = Model::Default()->engine();
            if (ui->lvw_inputs->selectedItems().size() > 1) {
                std::ostringstream message;
                message << "Do you want to edit the following input variables?\n\n";
                for (int i = 0; i < ui->lvw_inputs->count(); ++i) {
                    if (ui->lvw_inputs->item(i)->isSelected()) {
                        message << "<" << engine->getInputVariable(i)->getName() << ">: "
                                << engine->getInputVariable(i)->toString();
                        if (i < ui->lvw_inputs->selectedItems().size() - 1) message << "\n\n";
                    }
                }
                QMessageBox::StandardButton clicked = QMessageBox::information(this,
                        "Edit multiple variables",
                        QString::fromStdString(message.str()),
                        QMessageBox::Yes | QMessageBox::No);
                if (clicked == QMessageBox::No) {
                    return;
                }
            }

            for (int i = 0; i < ui->lvw_inputs->count(); ++i) {
                if (ui->lvw_inputs->item(i)->isSelected()) {
                    Variable* window = new Variable(this);
                    window->setup(Variable::INPUT_VARIABLE);
                    window->edit(engine->getInputVariable(i));
                    if (window->exec()) {
                        delete engine->removeInputVariable(i);
                        engine->insertInputVariable(
                                dynamic_cast<InputVariable*> (window->variable),
                                i);
                        fixDependencies();
                    }
                }
            }
            reloadModel();

        }

        void Window::onClickAddOutputVariable() {
            Variable* window = new Variable(this);
            window->setup(Variable::OUTPUT_VARIABLE);
            if (window->exec()) {
                Model::Default()->engine()->addOutputVariable(
                        dynamic_cast<OutputVariable*> (window->variable));
                reloadModel();
            }
            delete window;
        }

        void Window::onClickRemoveOutputVariable() {
            std::ostringstream message;
            message << "Do you want to delete the following output variable";
            std::string title;
            if (ui->lvw_outputs->selectedItems().size() > 1){
                title = "Delete multiple output variables";
                message << "s";
            }
            else title = "Delete output variable";
            message << "?\n\n";
            
            Engine* engine = Model::Default()->engine();
            for (int i = 0; i < ui->lvw_outputs->count(); ++i) {
                if (ui->lvw_outputs->item(i)->isSelected()) {
                    message << "<" << engine->getOutputVariable(i)->getName() << ">: "
                            << engine->getOutputVariable(i)->toString();
                    if (i < ui->lvw_outputs->selectedItems().size() - 1) message << "\n\n";
                }
            }
            
            QMessageBox::StandardButton clicked = QMessageBox::warning(this,
                    QString::fromStdString(title),
                    QString::fromStdString(message.str()),
                    QMessageBox::Yes | QMessageBox::No);
            if (clicked == QMessageBox::Yes) {
                for (int i = ui->lvw_outputs->count() - 1; i >= 0; --i) {
                    if (ui->lvw_outputs->item(i)->isSelected()) {
                        delete engine->removeOutputVariable(i);
                        fixDependencies();
                    }
                }
                reloadModel();
            }
        }

        void Window::onClickEditOutputVariable() {
            Engine* engine = Model::Default()->engine();
            if (ui->lvw_outputs->selectedItems().size() > 1) {
                std::ostringstream message;
                message << "Do you want to edit the following output variables?\n\n";
                for (int i = 0; i < ui->lvw_outputs->count(); ++i) {
                    if (ui->lvw_outputs->item(i)->isSelected()) {
                        message << "<" << engine->getOutputVariable(i)->getName() << ">: "
                                << engine->getOutputVariable(i)->toString();
                        if (i < ui->lvw_outputs->selectedItems().size() - 1) message << "\n\n";
                    }
                }
                QMessageBox::StandardButton clicked = QMessageBox::information(this,
                        "Edit multiple variables",
                        QString::fromStdString(message.str()),
                        QMessageBox::Yes | QMessageBox::No);
                if (clicked == QMessageBox::No) {
                    return;
                }
            }

            for (int i = 0; i < ui->lvw_outputs->count(); ++i) {
                if (ui->lvw_outputs->item(i)->isSelected()) {
                    Variable* window = new Variable(this);
                    window->setup(Variable::OUTPUT_VARIABLE);
                    window->edit(engine->getOutputVariable(i));
                    if (window->exec()) {
                        delete engine->removeOutputVariable(i);
                        engine->insertOutputVariable(
                                dynamic_cast<OutputVariable*> (window->variable),
                                i);
                        fixDependencies();
                    }
                }
            }
            reloadModel();
        }

        void Window::onClickGenerateAllRules() {
            removeRules();
            Engine* engine = Model::Default()->engine();

            int numberOfRules = 1;
            for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
                numberOfRules *= engine->getInputVariable(i)->numberOfTerms();
            }
            std::vector<int> terms(engine->numberOfInputVariables(), 0);

            for (int nRule = 0; nRule < numberOfRules; ++nRule) {
                std::ostringstream rule;
                rule << fl::Rule::ifKeyword() << " ";
                for (int input = 0; input < engine->numberOfInputVariables();
                        ++input) {
                    fl::Variable* var = engine->getInputVariable(input);
                    rule << var->getName() << " " << fl::Rule::isKeyword() << " "
                            << var->getTerm(terms[input])->getName() << " ";
                    if (input < engine->numberOfInputVariables() - 1) {
                        rule << fl::Rule::andKeyword() << " ";
                    }
                }
                rule << fl::Rule::thenKeyword();
                for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                    rule << " " << engine->getOutputVariable(i)->getName() << " "
                            << fl::Rule::isKeyword() << " ?";
                    if (i < engine->numberOfOutputVariables() - 1) {
                        rule << " " << fl::Rule::andKeyword() << " ";
                    }
                }
                ui->ptx_rules->appendPlainText(QString::fromStdString(rule.str()));
                //Increment...
                bool change = true;
                for (int i = engine->numberOfInputVariables() - 1; i >= 0;
                        --i) {
                    if (change) {
                        if (terms[i] + 1 >= engine->getInputVariable(i)->numberOfTerms()) {
                            terms[i] = 0;
                            change = true;
                        } else {
                            ++terms[i];
                            change = false;
                        }
                    }
                }
            }
        }

        void Window::onClickParseAllRules() {
            QStringList rules = ui->ptx_rules->toPlainText().split("\n",
                    QString::SkipEmptyParts);
            removeRules();
            Engine* engine = Model::Default()->engine();
            RuleBlock* ruleblock = Model::Default()->engine()->getRuleBlock(0);
            int goodRules = 0, badRules = 0;
            for (int i = 0; i < rules.size(); ++i) {
                if (rules[i].trimmed().startsWith("#")) continue;
                try {
                    ruleblock->addRule(MamdaniRule::parse(rules[i].toStdString(), engine));
                    ui->ptx_rules->appendHtml("<font color='green'>" +
                            Qt::escape(rules[i]) + "</font>");
                    ++goodRules;
                } catch (fl::Exception& ex) {
                    ui->ptx_rules->appendHtml("<font color='red'>" +
                            Qt::escape(rules[i]) + "</font>");
                    ui->ptx_rules->appendHtml("<font color='gray'>#" +
                            Qt::escape(QString::fromStdString(ex.what()))
                            + "</font>");
                    ++badRules;
                }
            }
            ui->ptx_rules->appendHtml("<font color='gray'>"
                    + QString("#---------------------------------------------")
                    + "</font>");

            std::ostringstream message;
            message << "<font color='gray'>"
                    << "# Total rules: " << badRules + goodRules << ". "
                    << "</font>"
                    << "<font color='green'>"
                    << "Good Rules: " << goodRules << ". "
                    << "</font>"
                    << "<font color='red'>"
                    << "Bad Rules: " << badRules << "."
                    << "</font>";
            ui->ptx_rules->appendHtml("" +
                    QString::fromStdString(message.str()));
            if (goodRules > 0 and badRules == 0) {
                ui->ptx_rules->appendHtml("<font color='gray'>"
                        "# Rules successfully processed at "
                        + QDateTime::currentDateTime().toString("h:mm ap (dd/MM/yy)")
                        + "</font>");
                ui->ptx_rules->appendHtml("<font color='blue'>"
                        "# You may proceed to control the engine"
                        "</font>");

            }
            reloadTest();
        }

        void Window::onTabChange(int index) {
            if (index == 1) {
            }
        }

        void Window::onMenuPreferences() {
            preferences->hide();
            preferences->show();
        }

        void Window::onMenuTerms() {
            Term* window = new Term(this);
            window->setup(fl::Variable("Terms", 0, 1));
            window->setWindowTitle("Terms");
            window->ui->qfr_name->setVisible(false);
            window->ui->buttonBox->setVisible(false);
            window->show();

            //            delete window;

        }

        void Window::onMenuImport() {
            if (ui->actionImport->isChecked()) {
                QMenu menu(this);
                menu.addAction("from &file...", this, SLOT(onMenuImportFromFile()));
                menu.addSeparator();
                menu.addAction("Fuzzy Control Language (FC&L)", this, SLOT(onMenuImportFromFCL()));
                menu.addAction("Fuzzy Inference System (FI&S)", this, SLOT(onMenuImportFromFIS()));
                menu.exec(QCursor::pos() + QPoint(1, 0));
                ui->actionImport->setChecked(false);
            }
        }

        bool Window::confirmImporting() {
            Engine* engine = Model::Default()->engine();
            if (not (engine->numberOfInputVariables() or engine->numberOfOutputVariables())) {
                return true;
            }
            QMessageBox::StandardButton clicked =
                    QMessageBox::warning(this, "qtfuzzylite",
                    "Importing a fuzzy logic engine will reset the current engine. \n\n"
                    "Do you want to continue importing?",
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);

            return clicked == QMessageBox::Yes;
        }

        void Window::onMenuImportFromFCL() {
            Ui::ImEx fclUi;
            QDialog fclDialog(this);
            fclUi.setupUi(&fclDialog);
            fclDialog.setWindowTitle("Import...");
            fclUi.lbl_format->setText("Import from Fuzzy Controller Language (FCL):");
            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            fclUi.pte_code->setFont(font);

            if (fclDialog.exec()) {
                std::string fclString = fclUi.pte_code->document()->toPlainText().toStdString();
                Engine* engine = NULL;
                FclImporter importer;
                try {
                    engine = importer.fromString(fclString);
                    if (not confirmImporting()) {
                        delete engine;
                        return;
                    }
                    Model::Default()->change(engine);
                    reloadModel();
                    onClickParseAllRules();
                } catch (fl::Exception& ex) {
                    if (engine) delete engine;
                    QMessageBox::critical(this, "Error importing from FCL",
                            Qt::escape(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                            QMessageBox::Ok);

                    return;
                }
            }
        }

        void Window::onMenuImportFromFIS() {
            Ui::ImEx fisUi;
            QDialog fclDialog(this);
            fisUi.setupUi(&fclDialog);
            fclDialog.setWindowTitle("Import...");
            fisUi.lbl_format->setText("Import from Fuzzy Inference System (FIS):");
            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            fisUi.pte_code->setFont(font);

            if (fclDialog.exec()) {
                std::string fclString = fisUi.pte_code->document()->toPlainText().toStdString();
                Engine* engine = NULL;
                FisImporter importer;
                try {
                    engine = importer.fromString(fclString);
                    if (not confirmImporting()) {
                        delete engine;
                        return;
                    }
                    Model::Default()->change(engine);
                    reloadModel();
                    onClickParseAllRules();
                } catch (fl::Exception& ex) {
                    QMessageBox::critical(this, "Error importing from FIS",
                            Qt::escape(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                            QMessageBox::Ok);
                    return;
                }

            }
        }

        void Window::onMenuImportFromFile() {
            QString filename = QFileDialog::getOpenFileName(this,
                    "Import fuzzy logic engine from file", _lastOpenedFilePath,
                    "Supported formats (*.fcl *.fis);;"
                    "Fuzzy Logic Controller (*.fcl);;"
                    "Fuzzy Inference System (*.fis)");

            if (filename.size() == 0) return;
            _lastOpenedFilePath = QFileInfo(filename).path();

            enum Format {
                FCL, FIS, UNSUPPORTED
            } format;

            if (filename.endsWith("fcl", Qt::CaseInsensitive)) format = FCL;
            else if (filename.endsWith(".fis", Qt::CaseInsensitive)) format = FIS;
            else format = UNSUPPORTED;

            if (format == UNSUPPORTED) {
                QMessageBox::critical(this, "qtfuzzylite",
                        "Unsupported file for a fuzzy logic engine: " + filename
                        + ". The only supported files are *.fcl and *.fis",
                        QMessageBox::Ok);
                return;
            }

            QFile file(filename);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::critical(this, "qtfuzzylite",
                        "Error opening file: " + filename,
                        QMessageBox::Ok);
                return;
            }
            QTextStream in(&file);
            std::ostringstream reader;
            while (!in.atEnd()) {
                reader << in.readLine().toStdString() << "\n";
            }

            Engine* engine = NULL;
            Importer* importer = NULL;
            if (format == FCL) importer = new FclImporter;
            else if (format == FIS) importer = new FisImporter;
            try {
                engine = importer->fromString(reader.str());
                if (not confirmImporting()) {
                    delete engine;
                    delete importer;
                    return;
                }
                Model::Default()->change(engine);
                reloadModel();
                onClickParseAllRules();
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error importing from FIS",
                        Qt::escape(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                delete importer;
                delete engine;
                return;
            }
            delete importer;
        }

        void Window::onMenuExport() {
            if (ui->actionExport->isChecked()) {
                QMenu menu(this);
                menu.addAction("fuzzylite (&C++)", this, SLOT(onMenuExportToCpp()));
                menu.addSeparator();
                menu.addAction("Fuzzy Control Language (FC&L)", this, SLOT(onMenuExportToFCL()));
                menu.addAction("Fuzzy Inference System (FI&S)", this, SLOT(onMenuExportToFIS()));
                menu.exec(QCursor::pos() + QPoint(1, 0));
                ui->actionExport->setChecked(false);
            }
        }

        void Window::onMenuExportToFCL() {
            FclExporter exporter;
            std::string fclString;
            try {
                fclString = exporter.toString(Model::Default()->engine());
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error exporting to FCL",
                        Qt::escape(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                return;
            }
            Ui::ImEx fclUi;
            QDialog fclDialog(this);
            fclUi.setupUi(&fclDialog);
            fclUi.buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);
            fclDialog.setWindowTitle("Export...");
            fclUi.lbl_format->setText("Export to Fuzzy Controller Language (FCL):");
            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            fclUi.pte_code->setFont(font);
            fclUi.pte_code->setReadOnly(true);
            fclUi.pte_code->document()->setPlainText(
                    QString::fromStdString(fclString));
            QTextCursor tc = fclUi.pte_code->textCursor();
            tc.movePosition(QTextCursor::Start);
            fclUi.pte_code->setTextCursor(tc);
            fclDialog.exec();
        }

        void Window::onMenuExportToFIS() {
            FisExporter exporter;
            std::string fis;
            try {
                fis = exporter.toString(Model::Default()->engine());
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error exporting to FIS",
                        Qt::escape(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                return;
            }
            Ui::ImEx fisUi;
            QDialog fclDialog(this);
            fisUi.setupUi(&fclDialog);
            fisUi.buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);
            fclDialog.setWindowTitle("Export...");
            fisUi.lbl_format->setText("Export to Fuzzy Inference System (FIS):");
            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            fisUi.pte_code->setFont(font);
            fisUi.pte_code->setReadOnly(true);
            fisUi.pte_code->document()->setPlainText(
                    QString::fromStdString(fis));
            QTextCursor tc = fisUi.pte_code->textCursor();
            tc.movePosition(QTextCursor::Start);
            fisUi.pte_code->setTextCursor(tc);
            fclDialog.exec();
        }

        void Window::onMenuExportToCpp() {
            CppExporter exporter;
            std::string cpp;
            try {
                cpp = exporter.toString(Model::Default()->engine());
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error exporting to fuzzylite (C++)",
                        Qt::escape(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                return;
            }
            Ui::ImEx cppUi;
            QDialog fclDialog(this);
            cppUi.setupUi(&fclDialog);
            cppUi.buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);
            fclDialog.setWindowTitle("Export...");
            cppUi.lbl_format->setText("Export to fuzzylite (C++):");
            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            cppUi.pte_code->setFont(font);
            cppUi.pte_code->setReadOnly(true);
            cppUi.pte_code->document()->setPlainText(
                    QString::fromStdString(cpp));
            QTextCursor tc = cppUi.pte_code->textCursor();
            tc.movePosition(QTextCursor::Start);
            cppUi.pte_code->setTextCursor(tc);
            fclDialog.exec();
        }

        void Window::onMenuNew() {
            QMessageBox::StandardButton clicked =
                    QMessageBox::question(this, "New engine",
                    "Do you want to create a new engine?",
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);

            if (clicked == QMessageBox::No) return;

            Model::Default()->reset();

            reloadModel();
        }

        void Window::onMenuAbout() {
            About about;
            about.setup();
            about.exec();
        }

        void Window::onMenuQuit() {
            this->close();
        }

        void Window::closeEvent(QCloseEvent * e) {
            int result = QMessageBox::question(this, "Quit",
                    "<qt>Do you want to quit <b>qtfuzzylite</b>?</qt>",
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);
            if (result == QMessageBox::No) {
                e->ignore();
            } else {
                e->accept();
            }
        }

        QFont Window::typeWriterFont() const {

#ifdef Q_WS_X11
            QFont tt("Ubuntu Mono");
            tt.setStyleHint(QFont::TypeWriter);
            return tt;
#endif

#ifdef Q_WS_MAC
            QFont tt("Monaco");
            tt.setStyleHint(QFont::TypeWriter);
            return tt;
#endif

#ifdef Q_WS_WIN
            QFont tt("Fixedsys");
            tt.setStyleHint(QFont::TypeWriter);
            return tt;
#endif
        }

        void Window::main() {
            Window* w = mainWindow();
            w->setup();
            w->onMenuAbout();
            w->show();
        }

    }
}
