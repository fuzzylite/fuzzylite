/********************************************************************************
** Form generated from reading UI file 'LinguisticTerm.ui'
**
** Created: Sat Feb 12 19:33:24 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINGUISTICTERM_H
#define UI_LINGUISTICTERM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "gui/GuiCanvas.h"

QT_BEGIN_NAMESPACE

class Ui_LinguisticTerm
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lbl_name;
    QLineEdit *led_name;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tbw_lterms;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *lbl_triangular_a;
    QDoubleSpinBox *dsx_triangular_a;
    QVBoxLayout *verticalLayout_5;
    QLabel *lbl_triangular_b;
    QDoubleSpinBox *dsx_triangular_b;
    QVBoxLayout *verticalLayout_6;
    QLabel *lbl_triangular_c;
    QDoubleSpinBox *dsx_triangular_c;
    QWidget *tab_4;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_16;
    QLabel *lbl_rectangular_min;
    QDoubleSpinBox *dsx_rectangular_min;
    QVBoxLayout *verticalLayout_15;
    QLabel *lbl_rectangular_max;
    QDoubleSpinBox *dsx_rectangular_max;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_7;
    QLabel *lbl_trapezoidal_a;
    QDoubleSpinBox *dsx_trapezoidal_a;
    QVBoxLayout *verticalLayout_8;
    QLabel *lbl_trapezoidal_b;
    QDoubleSpinBox *dsx_trapezoidal_b;
    QVBoxLayout *verticalLayout_10;
    QLabel *lbl_trapezoidal_c;
    QDoubleSpinBox *dsx_trapezoidal_c;
    QVBoxLayout *verticalLayout_9;
    QLabel *lbl_trapezoidal_d;
    QDoubleSpinBox *dsx_trapezoidal_d;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_13;
    QRadioButton *rbt_shoulder_left;
    QRadioButton *rbt_shoulder_right;
    QVBoxLayout *verticalLayout_12;
    QLabel *lbl_shoulder_min;
    QDoubleSpinBox *dsx_shoulder_min;
    QVBoxLayout *verticalLayout_11;
    QLabel *lbl_shoulder_max;
    QDoubleSpinBox *dsx_shoulder_max;
    QWidget *tab_5;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_14;
    QLabel *lbl_singleton_value;
    QDoubleSpinBox *dsx_singleton_value;
    QSpacerItem *horizontalSpacer_2;
    QWidget *tab_6;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *horizontalLayout_8;
    QLabel *lbl_fx;
    QLineEdit *led_fx;
    QVBoxLayout *verticalLayout_17;
    QLabel *lbl_fx_min;
    QDoubleSpinBox *dsx_fx_min;
    QVBoxLayout *verticalLayout_18;
    QLabel *lbl_fx_max;
    QDoubleSpinBox *dsx_fx_max;
    fl_gui::GuiCanvas *gvw_canvas;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *LinguisticTerm)
    {
        if (LinguisticTerm->objectName().isEmpty())
            LinguisticTerm->setObjectName(QString::fromUtf8("LinguisticTerm"));
        LinguisticTerm->resize(413, 378);
        verticalLayout = new QVBoxLayout(LinguisticTerm);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        lbl_name = new QLabel(LinguisticTerm);
        lbl_name->setObjectName(QString::fromUtf8("lbl_name"));

        horizontalLayout_5->addWidget(lbl_name);

        led_name = new QLineEdit(LinguisticTerm);
        led_name->setObjectName(QString::fromUtf8("led_name"));

        horizontalLayout_5->addWidget(led_name);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tbw_lterms = new QTabWidget(LinguisticTerm);
        tbw_lterms->setObjectName(QString::fromUtf8("tbw_lterms"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tbw_lterms->sizePolicy().hasHeightForWidth());
        tbw_lterms->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(12);
        tbw_lterms->setFont(font);
        tbw_lterms->setTabPosition(QTabWidget::North);
        tbw_lterms->setTabShape(QTabWidget::Rounded);
        tbw_lterms->setElideMode(Qt::ElideMiddle);
        tbw_lterms->setUsesScrollButtons(true);
        tbw_lterms->setDocumentMode(false);
        tbw_lterms->setTabsClosable(false);
        tbw_lterms->setMovable(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setSpacing(2);
        verticalLayout_2->setContentsMargins(4, 4, 4, 4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, -1, 0, -1);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        lbl_triangular_a = new QLabel(tab);
        lbl_triangular_a->setObjectName(QString::fromUtf8("lbl_triangular_a"));
        QFont font1;
        font1.setPointSize(11);
        lbl_triangular_a->setFont(font1);
        lbl_triangular_a->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(lbl_triangular_a);

        dsx_triangular_a = new QDoubleSpinBox(tab);
        dsx_triangular_a->setObjectName(QString::fromUtf8("dsx_triangular_a"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dsx_triangular_a->sizePolicy().hasHeightForWidth());
        dsx_triangular_a->setSizePolicy(sizePolicy1);
        dsx_triangular_a->setFont(font);
        dsx_triangular_a->setAlignment(Qt::AlignCenter);
        dsx_triangular_a->setAccelerated(true);
        dsx_triangular_a->setMinimum(-1e+09);
        dsx_triangular_a->setMaximum(1e+09);
        dsx_triangular_a->setSingleStep(0.1);

        verticalLayout_4->addWidget(dsx_triangular_a);


        horizontalLayout_2->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        lbl_triangular_b = new QLabel(tab);
        lbl_triangular_b->setObjectName(QString::fromUtf8("lbl_triangular_b"));
        lbl_triangular_b->setFont(font1);
        lbl_triangular_b->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(lbl_triangular_b);

        dsx_triangular_b = new QDoubleSpinBox(tab);
        dsx_triangular_b->setObjectName(QString::fromUtf8("dsx_triangular_b"));
        sizePolicy1.setHeightForWidth(dsx_triangular_b->sizePolicy().hasHeightForWidth());
        dsx_triangular_b->setSizePolicy(sizePolicy1);
        dsx_triangular_b->setFont(font);
        dsx_triangular_b->setAlignment(Qt::AlignCenter);
        dsx_triangular_b->setAccelerated(true);
        dsx_triangular_b->setMinimum(-1e+09);
        dsx_triangular_b->setMaximum(1e+09);
        dsx_triangular_b->setSingleStep(0.1);

        verticalLayout_5->addWidget(dsx_triangular_b);


        horizontalLayout_2->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        lbl_triangular_c = new QLabel(tab);
        lbl_triangular_c->setObjectName(QString::fromUtf8("lbl_triangular_c"));
        lbl_triangular_c->setFont(font1);
        lbl_triangular_c->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(lbl_triangular_c);

        dsx_triangular_c = new QDoubleSpinBox(tab);
        dsx_triangular_c->setObjectName(QString::fromUtf8("dsx_triangular_c"));
        sizePolicy1.setHeightForWidth(dsx_triangular_c->sizePolicy().hasHeightForWidth());
        dsx_triangular_c->setSizePolicy(sizePolicy1);
        dsx_triangular_c->setFont(font);
        dsx_triangular_c->setAlignment(Qt::AlignCenter);
        dsx_triangular_c->setAccelerated(true);
        dsx_triangular_c->setMinimum(-1e+09);
        dsx_triangular_c->setMaximum(1e+09);
        dsx_triangular_c->setSingleStep(0.1);

        verticalLayout_6->addWidget(dsx_triangular_c);


        horizontalLayout_2->addLayout(verticalLayout_6);


        verticalLayout_2->addLayout(horizontalLayout_2);

        tbw_lterms->addTab(tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        horizontalLayout_6 = new QHBoxLayout(tab_4);
        horizontalLayout_6->setSpacing(2);
        horizontalLayout_6->setContentsMargins(4, 4, 4, 4);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(0);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        lbl_rectangular_min = new QLabel(tab_4);
        lbl_rectangular_min->setObjectName(QString::fromUtf8("lbl_rectangular_min"));
        lbl_rectangular_min->setFont(font1);
        lbl_rectangular_min->setAlignment(Qt::AlignCenter);

        verticalLayout_16->addWidget(lbl_rectangular_min);

        dsx_rectangular_min = new QDoubleSpinBox(tab_4);
        dsx_rectangular_min->setObjectName(QString::fromUtf8("dsx_rectangular_min"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dsx_rectangular_min->sizePolicy().hasHeightForWidth());
        dsx_rectangular_min->setSizePolicy(sizePolicy2);
        dsx_rectangular_min->setFont(font);
        dsx_rectangular_min->setAlignment(Qt::AlignCenter);
        dsx_rectangular_min->setAccelerated(true);
        dsx_rectangular_min->setMinimum(-1e+09);
        dsx_rectangular_min->setMaximum(1e+09);
        dsx_rectangular_min->setSingleStep(0.1);

        verticalLayout_16->addWidget(dsx_rectangular_min);


        horizontalLayout_6->addLayout(verticalLayout_16);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(0);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        lbl_rectangular_max = new QLabel(tab_4);
        lbl_rectangular_max->setObjectName(QString::fromUtf8("lbl_rectangular_max"));
        lbl_rectangular_max->setFont(font1);
        lbl_rectangular_max->setAlignment(Qt::AlignCenter);

        verticalLayout_15->addWidget(lbl_rectangular_max);

        dsx_rectangular_max = new QDoubleSpinBox(tab_4);
        dsx_rectangular_max->setObjectName(QString::fromUtf8("dsx_rectangular_max"));
        sizePolicy2.setHeightForWidth(dsx_rectangular_max->sizePolicy().hasHeightForWidth());
        dsx_rectangular_max->setSizePolicy(sizePolicy2);
        dsx_rectangular_max->setFont(font);
        dsx_rectangular_max->setAlignment(Qt::AlignCenter);
        dsx_rectangular_max->setAccelerated(true);
        dsx_rectangular_max->setMinimum(-1e+09);
        dsx_rectangular_max->setMaximum(1e+09);
        dsx_rectangular_max->setSingleStep(0.1);

        verticalLayout_15->addWidget(dsx_rectangular_max);


        horizontalLayout_6->addLayout(verticalLayout_15);

        tbw_lterms->addTab(tab_4, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayout_4 = new QHBoxLayout(tab_2);
        horizontalLayout_4->setSpacing(2);
        horizontalLayout_4->setContentsMargins(4, 4, 4, 4);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, -1, 0, -1);
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        lbl_trapezoidal_a = new QLabel(tab_2);
        lbl_trapezoidal_a->setObjectName(QString::fromUtf8("lbl_trapezoidal_a"));
        lbl_trapezoidal_a->setFont(font1);
        lbl_trapezoidal_a->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(lbl_trapezoidal_a);

        dsx_trapezoidal_a = new QDoubleSpinBox(tab_2);
        dsx_trapezoidal_a->setObjectName(QString::fromUtf8("dsx_trapezoidal_a"));
        sizePolicy1.setHeightForWidth(dsx_trapezoidal_a->sizePolicy().hasHeightForWidth());
        dsx_trapezoidal_a->setSizePolicy(sizePolicy1);
        dsx_trapezoidal_a->setFont(font);
        dsx_trapezoidal_a->setAlignment(Qt::AlignCenter);
        dsx_trapezoidal_a->setAccelerated(true);
        dsx_trapezoidal_a->setMinimum(-1e+09);
        dsx_trapezoidal_a->setMaximum(1e+09);
        dsx_trapezoidal_a->setSingleStep(0.1);

        verticalLayout_7->addWidget(dsx_trapezoidal_a);


        horizontalLayout_3->addLayout(verticalLayout_7);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        lbl_trapezoidal_b = new QLabel(tab_2);
        lbl_trapezoidal_b->setObjectName(QString::fromUtf8("lbl_trapezoidal_b"));
        lbl_trapezoidal_b->setFont(font1);
        lbl_trapezoidal_b->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(lbl_trapezoidal_b);

        dsx_trapezoidal_b = new QDoubleSpinBox(tab_2);
        dsx_trapezoidal_b->setObjectName(QString::fromUtf8("dsx_trapezoidal_b"));
        sizePolicy1.setHeightForWidth(dsx_trapezoidal_b->sizePolicy().hasHeightForWidth());
        dsx_trapezoidal_b->setSizePolicy(sizePolicy1);
        dsx_trapezoidal_b->setFont(font);
        dsx_trapezoidal_b->setAlignment(Qt::AlignCenter);
        dsx_trapezoidal_b->setAccelerated(true);
        dsx_trapezoidal_b->setMinimum(-1e+09);
        dsx_trapezoidal_b->setMaximum(1e+09);
        dsx_trapezoidal_b->setSingleStep(0.1);

        verticalLayout_8->addWidget(dsx_trapezoidal_b);


        horizontalLayout_3->addLayout(verticalLayout_8);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        lbl_trapezoidal_c = new QLabel(tab_2);
        lbl_trapezoidal_c->setObjectName(QString::fromUtf8("lbl_trapezoidal_c"));
        lbl_trapezoidal_c->setFont(font1);
        lbl_trapezoidal_c->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(lbl_trapezoidal_c);

        dsx_trapezoidal_c = new QDoubleSpinBox(tab_2);
        dsx_trapezoidal_c->setObjectName(QString::fromUtf8("dsx_trapezoidal_c"));
        sizePolicy1.setHeightForWidth(dsx_trapezoidal_c->sizePolicy().hasHeightForWidth());
        dsx_trapezoidal_c->setSizePolicy(sizePolicy1);
        dsx_trapezoidal_c->setFont(font);
        dsx_trapezoidal_c->setAlignment(Qt::AlignCenter);
        dsx_trapezoidal_c->setAccelerated(true);
        dsx_trapezoidal_c->setMinimum(-1e+09);
        dsx_trapezoidal_c->setMaximum(1e+09);
        dsx_trapezoidal_c->setSingleStep(0.1);

        verticalLayout_10->addWidget(dsx_trapezoidal_c);


        horizontalLayout_3->addLayout(verticalLayout_10);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        lbl_trapezoidal_d = new QLabel(tab_2);
        lbl_trapezoidal_d->setObjectName(QString::fromUtf8("lbl_trapezoidal_d"));
        lbl_trapezoidal_d->setFont(font1);
        lbl_trapezoidal_d->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(lbl_trapezoidal_d);

        dsx_trapezoidal_d = new QDoubleSpinBox(tab_2);
        dsx_trapezoidal_d->setObjectName(QString::fromUtf8("dsx_trapezoidal_d"));
        sizePolicy1.setHeightForWidth(dsx_trapezoidal_d->sizePolicy().hasHeightForWidth());
        dsx_trapezoidal_d->setSizePolicy(sizePolicy1);
        dsx_trapezoidal_d->setFont(font);
        dsx_trapezoidal_d->setAlignment(Qt::AlignCenter);
        dsx_trapezoidal_d->setAccelerated(true);
        dsx_trapezoidal_d->setMinimum(-1e+09);
        dsx_trapezoidal_d->setMaximum(1e+09);
        dsx_trapezoidal_d->setSingleStep(0.1);

        verticalLayout_9->addWidget(dsx_trapezoidal_d);


        horizontalLayout_3->addLayout(verticalLayout_9);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        tbw_lterms->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        horizontalLayout = new QHBoxLayout(tab_3);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setContentsMargins(4, 4, 4, 4);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        rbt_shoulder_left = new QRadioButton(tab_3);
        rbt_shoulder_left->setObjectName(QString::fromUtf8("rbt_shoulder_left"));
        rbt_shoulder_left->setFont(font1);
        rbt_shoulder_left->setChecked(true);

        verticalLayout_13->addWidget(rbt_shoulder_left);

        rbt_shoulder_right = new QRadioButton(tab_3);
        rbt_shoulder_right->setObjectName(QString::fromUtf8("rbt_shoulder_right"));
        rbt_shoulder_right->setFont(font1);

        verticalLayout_13->addWidget(rbt_shoulder_right);


        horizontalLayout->addLayout(verticalLayout_13);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(0);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        lbl_shoulder_min = new QLabel(tab_3);
        lbl_shoulder_min->setObjectName(QString::fromUtf8("lbl_shoulder_min"));
        lbl_shoulder_min->setFont(font1);
        lbl_shoulder_min->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(lbl_shoulder_min);

        dsx_shoulder_min = new QDoubleSpinBox(tab_3);
        dsx_shoulder_min->setObjectName(QString::fromUtf8("dsx_shoulder_min"));
        sizePolicy1.setHeightForWidth(dsx_shoulder_min->sizePolicy().hasHeightForWidth());
        dsx_shoulder_min->setSizePolicy(sizePolicy1);
        dsx_shoulder_min->setFont(font);
        dsx_shoulder_min->setAlignment(Qt::AlignCenter);
        dsx_shoulder_min->setAccelerated(true);
        dsx_shoulder_min->setMinimum(-1e+09);
        dsx_shoulder_min->setMaximum(1e+09);
        dsx_shoulder_min->setSingleStep(0.1);

        verticalLayout_12->addWidget(dsx_shoulder_min);


        horizontalLayout->addLayout(verticalLayout_12);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        lbl_shoulder_max = new QLabel(tab_3);
        lbl_shoulder_max->setObjectName(QString::fromUtf8("lbl_shoulder_max"));
        lbl_shoulder_max->setFont(font1);
        lbl_shoulder_max->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(lbl_shoulder_max);

        dsx_shoulder_max = new QDoubleSpinBox(tab_3);
        dsx_shoulder_max->setObjectName(QString::fromUtf8("dsx_shoulder_max"));
        sizePolicy1.setHeightForWidth(dsx_shoulder_max->sizePolicy().hasHeightForWidth());
        dsx_shoulder_max->setSizePolicy(sizePolicy1);
        dsx_shoulder_max->setFont(font);
        dsx_shoulder_max->setAlignment(Qt::AlignCenter);
        dsx_shoulder_max->setAccelerated(true);
        dsx_shoulder_max->setMinimum(-1e+09);
        dsx_shoulder_max->setMaximum(1e+09);
        dsx_shoulder_max->setSingleStep(0.1);

        verticalLayout_11->addWidget(dsx_shoulder_max);


        horizontalLayout->addLayout(verticalLayout_11);

        tbw_lterms->addTab(tab_3, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        horizontalLayout_7 = new QHBoxLayout(tab_5);
        horizontalLayout_7->setSpacing(2);
        horizontalLayout_7->setContentsMargins(4, 4, 4, 4);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(0);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        lbl_singleton_value = new QLabel(tab_5);
        lbl_singleton_value->setObjectName(QString::fromUtf8("lbl_singleton_value"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lbl_singleton_value->sizePolicy().hasHeightForWidth());
        lbl_singleton_value->setSizePolicy(sizePolicy3);
        lbl_singleton_value->setFont(font1);
        lbl_singleton_value->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(lbl_singleton_value);

        dsx_singleton_value = new QDoubleSpinBox(tab_5);
        dsx_singleton_value->setObjectName(QString::fromUtf8("dsx_singleton_value"));
        sizePolicy2.setHeightForWidth(dsx_singleton_value->sizePolicy().hasHeightForWidth());
        dsx_singleton_value->setSizePolicy(sizePolicy2);
        dsx_singleton_value->setAlignment(Qt::AlignCenter);
        dsx_singleton_value->setMinimum(-1e+09);
        dsx_singleton_value->setMaximum(1e+09);
        dsx_singleton_value->setSingleStep(0.1);

        verticalLayout_14->addWidget(dsx_singleton_value);


        horizontalLayout_7->addLayout(verticalLayout_14);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_2);

        tbw_lterms->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        horizontalLayout_9 = new QHBoxLayout(tab_6);
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, -1, -1, -1);
        lbl_fx = new QLabel(tab_6);
        lbl_fx->setObjectName(QString::fromUtf8("lbl_fx"));
        lbl_fx->setFont(font1);

        horizontalLayout_8->addWidget(lbl_fx);

        led_fx = new QLineEdit(tab_6);
        led_fx->setObjectName(QString::fromUtf8("led_fx"));

        horizontalLayout_8->addWidget(led_fx);


        horizontalLayout_9->addLayout(horizontalLayout_8);

        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setSpacing(0);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        lbl_fx_min = new QLabel(tab_6);
        lbl_fx_min->setObjectName(QString::fromUtf8("lbl_fx_min"));
        lbl_fx_min->setFont(font1);
        lbl_fx_min->setAlignment(Qt::AlignCenter);

        verticalLayout_17->addWidget(lbl_fx_min);

        dsx_fx_min = new QDoubleSpinBox(tab_6);
        dsx_fx_min->setObjectName(QString::fromUtf8("dsx_fx_min"));
        sizePolicy1.setHeightForWidth(dsx_fx_min->sizePolicy().hasHeightForWidth());
        dsx_fx_min->setSizePolicy(sizePolicy1);
        dsx_fx_min->setFont(font);
        dsx_fx_min->setAlignment(Qt::AlignCenter);
        dsx_fx_min->setAccelerated(true);
        dsx_fx_min->setMinimum(-1e+09);
        dsx_fx_min->setMaximum(1e+09);
        dsx_fx_min->setSingleStep(0.1);

        verticalLayout_17->addWidget(dsx_fx_min);


        horizontalLayout_9->addLayout(verticalLayout_17);

        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setSpacing(0);
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        lbl_fx_max = new QLabel(tab_6);
        lbl_fx_max->setObjectName(QString::fromUtf8("lbl_fx_max"));
        lbl_fx_max->setFont(font1);
        lbl_fx_max->setAlignment(Qt::AlignCenter);

        verticalLayout_18->addWidget(lbl_fx_max);

        dsx_fx_max = new QDoubleSpinBox(tab_6);
        dsx_fx_max->setObjectName(QString::fromUtf8("dsx_fx_max"));
        sizePolicy1.setHeightForWidth(dsx_fx_max->sizePolicy().hasHeightForWidth());
        dsx_fx_max->setSizePolicy(sizePolicy1);
        dsx_fx_max->setFont(font);
        dsx_fx_max->setAlignment(Qt::AlignCenter);
        dsx_fx_max->setAccelerated(true);
        dsx_fx_max->setMinimum(-1e+09);
        dsx_fx_max->setMaximum(1e+09);
        dsx_fx_max->setSingleStep(0.1);

        verticalLayout_18->addWidget(dsx_fx_max);


        horizontalLayout_9->addLayout(verticalLayout_18);

        tbw_lterms->addTab(tab_6, QString());

        verticalLayout_3->addWidget(tbw_lterms);


        verticalLayout->addLayout(verticalLayout_3);

        gvw_canvas = new fl_gui::GuiCanvas(LinguisticTerm);
        gvw_canvas->setObjectName(QString::fromUtf8("gvw_canvas"));
        gvw_canvas->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gvw_canvas->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout->addWidget(gvw_canvas);

        buttonBox = new QDialogButtonBox(LinguisticTerm);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(LinguisticTerm);
        QObject::connect(buttonBox, SIGNAL(accepted()), LinguisticTerm, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LinguisticTerm, SLOT(reject()));

        tbw_lterms->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LinguisticTerm);
    } // setupUi

    void retranslateUi(QDialog *LinguisticTerm)
    {
        LinguisticTerm->setWindowTitle(QApplication::translate("LinguisticTerm", "Linguistic Term", 0, QApplication::UnicodeUTF8));
        lbl_name->setText(QApplication::translate("LinguisticTerm", "Name", 0, QApplication::UnicodeUTF8));
        lbl_triangular_a->setText(QApplication::translate("LinguisticTerm", "A", 0, QApplication::UnicodeUTF8));
        lbl_triangular_b->setText(QApplication::translate("LinguisticTerm", "B", 0, QApplication::UnicodeUTF8));
        lbl_triangular_c->setText(QApplication::translate("LinguisticTerm", "C", 0, QApplication::UnicodeUTF8));
        tbw_lterms->setTabText(tbw_lterms->indexOf(tab), QApplication::translate("LinguisticTerm", "Triangular", 0, QApplication::UnicodeUTF8));
        lbl_rectangular_min->setText(QApplication::translate("LinguisticTerm", "Minimum", 0, QApplication::UnicodeUTF8));
        lbl_rectangular_max->setText(QApplication::translate("LinguisticTerm", "Maximum", 0, QApplication::UnicodeUTF8));
        tbw_lterms->setTabText(tbw_lterms->indexOf(tab_4), QApplication::translate("LinguisticTerm", "Rectangular", 0, QApplication::UnicodeUTF8));
        lbl_trapezoidal_a->setText(QApplication::translate("LinguisticTerm", "A", 0, QApplication::UnicodeUTF8));
        lbl_trapezoidal_b->setText(QApplication::translate("LinguisticTerm", "B", 0, QApplication::UnicodeUTF8));
        lbl_trapezoidal_c->setText(QApplication::translate("LinguisticTerm", "C", 0, QApplication::UnicodeUTF8));
        lbl_trapezoidal_d->setText(QApplication::translate("LinguisticTerm", "D", 0, QApplication::UnicodeUTF8));
        tbw_lterms->setTabText(tbw_lterms->indexOf(tab_2), QApplication::translate("LinguisticTerm", "Trapezoidal", 0, QApplication::UnicodeUTF8));
        rbt_shoulder_left->setText(QApplication::translate("LinguisticTerm", "Left", 0, QApplication::UnicodeUTF8));
        rbt_shoulder_right->setText(QApplication::translate("LinguisticTerm", "Right", 0, QApplication::UnicodeUTF8));
        lbl_shoulder_min->setText(QApplication::translate("LinguisticTerm", "Min", 0, QApplication::UnicodeUTF8));
        lbl_shoulder_max->setText(QApplication::translate("LinguisticTerm", "Max", 0, QApplication::UnicodeUTF8));
        tbw_lterms->setTabText(tbw_lterms->indexOf(tab_3), QApplication::translate("LinguisticTerm", "Shoulder", 0, QApplication::UnicodeUTF8));
        lbl_singleton_value->setText(QApplication::translate("LinguisticTerm", "Value", 0, QApplication::UnicodeUTF8));
        tbw_lterms->setTabText(tbw_lterms->indexOf(tab_5), QApplication::translate("LinguisticTerm", "Singleton", 0, QApplication::UnicodeUTF8));
        lbl_fx->setText(QApplication::translate("LinguisticTerm", "f(x)=", 0, QApplication::UnicodeUTF8));
        lbl_fx_min->setText(QApplication::translate("LinguisticTerm", "Min", 0, QApplication::UnicodeUTF8));
        lbl_fx_max->setText(QApplication::translate("LinguisticTerm", "Max", 0, QApplication::UnicodeUTF8));
        tbw_lterms->setTabText(tbw_lterms->indexOf(tab_6), QApplication::translate("LinguisticTerm", "f(x)", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LinguisticTerm: public Ui_LinguisticTerm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINGUISTICTERM_H
