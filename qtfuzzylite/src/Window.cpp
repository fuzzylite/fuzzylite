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

//TODO: Change most QSettings properties to [General]
//TODO: Update forums and links.

#include "fl/qt/Window.h"
#include "fl/qt/About.h"
#include "fl/qt/ImEx.h"
#include "fl/qt/Term.h"
#include "fl/qt/Variable.h"
#include "fl/qt/Model.h"
#include "fl/qt/Control.h"
#include "fl/qt/Surface2D.h"

#include "fl/qt/qtfuzzylite.h"

#include <QListWidgetItem>
#include <QScrollBar>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QMenu>
#include <QSignalMapper>
#include <QDateTime>
#include <QMenuBar>
#include <QThread>
#include <QSplashScreen>
#include <QSettings>
#include <QDesktopServices>
#include <QUrl>
#include <QInputDialog>

#include <fstream>

#ifdef FL_UNIX
#include <unistd.h>
#elif defined FL_WINDOWS
#include <windows.h>
#endif


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
        QMainWindow(parent, flags), _recentFiles(NULL), _currentFile(""),
        _currentFileModified(false), ui(new Ui::Window) {
        }

        Window::~Window() {
            if (_inputViewer) delete _inputViewer;
            if (_outputViewer) delete _outputViewer;
            delete ui;
        }

        void Window::setup() {
            ui->setupUi(this);

            updateWindowTitle();

            setupMenuAndToolbar();

            setGeometry(0, 0, 800, 600);
            QRect scr = QApplication::desktop()->screenGeometry();
            move(scr.center() - rect().center());

            ui->tab_container->setCurrentIndex(0);

            QList<int> sizes;
            sizes << .75 * size().width() << .25 * size().width();
            ui->spl_control_inout_rules->setSizes(sizes);
            sizes.clear();
            sizes << .90 * size().width() << .10 * size().width();
            ui->spl_control_rule_strength->setSizes(sizes);

            std::vector<std::string> tnorms = FactoryManager::instance()->tnorm()->available();
            ui->cbxTnorm->addItem("");
            ui->cbxActivation->addItem("");
            for (std::size_t i = 0; i < tnorms.size(); ++i) {
                ui->cbxTnorm->addItem(QString::fromStdString(tnorms.at(i)));
                ui->cbxActivation->addItem(QString::fromStdString(tnorms.at(i)));
            }

            std::vector<std::string> snorms = FactoryManager::instance()->snorm()->available();
            ui->cbxSnorm->addItem("");
            for (std::size_t i = 0; i < snorms.size(); ++i) {
                ui->cbxSnorm->addItem(QString::fromStdString(snorms.at(i)));
            }

            ui->btn_inputs->setEnabled(false);
            ui->btn_outputs->setEnabled(false);

            ui->scrollAreaInput->viewport()->installEventFilter(new fl::qt::QScrollAreaFilter);
            ui->scrollAreaOutput->viewport()->installEventFilter(new fl::qt::QScrollAreaFilter);

            ui->lvw_inputs->viewport()->installEventFilter(
                    new fl::qt::VariableContextMenu(ui->lvw_inputs, "input"));
            ui->lvw_outputs->viewport()->installEventFilter(
                    new fl::qt::VariableContextMenu(ui->lvw_outputs, "output"));


            connect();
        }

        void Window::setCurrentFile(bool modified, const QString* filename) {
            this->_currentFileModified = modified;
            if (filename) this->_currentFile = *filename;
            updateWindowTitle();
        }

        void Window::updateWindowTitle() {
            QString title = "qtfuzzylite - ";
            if (_currentFile.isEmpty()) title += "untitled";
            else title += QFileInfo(_currentFile).fileName();
            if (_currentFileModified) title += "*";

            setWindowTitle(title);

            ui->actionReload->setEnabled(not _currentFile.isEmpty());
            ui->actionNew->setEnabled(not _currentFile.isEmpty() or _currentFileModified);
            ui->actionSave->setEnabled(_currentFileModified and not _currentFile.isEmpty());
        }

        void Window::setupMenuAndToolbar() {

            setUnifiedTitleAndToolBarOnMac(true);
            setContextMenuPolicy(Qt::NoContextMenu);

            QMenu* menuFile = new QMenu("&File", this);

            menuFile->addAction(ui->actionNew);
            QObject::connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(onMenuNew()));

            menuFile->addSeparator();

            menuFile->addAction(ui->actionOpen);
            QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onMenuOpen()));

            _recentFiles = new QMenu("Open re&cent", menuFile);
            _recentFiles->setIcon(QIcon(":/open-recent.png"));
            updateRecentFiles();
            menuFile->addMenu(_recentFiles);

            menuFile->addSeparator();

            menuFile->addAction(ui->actionSave);
            QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(onMenuSave()));
            menuFile->addAction(ui->actionSaveAs);
            QObject::connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(onMenuSaveAs()));
            menuFile->addAction(ui->actionReload);
            QObject::connect(ui->actionReload, SIGNAL(triggered()), this, SLOT(onMenuReload()));
            menuFile->addSeparator();

            QMenu* menuImport = new QMenu("&Import from...", menuFile);
            menuImport->setIcon(QIcon(":/import.png"));
            QObject::connect(ui->actionImport, SIGNAL(triggered()), this, SLOT(onMenuImport()));

            menuImport->addAction("FuzzyLite Language (&FLL)", this, SLOT(onMenuImportFromFLL()));
            menuImport->addAction("Fuzzy Controller Language (FC&L)", this, SLOT(onMenuImportFromFCL()));
            menuImport->addAction("Fuzzy Inference System (FI&S)", this, SLOT(onMenuImportFromFIS()));

            menuFile->addMenu(menuImport);

            QMenu* menuExport = new QMenu("&Export to...", menuFile);
            menuExport->setIcon(QIcon(":/export.png"));
            QObject::connect(ui->actionExport, SIGNAL(triggered()), this, SLOT(onMenuExport()));

            menuExport->addAction("&FuzzyLite Language (FLL)", this, SLOT(onMenuExportToFLL()));
            menuExport->addAction("Fuzzy Controller &Language (FCL)", this, SLOT(onMenuExportToFCL()));
            menuExport->addAction("Fuzzy Inference &System (FIS)", this, SLOT(onMenuExportToFIS()));
            menuExport->addSeparator();
            menuExport->addAction("FuzzyLite Dataset (FLD) vie&w", this, SLOT(onMenuExportToDatasetView()));
            menuExport->addAction("FuzzyLite Dataset (FLD) fil&e", this, SLOT(onMenuExportToDatasetFile()));
            menuExport->addSeparator();
            menuExport->addAction("fuzzylite (&C++)", this, SLOT(onMenuExportToCpp()));
            menuExport->addAction("jfuzzylite (&Java)", this, SLOT(onMenuExportToJava()));
            menuFile->addMenu(menuExport);

            menuFile->addSeparator();

            menuFile->addAction(ui->actionQuit);
            QObject::connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(onMenuQuit()));

            ui->menuBar->addMenu(menuFile);

            QMenu* menuTools = new QMenu("&Tools", this);
            menuTools->addAction(ui->actionTerms);
            QObject::connect(ui->actionTerms, SIGNAL(triggered()), this, SLOT(onMenuTerms()));

            menuTools->addSeparator();

            menuTools->addAction(ui->actionSurface2D);
            QObject::connect(ui->actionSurface2D, SIGNAL(triggered()), this, SLOT(onMenuSurface2D()));

            ui->menuBar->addMenu(menuTools);

            QMenu* menuHelp = new QMenu("&Help", this);

            menuHelp->addAction(ui->actionAskForHelp);
            QObject::connect(ui->actionAskForHelp, SIGNAL(triggered()), this, SLOT(onMenuAskForHelp()));

//            menuHelp->addAction(ui->actionJoinTheCommunity);
//            QObject::connect(ui->actionJoinTheCommunity, SIGNAL(triggered()), this, SLOT(onMenuJoinTheCommunity()));

