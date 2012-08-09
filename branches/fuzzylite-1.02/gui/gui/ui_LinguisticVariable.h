/********************************************************************************
** Form generated from reading UI file 'LinguisticVariable.ui'
**
** Created: Sat Feb 12 19:33:24 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINGUISTICVARIABLE_H
#define UI_LINGUISTICVARIABLE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include "gui/GuiCanvas.h"

QT_BEGIN_NAMESPACE

class Ui_LinguisticVariable
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *grx_lvar;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lbl_name;
    QLineEdit *led_name;
    QToolButton *btn_wizard;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbl_terms;
    QToolButton *btn_add_term;
    QToolButton *btn_remove_term;
    QToolButton *btn_edit_term;
    QListWidget *lvw_terms;
    fl_gui::GuiCanvas *gvw_canvas;
    QHBoxLayout *horizontalLayout;
    QLabel *lbl_lvar_min;
    QLineEdit *led_lvar_min;
    QLabel *lbl_lvar_max;
    QLineEdit *led_lvar_max;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *btn_dialog;

    void setupUi(QDialog *LinguisticVariable)
    {
        if (LinguisticVariable->objectName().isEmpty())
            LinguisticVariable->setObjectName(QString::fromUtf8("LinguisticVariable"));
        LinguisticVariable->setWindowModality(Qt::ApplicationModal);
        LinguisticVariable->resize(257, 399);
        verticalLayout = new QVBoxLayout(LinguisticVariable);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        grx_lvar = new QGroupBox(LinguisticVariable);
        grx_lvar->setObjectName(QString::fromUtf8("grx_lvar"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(grx_lvar->sizePolicy().hasHeightForWidth());
        grx_lvar->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(grx_lvar);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(12, -1, -1, -1);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lbl_name = new QLabel(grx_lvar);
        lbl_name->setObjectName(QString::fromUtf8("lbl_name"));

        horizontalLayout_3->addWidget(lbl_name);

        led_name = new QLineEdit(grx_lvar);
        led_name->setObjectName(QString::fromUtf8("led_name"));

        horizontalLayout_3->addWidget(led_name);

        btn_wizard = new QToolButton(grx_lvar);
        btn_wizard->setObjectName(QString::fromUtf8("btn_wizard"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/wizard.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_wizard->setIcon(icon);

        horizontalLayout_3->addWidget(btn_wizard);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lbl_terms = new QLabel(grx_lvar);
        lbl_terms->setObjectName(QString::fromUtf8("lbl_terms"));

        horizontalLayout_2->addWidget(lbl_terms);

        btn_add_term = new QToolButton(grx_lvar);
        btn_add_term->setObjectName(QString::fromUtf8("btn_add_term"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/edit_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_add_term->setIcon(icon1);

        horizontalLayout_2->addWidget(btn_add_term);

        btn_remove_term = new QToolButton(grx_lvar);
        btn_remove_term->setObjectName(QString::fromUtf8("btn_remove_term"));
        btn_remove_term->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/edit_remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_remove_term->setIcon(icon2);

        horizontalLayout_2->addWidget(btn_remove_term);

        btn_edit_term = new QToolButton(grx_lvar);
        btn_edit_term->setObjectName(QString::fromUtf8("btn_edit_term"));
        btn_edit_term->setEnabled(false);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/pencil.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_edit_term->setIcon(icon3);

        horizontalLayout_2->addWidget(btn_edit_term);


        verticalLayout_3->addLayout(horizontalLayout_2);

        lvw_terms = new QListWidget(grx_lvar);
        lvw_terms->setObjectName(QString::fromUtf8("lvw_terms"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lvw_terms->sizePolicy().hasHeightForWidth());
        lvw_terms->setSizePolicy(sizePolicy1);
        lvw_terms->setMaximumSize(QSize(16777215, 100));
        lvw_terms->setFocusPolicy(Qt::StrongFocus);
        lvw_terms->setSelectionMode(QAbstractItemView::ExtendedSelection);
        lvw_terms->setViewMode(QListView::ListMode);

        verticalLayout_3->addWidget(lvw_terms);


        verticalLayout_2->addLayout(verticalLayout_3);

        gvw_canvas = new fl_gui::GuiCanvas(grx_lvar);
        gvw_canvas->setObjectName(QString::fromUtf8("gvw_canvas"));
        gvw_canvas->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gvw_canvas->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gvw_canvas->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);

        verticalLayout_2->addWidget(gvw_canvas);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lbl_lvar_min = new QLabel(grx_lvar);
        lbl_lvar_min->setObjectName(QString::fromUtf8("lbl_lvar_min"));

        horizontalLayout->addWidget(lbl_lvar_min);

        led_lvar_min = new QLineEdit(grx_lvar);
        led_lvar_min->setObjectName(QString::fromUtf8("led_lvar_min"));
        led_lvar_min->setAlignment(Qt::AlignCenter);
        led_lvar_min->setReadOnly(true);

        horizontalLayout->addWidget(led_lvar_min);

        lbl_lvar_max = new QLabel(grx_lvar);
        lbl_lvar_max->setObjectName(QString::fromUtf8("lbl_lvar_max"));

        horizontalLayout->addWidget(lbl_lvar_max);

        led_lvar_max = new QLineEdit(grx_lvar);
        led_lvar_max->setObjectName(QString::fromUtf8("led_lvar_max"));
        led_lvar_max->setAlignment(Qt::AlignCenter);
        led_lvar_max->setReadOnly(true);

        horizontalLayout->addWidget(led_lvar_max);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addWidget(grx_lvar);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        btn_dialog = new QDialogButtonBox(LinguisticVariable);
        btn_dialog->setObjectName(QString::fromUtf8("btn_dialog"));
        btn_dialog->setOrientation(Qt::Horizontal);
        btn_dialog->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(btn_dialog);


        retranslateUi(LinguisticVariable);
        QObject::connect(btn_dialog, SIGNAL(accepted()), LinguisticVariable, SLOT(accept()));
        QObject::connect(btn_dialog, SIGNAL(rejected()), LinguisticVariable, SLOT(reject()));

        QMetaObject::connectSlotsByName(LinguisticVariable);
    } // setupUi

    void retranslateUi(QDialog *LinguisticVariable)
    {
        LinguisticVariable->setWindowTitle(QApplication::translate("LinguisticVariable", "Linguistic Variable", 0, QApplication::UnicodeUTF8));
        grx_lvar->setTitle(QApplication::translate("LinguisticVariable", "Linguistic Variable", 0, QApplication::UnicodeUTF8));
        lbl_name->setText(QApplication::translate("LinguisticVariable", "Name", 0, QApplication::UnicodeUTF8));
        btn_wizard->setText(QString());
        lbl_terms->setText(QApplication::translate("LinguisticVariable", "Terms", 0, QApplication::UnicodeUTF8));
        btn_add_term->setText(QString());
        btn_remove_term->setText(QString());
        btn_edit_term->setText(QString());
        lbl_lvar_min->setText(QApplication::translate("LinguisticVariable", "Min", 0, QApplication::UnicodeUTF8));
        lbl_lvar_max->setText(QApplication::translate("LinguisticVariable", "Max", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LinguisticVariable: public Ui_LinguisticVariable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINGUISTICVARIABLE_H
