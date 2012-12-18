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

#include "ui/ui_FCL.h"

#include <QtGui/QListWidgetItem>
#include <QtGui/QScrollBar>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopWidget>
#include <QtGui/QPushButton>

namespace fl {
    namespace qt {
        Window::Window(QWidget* parent, Qt::WindowFlags flags)
                :
                        QMainWindow(parent, flags),
                        ui(new Ui::Window) {

        }

        Window::~Window() {
            disconnect();
            delete ui;
        }

        void Window::setup() {
            ui->setupUi(this);
            ui->tab_container->setCurrentIndex(0);

            QRect scr = QApplication::desktop()->screenGeometry();
            move(scr.center() - rect().center());

            connect();
        }

        void Window::connect() {
            QObject::connect(ui->actionConfiguration, SIGNAL(triggered()),
                    this, SLOT(onMenuConfiguration()));
            QObject::connect(ui->actionTerms, SIGNAL(triggered()),
                    this, SLOT(onMenuTerms()));
            QObject::connect(ui->actionImport, SIGNAL(triggered()),
                    this, SLOT(onMenuImport()));
            QObject::connect(ui->actionExport, SIGNAL(triggered()),
                    this, SLOT(onMenuExport()));
            QObject::connect(ui->actionReset, SIGNAL(triggered()),
                    this, SLOT(onMenuReset()));
            QObject::connect(ui->actionAbout, SIGNAL(triggered()),
                    this, SLOT(onMenuAbout()));
            QObject::connect(ui->actionQuit, SIGNAL(triggered()),
                    this, SLOT(onMenuQuit()));

            QObject::connect(ui->actionExample1, SIGNAL(triggered()),
                    this, SLOT(onMenuExample1()));
            QObject::connect(ui->actionExample2, SIGNAL(triggered()),
                    this, SLOT(onMenuExample2()));
            QObject::connect(ui->actionExample3, SIGNAL(triggered()),
                    this, SLOT(onMenuExample3()));
            QObject::connect(ui->actionExample4, SIGNAL(triggered()),
                    this, SLOT(onMenuExample4()));

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

            QObject::connect(ui->lsw_test_rules->verticalScrollBar(), SIGNAL(valueChanged(int)),
                    ui->lsw_test_rules_activation->verticalScrollBar(), SLOT(setValue(int)));
            QObject::connect(ui->lsw_test_rules_activation->verticalScrollBar(), SIGNAL(valueChanged(int)),
                    ui->lsw_test_rules->verticalScrollBar(), SLOT(setValue(int)));

        }

        void Window::disconnect() {
            FL_LOG("TODO: disconnect");
        }

        void Window::reloadModel() {
            Engine* engine = Model::Default()->engine();
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
            ui->ptx_rules->clear();
            RuleBlock* ruleblock = engine->getRuleBlock(0);
            for (int i = 0; i < ruleblock->numberOfRules(); ++i) {
                ui->ptx_rules->appendPlainText(
                        QString::fromStdString(ruleblock->getRule(i)->toString()));
            }
            Model::Default()->update();
            reloadTest();
        }

        void Window::resetTest() {
            //Inputs
            QLayout* layout = ui->grx_test_inputs->layout();

            for (int i = layout->count() - 1; i >= 0; --i) {
                QLayoutItem* item = layout->itemAt(i);
                Control* control = dynamic_cast<Control*>(item->widget());
                if (control) {
                    QObject::disconnect(control, SIGNAL(inputValueChanged()),
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
                Control* control = dynamic_cast<Control*>(item->widget());
                if (control) {
                    QObject::disconnect(control, SIGNAL(inputValueChanged()),
                            this, SLOT(onInputValueChanged()));
                }
                layout->removeItem(item);
                delete item->widget();
                delete item;
            }

            //
        }

        void Window::reloadTest() {
            resetTest();
            Engine* engine = Model::Default()->engine();
            QLayout* layout = ui->grx_test_inputs->layout();
            for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
                Control* control = new Control;
                control->setup(engine->getInputVariable(i));
                layout->addWidget(control);
                QObject::connect(control, SIGNAL(inputValueChanged()), this,
                        SLOT(onInputValueChanged()));
            }
            layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Ignored, QSizePolicy::Expanding));

