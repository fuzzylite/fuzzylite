/********************************************************************************
** Form generated from reading ui file 'MainWindow.ui'
**
** Created: Sat Dec 12 11:24:29 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
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
#include <QtGui/QToolBox>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QToolBox *tlx_main;
    QWidget *fe_setup;
    QVBoxLayout *verticalLayout_5;
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
    QWidget *fe_test;
    QVBoxLayout *verticalLayout_6;
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
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setMargin(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tlx_main = new QToolBox(centralwidget);
        tlx_main->setObjectName(QString::fromUtf8("tlx_main"));
        fe_setup = new QWidget();
        fe_setup->setObjectName(QString::fromUtf8("fe_setup"));
        fe_setup->setGeometry(QRect(0, 0, 800, 486));
        verticalLayout_5 = new QVBoxLayout(fe_setup);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lbl_inputs = new QLabel(fe_setup);
        lbl_inputs->setObjectName(QString::fromUtf8("lbl_inputs"));

        horizontalLayout_2->addWidget(lbl_inputs);

        btn_add_input = new QToolButton(fe_setup);
        btn_add_input->setObjectName(QString::fromUtf8("btn_add_input"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/edit_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_add_input->setIcon(icon);

        horizontalLayout_2->addWidget(btn_add_input);

        btn_remove_input = new QToolButton(fe_setup);
        btn_remove_input->setObjectName(QString::fromUtf8("btn_remove_input"));
        btn_remove_input->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/edit_remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_remove_input->setIcon(icon1);

        horizontalLayout_2->addWidget(btn_remove_input);

        btn_edit_input = new QToolButton(fe_setup);
        btn_edit_input->setObjectName(QString::fromUtf8("btn_edit_input"));
        btn_edit_input->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/pencil.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_edit_input->setIcon(icon2);

        horizontalLayout_2->addWidget(btn_edit_input);


        verticalLayout_2->addLayout(horizontalLayout_2);

        lvw_inputs = new QListWidget(fe_setup);
        lvw_inputs->setObjectName(QString::fromUtf8("lvw_inputs"));

        verticalLayout_2->addWidget(lvw_inputs);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lbl_outputs = new QLabel(fe_setup);
        lbl_outputs->setObjectName(QString::fromUtf8("lbl_outputs"));

        horizontalLayout_3->addWidget(lbl_outputs);

        btn_add_output = new QToolButton(fe_setup);
        btn_add_output->setObjectName(QString::fromUtf8("btn_add_output"));
        btn_add_output->setIcon(icon);

        horizontalLayout_3->addWidget(btn_add_output);

        btn_remove_output = new QToolButton(fe_setup);
        btn_remove_output->setObjectName(QString::fromUtf8("btn_remove_output"));
        btn_remove_output->setEnabled(false);
        btn_remove_output->setIcon(icon1);

        horizontalLayout_3->addWidget(btn_remove_output);

        btn_edit_output = new QToolButton(fe_setup);
        btn_edit_output->setObjectName(QString::fromUtf8("btn_edit_output"));
        btn_edit_output->setEnabled(false);
        btn_edit_output->setIcon(icon2);

        horizontalLayout_3->addWidget(btn_edit_output);


        verticalLayout_3->addLayout(horizontalLayout_3);

        lvw_outputs = new QListWidget(fe_setup);
        lvw_outputs->setObjectName(QString::fromUtf8("lvw_outputs"));

        verticalLayout_3->addWidget(lvw_outputs);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout_4->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        ptx_rules = new QPlainTextEdit(fe_setup);
        ptx_rules->setObjectName(QString::fromUtf8("ptx_rules"));
        ptx_rules->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ptx_rules->setLineWrapMode(QPlainTextEdit::NoWrap);
        ptx_rules->setBackgroundVisible(false);

        horizontalLayout_4->addWidget(ptx_rules);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        btn_generate_rules = new QToolButton(fe_setup);
        btn_generate_rules->setObjectName(QString::fromUtf8("btn_generate_rules"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/wizard.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_generate_rules->setIcon(icon3);

        verticalLayout_7->addWidget(btn_generate_rules);

        btn_check_rules = new QToolButton(fe_setup);
        btn_check_rules->setObjectName(QString::fromUtf8("btn_check_rules"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/icons/spellcheck.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_check_rules->setIcon(icon4);

        verticalLayout_7->addWidget(btn_check_rules);


        horizontalLayout_4->addLayout(verticalLayout_7);


        verticalLayout_4->addLayout(horizontalLayout_4);


        verticalLayout_5->addLayout(verticalLayout_4);

        tlx_main->addItem(fe_setup, QString::fromUtf8("Fuzzy Engine: Setup"));
        fe_test = new QWidget();
        fe_test->setObjectName(QString::fromUtf8("fe_test"));
        fe_test->setGeometry(QRect(0, 0, 800, 486));
        verticalLayout_6 = new QVBoxLayout(fe_test);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        grx_test_inputs_outer_container = new QGroupBox(fe_test);
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
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 194, 428));
        verticalLayout_11 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setMargin(0);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        grx_test_inputs = new QGroupBox(scrollAreaWidgetContents);
        grx_test_inputs->setObjectName(QString::fromUtf8("grx_test_inputs"));
        grx_test_inputs->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        verticalLayout_8 = new QVBoxLayout(grx_test_inputs);
        verticalLayout_8->setMargin(4);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer);


        verticalLayout_11->addWidget(grx_test_inputs);

        sca_test_inputs->setWidget(scrollAreaWidgetContents);

        verticalLayout_9->addWidget(sca_test_inputs);


        horizontalLayout_5->addWidget(grx_test_inputs_outer_container);

        grx_test_rules = new QGroupBox(fe_test);
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

        grx_test_outputs_outer_container = new QGroupBox(fe_test);
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
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 194, 428));
        verticalLayout_13 = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout_13->setSpacing(0);
        verticalLayout_13->setMargin(0);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        grx_test_outputs = new QGroupBox(scrollAreaWidgetContents_2);
        grx_test_outputs->setObjectName(QString::fromUtf8("grx_test_outputs"));
        grx_test_outputs->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        verticalLayout_15 = new QVBoxLayout(grx_test_outputs);
        verticalLayout_15->setMargin(4);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_15->addItem(verticalSpacer_2);


        verticalLayout_13->addWidget(grx_test_outputs);

        sca_test_outputs->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_12->addWidget(sca_test_outputs);


        horizontalLayout_5->addWidget(grx_test_outputs_outer_container);


        verticalLayout_6->addLayout(horizontalLayout_5);

        tlx_main->addItem(fe_test, QString::fromUtf8("Fuzzy Engine: Test"));

        verticalLayout->addWidget(tlx_main);

        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 22));
        menuGui = new QMenu(menuBar);
        menuGui->setObjectName(QString::fromUtf8("menuGui"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuGui->menuAction());

        retranslateUi(MainWindow);

        tlx_main->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Fuzzy Lite GUI", 0, QApplication::UnicodeUTF8));
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
        tlx_main->setItemText(tlx_main->indexOf(fe_setup), QApplication::translate("MainWindow", "Fuzzy Engine: Setup", 0, QApplication::UnicodeUTF8));
        grx_test_inputs_outer_container->setTitle(QApplication::translate("MainWindow", "Inputs", 0, QApplication::UnicodeUTF8));
        grx_test_inputs->setTitle(QString());
        grx_test_rules->setTitle(QApplication::translate("MainWindow", "Rules", 0, QApplication::UnicodeUTF8));
        grx_test_outputs_outer_container->setTitle(QApplication::translate("MainWindow", "Outputs", 0, QApplication::UnicodeUTF8));
        grx_test_outputs->setTitle(QString());
        tlx_main->setItemText(tlx_main->indexOf(fe_test), QApplication::translate("MainWindow", "Fuzzy Engine: Test", 0, QApplication::UnicodeUTF8));
        menuGui->setTitle(QApplication::translate("MainWindow", "gui", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
