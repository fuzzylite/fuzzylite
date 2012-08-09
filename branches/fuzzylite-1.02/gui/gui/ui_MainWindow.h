/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Sat Feb 12 19:33:24 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSimple_1x1;
    QAction *actionPower_3x1;
    QAction *actionSetup;
    QAction *actionTest;
    QAction *actionTile;
    QAction *actionCascade;
    QAction *actionLog;
    QAction *actionApproximation_1x1;
    QAction *actionMamdani;
    QAction *actionTakagi_Sugeno;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tab_container;
    QWidget *tab_setup;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbl_inputs;
    QToolButton *btn_add_input;
    QToolButton *btn_remove_input;
    QToolButton *btn_edit_input;
    QListWidget *lvw_inputs;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lbl_outputs;
    QToolButton *btn_add_output;
    QToolButton *btn_remove_output;
    QToolButton *btn_edit_output;
    QListWidget *lvw_outputs;
    QHBoxLayout *horizontalLayout_4;
    QPlainTextEdit *ptx_rules;
    QVBoxLayout *verticalLayout_7;
    QToolButton *btn_generate_rules;
    QToolButton *btn_check_rules;
    QToolButton *btn_ok;
    QToolButton *btn_clear;
    QWidget *tab_test;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_5;
    QGroupBox *grx_test_inputs_outer_container;
    QVBoxLayout *verticalLayout_9;
    QScrollArea *sca_test_inputs;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_11;
    QGroupBox *grx_test_inputs;
    QVBoxLayout *verticalLayout_8;
    QSpacerItem *verticalSpacer;
    QGroupBox *grx_test_rules;
    QHBoxLayout *horizontalLayout_6;
    QListWidget *lsw_test_rules;
    QListWidget *lsw_test_rules_activation;
    QGroupBox *grx_test_outputs_outer_container;
    QVBoxLayout *verticalLayout_12;
    QScrollArea *sca_test_outputs;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout_13;
    QGroupBox *grx_test_outputs;
    QVBoxLayout *verticalLayout_15;
    QSpacerItem *verticalSpacer_2;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuGui;
    QMenu *menuExamples;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setBaseSize(QSize(0, 0));
        MainWindow->setUnifiedTitleAndToolBarOnMac(true);
        actionSimple_1x1 = new QAction(MainWindow);
        actionSimple_1x1->setObjectName(QString::fromUtf8("actionSimple_1x1"));
        actionPower_3x1 = new QAction(MainWindow);
        actionPower_3x1->setObjectName(QString::fromUtf8("actionPower_3x1"));
        actionSetup = new QAction(MainWindow);
        actionSetup->setObjectName(QString::fromUtf8("actionSetup"));
        actionSetup->setCheckable(false);
        actionTest = new QAction(MainWindow);
        actionTest->setObjectName(QString::fromUtf8("actionTest"));
        actionTest->setCheckable(false);
        actionTile = new QAction(MainWindow);
        actionTile->setObjectName(QString::fromUtf8("actionTile"));
        actionCascade = new QAction(MainWindow);
        actionCascade->setObjectName(QString::fromUtf8("actionCascade"));
        actionLog = new QAction(MainWindow);
        actionLog->setObjectName(QString::fromUtf8("actionLog"));
        actionLog->setCheckable(false);
        actionApproximation_1x1 = new QAction(MainWindow);
        actionApproximation_1x1->setObjectName(QString::fromUtf8("actionApproximation_1x1"));
        actionMamdani = new QAction(MainWindow);
        actionMamdani->setObjectName(QString::fromUtf8("actionMamdani"));
        actionMamdani->setEnabled(false);
        actionTakagi_Sugeno = new QAction(MainWindow);
        actionTakagi_Sugeno->setObjectName(QString::fromUtf8("actionTakagi_Sugeno"));
        actionTakagi_Sugeno->setEnabled(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tab_container = new QTabWidget(centralwidget);
        tab_container->setObjectName(QString::fromUtf8("tab_container"));
        tab_setup = new QWidget();
        tab_setup->setObjectName(QString::fromUtf8("tab_setup"));
        verticalLayout_6 = new QVBoxLayout(tab_setup);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lbl_inputs = new QLabel(tab_setup);
        lbl_inputs->setObjectName(QString::fromUtf8("lbl_inputs"));

        horizontalLayout_2->addWidget(lbl_inputs);

        btn_add_input = new QToolButton(tab_setup);
        btn_add_input->setObjectName(QString::fromUtf8("btn_add_input"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/edit_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_add_input->setIcon(icon);

        horizontalLayout_2->addWidget(btn_add_input);

        btn_remove_input = new QToolButton(tab_setup);
        btn_remove_input->setObjectName(QString::fromUtf8("btn_remove_input"));
        btn_remove_input->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/edit_remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_remove_input->setIcon(icon1);

        horizontalLayout_2->addWidget(btn_remove_input);

        btn_edit_input = new QToolButton(tab_setup);
        btn_edit_input->setObjectName(QString::fromUtf8("btn_edit_input"));
        btn_edit_input->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/pencil.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_edit_input->setIcon(icon2);

        horizontalLayout_2->addWidget(btn_edit_input);


        verticalLayout_2->addLayout(horizontalLayout_2);

        lvw_inputs = new QListWidget(tab_setup);
        lvw_inputs->setObjectName(QString::fromUtf8("lvw_inputs"));

        verticalLayout_2->addWidget(lvw_inputs);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lbl_outputs = new QLabel(tab_setup);
        lbl_outputs->setObjectName(QString::fromUtf8("lbl_outputs"));

        horizontalLayout_3->addWidget(lbl_outputs);

        btn_add_output = new QToolButton(tab_setup);
        btn_add_output->setObjectName(QString::fromUtf8("btn_add_output"));
        btn_add_output->setIcon(icon);

        horizontalLayout_3->addWidget(btn_add_output);

        btn_remove_output = new QToolButton(tab_setup);
        btn_remove_output->setObjectName(QString::fromUtf8("btn_remove_output"));
        btn_remove_output->setEnabled(false);
        btn_remove_output->setIcon(icon1);

        horizontalLayout_3->addWidget(btn_remove_output);

        btn_edit_output = new QToolButton(tab_setup);
        btn_edit_output->setObjectName(QString::fromUtf8("btn_edit_output"));
        btn_edit_output->setEnabled(false);
        btn_edit_output->setIcon(icon2);

        horizontalLayout_3->addWidget(btn_edit_output);


        verticalLayout_3->addLayout(horizontalLayout_3);

        lvw_outputs = new QListWidget(tab_setup);
        lvw_outputs->setObjectName(QString::fromUtf8("lvw_outputs"));

        verticalLayout_3->addWidget(lvw_outputs);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout_4->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        ptx_rules = new QPlainTextEdit(tab_setup);
        ptx_rules->setObjectName(QString::fromUtf8("ptx_rules"));
        ptx_rules->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ptx_rules->setLineWrapMode(QPlainTextEdit::NoWrap);
        ptx_rules->setBackgroundVisible(false);

        horizontalLayout_4->addWidget(ptx_rules);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        btn_generate_rules = new QToolButton(tab_setup);
        btn_generate_rules->setObjectName(QString::fromUtf8("btn_generate_rules"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/wizard.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_generate_rules->setIcon(icon3);

        verticalLayout_7->addWidget(btn_generate_rules);

        btn_check_rules = new QToolButton(tab_setup);
        btn_check_rules->setObjectName(QString::fromUtf8("btn_check_rules"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/icons/spellcheck.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_check_rules->setIcon(icon4);

        verticalLayout_7->addWidget(btn_check_rules);

        btn_ok = new QToolButton(tab_setup);
        btn_ok->setObjectName(QString::fromUtf8("btn_ok"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/icons/apply.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_ok->setIcon(icon5);

        verticalLayout_7->addWidget(btn_ok);

        btn_clear = new QToolButton(tab_setup);
        btn_clear->setObjectName(QString::fromUtf8("btn_clear"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_clear->setIcon(icon6);

        verticalLayout_7->addWidget(btn_clear);


        horizontalLayout_4->addLayout(verticalLayout_7);


        verticalLayout_4->addLayout(horizontalLayout_4);


        verticalLayout_6->addLayout(verticalLayout_4);

        tab_container->addTab(tab_setup, QString());
        tab_test = new QWidget();
        tab_test->setObjectName(QString::fromUtf8("tab_test"));
        verticalLayout_5 = new QVBoxLayout(tab_test);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        grx_test_inputs_outer_container = new QGroupBox(tab_test);
        grx_test_inputs_outer_container->setObjectName(QString::fromUtf8("grx_test_inputs_outer_container"));
        grx_test_inputs_outer_container->setAlignment(Qt::AlignCenter);
        grx_test_inputs_outer_container->setFlat(true);
        verticalLayout_9 = new QVBoxLayout(grx_test_inputs_outer_container);
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(0, 8, 0, 0);
        sca_test_inputs = new QScrollArea(grx_test_inputs_outer_container);
        sca_test_inputs->setObjectName(QString::fromUtf8("sca_test_inputs"));
        sca_test_inputs->setFrameShape(QFrame::NoFrame);
        sca_test_inputs->setFrameShadow(QFrame::Plain);
        sca_test_inputs->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sca_test_inputs->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 183, 461));
        verticalLayout_11 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setContentsMargins(0, 0, 0, 0);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        grx_test_inputs = new QGroupBox(scrollAreaWidgetContents);
        grx_test_inputs->setObjectName(QString::fromUtf8("grx_test_inputs"));
        grx_test_inputs->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        verticalLayout_8 = new QVBoxLayout(grx_test_inputs);
        verticalLayout_8->setContentsMargins(4, 4, 4, 4);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer);


        verticalLayout_11->addWidget(grx_test_inputs);

        sca_test_inputs->setWidget(scrollAreaWidgetContents);

        verticalLayout_9->addWidget(sca_test_inputs);


        horizontalLayout_5->addWidget(grx_test_inputs_outer_container);

        grx_test_rules = new QGroupBox(tab_test);
        grx_test_rules->setObjectName(QString::fromUtf8("grx_test_rules"));
        grx_test_rules->setAlignment(Qt::AlignCenter);
        grx_test_rules->setFlat(true);
        horizontalLayout_6 = new QHBoxLayout(grx_test_rules);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 8, 0, 0);
        lsw_test_rules = new QListWidget(grx_test_rules);
        lsw_test_rules->setObjectName(QString::fromUtf8("lsw_test_rules"));
        lsw_test_rules->setAlternatingRowColors(false);
        lsw_test_rules->setSelectionMode(QAbstractItemView::NoSelection);
        lsw_test_rules->setMovement(QListView::Static);
        lsw_test_rules->setResizeMode(QListView::Adjust);
        lsw_test_rules->setUniformItemSizes(false);
        lsw_test_rules->setWordWrap(true);

        horizontalLayout_6->addWidget(lsw_test_rules);

        lsw_test_rules_activation = new QListWidget(grx_test_rules);
        lsw_test_rules_activation->setObjectName(QString::fromUtf8("lsw_test_rules_activation"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lsw_test_rules_activation->sizePolicy().hasHeightForWidth());
        lsw_test_rules_activation->setSizePolicy(sizePolicy1);
        lsw_test_rules_activation->setMaximumSize(QSize(80, 16777215));
        lsw_test_rules_activation->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        lsw_test_rules_activation->setSelectionMode(QAbstractItemView::NoSelection);
        lsw_test_rules_activation->setUniformItemSizes(false);

        horizontalLayout_6->addWidget(lsw_test_rules_activation);


        horizontalLayout_5->addWidget(grx_test_rules);

        grx_test_outputs_outer_container = new QGroupBox(tab_test);
        grx_test_outputs_outer_container->setObjectName(QString::fromUtf8("grx_test_outputs_outer_container"));
        grx_test_outputs_outer_container->setAlignment(Qt::AlignCenter);
        grx_test_outputs_outer_container->setFlat(true);
        verticalLayout_12 = new QVBoxLayout(grx_test_outputs_outer_container);
        verticalLayout_12->setSpacing(0);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalLayout_12->setContentsMargins(0, 8, 0, 0);
        sca_test_outputs = new QScrollArea(grx_test_outputs_outer_container);
        sca_test_outputs->setObjectName(QString::fromUtf8("sca_test_outputs"));
        sca_test_outputs->setFrameShape(QFrame::NoFrame);
        sca_test_outputs->setFrameShadow(QFrame::Plain);
        sca_test_outputs->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sca_test_outputs->setWidgetResizable(true);
        sca_test_outputs->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 183, 461));
        verticalLayout_13 = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout_13->setSpacing(0);
        verticalLayout_13->setContentsMargins(0, 0, 0, 0);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        grx_test_outputs = new QGroupBox(scrollAreaWidgetContents_2);
        grx_test_outputs->setObjectName(QString::fromUtf8("grx_test_outputs"));
        grx_test_outputs->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        verticalLayout_15 = new QVBoxLayout(grx_test_outputs);
        verticalLayout_15->setContentsMargins(4, 4, 4, 4);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_15->addItem(verticalSpacer_2);


        verticalLayout_13->addWidget(grx_test_outputs);

        sca_test_outputs->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_12->addWidget(sca_test_outputs);


        horizontalLayout_5->addWidget(grx_test_outputs_outer_container);


        verticalLayout_5->addLayout(horizontalLayout_5);

        tab_container->addTab(tab_test, QString());

        verticalLayout->addWidget(tab_container);

        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 22));
        menuGui = new QMenu(menuBar);
        menuGui->setObjectName(QString::fromUtf8("menuGui"));
        menuExamples = new QMenu(menuBar);
        menuExamples->setObjectName(QString::fromUtf8("menuExamples"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuGui->menuAction());
        menuBar->addAction(menuExamples->menuAction());
        menuExamples->addAction(actionMamdani);
        menuExamples->addAction(actionSimple_1x1);
        menuExamples->addAction(actionPower_3x1);
        menuExamples->addSeparator();
        menuExamples->addAction(actionTakagi_Sugeno);
        menuExamples->addAction(actionApproximation_1x1);

        retranslateUi(MainWindow);

        tab_container->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "FuzzyLite GUI", 0, QApplication::UnicodeUTF8));
        actionSimple_1x1->setText(QApplication::translate("MainWindow", "Health (1x1)", 0, QApplication::UnicodeUTF8));
        actionPower_3x1->setText(QApplication::translate("MainWindow", "Power (3x1)", 0, QApplication::UnicodeUTF8));
        actionSetup->setText(QApplication::translate("MainWindow", "Setup", 0, QApplication::UnicodeUTF8));
        actionTest->setText(QApplication::translate("MainWindow", "Test", 0, QApplication::UnicodeUTF8));
        actionTile->setText(QApplication::translate("MainWindow", "Tile", 0, QApplication::UnicodeUTF8));
        actionCascade->setText(QApplication::translate("MainWindow", "Cascade", 0, QApplication::UnicodeUTF8));
        actionLog->setText(QApplication::translate("MainWindow", "Log", 0, QApplication::UnicodeUTF8));
        actionApproximation_1x1->setText(QApplication::translate("MainWindow", "Approximation (1x1)", 0, QApplication::UnicodeUTF8));
        actionMamdani->setText(QApplication::translate("MainWindow", "Mamdani", 0, QApplication::UnicodeUTF8));
        actionTakagi_Sugeno->setText(QApplication::translate("MainWindow", "Takagi-Sugeno", 0, QApplication::UnicodeUTF8));
        lbl_inputs->setText(QApplication::translate("MainWindow", "Inputs", 0, QApplication::UnicodeUTF8));
        btn_add_input->setText(QString());
        btn_remove_input->setText(QString());
        btn_edit_input->setText(QString());
        lbl_outputs->setText(QApplication::translate("MainWindow", "Outputs", 0, QApplication::UnicodeUTF8));
        btn_add_output->setText(QString());
        btn_remove_output->setText(QString());
        btn_edit_output->setText(QString());
        ptx_rules->setPlainText(QString());
        btn_generate_rules->setText(QString());
        btn_check_rules->setText(QString());
        btn_ok->setText(QString());
        btn_clear->setText(QString());
        tab_container->setTabText(tab_container->indexOf(tab_setup), QApplication::translate("MainWindow", "Setup", 0, QApplication::UnicodeUTF8));
        grx_test_inputs_outer_container->setTitle(QApplication::translate("MainWindow", "Inputs", 0, QApplication::UnicodeUTF8));
        grx_test_inputs->setTitle(QString());
        grx_test_rules->setTitle(QApplication::translate("MainWindow", "Rules", 0, QApplication::UnicodeUTF8));
        grx_test_outputs_outer_container->setTitle(QApplication::translate("MainWindow", "Outputs", 0, QApplication::UnicodeUTF8));
        grx_test_outputs->setTitle(QString());
        tab_container->setTabText(tab_container->indexOf(tab_test), QApplication::translate("MainWindow", "Test", 0, QApplication::UnicodeUTF8));
        menuGui->setTitle(QApplication::translate("MainWindow", "gui", 0, QApplication::UnicodeUTF8));
        menuExamples->setTitle(QApplication::translate("MainWindow", "Examples", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
