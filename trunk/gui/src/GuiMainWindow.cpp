/*   Copyright 2010 Juan Rada-Vilela

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
#include "gui/GuiMainWindow.h"
#include "gui/GuiGrapher.h"
#include "gui/GuiPreferences.h"

#include <QtGui/QScrollBar>
#include <QtGui/QAction>

namespace fl_gui {

    GuiMainWindow::GuiMainWindow(QWidget* parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags), _ui(new Ui::MainWindow),
    _preferences(new GuiPreferences(fl::FuzzyOperator::DefaultFuzzyOperator())),
    _action_preferences(new QAction("Preferences", NULL)) {

    }

    GuiMainWindow::~GuiMainWindow() {
        disconnect();
        delete _action_preferences;
        delete _preferences;
        delete _ui;
    }

    Ui::MainWindow& GuiMainWindow::getUi() const {
        return *this->_ui;
    }

    void GuiMainWindow::setup() {
        getUi().setupUi(this);
        _preferences->setWindowFlags(_preferences->windowFlags() | Qt::WindowCloseButtonHint
                | Qt::CustomizeWindowHint);
        _preferences->setFixedSize(320, 270);
        _preferences->setup();
        getUi().tab_container->setCurrentIndex(0);
        
        connect();
    }

    void GuiMainWindow::connect() {
        QObject::connect(getUi().lvw_inputs, SIGNAL(itemSelectionChanged()),
                this, SLOT(onChangeInputSelection()));
        QObject::connect(getUi().lvw_outputs, SIGNAL(itemSelectionChanged()),
                this, SLOT(onChangeOutputSelection()));
        QObject::connect(getUi().lvw_inputs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this, SLOT(onDoubleClickInputItem(QListWidgetItem*)));
        QObject::connect(getUi().lvw_outputs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this, SLOT(onDoubleClickOutputItem(QListWidgetItem*)));

        QObject::connect(getUi().lsw_test_rules->verticalScrollBar(), SIGNAL(valueChanged(int)),
                getUi().lsw_test_rules_activation->verticalScrollBar(), SLOT(setValue(int)));
        QObject::connect(getUi().lsw_test_rules_activation->verticalScrollBar(), SIGNAL(valueChanged(int)),
                getUi().lsw_test_rules->verticalScrollBar(), SLOT(setValue(int)));


        _action_preferences->setParent(this);
        _action_preferences->setMenuRole(QAction::PreferencesRole);
        getUi().menuGui->addAction(_action_preferences);

        QObject::connect(_action_preferences, SIGNAL(triggered()),
                this, SLOT(onShowPreferences()));
        QObject::connect(_preferences, SIGNAL(finished(int)),
                this, SLOT(onClosePreferences(int)));
    }

    void GuiMainWindow::disconnect() {
        QObject::disconnect(getUi().lvw_inputs, SIGNAL(itemSelectionChanged()),
                this, SLOT(onChangeInputSelection()));
        QObject::disconnect(getUi().lvw_outputs, SIGNAL(itemSelectionChanged()),
                this, SLOT(onChangeOutputSelection()));
        QObject::disconnect(getUi().lvw_inputs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this, SLOT(onDoubleClickInputItem(QListWidgetItem*)));
        QObject::disconnect(getUi().lvw_outputs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this, SLOT(onDoubleClickOutputItem(QListWidgetItem*)));

        QObject::disconnect(getUi().lsw_test_rules->verticalScrollBar(), SIGNAL(valueChanged(int)),
                getUi().lsw_test_rules_activation->verticalScrollBar(), SLOT(setValue(int)));
        QObject::disconnect(getUi().lsw_test_rules->verticalScrollBar(), SIGNAL(valueChanged(int)),
                getUi().lsw_test_rules_activation->verticalScrollBar(), SLOT(setValue(int)));

        QObject::disconnect(_action_preferences, SIGNAL(triggered()),
                this, SLOT(onShowPreferences()));
        QObject::disconnect(_preferences, SIGNAL(finished(int)),
                this, SLOT(onClosePreferences(int)));
    }

    void GuiMainWindow::onShowPreferences() {
        _preferences->show();
    }

    void GuiMainWindow::onClosePreferences(int result) {
        (void) result;
    }

    void GuiMainWindow::onChangeInputSelection() {
        getUi().btn_remove_input->setEnabled(
                getUi().lvw_inputs->selectedItems().size() > 0);
        getUi().btn_edit_input->setEnabled(
                getUi().lvw_inputs->selectedItems().size() > 0);
    }

    void GuiMainWindow::onChangeOutputSelection() {
        getUi().btn_remove_output->setEnabled(
                getUi().lvw_outputs->selectedItems().size() > 0);
        getUi().btn_edit_output->setEnabled(
                getUi().lvw_outputs->selectedItems().size() > 0);
    }

    void GuiMainWindow::onDoubleClickInputItem(QListWidgetItem* item) {
        (void) item;
        getUi().btn_edit_input->click();
    }

    void GuiMainWindow::onDoubleClickOutputItem(QListWidgetItem* item) {
        (void) item;
        getUi().btn_edit_output->click();
    }

    void GuiMainWindow::onSelectTestRule(int selected) {
        getUi().lsw_test_rules_activation->item(selected)->setSelected(true);
    }

    void GuiMainWindow::onSelectTestActivation(int selected) {
        getUi().lsw_test_rules->item(selected)->setSelected(true);
    }


}
