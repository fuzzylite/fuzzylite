/********************************************************************************
** Form generated from reading ui file 'DiscreteTerm.ui'
**
** Created: Sat Dec 12 11:24:29 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DISCRETETERM_H
#define UI_DISCRETETERM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DiscreteTerm
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbl_name;
    QLineEdit *led_name;
    QCheckBox *ckx_from_infinity;
    QCheckBox *ckx_to_infinity;
    QGroupBox *grx_triangle;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QLabel *lbl_triangle_a;
    QDoubleSpinBox *dsx_triangle_a;
    QVBoxLayout *verticalLayout_5;
    QLabel *lbl_triangle_b;
    QDoubleSpinBox *dsx_triangle_b;
    QVBoxLayout *verticalLayout_6;
    QLabel *lbl_triangle_c;
    QDoubleSpinBox *dsx_triangle_c;
    QToolButton *btn_add_triangle;
    QGroupBox *grx_trapezoid;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_7;
    QLabel *lbl_trapezoid_a;
    QDoubleSpinBox *dsx_trapezoid_a;
    QVBoxLayout *verticalLayout_9;
    QLabel *lbl_trapezoid_b;
    QDoubleSpinBox *dsx_trapezoid_b;
    QVBoxLayout *verticalLayout_12;
    QLabel *lbl_trapezoid_c;
    QDoubleSpinBox *dsx_trapezoid_c;
    QVBoxLayout *verticalLayout_8;
    QLabel *lbl_trapezoid_d;
    QDoubleSpinBox *dsx_trapezoid_d;
    QToolButton *btn_add_trapezoid;
    QVBoxLayout *verticalLayout_3;
    QLabel *lbl_coords;
    QPlainTextEdit *ptx_coords;
    QPushButton *btn_graph;
    QDialogButtonBox *btn_dialog;

    void setupUi(QDialog *DiscreteTerm)
    {
        if (DiscreteTerm->objectName().isEmpty())
            DiscreteTerm->setObjectName(QString::fromUtf8("DiscreteTerm"));
        DiscreteTerm->setWindowModality(Qt::WindowModal);
        DiscreteTerm->resize(458, 281);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DiscreteTerm->sizePolicy().hasHeightForWidth());
        DiscreteTerm->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(DiscreteTerm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
#ifndef Q_OS_MAC
        verticalLayout->setSpacing(-1);
#endif
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_2 = new QHBoxLayout();
#ifndef Q_OS_MAC
        horizontalLayout_2->setSpacing(-1);
#endif
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetNoConstraint);
        lbl_name = new QLabel(DiscreteTerm);
        lbl_name->setObjectName(QString::fromUtf8("lbl_name"));

        horizontalLayout_2->addWidget(lbl_name);

        led_name = new QLineEdit(DiscreteTerm);
        led_name->setObjectName(QString::fromUtf8("led_name"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(led_name->sizePolicy().hasHeightForWidth());
        led_name->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(led_name);

        ckx_from_infinity = new QCheckBox(DiscreteTerm);
        ckx_from_infinity->setObjectName(QString::fromUtf8("ckx_from_infinity"));

        horizontalLayout_2->addWidget(ckx_from_infinity);

        ckx_to_infinity = new QCheckBox(DiscreteTerm);
        ckx_to_infinity->setObjectName(QString::fromUtf8("ckx_to_infinity"));

        horizontalLayout_2->addWidget(ckx_to_infinity);


        verticalLayout->addLayout(horizontalLayout_2);

        grx_triangle = new QGroupBox(DiscreteTerm);
        grx_triangle->setObjectName(QString::fromUtf8("grx_triangle"));
        sizePolicy.setHeightForWidth(grx_triangle->sizePolicy().hasHeightForWidth());
        grx_triangle->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(grx_triangle);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        lbl_triangle_a = new QLabel(grx_triangle);
        lbl_triangle_a->setObjectName(QString::fromUtf8("lbl_triangle_a"));
        lbl_triangle_a->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(lbl_triangle_a);

        dsx_triangle_a = new QDoubleSpinBox(grx_triangle);
        dsx_triangle_a->setObjectName(QString::fromUtf8("dsx_triangle_a"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dsx_triangle_a->sizePolicy().hasHeightForWidth());
        dsx_triangle_a->setSizePolicy(sizePolicy2);
        dsx_triangle_a->setAlignment(Qt::AlignCenter);
        dsx_triangle_a->setAccelerated(true);
        dsx_triangle_a->setMinimum(-1e+09);
        dsx_triangle_a->setMaximum(1e+09);

        verticalLayout_4->addWidget(dsx_triangle_a);


        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        lbl_triangle_b = new QLabel(grx_triangle);
        lbl_triangle_b->setObjectName(QString::fromUtf8("lbl_triangle_b"));
        lbl_triangle_b->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(lbl_triangle_b);

        dsx_triangle_b = new QDoubleSpinBox(grx_triangle);
        dsx_triangle_b->setObjectName(QString::fromUtf8("dsx_triangle_b"));
        sizePolicy2.setHeightForWidth(dsx_triangle_b->sizePolicy().hasHeightForWidth());
        dsx_triangle_b->setSizePolicy(sizePolicy2);
        dsx_triangle_b->setAlignment(Qt::AlignCenter);
        dsx_triangle_b->setAccelerated(true);
        dsx_triangle_b->setMinimum(-1e+09);
        dsx_triangle_b->setMaximum(1e+09);

        verticalLayout_5->addWidget(dsx_triangle_b);


        horizontalLayout_3->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        lbl_triangle_c = new QLabel(grx_triangle);
        lbl_triangle_c->setObjectName(QString::fromUtf8("lbl_triangle_c"));
        lbl_triangle_c->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(lbl_triangle_c);

        dsx_triangle_c = new QDoubleSpinBox(grx_triangle);
        dsx_triangle_c->setObjectName(QString::fromUtf8("dsx_triangle_c"));
        sizePolicy2.setHeightForWidth(dsx_triangle_c->sizePolicy().hasHeightForWidth());
        dsx_triangle_c->setSizePolicy(sizePolicy2);
        dsx_triangle_c->setAlignment(Qt::AlignCenter);
        dsx_triangle_c->setAccelerated(true);
        dsx_triangle_c->setMinimum(-1e+09);
        dsx_triangle_c->setMaximum(1e+09);

        verticalLayout_6->addWidget(dsx_triangle_c);


        horizontalLayout_3->addLayout(verticalLayout_6);

        btn_add_triangle = new QToolButton(grx_triangle);
        btn_add_triangle->setObjectName(QString::fromUtf8("btn_add_triangle"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/edit_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_add_triangle->setIcon(icon);

        horizontalLayout_3->addWidget(btn_add_triangle);


        verticalLayout->addWidget(grx_triangle);

        grx_trapezoid = new QGroupBox(DiscreteTerm);
        grx_trapezoid->setObjectName(QString::fromUtf8("grx_trapezoid"));
        sizePolicy.setHeightForWidth(grx_trapezoid->sizePolicy().hasHeightForWidth());
        grx_trapezoid->setSizePolicy(sizePolicy);
        horizontalLayout_4 = new QHBoxLayout(grx_trapezoid);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        lbl_trapezoid_a = new QLabel(grx_trapezoid);
        lbl_trapezoid_a->setObjectName(QString::fromUtf8("lbl_trapezoid_a"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lbl_trapezoid_a->sizePolicy().hasHeightForWidth());
        lbl_trapezoid_a->setSizePolicy(sizePolicy3);
        lbl_trapezoid_a->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(lbl_trapezoid_a);

        dsx_trapezoid_a = new QDoubleSpinBox(grx_trapezoid);
        dsx_trapezoid_a->setObjectName(QString::fromUtf8("dsx_trapezoid_a"));
        sizePolicy2.setHeightForWidth(dsx_trapezoid_a->sizePolicy().hasHeightForWidth());
        dsx_trapezoid_a->setSizePolicy(sizePolicy2);
        dsx_trapezoid_a->setAlignment(Qt::AlignCenter);
        dsx_trapezoid_a->setAccelerated(true);
        dsx_trapezoid_a->setMinimum(-1e+09);
        dsx_trapezoid_a->setMaximum(1e+09);

        verticalLayout_7->addWidget(dsx_trapezoid_a);


        horizontalLayout_4->addLayout(verticalLayout_7);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        lbl_trapezoid_b = new QLabel(grx_trapezoid);
        lbl_trapezoid_b->setObjectName(QString::fromUtf8("lbl_trapezoid_b"));
        sizePolicy3.setHeightForWidth(lbl_trapezoid_b->sizePolicy().hasHeightForWidth());
        lbl_trapezoid_b->setSizePolicy(sizePolicy3);
        lbl_trapezoid_b->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(lbl_trapezoid_b);

        dsx_trapezoid_b = new QDoubleSpinBox(grx_trapezoid);
        dsx_trapezoid_b->setObjectName(QString::fromUtf8("dsx_trapezoid_b"));
        sizePolicy2.setHeightForWidth(dsx_trapezoid_b->sizePolicy().hasHeightForWidth());
        dsx_trapezoid_b->setSizePolicy(sizePolicy2);
        dsx_trapezoid_b->setAlignment(Qt::AlignCenter);
        dsx_trapezoid_b->setAccelerated(true);
        dsx_trapezoid_b->setMinimum(-1e+09);
        dsx_trapezoid_b->setMaximum(1e+09);

        verticalLayout_9->addWidget(dsx_trapezoid_b);


        horizontalLayout_4->addLayout(verticalLayout_9);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        lbl_trapezoid_c = new QLabel(grx_trapezoid);
        lbl_trapezoid_c->setObjectName(QString::fromUtf8("lbl_trapezoid_c"));
        sizePolicy3.setHeightForWidth(lbl_trapezoid_c->sizePolicy().hasHeightForWidth());
        lbl_trapezoid_c->setSizePolicy(sizePolicy3);
        lbl_trapezoid_c->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(lbl_trapezoid_c);

        dsx_trapezoid_c = new QDoubleSpinBox(grx_trapezoid);
        dsx_trapezoid_c->setObjectName(QString::fromUtf8("dsx_trapezoid_c"));
        sizePolicy2.setHeightForWidth(dsx_trapezoid_c->sizePolicy().hasHeightForWidth());
        dsx_trapezoid_c->setSizePolicy(sizePolicy2);
        dsx_trapezoid_c->setAlignment(Qt::AlignCenter);
        dsx_trapezoid_c->setAccelerated(true);
        dsx_trapezoid_c->setMinimum(-1e+09);
        dsx_trapezoid_c->setMaximum(1e+09);

        verticalLayout_12->addWidget(dsx_trapezoid_c);


        horizontalLayout_4->addLayout(verticalLayout_12);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        lbl_trapezoid_d = new QLabel(grx_trapezoid);
        lbl_trapezoid_d->setObjectName(QString::fromUtf8("lbl_trapezoid_d"));
        sizePolicy3.setHeightForWidth(lbl_trapezoid_d->sizePolicy().hasHeightForWidth());
        lbl_trapezoid_d->setSizePolicy(sizePolicy3);
        lbl_trapezoid_d->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(lbl_trapezoid_d);

        dsx_trapezoid_d = new QDoubleSpinBox(grx_trapezoid);
        dsx_trapezoid_d->setObjectName(QString::fromUtf8("dsx_trapezoid_d"));
        sizePolicy2.setHeightForWidth(dsx_trapezoid_d->sizePolicy().hasHeightForWidth());
        dsx_trapezoid_d->setSizePolicy(sizePolicy2);
        dsx_trapezoid_d->setAlignment(Qt::AlignCenter);
        dsx_trapezoid_d->setAccelerated(true);
        dsx_trapezoid_d->setMinimum(-1e+09);
        dsx_trapezoid_d->setMaximum(1e+09);

        verticalLayout_8->addWidget(dsx_trapezoid_d);


        horizontalLayout_4->addLayout(verticalLayout_8);

        btn_add_trapezoid = new QToolButton(grx_trapezoid);
        btn_add_trapezoid->setObjectName(QString::fromUtf8("btn_add_trapezoid"));
        btn_add_trapezoid->setIcon(icon);

        horizontalLayout_4->addWidget(btn_add_trapezoid);


        verticalLayout->addWidget(grx_trapezoid);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        lbl_coords = new QLabel(DiscreteTerm);
        lbl_coords->setObjectName(QString::fromUtf8("lbl_coords"));

        verticalLayout_3->addWidget(lbl_coords);

        ptx_coords = new QPlainTextEdit(DiscreteTerm);
        ptx_coords->setObjectName(QString::fromUtf8("ptx_coords"));
        ptx_coords->setLineWrapMode(QPlainTextEdit::NoWrap);

        verticalLayout_3->addWidget(ptx_coords);

        btn_graph = new QPushButton(DiscreteTerm);
        btn_graph->setObjectName(QString::fromUtf8("btn_graph"));

        verticalLayout_3->addWidget(btn_graph);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout_2->addLayout(horizontalLayout);

        btn_dialog = new QDialogButtonBox(DiscreteTerm);
        btn_dialog->setObjectName(QString::fromUtf8("btn_dialog"));
        QSizePolicy sizePolicy4(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(btn_dialog->sizePolicy().hasHeightForWidth());
        btn_dialog->setSizePolicy(sizePolicy4);
        btn_dialog->setOrientation(Qt::Horizontal);
        btn_dialog->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(btn_dialog);


        retranslateUi(DiscreteTerm);
        QObject::connect(btn_dialog, SIGNAL(accepted()), DiscreteTerm, SLOT(accept()));
        QObject::connect(btn_dialog, SIGNAL(rejected()), DiscreteTerm, SLOT(reject()));

        QMetaObject::connectSlotsByName(DiscreteTerm);
    } // setupUi

    void retranslateUi(QDialog *DiscreteTerm)
    {
        DiscreteTerm->setWindowTitle(QApplication::translate("DiscreteTerm", "Discrete Term", 0, QApplication::UnicodeUTF8));
        lbl_name->setText(QApplication::translate("DiscreteTerm", "Name:", 0, QApplication::UnicodeUTF8));
        ckx_from_infinity->setText(QApplication::translate("DiscreteTerm", "-inf", 0, QApplication::UnicodeUTF8));
        ckx_to_infinity->setText(QApplication::translate("DiscreteTerm", "+inf", 0, QApplication::UnicodeUTF8));
        grx_triangle->setTitle(QApplication::translate("DiscreteTerm", "Triangle", 0, QApplication::UnicodeUTF8));
        lbl_triangle_a->setText(QApplication::translate("DiscreteTerm", "A", 0, QApplication::UnicodeUTF8));
        lbl_triangle_b->setText(QApplication::translate("DiscreteTerm", "B", 0, QApplication::UnicodeUTF8));
        lbl_triangle_c->setText(QApplication::translate("DiscreteTerm", "C", 0, QApplication::UnicodeUTF8));
        btn_add_triangle->setText(QString());
        grx_trapezoid->setTitle(QApplication::translate("DiscreteTerm", "Trapezoid", 0, QApplication::UnicodeUTF8));
        lbl_trapezoid_a->setText(QApplication::translate("DiscreteTerm", "A", 0, QApplication::UnicodeUTF8));
        lbl_trapezoid_b->setText(QApplication::translate("DiscreteTerm", "B", 0, QApplication::UnicodeUTF8));
        lbl_trapezoid_c->setText(QApplication::translate("DiscreteTerm", "C", 0, QApplication::UnicodeUTF8));
        lbl_trapezoid_d->setText(QApplication::translate("DiscreteTerm", "D", 0, QApplication::UnicodeUTF8));
        btn_add_trapezoid->setText(QString());
        lbl_coords->setText(QApplication::translate("DiscreteTerm", "Coords (x:y)", 0, QApplication::UnicodeUTF8));
        btn_graph->setText(QApplication::translate("DiscreteTerm", "Graph", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DiscreteTerm);
    } // retranslateUi

};

namespace Ui {
    class DiscreteTerm: public Ui_DiscreteTerm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISCRETETERM_H