            //Rules
            for (int i = 0; i < engine->getRuleBlock(0)->numberOfRules(); ++i) {
                QString rule = QString::fromStdString(
                        engine->getRuleBlock(0)->getRule(i)->toString());
                ui->lsw_test_rules->addItem(rule);
                QListWidgetItem* item = new QListWidgetItem;
                item->setText("-");
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->lsw_test_rules_activation->addItem(item);
            }
            for (int i = 0; i < engine->getRuleBlock(0)->numberOfRules(); ++i) {
                QListWidgetItem* rule = ui->lsw_test_rules->item(i);
                QListWidgetItem* act = ui->lsw_test_rules_activation->item(i);
                QRect rect = ui->lsw_test_rules->visualItemRect(rule);
                act->setSizeHint(rect.size());
            }

            layout = ui->grx_test_outputs->layout();
            //Outputs
            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                Control* control = new Control;
                control->setup(engine->getOutputVariable(i));
                layout->addWidget(control);
                QObject::connect(this, SIGNAL(inputValueChanged()),
                        control, SLOT(updateOutputValue()));
            }
            layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Ignored,
                    QSizePolicy::Expanding));

            ui->tab_container->setCurrentIndex(1);
        }

        void Window::removeRules() {
            ui->ptx_rules->clear();
            RuleBlock* ruleblock = Model::Default()->engine()->getRuleBlock(0);
            for (int i = ruleblock->numberOfRules() - 1; i >= 0; --i) {
                delete ruleblock->removeRule(i);
            }
            reloadModel();
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

        }

        /**
         * Button actions
         */

        void Window::onClickAddInputVariable() {
            Variable* window = new Variable(this);
            window->setup(Variable::INPUT_VARIABLE);
            if (window->exec()) {
                Model::Default()->engine()->addInputVariable(
                        dynamic_cast<InputVariable*>(window->variable));
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
                                dynamic_cast<InputVariable*>(window->variable),
                                i);
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
                        dynamic_cast<OutputVariable*>(window->variable));
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
                                dynamic_cast<OutputVariable*>(window->variable),
                                i);
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
                        rule << fl::Rule::FL_AND;
                    }
                }
                rule << fl::Rule::FL_THEN;
                for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                    rule << " " << engine->getOutputVariable(i)->getName() << " "
                            << fl::Rule::FL_IS << " ? ";
                    if (i < engine->numberOfOutputVariables() - 1) {
                        rule << fl::Rule::FL_AND << " ";
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
            message << "# Total rules: " << badRules + goodRules
                    << ". Good Rules: " << goodRules
                    << ". Bad Rules: " << badRules << ".";
            ui->ptx_rules->appendHtml("<font color='blue'>" +
                    QString::fromStdString(message.str()) + "</font>");
            if (goodRules > 0 and badRules == 0) {
                ui->ptx_rules->appendHtml("<font color='blue'>" +
                        QString("# You may proceed to test the engine") + "</font>");
            }
        }

        /**
         * Menu actions...
         */

        bool Window::onMenuExample(const std::string& example) {
            Engine* engine = Model::Default()->engine();
            if (not (engine->numberOfInputVariables() or engine->numberOfOutputVariables())) {
                return true;
            }
            std::ostringstream message;
            message << "Example " << example << " needs to reset the engine."
                    << "All the input and output variables, together with the rules "
                    << "in current engine will be deleted"
                    << "Do you want to continue loading the example?" << std::endl;
            QMessageBox::StandardButton clicked =
                    QMessageBox::warning(this, "Loading example",
                            QString::fromStdString(message.str()),
                            QMessageBox::Yes | QMessageBox::No);

            return clicked == QMessageBox::Yes;
        }

        void Window::onMenuExample1() {
            if (onMenuExample(ui->actionExample1->text().toStdString())) {
                Example1* example = new Example1;
                Model::Default()->changeEngine(example->engine);
                //Not deleted, as it will be deleted upon change.
                reloadModel();
                onClickParseAllRules();
            }
        }
        void Window::onMenuExample2() {

        }
        void Window::onMenuExample3() {

        }
        void Window::onMenuExample4() {

        }

        void Window::onMenuConfiguration() {
            Configuration* window = new Configuration(this);
            window->setModal(true);
            window->setup();
            window->exec();
            delete window;
        }

        void Window::onMenuTerms() {
            Term* termWindow = new Term(this);
            termWindow->setModal(false);
            termWindow->setup();
            termWindow->exec();
            delete termWindow;

        }
        void Window::onMenuImport() {
            Ui::FCL fclUi;
            QDialog fclDialog(this);
            fclUi.setupUi(&fclDialog);
            fclDialog.setWindowTitle("Import from Fuzzy Controller Language");
            if (fclDialog.exec()) {
                std::string fclString = fclUi.pte_fcl->document()->toPlainText().toStdString();
                Engine* engine = NULL;
                FclImporter importer;
                try {
                    engine = importer.fromFcl(fclString);
                } catch (fl::Exception& ex) {
                    QMessageBox::critical(this, "Error importing from FCL",
                            QString::fromStdString(ex.what()),
                            QMessageBox::Ok);
                    return;
                }
                Model::Default()->changeEngine(engine);
                reloadModel();
                onClickParseAllRules();
            }
        }
        void Window::onMenuExport() {
            FclExporter exporter;
            std::string fclString = exporter.toFcl(Model::Default()->engine());
            Ui::FCL fclUi;
            QDialog fclDialog(this);
            fclUi.setupUi(&fclDialog);
            fclUi.buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);
            fclDialog.setWindowTitle("Export to Fuzzy Controller Language");
            fclUi.pte_fcl->setReadOnly(true);
            fclUi.pte_fcl->document()->setPlainText(
                    QString::fromStdString(fclString));
            fclDialog.exec();
        }

        void Window::onMenuReset() {
            std::ostringstream message;
            message << "Do you want to reset the engine?" << std::endl
                    << "All the input and output variables, together with the rules "
                    << "will be deleted";
            QMessageBox::StandardButton clicked =
                    QMessageBox::warning(this, "Reset engine",
                            QString::fromStdString(message.str()),
                            QMessageBox::Yes | QMessageBox::No);

            if (clicked == QMessageBox::No) return;

            Engine* engine = Model::Default()->engine();

            for (int i = engine->getRuleBlock(0)->numberOfRules() - 1; i >= 0;
                    --i) {
                delete engine->getRuleBlock(0)->removeRule(i);
            }

            for (int i = engine->numberOfInputVariables() - 1; i >= 0; --i) {
                delete engine->removeInputVariable(i);
            }

            for (int i = engine->numberOfOutputVariables() - 1; i >= 0; --i) {
                delete engine->removeOutputVariable(i);
            }
            reloadModel();
        }

        void Window::onMenuAbout() {
            std::ostringstream message;
            message << "qtfuzzylite v." << FL_VERSION <<
            " (" << FL_DATE << ")" << std::endl;
            message << "http://code.google.com/p/fuzzylite" << std::endl
                    << std::endl;
            message << "Developed by Juan Rada-Vilela." << std::endl;
            message << "jcrada@gmail.com" << std::endl;
            QMessageBox::about(this, "qtfuzzylite",
                    QString::fromStdString(message.str()));
        }
        void Window::onMenuQuit() {
            int result = QMessageBox::question(this, tr("qtfuzzylite"),
                    tr("Do you want to quit?"),
                    QMessageBox::Yes | QMessageBox::No);
            if (result == QMessageBox::Yes) {
                this->close();
            } else {
                // do nothing
            }
        }
        void Window::main() {
            Window* w = new Window;
            w->setup();
            w->show();
        }

    }
} /* namespace fl */
