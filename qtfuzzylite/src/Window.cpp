/*
 * Window.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */ 

#include "fl/qt/Window.h"
#include "fl/qt/Configuration.h"
#include "fl/qt/Term.h"
#include "fl/qt/Variable.h"
#include "fl/qt/Model.h"
#include "fl/qt/Control.h"

#include "fl/qt/definitions.h"
#include "ui/ui_ImEx.h"

#include <QtGui/QListWidgetItem>
#include <QtGui/QScrollBar>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopWidget>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>
#include <QtCore/QTextStream>
#include <QtGui/QMenu>

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
        ui(new Ui::Window), configuration(NULL) { }

        Window::~Window() {
            disconnect();
            if (configuration) delete configuration;
            delete ui;
        }

        void Window::setup() {
            ui->setupUi(this);
            setGeometry(0, 0, 800, 600);
            configuration = new fl::qt::Configuration(this);
            configuration->setup();
            ui->tab_container->setCurrentIndex(0);

            QList<int> sizes;
            sizes << .75 * size().width() << .25 * size().width();
            ui->spl_control_inout_rules->setSizes(sizes);
            sizes.clear();
            sizes << .90 * size().width() << .10 * size().width();
            ui->spl_control_rule_strength->setSizes(sizes);

            QRect scr = QApplication::desktop()->screenGeometry();
            move(scr.center() - rect().center());

            connect();
        }

        void Window::connect() {
            QObject::connect(ui->actionConfigure, SIGNAL(triggered()),
                    this, SLOT(onMenuConfigure()));
            QObject::connect(ui->actionTerms, SIGNAL(triggered()),
                    this, SLOT(onMenuTerms()));
            QObject::connect(ui->actionReset, SIGNAL(triggered()),
                    this, SLOT(onMenuReset()));
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
            QObject::disconnect(ui->actionConfigure, SIGNAL(triggered()),
                    this, SLOT(onMenuConfigure()));
            QObject::disconnect(ui->actionTerms, SIGNAL(triggered()),
                    this, SLOT(onMenuTerms()));
            QObject::disconnect(ui->actionReset, SIGNAL(triggered()),
                    this, SLOT(onMenuReset()));
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

            configuration->applyDefaults();
            if (configuration->isVisible()) configuration->loadFromModel();

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
                layout->addWidget(control);

                QObject::connect(control, SIGNAL(valueChanged(double)),
                        this, SLOT(onInputValueChanged()));
            }
            //            layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Ignored, QSizePolicy::Expanding));

            //Rules
            for (int i = 0; i < engine->getRuleBlock(0)->numberOfRules(); ++i) {

                QString rule = QString::number(i + 1) + ": " +
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
                layout->addWidget(control);

                QObject::connect(this, SIGNAL(processOutput()),
                        control, SLOT(updateOutput()), Qt::QueuedConnection);
            }
            //            layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Ignored, QSizePolicy::Expanding));
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
            message << "Do you want to delete the following input variables?"
                    << std::endl;
            Engine* engine = Model::Default()->engine();
            for (int i = 0; i < ui->lvw_inputs->count(); ++i) {
                if (ui->lvw_inputs->item(i)->isSelected()) {
                    message << "<" << engine->getInputVariable(i)->getName() << ">: "
                            << engine->getInputVariable(i)->toString() << std::endl;
                }
            }

            QMessageBox::StandardButton clicked = QMessageBox::warning(this,
                    "Delete input variable",
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
                message << "Do you want to edit the following input variables?"
                        << std::endl;
                for (int i = 0; i < ui->lvw_inputs->count(); ++i) {
                    if (ui->lvw_inputs->item(i)->isSelected()) {
                        message << "<" << engine->getInputVariable(i)->getName() << ">: "
                                << engine->getInputVariable(i)->toString() << std::endl;
                    }
                }
                QMessageBox::StandardButton clicked = QMessageBox::warning(this,
                        "Edit multiple input variables",
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
            message << "Do you want to delete the following output variables?"
                    << std::endl;
            Engine* engine = Model::Default()->engine();
            for (int i = 0; i < ui->lvw_outputs->count(); ++i) {
                if (ui->lvw_outputs->item(i)->isSelected()) {
                    message << "<" << engine->getOutputVariable(i)->getName() << ">: "
                            << engine->getOutputVariable(i)->toString() << std::endl;
                }
            }

            QMessageBox::StandardButton clicked = QMessageBox::warning(this,
                    "Delete output variable",
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
                message << "Do you want to edit the following output variables?"
                        << std::endl;
                for (int i = 0; i < ui->lvw_outputs->count(); ++i) {
                    if (ui->lvw_outputs->item(i)->isSelected()) {
                        message << "<" << engine->getOutputVariable(i)->getName() << ">: "
                                << engine->getOutputVariable(i)->toString() << std::endl;
                    }
                }
                QMessageBox::StandardButton clicked = QMessageBox::warning(this,
                        "Edit multiple output variables",
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
                rule << fl::Rule::FL_IF << " ";
                for (int input = 0; input < engine->numberOfInputVariables();
                        ++input) {
                    fl::Variable* var = engine->getInputVariable(input);
                    rule << var->getName() << " " << fl::Rule::FL_IS << " "
                            << var->getTerm(terms[input])->getName() << " ";
                    if (input < engine->numberOfInputVariables() - 1) {
                        rule << fl::Rule::FL_AND << " ";
                    }
                }
                rule << fl::Rule::FL_THEN;
                for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                    rule << " " << engine->getOutputVariable(i)->getName() << " "
                            << fl::Rule::FL_IS << " ?";
                    if (i < engine->numberOfOutputVariables() - 1) {
                        rule << " " << fl::Rule::FL_AND << " ";
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

        void Window::onMenuConfigure() {
            configuration->setFocus();
            configuration->show();
        }

        void Window::onMenuTerms() {
            Term* window = new Term(this);
            window->setup(fl::Variable("Term-Toolbox", 0, 1));
            window->setWindowTitle("Term toolbox");
            window->ui->qfr_name->setVisible(false);
            window->ui->buttonBox->setVisible(false);
            window->show();
            //            delete window;

        }

        void Window::onMenuImport() {
            if (ui->actionImport->isChecked()) {
                QMenu menu(this);
                menu.addAction("Fuzzy Control Language (FCL)", this, SLOT(onMenuImportFromFCL()));
                menu.addAction("Fuzzy Inference System (FIS)", this, SLOT(onMenuImportFromFIS()));
                menu.addSeparator();
                menu.addAction("from file...", this, SLOT(onMenuImportFromFile()));
                menu.exec(QCursor::pos());
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
            fclDialog.setWindowTitle("Import from FCL");
            fclUi.lbl_format->setText("Fuzzy Controller Language (FCL):");

            if (fclDialog.exec()) {
                std::string fclString = fclUi.pte_fcl->document()->toPlainText().toStdString();
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
            Ui::ImEx fclUi;
            QDialog fclDialog(this);
            fclUi.setupUi(&fclDialog);
            fclDialog.setWindowTitle("Import from FIS");
            fclUi.lbl_format->setText("Fuzzy Inference System (FIS):");

            if (fclDialog.exec()) {
                std::string fclString = fclUi.pte_fcl->document()->toPlainText().toStdString();
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
                    "Supported formats [*.fcl, *.fis] (*.fcl *.fis);;"
                    "Fuzzy Logic Controller [*.fcl] (*.fcl);;"
                    "Fuzzy Inference System [*.fis] (*.fis)");

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
                menu.addAction("fuzzylite (C++)", this, SLOT(onMenuExportToCpp()));
                menu.addSeparator();
                menu.addAction("Fuzzy Control Language (FCL)", this, SLOT(onMenuExportToFCL()));
                menu.addAction("Fuzzy Inference System (FIS)", this, SLOT(onMenuExportToFIS()));
                menu.exec(QCursor::pos());
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
            fclDialog.setWindowTitle("Export to FCL");
            fclUi.lbl_format->setText("Fuzzy Controller Language (FCL):");
            fclUi.pte_fcl->setReadOnly(true);
            fclUi.pte_fcl->document()->setPlainText(
                    QString::fromStdString(fclString));
            QTextCursor tc = fclUi.pte_fcl->textCursor();
            tc.movePosition(QTextCursor::Start);
            fclUi.pte_fcl->setTextCursor(tc);
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
            Ui::ImEx fclUi;
            QDialog fclDialog(this);
            fclUi.setupUi(&fclDialog);
            fclUi.buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);
            fclDialog.setWindowTitle("Export to FIS");
            fclUi.lbl_format->setText("Fuzzy Inference System (FIS):");
            fclUi.pte_fcl->setReadOnly(true);
            fclUi.pte_fcl->document()->setPlainText(
                    QString::fromStdString(fis));
            QTextCursor tc = fclUi.pte_fcl->textCursor();
            tc.movePosition(QTextCursor::Start);
            fclUi.pte_fcl->setTextCursor(tc);
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
            Ui::ImEx fclUi;
            QDialog fclDialog(this);
            fclUi.setupUi(&fclDialog);
            fclUi.buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);
            fclDialog.setWindowTitle("Export to fuzzylite");
            fclUi.lbl_format->setText("fuzzylite (C++):");
            fclUi.pte_fcl->setReadOnly(true);
            fclUi.pte_fcl->document()->setPlainText(
                    QString::fromStdString(cpp));
            QTextCursor tc = fclUi.pte_fcl->textCursor();
            tc.movePosition(QTextCursor::Start);
            fclUi.pte_fcl->setTextCursor(tc);
            fclDialog.exec();
        }

        void Window::onMenuReset() {
            QMessageBox::StandardButton clicked =
                    QMessageBox::warning(this, "Reset engine",
                    "<qt>Do you want to reset the engine?<br><br>"
                    "All the input and output variables, together with the rules "
                    "will be deleted!</qt>",
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);

            if (clicked == QMessageBox::No) return;

            Model::Default()->reset();

            reloadModel();
        }

        void Window::onMenuAbout() {
            std::ostringstream message;
            message << "qtfuzzylite v." << FLQT_VERSION <<
                    " (" << FLQT_DATE << ")" << std::endl;
            message << "with fuzzylite v." << FL_VERSION <<
                    " (" << FL_DATE << ")" << std::endl;
            message << "<a href='http://code.google.com/p/fuzzylite'>" << std::endl
                    << std::endl;
            message << "Developed by Juan Rada-Vilela." << std::endl;
            message << "jcrada@gmail.com" << std::endl;
            QMessageBox::about(this, "qtfuzzylite",
                    "<qt>"
                    "<b>qtfuzzylite v. " FLQT_VERSION " (" FLQT_DATE ")</b><br>"
                    "<b>fuzzylite v. " FL_VERSION " (" FL_DATE ")</b><br>"
                    "<a href='http://code.google.com/p/fuzzylite'>http://code.google.com/p/fuzzylite</a><br><br>"
                    "Developed by Juan Rada-Vilela &nbsp;"
                    "<a href='mailto:jcrada@gmail.com'>jcrada@gmail.com</a><br><br>"
                    "Please consider making a <b>donation</b> to help maintain this project! "
                    "It would be mostly appreciated!<br><br>Visit &nbsp;"
                    "<a href='http://code.google.com/p/fuzzylite'>http://code.google.com/p/fuzzylite</a><br><br>"
                    "... and do not hesitate to provide feedback, feature requests, "
                    "alternative licencing options, custom enhancements, or anything else!"
                    "</qt>"
                    );
        }

        void Window::onMenuQuit() {
            int result = QMessageBox::question(this, tr("qtfuzzylite"),
                    tr("<qt>Do you want to quit <b>qtfuzzylite</b>?</qt>"),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);
            if (result == QMessageBox::Yes) {
                this->close();
            } else {
                // do nothing
            }
        }

        void Window::main() {
            Window* w = mainWindow();
            w->setup();
            w->show();
        }

    }
}