//            menuHelp->addAction(ui->actionFollowOnTwitter);
//            QObject::connect(ui->actionFollowOnTwitter, SIGNAL(triggered()), this, SLOT(onMenuFollowOnTwitter()));

            menuHelp->addSeparator();
            menuHelp->addAction(ui->actionCheckForUpdates);
            QObject::connect(ui->actionCheckForUpdates, SIGNAL(triggered()), this, SLOT(onMenuCheckForUpdates()));
            menuHelp->addSeparator();

            menuHelp->addAction(ui->actionAbout);
            QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onMenuAbout()));

            menuHelp->addAction(ui->actionAboutQt);
            QObject::connect(ui->actionAboutQt, SIGNAL(triggered()), this, SLOT(onMenuAboutQt()));

            ui->menuBar->addMenu(menuHelp);

            QWidget* widget = new QWidget;
            QHBoxLayout* spacerLayout = new QHBoxLayout;
            QSpacerItem* spacer = new QSpacerItem(1, 1,
                    QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
            spacerLayout->addSpacerItem(spacer);
            widget->setLayout(spacerLayout);
            ui->toolBar->insertWidget(ui->toolBar->actions().last(), widget);
        }

        void Window::connect() {
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

            QObject::connect(ui->cbxTnorm, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onSelectTnorm(int)));
            QObject::connect(ui->cbxSnorm, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onSelectSnorm(int)));
            QObject::connect(ui->cbxActivation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onSelectActivation(int)));
            QObject::connect(ui->btn_hedges, SIGNAL(clicked()),
                    this, SLOT(onClickHedges()));

            QObject::connect(ui->btn_inputs, SIGNAL(clicked()),
                    this, SLOT(onClickInputOutputButton()));
            QObject::connect(ui->btn_outputs, SIGNAL(clicked()),
                    this, SLOT(onClickInputOutputButton()));

            QObject::connect(ui->led_name, SIGNAL(textEdited(const QString&)),
                    this, SLOT(onChangeEngineName(const QString&)));

        }

        void Window::reloadModel() {
            Engine* engine = Model::Default()->engine();

            updateWindowTitle();

            ui->led_name->setText(QString::fromStdString(engine->getName()));

            QFont typeWriter = typeWriterFont();
            ui->ptx_rules->setFont(typeWriter);
            ui->lsw_test_rules->setFont(typeWriter);
            ui->lsw_test_rules_activation->setFont(typeWriter);

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
                        QString::fromStdString(ruleblock->getRule(i)->getText()));
            }

            if (ruleblock->getConjunction()) {
                ui->cbxTnorm->setCurrentIndex(
                        ui->cbxTnorm->findText(QString::fromStdString(
                        ruleblock->getConjunction()->className())));
            } else ui->cbxTnorm->setCurrentIndex(0);

            if (ruleblock->getDisjunction()) {
                ui->cbxSnorm->setCurrentIndex(
                        ui->cbxSnorm->findText(QString::fromStdString(
                        ruleblock->getDisjunction()->className())));
            } else ui->cbxSnorm->setCurrentIndex(0);

            if (ruleblock->getActivation()) {
                ui->cbxActivation->setCurrentIndex(
                        ui->cbxActivation->findText(QString::fromStdString(
                        ruleblock->getActivation()->className())));
            } else ui->cbxActivation->setCurrentIndex(0);

            reloadTest();
        }

        void Window::resetTest() {
            //Inputs
            ui->btn_inputs->setEnabled(false);
            ui->btn_outputs->setEnabled(false);
            QLayout* layout = ui->inputVariables->layout();

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

            //Outputs
            layout = ui->outputVariables->layout();
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

            //Rules
            ui->lsw_test_rules->clear();
            ui->lsw_test_rules_activation->clear();
        }

        void Window::reloadTest() {
            resetTest();
            Engine* engine = Model::Default()->engine();

            QVBoxLayout* layout = dynamic_cast<QVBoxLayout*> (ui->inputVariables->layout());
            for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
                Control* control = new Control(ui->inputVariables);
                InputVariable* inputVariable = engine->getInputVariable(i);
                inputVariable->setInputValue(inputVariable->getMinimum());
                control->setup(inputVariable);
                control->ui->bottom_line->setVisible(i != engine->numberOfInputVariables() - 1);
                layout->addWidget(control);


                QObject::connect(control, SIGNAL(valueChanged(double)),
                        this, SLOT(onInputValueChanged()));
            }
            ui->btn_inputs->setEnabled(engine->numberOfInputVariables() > 0);


            layout = dynamic_cast<QVBoxLayout*> (ui->outputVariables->layout());
            //Outputs
            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                Control* control = new Control(ui->outputVariables);
                control->setup(engine->getOutputVariable(i));
                control->setAllowOutputView(true);
                control->ui->bottom_line->setVisible(i != engine->numberOfOutputVariables() - 1);
                layout->addWidget(control);

                QObject::connect(this, SIGNAL(processOutput()),
                        control, SLOT(updateOutput()), Qt::QueuedConnection);
            }
            ui->btn_outputs->setEnabled(engine->numberOfOutputVariables() > 0);

            //Rules
            for (int i = 0; i < engine->getRuleBlock(0)->numberOfRules(); ++i) {
                QString number;
                if (i + 1 < 10 and engine->getRuleBlock(0)->numberOfRules() >= 10)
                    number = "0";
                number += QString::number(i + 1);

                QString rule = number + ": " +
                        QString::fromStdString(engine->getRuleBlock(0)->getRule(i)->getText());
                QListWidgetItem* item = new QListWidgetItem(rule);
                item->setToolTip(rule);
                ui->lsw_test_rules->addItem(item);

                item = new QListWidgetItem;
                item->setText("-");
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->lsw_test_rules_activation->addItem(item);
            }
            ui->inputVariables->adjustSize();
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

        void Window::fixVariableDependencies() {
            Engine* engine = Model::Default()->engine();
            std::vector<fl::Variable*> variables;
            for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
                variables.push_back(engine->getInputVariable(i));
            }
            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                variables.push_back(engine->getOutputVariable(i));
            }

            QString updatedReferences, errorMessages;
            for (std::size_t i = 0; i < variables.size(); ++i) {
                fl::Variable* variable = variables.at(i);
                for (int t = 0; t < variable->numberOfTerms(); ++t) {
                    fl::Term* term = variable->getTerm(t);
                    if (term->className() == Linear().className()) {
                        Linear* linear = dynamic_cast<Linear*> (term);
                        if (linear->coefficients.size() != engine->numberOfInputVariables() + 1) {
                            linear->coefficients.resize(engine->numberOfInputVariables() + 1);
                            updatedReferences += QString::fromStdString(
                                    "- " + variable->getName() + "::" +
                                    linear->getName() + " [" +
                                    linear->toString() + "]\n");
                        }
                        linear->inputVariables = std::vector<const InputVariable*>
                                (engine->inputVariables().begin(),
                                engine->inputVariables().end());

                    } else if (term->className() == Function().className()) {
                        Function* function = dynamic_cast<Function*> (term);
                        try {
                            function->load(function->getFormula(), engine);
                        } catch (fl::Exception& ex) {
                            updatedReferences += QString::fromStdString(
                                    "- " + variable->getName() + "::" +
                                    function->getName() + " [" +
                                    function->toString() + "]\n");
                            errorMessages += QString::fromStdString(
                                    "- " + variable->getName() + "::" +
                                    function->getName() + " " + ex.getWhat());
                        }
                    }
                }
            }

            if (not updatedReferences.isEmpty()) {
                QString message = "The following references have been updated due to "
                        "your recent changes:<br><br>"
                        + toHtmlEscaped(updatedReferences).replace("\n", "<br>");
                if (errorMessages.isEmpty())
                    QMessageBox::information(this, "References updated", message);
                else {
                    message += "<br>However, such changes led to the following errors "
                            "which you need to fix:<br><br>" + errorMessages.replace("\n", "<br>");
                    QMessageBox::critical(this, "Error updating references", message);
                }
            }
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


        void Window::onChangeEngineName(const QString& text) {
            (void) text;
            Model::Default()->engine()->setName(text.toStdString());
            setCurrentFile(true);
        }

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
            Engine* engine = Model::Default()->engine();

            std::string status;
            if (not engine->isReady(&status)) {
                QMessageBox::critical(this, "Engine not ready",
                        "<qt>The following errors were encountered:<br><br>" +
                        toHtmlEscaped(QString::fromStdString(status)).replace("\n", "<br>")
                        + "</qt>");

                return;
            }

            QColor from_color(Qt::white);
            QColor to_color(0, 200, 0);
            fl::RuleBlock* ruleblock = Model::Default()->engine()->getRuleBlock(0);
            for (int i = 0; i < ruleblock->numberOfRules(); ++i) {
                fl::Rule* rule = ruleblock->getRule(i);
                scalar degree = rule->activationDegree(ruleblock->getConjunction(),
                        ruleblock->getDisjunction());
                if (not fl::Op::isInf(degree) and not fl::Op::isNan(degree)) {
                    QColor color = Window::mainWindow()->gradient(degree * 255,
                            from_color, to_color);

                    ui->lsw_test_rules->item(i)->setBackground(QBrush(color));
                    ui->lsw_test_rules_activation->item(i)->setBackground(QBrush(
                            color));
                    ui->lsw_test_rules_activation->item(i)->setText(
                            QString::number(degree, 'f', 3));
                }
            }

            engine->process();
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
                setCurrentFile(true);
                fixVariableDependencies();
                QString rules = ui->ptx_rules->toPlainText();
                reloadModel();
                ui->ptx_rules->setPlainText(rules);
                onClickParseAllRules();
                emit engineVariableChanged();
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
                    message << "- " << engine->getInputVariable(i)->toString();
                    if (i + 1 < ui->lvw_inputs->selectedItems().size()) message << "\n\n";
                }
            }



            QMessageBox::StandardButton clicked = QMessageBox::warning(this,
                    QString::fromStdString(title),
                    QString::fromStdString(message.str()),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);
            if (clicked == QMessageBox::Yes) {
                for (int i = ui->lvw_inputs->count() - 1; i >= 0; --i) {
                    if (ui->lvw_inputs->item(i)->isSelected()) {
                        delete engine->removeInputVariable(i);
                    }
                }
                fixVariableDependencies();
                QString rules = ui->ptx_rules->toPlainText();
                fixDependencies();
                setCurrentFile(true);
                reloadModel();
                ui->ptx_rules->setPlainText(rules);
                onClickParseAllRules();

                emit engineVariableChanged();
            }
        }

        void Window::onClickEditInputVariable() {
            Engine* engine = Model::Default()->engine();
            if (ui->lvw_inputs->selectedItems().size() > 1) {
                std::ostringstream message;
                message << "Do you want to edit the following input variables?\n\n";
                for (int i = 0; i < ui->lvw_inputs->count(); ++i) {
                    if (ui->lvw_inputs->item(i)->isSelected()) {
                        message << "- " << engine->getInputVariable(i)->toString();
                        if (i + 1 < ui->lvw_inputs->selectedItems().size()) message << "\n\n";
                    }
                }
                QMessageBox::StandardButton clicked = QMessageBox::information(this,
                        "Edit multiple variables",
                        QString::fromStdString(message.str()),
                        QMessageBox::Yes | QMessageBox::No,
                        QMessageBox::Yes);
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
                                dynamic_cast<InputVariable*> (window->variable), i);
                    }
                    delete window;
                }
            }
            fixVariableDependencies();
            QString rules = ui->ptx_rules->toPlainText();
            fixDependencies();
            setCurrentFile(true);
            reloadModel();
            ui->ptx_rules->setPlainText(rules);
            onClickParseAllRules();

            emit engineVariableChanged();

        }

        void Window::onClickAddOutputVariable() {
            Variable* window = new Variable(this);
            window->setup(Variable::OUTPUT_VARIABLE);
            if (window->exec()) {
                Model::Default()->engine()->addOutputVariable(
                        dynamic_cast<OutputVariable*> (window->variable));

                fixVariableDependencies();

                setCurrentFile(true);
                QString rules = ui->ptx_rules->toPlainText();
                reloadModel();
                ui->ptx_rules->setPlainText(rules);
                onClickParseAllRules();

                emit engineVariableChanged();
            }
            delete window;
        }

        void Window::onClickRemoveOutputVariable() {
            std::ostringstream message;
            message << "Do you want to delete the following output variable";
            std::string title;
            if (ui->lvw_outputs->selectedItems().size() > 1) {
                title = "Delete multiple output variables";
                message << "s";
            } else title = "Delete output variable";
            message << "?\n\n";

            Engine* engine = Model::Default()->engine();
            for (int i = 0; i < ui->lvw_outputs->count(); ++i) {
                if (ui->lvw_outputs->item(i)->isSelected()) {
                    message << "- " << engine->getOutputVariable(i)->toString();
                    if (i + 1 < ui->lvw_outputs->selectedItems().size()) message << "\n\n";
                }
            }

            QMessageBox::StandardButton clicked = QMessageBox::warning(this,
                    QString::fromStdString(title),
                    QString::fromStdString(message.str()),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);
            if (clicked == QMessageBox::Yes) {
                for (int i = ui->lvw_outputs->count() - 1; i >= 0; --i) {
                    if (ui->lvw_outputs->item(i)->isSelected()) {
                        delete engine->removeOutputVariable(i);
                    }
                }
                fixVariableDependencies();
                QString rules = ui->ptx_rules->toPlainText();
                fixDependencies();
                setCurrentFile(true);
                reloadModel();
                ui->ptx_rules->setPlainText(rules);
                onClickParseAllRules();

                emit engineVariableChanged();
            }
        }

        void Window::onClickEditOutputVariable() {
            Engine* engine = Model::Default()->engine();
            if (ui->lvw_outputs->selectedItems().size() > 1) {
                std::ostringstream message;
                message << "Do you want to edit the following output variables?\n\n";
                for (int i = 0; i < ui->lvw_outputs->count(); ++i) {
                    if (ui->lvw_outputs->item(i)->isSelected()) {
                        message << "- " << engine->getOutputVariable(i)->toString();
                        if (i + 1 < ui->lvw_outputs->selectedItems().size()) message << "\n\n";
                    }
                }
                QMessageBox::StandardButton clicked = QMessageBox::information(this,
                        "Edit multiple variables",
                        QString::fromStdString(message.str()),
                        QMessageBox::Yes | QMessageBox::No,
                        QMessageBox::Yes);
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
                    }
                    delete window;
                }
            }
            fixVariableDependencies();
            QString rules = ui->ptx_rules->toPlainText();
            fixDependencies();
            setCurrentFile(true);
            reloadModel();
            ui->ptx_rules->setPlainText(rules);
            onClickParseAllRules();

            emit engineVariableChanged();
        }

        void Window::onClickGenerateAllRules() {
            removeRules();
            Engine* engine = Model::Default()->engine();
            setCurrentFile(true);
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
            setCurrentFile(true);
            Engine* engine = Model::Default()->engine();
            RuleBlock* ruleblock = Model::Default()->engine()->getRuleBlock(0);
            int goodRules = 0, badRules = 0;
            for (int i = 0; i < rules.size(); ++i) {
                if (rules[i].trimmed().startsWith("#")) continue;
                try {
                    ruleblock->addRule(Rule::parse(rules[i].toStdString(), engine));
                    ui->ptx_rules->appendHtml("<font color='green'>" +
                            toHtmlEscaped(rules[i]) + "</font>");
                    ++goodRules;
                } catch (fl::Exception& ex) {
                    ui->ptx_rules->appendHtml("<font color='red'>" +
                            toHtmlEscaped(rules[i]) + "</font>");
                    ui->ptx_rules->appendHtml("<font color='gray'>#" +
                            toHtmlEscaped(QString::fromStdString(ex.what()))
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
                        + QDateTime::currentDateTime().toString("h:mm.ss ap (dd/MM/yy)")
                        + "</font>");
                ui->ptx_rules->appendHtml("<font color='blue'>"
                        "# You may proceed to control the engine"
                        "</font>");

            }
            reloadTest();
        }

        void Window::onSelectTnorm(int selected) {
            TNorm* tnorm = FactoryManager::instance()->tnorm()->createInstance(
                    ui->cbxTnorm->currentText().toStdString());
            Model::Default()->engine()->getRuleBlock(0)->setConjunction(tnorm);
            _currentFileModified = true;
            updateWindowTitle();
        }

        void Window::onSelectSnorm(int selected) {
            SNorm* snorm = FactoryManager::instance()->snorm()->createInstance(
                    ui->cbxSnorm->currentText().toStdString());
            Model::Default()->engine()->getRuleBlock(0)->setDisjunction(snorm);
            _currentFileModified = true;
            updateWindowTitle();
        }

        void Window::onSelectActivation(int selected) {
            TNorm* tnorm = FactoryManager::instance()->tnorm()->createInstance(
                    ui->cbxActivation->currentText().toStdString());
            Model::Default()->engine()->getRuleBlock(0)->setActivation(tnorm);
            _currentFileModified = true;
            updateWindowTitle();
        }

        void Window::onClickHedges() {
            QSignalMapper signalMapper(this);
            QMenu menu;
            std::vector<QAction*> actions;

            QAction* all = new QAction("all", this);
            signalMapper.setMapping(all, all->text());
            actions.push_back(all);
            actions.push_back(NULL);

            std::vector<std::string> hedges = FactoryManager::instance()->hedge()->available();
            for (std::size_t i = 0; i < hedges.size(); ++i) {
                QAction* action = new QAction(this);
                action->setText(QString::fromStdString(hedges.at(i)));
                action->setCheckable(true);
                action->setChecked(Model::Default()->engine()->hasHedge(hedges.at(i)));
                actions.push_back(action);
            }

            actions.push_back(NULL);

            QAction* none = new QAction("none", this);
            signalMapper.setMapping(none, none->text());
            actions.push_back(none);

            for (std::size_t i = 0; i < actions.size(); ++i) {
                QAction* action = actions.at(i);
                if (action) {
                    signalMapper.setMapping(action, action->text());
                    QObject::connect(action, SIGNAL(triggered()),
                            &signalMapper, SLOT(map()));
                    menu.addAction(action);
                } else menu.addSeparator();
            }

            QObject::connect(&signalMapper, SIGNAL(mapped(const QString &)),
                    this, SLOT(onActionHedge(const QString &)));

            menu.exec(QCursor::pos() + QPoint(1, 0));
            for (std::size_t i = 0; i < actions.size(); ++i) {
                if (actions.at(i)) {
                    actions.at(i)->deleteLater();
                }
            }
            ui->btn_hedges->setChecked(false);
        }

        void Window::onActionHedge(const QString& action) {
            Engine* engine = Model::Default()->engine();

            if (action.toStdString() == "all") {
                std::vector<std::string> hedges = FactoryManager::instance()->
                        hedge()->available();
                for (std::size_t i = 0; i < hedges.size(); ++i) {
                    if (not engine->hasHedge(hedges.at(i))) {
                        engine->addHedge(FactoryManager::instance()->
                                hedge()->createInstance(hedges.at(i)));
                    }
                }
            } else if (action.toStdString() == "none") {
                for (int i = engine->numberOfHedges() - 1; i >= 0; --i) {
                    delete engine->removeHedge(i);
                }
            } else {

                if (engine->hasHedge(action.toStdString())) {
                    delete engine->removeHedge(action.toStdString());
                } else {
                    engine->addHedge(FactoryManager::instance()->
                            hedge()->createInstance(action.toStdString()));
                }
            }
            onClickParseAllRules();
        }

        void Window::onTabChange(int index) {
            if (index == 1) {
                ui->hly_rules_control->insertWidget(2, ui->qwd_cbx_norms);
            } else if (index == 0) {
                ui->hly_rules_setup->insertWidget(2, ui->qwd_cbx_norms);
            }
        }

        void Window::onClickInputOutputButton() {
            QToolButton* button = qobject_cast<QToolButton*>(sender());
            if (not button) return;
            QString type;
            QLayout* layout;
            if (button == ui->btn_inputs) {
                type = "input";
                layout = ui->inputVariables->layout();
            } else if (button == ui->btn_outputs) {
                type = "output";
                layout = ui->outputVariables->layout();
            }

            bool allMaximized = true;
            bool allMinimized = true;
            for (int i = 0; i < layout->count(); ++i) {
                QLayoutItem* item = layout->itemAt(i);
                Control* control = dynamic_cast<Control*> (item->widget());
                if (control) {
                    allMaximized &= control->isMaximizedViewer();
                    allMinimized &= control->isMinimizedViewer();
                }
            }

            QMenu menu(this);
            QAction* minimizeAll = new QAction(&menu);
            minimizeAll->setText("minimize all");
            minimizeAll->setData(type);
            QObject::connect(minimizeAll, SIGNAL(triggered()),
                    this, SLOT(onActionInputOutputButton()));

            QAction* maximizeAll = new QAction(&menu);
            maximizeAll->setText("maximize all");
            maximizeAll->setData(type);
            QObject::connect(maximizeAll, SIGNAL(triggered()),
                    this, SLOT(onActionInputOutputButton()));

            if (allMaximized) {
                menu.addAction(minimizeAll);
            } else if (allMinimized) {
                menu.addAction(maximizeAll);
            } else {
                menu.addAction(minimizeAll);
                menu.addAction(maximizeAll);
            }
            menu.exec(QCursor::pos() + QPoint(1, 0));
            button->setChecked(false);
        }

        void Window::onActionInputOutputButton() {
            QAction* action = qobject_cast<QAction*>(sender());
            if (not action) return;
            QLayout* layout;
            if (action->data().value<QString>() == "input") {
                layout = ui->inputVariables->layout();
            }
            if (action->data().value<QString>() == "output") {
                layout = ui->outputVariables->layout();
            }

            for (int i = 0; i < layout->count(); ++i) {
                QLayoutItem* item = layout->itemAt(i);
                Control* control = dynamic_cast<Control*> (item->widget());
                if (control) {
                    if (action->text() == "minimize all") {
                        if (control->isMaximizedViewer()) control->minimizeViewer();
                    } else if (action->text() == "maximize all") {
                        if (control->isMinimizedViewer()) control->maximizeViewer();
                    } else throw fl::Exception("[internal error] unexpected action" +
                            action->text().toStdString(), FL_AT);
                }
            }

        }

        /**
         * Toolbar events
         **/

        bool Window::confirmChanges(const QString& before) {
            if (not _currentFileModified) return true;

            QString currentFilename;
            if (_currentFile.isEmpty()) currentFilename = "untitled";
            else currentFilename = QFileInfo(_currentFile).fileName();

            QMessageBox::StandardButton clicked =
                    QMessageBox::warning(this, before,
                    "Any unsaved changes to "
                    "\"" + currentFilename + "\""
                    " will be lost.<br><br>"
                    "Do you want to continue?",
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);
            return (clicked == QMessageBox::Yes);
        }

        void Window::onMenuNew() {
            if (_currentFileModified) {
                QString name = QFileInfo(_currentFile).fileName();
                if (name.isEmpty()) name = "untitled";
                QMessageBox::StandardButton clicked =
                        QMessageBox::warning(this, "New engine",
                        "Any unsaved changes to "
                        "\"" + name + "\""
                        " will be lost.<br><br>"
                        "Do you want to continue?",
                        QMessageBox::Yes | QMessageBox::No,
                        QMessageBox::Yes);
                if (clicked == QMessageBox::No) return;
            }

            Model::Default()->reset();
            reloadModel();
            QString empty;
            setCurrentFile(false, &empty);
        }

        void Window::onMenuOpen() {
            QSettings settings;
            QString recentLocation = settings.value("file/recentLocation", ".").toString();
            QString recentFilter = settings.value("file/recentFilter").toString();
            QStringList formats;
            formats << "Supported formats (*.fll *.fcl *.fis)"
                    << "FuzzyLite Language (*.fll)"
                    << "Fuzzy Logic Controller (*.fcl)"
                    << "Fuzzy Inference System (*.fis)"
                    << "All files (*.*)";
            int recentFilterIndex = formats.indexOf(recentFilter);
            if (recentFilterIndex < 0) recentFilterIndex = 0;
            QString filter = formats.at(recentFilterIndex);
            QString filename = QFileDialog::getOpenFileName(this,
                    "Open Engine", recentLocation,
                    formats.join(";;"),
                    &filter);
            settings.setValue("file/recentFilter", filter);
            if (filename.size() == 0) return;
            openFile(filename);
        }

        void Window::onMenuOpenRecent() {
            QAction* action = qobject_cast<QAction *>(sender());
            if (action) openFile(action->data().toString());
        }

        void Window::openFile(const QString& filename, const QString& unsavedChangesTitle) {
            QSettings settings;
            settings.setValue("file/recentLocation", QFileInfo(filename).path());

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

            enum Format {
                FLL, FCL, FIS, UNSUPPORTED
            } format;

            if (filename.endsWith(".fll", Qt::CaseInsensitive)) format = FLL;
            else if (filename.endsWith(".fcl", Qt::CaseInsensitive)) format = FCL;
            else if (filename.endsWith(".fis", Qt::CaseInsensitive)) format = FIS;
            else {
                QSettings settings;
                QString recentFormat = settings.value("file/recentFormat").toString();
                bool ok;
                QStringList formats;
                formats << "FuzzyLite Language (*.fll)"
                        << "Fuzzy Controller Language (*.fcl)"
                        << "Fuzzy Inference System (*.fis)";
                int recentFormatIndex = formats.indexOf(recentFormat);
                if (recentFormatIndex < 0) recentFormatIndex = 0;
                QString selectedFormat = QInputDialog::getItem(this, "File format",
                        "Please select the format of the file",
                        formats, recentFormatIndex, false, &ok);
                if (not ok) return;
                settings.setValue("file/recentFormat", selectedFormat);

                if (selectedFormat == formats.at(0)) {
                    format = FLL;
                } else if (selectedFormat == formats.at(1)) {
                    format = FCL;
                } else if (selectedFormat == formats.at(2)) {
                    format = FIS;
                } else {
                    format = UNSUPPORTED;
                }
            }

            Engine* engine = NULL;
            Importer* importer = NULL;
            try {
                if (format == FLL) importer = new FllImporter;
                else if (format == FCL) importer = new FclImporter;
                else if (format == FIS) importer = new FisImporter;
                else throw fl::Exception("[import error] Unsupported filetype for file: "
                        + filename.toStdString(), FL_AT);

                engine = importer->fromString(reader.str());

                addRecentFile(filename);
                updateRecentFiles();

                bool doOpen = true;
                if (_currentFileModified) {
                    QString name = QFileInfo(_currentFile).fileName();
                    if (name.isEmpty())name = "untitled";
                    QMessageBox::StandardButton clicked =
                            QMessageBox::warning(this, unsavedChangesTitle,
                            "Any unsaved changes to "
                            "\"" + name + "\""
                            " will be lost.<br><br>"
                            "Do you want to continue?",
                            QMessageBox::Yes | QMessageBox::No,
                            QMessageBox::Yes);
                    doOpen = clicked == QMessageBox::Yes;
                }

                if (doOpen) {
                    Model::Default()->change(engine);
                    reloadModel();
                    onClickParseAllRules();
                    setCurrentFile(false, &filename);
                } else {
                    delete importer;
                    delete engine;
                }
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error opening " + QFileInfo(filename).fileName(),
                        toHtmlEscaped(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                delete importer;
                delete engine;
                return;
            }
        }

        void Window::updateRecentFiles() {
            _recentFiles->clear();

            QSettings settings;
            int maxRecentFiles = settings.value("file/maxRecentFiles", 5).toInt();
            QStringList recentFiles = settings.value("file/recentFiles").toStringList();
            bool update = false;
            while (not recentFiles.empty() and recentFiles.size() > maxRecentFiles) {
                recentFiles.removeLast();
                update = true;
            }
            if (update) settings.setValue("file/recentFiles", recentFiles);

            for (int i = 0; i < recentFiles.size(); ++i) {
                QString text = QFileInfo(recentFiles.at(i)).absoluteFilePath();
                QString data = QFileInfo(recentFiles.at(i)).absoluteFilePath();

                QAction* actionFile = new QAction(_recentFiles);
                actionFile->setText(text);
                actionFile->setStatusTip(data);
                actionFile->setData(data);
                QObject::connect(actionFile, SIGNAL(triggered()),
                        this, SLOT(onMenuOpenRecent()));

                _recentFiles->addAction(actionFile);
            }
        }

        void Window::addRecentFile(const QString& filepath) {
            QSettings settings;
            int maxRecentFiles = settings.value("file/maxRecentFiles", 5).toInt();
            QStringList recentFiles = settings.value("file/recentFiles").toStringList();
            recentFiles.push_front(filepath);
            recentFiles.removeDuplicates();
            while (not recentFiles.empty() and recentFiles.size() > maxRecentFiles) {
                recentFiles.removeLast();
            }
            settings.setValue("file/recentFiles", recentFiles);
        }

        void Window::onMenuSave() {
            if (_currentFile.isEmpty()) {
                onMenuSaveAs();
                return;
            }
            saveFile(QFileInfo(_currentFile).absoluteFilePath());
        }

        void Window::onMenuSaveAs() {
            QSettings settings;
            QString recentLocation = settings.value("file/recentLocation", ".").toString();
            QString recentFilter = settings.value("file/recentFilter").toString();
            QStringList filters;
            filters << "Supported files (*.fll *.fcl *.fis)"
                    << "FuzzyLite Language (*.fll)"
                    << "Fuzzy Logic Controller (*.fcl)"
                    << "Fuzzy Inference System (*.fis)"
                    << "All files (*.*)";
            int recentFilterIndex = filters.indexOf(recentFilter);
            if (recentFilterIndex < 0) recentFilterIndex = 0;
            QString filter = filters.at(recentFilterIndex);
            QString filename = QFileDialog::getSaveFileName(this,
                    "Save engine as", recentLocation,
                    filters.join(";;"),
                    &filter);
            if (filename.size() == 0) return;
            settings.setValue("file/recentFilter", filter);
            settings.setValue("file/recentLocation", QFileInfo(filename).absoluteFilePath());
            saveFile(filename);
        }

        void Window::saveFile(QString filename) {

            enum Format {
                FLL, FCL, FIS, UNSUPPORTED
            } format;

            if (filename.endsWith(".fll", Qt::CaseInsensitive)) format = FLL;
            else if (filename.endsWith(".fcl", Qt::CaseInsensitive)) format = FCL;
            else if (filename.endsWith(".fis", Qt::CaseInsensitive)) format = FIS;
            else {
                QSettings settings;
                QString recentFormat = settings.value("file/recentFormat").toString();
                bool ok;
                QStringList formats;
                formats << "FuzzyLite Language (*.fll)"
                        << "Fuzzy Controller Language (*.fcl)"
                        << "Fuzzy Inference System (*.fis)";
                int recentFormatIndex = formats.indexOf(recentFormat);
                if (recentFormatIndex < 0) recentFormatIndex = 0;
                QString selectedFormat = QInputDialog::getItem(this, "File format",
                        "Please select the format of the file",
                        formats, recentFormatIndex, false, &ok);
                if (not ok) return;
                settings.setValue("file/recentFormat", selectedFormat);
                if (selectedFormat == formats.at(0)) {
                    format = FLL;
                    filename += ".fll";
                } else if (selectedFormat == formats.at(1)) {
                    format = FCL;
                    filename += ".fcl";
                } else if (selectedFormat == formats.at(2)) {
                    format = FIS;
                    filename += ".fis";
                } else {
                    format = UNSUPPORTED;
                }
            }

            Engine* engine = Model::Default()->engine();
            Exporter* exporter = NULL;
            try {
                if (format == FLL) exporter = new FllExporter;
                else if (format == FCL) exporter = new FclExporter;
                else if (format == FIS) exporter = new FisExporter;
                else throw fl::Exception("[import error] Unsupported filetype for file: "
                        + filename.toStdString(), FL_AT);

                QString exportedEngine = QString::fromStdString(exporter->toString(engine));
                QFile file(filename);
                if (not file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    throw fl::Exception("[save error] Error saving file: "
                            + filename.toStdString(), FL_AT);
                }

                QTextStream out(&file);
                out << exportedEngine;

                addRecentFile(filename);
                updateRecentFiles();

                setCurrentFile(false, &filename);
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error saving " + QFileInfo(filename).fileName(),
                        toHtmlEscaped(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                delete exporter;
                return;
            }
            delete exporter;
        }

        void Window::onMenuReload() {
            openFile(_currentFile, "Reload engine");
        }

        void Window::onMenuTerms() {
            Term* window = new Term(this);
            window->setup(fl::Variable("Terms", 0, 1));
            window->setWindowTitle("Terms");
            window->ui->qfr_name->setVisible(false);
            window->ui->buttonBox->setVisible(false);
            window->show();
        }

        void Window::onMenuSurface2D() {
            Surface2D* surface = new Surface2D(this);
            surface->setup();
            surface->show();
        }

        void Window::onMenuImport() {
            if (ui->actionImport->isChecked()) {
                QMenu menu(this);
                menu.addAction("&FuzzyLite Language (FLL)", this, SLOT(onMenuImportFromFLL()));
                menu.addAction("Fuzzy Control &Language (FCL)", this, SLOT(onMenuImportFromFCL()));
                menu.addAction("Fuzzy Inference &System (FIS)", this, SLOT(onMenuImportFromFIS()));
                menu.exec(QCursor::pos() + QPoint(1, 0));
                ui->actionImport->setChecked(false);
            }
        }

        void Window::onMenuImportFromFLL() {
            ImEx imex;
            imex.setup();
            imex.setWindowTitle("Import Engine from...");
            imex.ui->lbl_format->setText("FuzzyLite Language (FLL):");
            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            imex.ui->pte_code->setFont(font);

            if (imex.exec()) {
                std::string fllString = imex.ui->pte_code->document()->toPlainText().toStdString();
                Engine* engine = NULL;
                FllImporter importer;
                try {
                    engine = importer.fromString(fllString);
                    if (not confirmChanges("Import from FLL")) {
                        delete engine;
                        return;
                    }
                    Model::Default()->change(engine);
                    reloadModel();
                    onClickParseAllRules();
                    QString empty;
                    setCurrentFile(true, &empty);
                } catch (fl::Exception& ex) {
                    if (engine) delete engine;
                    QMessageBox::critical(this, "Error importing from FLL",
                            toHtmlEscaped(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                            QMessageBox::Ok);

                    return;
                }
            }
        }

        void Window::onMenuImportFromFCL() {
            ImEx imex;
            imex.setup();
            imex.setWindowTitle("Import Engine from...");
            imex.ui->lbl_format->setText("Fuzzy Controller Language (FCL):");
            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            imex.ui->pte_code->setFont(font);

            if (imex.exec()) {
                std::string fclString = imex.ui->pte_code->document()->toPlainText().toStdString();
                Engine* engine = NULL;
                FclImporter importer;
                try {
                    engine = importer.fromString(fclString);
                    if (not confirmChanges("Import from FCL")) {
                        delete engine;
                        return;
                    }
                    Model::Default()->change(engine);
                    reloadModel();
                    onClickParseAllRules();
                    QString empty;
                    setCurrentFile(true, &empty);
                } catch (fl::Exception& ex) {
                    if (engine) delete engine;
                    QMessageBox::critical(this, "Error importing from FCL",
                            toHtmlEscaped(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                            QMessageBox::Ok);

                    return;
                }
            }
        }

        void Window::onMenuImportFromFIS() {
            ImEx imex;
            imex.setup();
            imex.setWindowTitle("Import Engine from...");
            imex.ui->lbl_format->setText("Fuzzy Inference System (FIS):");
            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            imex.ui->pte_code->setFont(font);

            if (imex.exec()) {
                std::string fisString = imex.ui->pte_code->document()->toPlainText().toStdString();
                Engine* engine = NULL;
                FisImporter importer;
                try {
                    engine = importer.fromString(fisString);
                    if (not confirmChanges("Import from FIS")) {
                        delete engine;
                        return;
                    }
                    Model::Default()->change(engine);
                    reloadModel();
                    onClickParseAllRules();
                    QString empty;
                    setCurrentFile(true, &empty);
                } catch (fl::Exception& ex) {
                    QMessageBox::critical(this, "Error importing from FIS",
                            toHtmlEscaped(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                            QMessageBox::Ok);
                    return;
                }

            }
        }

        void Window::onMenuExport() {
            if (ui->actionExport->isChecked()) {
                QMenu menu(this);
                menu.addAction("&FuzzyLite Language (FLL)", this, SLOT(onMenuExportToFLL()));
                menu.addAction("Fuzzy Control &Language (FCL)", this, SLOT(onMenuExportToFCL()));
                menu.addAction("Fuzzy Inference &System (FIS)", this, SLOT(onMenuExportToFIS()));
                menu.addSeparator();
                menu.addAction("FuzzyLite Dataset (FLD) vie&w", this, SLOT(onMenuExportToDatasetView()));
                menu.addAction("FuzzyLite Dataset (FLD) fil&e", this, SLOT(onMenuExportToDatasetFile()));
                menu.addSeparator();
                menu.addAction("fuzzylite (&C++)", this, SLOT(onMenuExportToCpp()));
                menu.addAction("jfuzzylite (&Java)", this, SLOT(onMenuExportToJava()));
                menu.exec(QCursor::pos() + QPoint(1, 0));
                ui->actionExport->setChecked(false);
            }
        }

        void Window::onMenuExportToFLL() {
            FllExporter exporter;
            std::string fllString;
            try {
                fllString = exporter.toString(Model::Default()->engine());
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error exporting to FLL",
                        toHtmlEscaped(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                return;
            }

            ImEx imex;
            imex.setup();
            imex.setWindowTitle("Export engine to");
            imex.ui->lbl_format->setText("FuzzyLite Language (FLL):");
            imex.ui->buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);

            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            imex.ui->pte_code->setFont(font);
            imex.ui->pte_code->setReadOnly(true);
            imex.ui->pte_code->document()->setPlainText(
                    QString::fromStdString(fllString));
            QTextCursor tc = imex.ui->pte_code->textCursor();
            tc.movePosition(QTextCursor::Start);
            imex.ui->pte_code->setTextCursor(tc);
            imex.exec();
        }

        void Window::onMenuExportToFCL() {
            FclExporter exporter;
            std::string fclString;
            try {
                fclString = exporter.toString(Model::Default()->engine());
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error exporting to FCL",
                        toHtmlEscaped(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                return;
            }

            ImEx imex;
            imex.setup();
            imex.setWindowTitle("Export engine to");
            imex.ui->lbl_format->setText("Fuzzy Controller Language (FCL):");
            imex.ui->buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);

            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            imex.ui->pte_code->setFont(font);
            imex.ui->pte_code->setReadOnly(true);
            imex.ui->pte_code->document()->setPlainText(
                    QString::fromStdString(fclString));
            QTextCursor tc = imex.ui->pte_code->textCursor();
            tc.movePosition(QTextCursor::Start);
            imex.ui->pte_code->setTextCursor(tc);
            imex.exec();
        }

        void Window::onMenuExportToFIS() {
            FisExporter exporter;
            std::string fis;
            try {
                fis = exporter.toString(Model::Default()->engine());
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error exporting to FIS",
                        toHtmlEscaped(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                return;
            }

            ImEx imex;
            imex.setup();
            imex.setWindowTitle("Export engine to");
            imex.ui->lbl_format->setText("Fuzzy Inference System (FIS):");
            imex.ui->buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);

            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            imex.ui->pte_code->setFont(font);
            imex.ui->pte_code->setReadOnly(true);
            imex.ui->pte_code->document()->setPlainText(
                    QString::fromStdString(fis));
            QTextCursor tc = imex.ui->pte_code->textCursor();
            tc.movePosition(QTextCursor::Start);
            imex.ui->pte_code->setTextCursor(tc);
            imex.exec();
        }

        void Window::onMenuExportToCpp() {
            CppExporter exporter;
            std::string cpp;
            try {
                cpp = exporter.toString(Model::Default()->engine());
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error exporting to fuzzylite (C++)",
                        toHtmlEscaped(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                return;
            }

            ImEx imex;
            imex.setup();
            imex.setWindowTitle("Export engine to");
            imex.ui->lbl_format->setText("fuzzylite (C++):");
            imex.ui->buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);

            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            imex.ui->pte_code->setFont(font);
            imex.ui->pte_code->setReadOnly(true);
            imex.ui->pte_code->document()->setPlainText(
                    QString::fromStdString(cpp));
            QTextCursor tc = imex.ui->pte_code->textCursor();
            tc.movePosition(QTextCursor::Start);
            imex.ui->pte_code->setTextCursor(tc);
            imex.exec();
        }

        void Window::onMenuExportToJava() {
            JavaExporter exporter;
            std::string java;
            try {
                java = exporter.toString(Model::Default()->engine());
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error exporting to jfuzzylite (Java)",
                        toHtmlEscaped(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                return;
            }

            ImEx imex;
            imex.setup();
            imex.setWindowTitle("Export engine to");
            imex.ui->lbl_format->setText("jfuzzylite (Java):");
            imex.ui->buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);

            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            imex.ui->pte_code->setFont(font);
            imex.ui->pte_code->setReadOnly(true);
            imex.ui->pte_code->document()->setPlainText(
                    QString::fromStdString(java));
            QTextCursor tc = imex.ui->pte_code->textCursor();
            tc.movePosition(QTextCursor::Start);
            imex.ui->pte_code->setTextCursor(tc);
            imex.exec();
        }

        void Window::onMenuExportToDatasetView() {
            QSettings settings;
            int minResolution =
                    settings.value("view/minVariableResolution", 8).toInt();
            int maxResolution =
                    settings.value("view/maxVariableResolution", 1024 * 1024 * 1024).toInt();
            bool ok;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
            int results = QInputDialog::getInteger(this,
                    "Number of Results",
                    "Please, specify the maximum number of results you want to export:",
                    1024, minResolution, maxResolution, 8, &ok);
#else
            int results = QInputDialog::getInt(this,
                    "FuzzyLite Dataset (FLD)",
                    "Please, specify the maximum number of results you want to export:",
                    1024, minResolution, maxResolution, 8, &ok);
#endif
            if (not ok) {
                return;
            }

            FldExporter exporter;
            exporter.setMaximum(results);
            std::string data;
            try {
                data = exporter.toString(Model::Default()->engine());
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error exporting to FLD (view)",
                        toHtmlEscaped(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                return;
            }

            ImEx imex;
            imex.setup();
            imex.setWindowTitle("Export engine to");
            imex.ui->lbl_format->setText("FuzzyLite Dataset (FLD):");
            imex.ui->buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);

            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            imex.ui->pte_code->setFont(font);
            imex.ui->pte_code->setReadOnly(true);
            imex.ui->pte_code->document()->setPlainText(
                    QString::fromStdString(data));
            QTextCursor tc = imex.ui->pte_code->textCursor();
            tc.movePosition(QTextCursor::Start);
            imex.ui->pte_code->setTextCursor(tc);
            imex.exec();
        }

        void Window::onMenuExportToDatasetFile() {
            QSettings settings;
            QString recentLocation = settings.value("file/recentDataLocation", ".").toString();
            QStringList filters;
            filters << "All files (*.*)"
                    << "FuzzyLite Dataset file (*.fld)";

            QString filter = filters.at(1);
            QString filename = QFileDialog::getSaveFileName(this,
                    "Export data to", recentLocation,
                    filters.join(";;"),
                    &filter);
            if (filename.size() == 0) return;
            settings.setValue("file/recentDataLocation", QFileInfo(filename).absoluteFilePath());

            int minResolution =
                    settings.value("view/minVariableResolution", 8).toInt();
            int maxResolution =
                    settings.value("view/maxVariableResolution", 1024 * 1024 * 1024).toInt();
            bool ok;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
            int results = QInputDialog::getInteger(this,
                    "Number of Results",
                    "Please, specify the maximum number of results you want to export:",
                    1024, minResolution, maxResolution, 8, &ok);
#else
            int results = QInputDialog::getInt(this,
                    "Number of Results",
                    "Please, specify the maximum number of results you want to export:",
                    1024, minResolution, maxResolution, 8, &ok);
#endif
            if (not ok) {
                return;
            }

            std::ofstream dataFile(filename.toStdString().c_str());
            if (not dataFile.is_open()) {
                std::ostringstream ss;
                ss << "The file <" << filename.toStdString() << "> could not be created";
                QMessageBox::critical(this, "Error exporting to FLD (file)",
                        toHtmlEscaped(QString::fromStdString(ss.str())),
                        QMessageBox::Ok);
                return;
            }

            FldExporter exporter;
            try {
                exporter.toWriter(Model::Default()->engine(), dataFile, " ", results);
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error exporting to FLD (file)",
                        toHtmlEscaped(QString::fromStdString(ex.what())).replace("\n", "<br>"),
                        QMessageBox::Ok);
                return;
            }

            ImEx imex;
            imex.setup();
            imex.setWindowTitle("Export engine to");
            imex.ui->lbl_format->setText("FuzzyLite Dataset (FLD):");
            imex.ui->buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);

            QFont font = typeWriterFont();
            font.setPointSize(font.pointSize() - 1);
            imex.ui->pte_code->setFont(font);
            imex.ui->pte_code->setReadOnly(true);
            imex.ui->pte_code->document()->setPlainText(filename);
            QTextCursor tc = imex.ui->pte_code->textCursor();
            tc.movePosition(QTextCursor::Start);
            imex.ui->pte_code->setTextCursor(tc);
            imex.exec();
        }

        void Window::onMenuAbout() {
            About about;
            about.setup();
            about.exec();
        }

        void Window::onMenuAboutQt() {
            QApplication::aboutQt();
        }

        void Window::onMenuAskForHelp() {
            QMessageBox::StandardButton result =
                    QMessageBox::question(this, "Ask for help",
                    "If you need help, please visit "
                    "<a href='http://www.fuzzylite.com/forums'>www.fuzzylite.com/forums</a> "
                    "and post your question in the forum. "
                    "The community will be very happy to help you as "
                    "soon and best as possible. <br/><br/>"
                    "Do you want to visit the forums now?",
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if (result == QMessageBox::Yes)
                QDesktopServices::openUrl(QUrl("http://www.fuzzylite.com/forums"));
        }

        void Window::onMenuJoinTheCommunity() {
            QMessageBox::StandardButton result =
                    QMessageBox::information(this, "Join the community",
                    "Visit "
                    "<a href='http://www.fuzzylite.com/community'>http://www.fuzzylite.com/community</a> "
                    "to join the <b>fuzzylite</b> community, receive news and updates, and help others "
                    "with their questions if you can.<br><br>"
                    "Do you want to join the community now?",
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if (result == QMessageBox::Yes)
                QDesktopServices::openUrl(QUrl("http://www.fuzzylite.com/community"));
        }

        void Window::onMenuFollowOnTwitter() {
            QMessageBox::StandardButton result =
                    QMessageBox::information(this, "Follow on Twitter",
                    "Visit "
                    "<a href='http://www.twitter.com/fuzzylite'>http://www.twitter.com/fuzzylite</a> "
                    "to follow <b>@fuzzylite</b> for news and updates<br><br>"
                    "Do you want to start following @fuzzylite now?",
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if (result == QMessageBox::Yes)
                QDesktopServices::openUrl(QUrl("http://www.twitter.com/fuzzylite"));
        }

        void Window::onMenuQuit() {
            this->close();
        }

        void Window::onMenuCheckForUpdates() {
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(updatesReplyFinished(QNetworkReply*)));

            manager->get(QNetworkRequest(QUrl("http://www.fuzzylite.com/version.php")));
        }

        void Window::automaticUpdates() {
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(automaticUpdatesReplyFinished(QNetworkReply*)));

            manager->get(QNetworkRequest(QUrl("http://www.fuzzylite.com/version.php")));
        }

        std::pair<std::string, bool> Window::onReplyFinished(QNetworkReply* reply) {
            if (reply->error() != QNetworkReply::NoError) {
                throw fl::Exception("[network error] failed checking for updates", FL_AT);
            }
            static const std::string JFUZZYLITE = "1.0";
            bool updates = false;
            std::string message = "<table width='100%'>"
                    "<tr><th/><th>Current</th><th>Newest</th></tr>";
            QByteArray bytes = reply->readAll();
            QString fuzzyliteVersions(bytes);
            QList<QString> versions = fuzzyliteVersions.split("\n");
            for (int i = 0; i < versions.size(); ++i) {
                std::vector<std::string> keyValue =
                        fl::Op::split(versions.at(i).toStdString(), ":");
                if (keyValue.size() != 2) {
                    FL_LOG("[network error] expected <key:value>, but received <"
                            << versions.at(i).toStdString() << ">");
                    continue;
                }
                std::pair<std::string, std::string> currentNew;
                message += "<tr>";
                if ("fuzzylite" == keyValue.front()) {
                    message += "<td>fuzzylite</td>";
                    currentNew.first = fl::fuzzylite::version();
                } else if ("qtfuzzylite" == keyValue.front()) {
                    message += "<td>qtfuzzylite</td>";
                    currentNew.first = fl::qt::qtfuzzylite::version();
                } else if ("jfuzzylite" == keyValue.front()) {
                    message += "<td>jfuzzylite</td>";
                    currentNew.first = JFUZZYLITE;
                } else {
                    message += "<td>" + keyValue.front() + "</td>";
                    currentNew.first = "";
                }
                currentNew.second = Op::trim(keyValue.back());

                message += "<td align='center'>&nbsp;" + currentNew.first + "&nbsp;</td>";
                if (currentNew.first == currentNew.second) {
                    message += "<td align='center'>&nbsp;" + currentNew.second + "&nbsp;</td>";
                } else {
                    updates = true;
                    message += "<td align='center'>&nbsp;<b>" + currentNew.second + "</b>&nbsp;</td>";
                }
                message += "</tr>";
            }
            message += "</table><hr>";
            message += "Visit <a href='http://www.fuzzylite.com'>www.fuzzylite.com</a> for more information";

            return std::pair < std::string, bool>(message, updates);
        }

        void Window::updatesReplyFinished(QNetworkReply* reply) {
            std::pair < std::string, bool> updates;
            try {
                updates = onReplyFinished(reply);
            } catch (std::exception& ex) {
                QMessageBox::critical(this, "Software Update",
                        "An error occurred while checking for software updates.<br><br>"
                        "Please, try again later or visit "
                        "<a href='http://www.fuzzylite.com'>www.fuzzylite.com</a> "
                        "for more information");
                return;
            }
            QString message = (updates.second ? "There are <b>new</b> versions available!"
                    : "<tt>qtfuzzylite</tt> is already up-to-date");
            message += QString::fromStdString(updates.first) + "<hr/>";
            message += "Do you want to check for updates at startup?";
            QMessageBox::StandardButton result = QMessageBox::question(this,
                    "Software Update", message,
                    QMessageBox::Yes | QMessageBox::Default | QMessageBox::Escape,
                    QMessageBox::No);
            QSettings settings;
            if (result == QMessageBox::Yes) {
                settings.setValue("checkForUpdates", true);
            } else if (result == QMessageBox::No) {
                settings.setValue("checkForUpdates", false);
            }
        }

        void Window::automaticUpdatesReplyFinished(QNetworkReply* reply) {
            std::pair < std::string, bool> updates;
            try {
                updates = onReplyFinished(reply);
            } catch (std::exception& ex) {
                FL_LOG("[network error] failed checking for software updates");
                return;
            }
            if (updates.second) {
                QString message = "There are <b>new</b> versions available!";
                message += QString::fromStdString(updates.first) + "<hr/>";
                message += "Do you want to check for updates at startup?";
                QMessageBox::StandardButton result = QMessageBox::question(this,
                        "Software Update", message,
                        QMessageBox::Yes | QMessageBox::Default | QMessageBox::Escape,
                        QMessageBox::No);
                QSettings settings;
                if (result == QMessageBox::Yes) {
                    settings.setValue("checkForUpdates", true);
                } else if (result == QMessageBox::No) {
                    settings.setValue("checkForUpdates", false);
                }
            } else {
                FL_DBG("[automatic updates] qtfuzzylite is already up-to-date");
            }
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

        QColor Window::gradient(int x, const QColor& min, const QColor& max) {
            QColor result;
            result.setRed(fl::Op::scale(x, 0, 255, min.red(), max.red()));
            result.setBlue(fl::Op::scale(x, 0, 255, min.blue(), max.blue()));
            result.setGreen(fl::Op::scale(x, 0, 255, min.green(), max.green()));
            result.setAlpha(fl::Op::scale(x, 0, 255, min.alpha(), max.alpha()));
            return result;
        }

        QFont Window::typeWriterFont() {
            std::string font = "Courier";
#ifdef Q_OS_MAC
            font = "Monaco";
#endif
#ifdef Q_OS_WIN
            font = "Fixedsys";
#endif
#ifdef Q_OS_LINUX
            font = "Ubuntu Mono";
#endif
            QSettings settings;
            std::string preferredFontFamily = settings.value("view/monospacedFontFamily").toString().toStdString();
            if (not preferredFontFamily.empty()) font = preferredFontFamily;

            QFont tt(QString::fromStdString(font));
            tt.setStyleHint(QFont::TypeWriter);
            return tt;
        }

        QString Window::toHtmlEscaped(const QString& x) {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
            return Qt::escape(x);
#else
            return x.toHtmlEscaped();
#endif
        }

        void Window::main(int argc, char** argv) {
            QPixmap pixmap(":/qtfuzzylite.png");
            QSplashScreen splash(pixmap);
            splash.setWindowFlags(splash.windowFlags() | Qt::WindowStaysOnTopHint);
            splash.setEnabled(false);
            splash.show();

            Window* w = mainWindow();
            w->setup();
            QApplication::processEvents();
#ifdef FL_UNIX
            sleep(1);
#elif defined FL_WINDOWS
            Sleep(1000);
            FL_LOG("Slept 1");
#endif
            w->show();
            splash.finish(w);
            w->onMenuAbout();
            QSettings settings;
            bool checkForUpdates = settings.value("checkForUpdates", true).toBool();
            if (checkForUpdates) {
                w->automaticUpdates();
            } 
            //            w->showMinimized();
            //            splash.finish(w);
            //            w->onMenuAbout();
            //            w->activateWindow(); //Ubuntu needs to display filebar properly
            //            w->show(); //Windows needs it to maximize window
            //            w->showMinimized();
            QString openFile;
            if (argc > 1) {
                openFile = QFileInfo(QString(argv[1])).absoluteFilePath();
                w->openFile(openFile);
            }
        }

    }
}
