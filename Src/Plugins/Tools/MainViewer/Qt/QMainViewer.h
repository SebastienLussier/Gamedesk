#ifndef QMAINVIEWER_H
#define QMAINVIEWER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_QMainViewer
{
public:
    QWidget *widget_2;
    QWidget *widget_3;
    QWidget *widget_4;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *mBtnRecord;
    QFrame *mViewerFrame;

    void setupUi(QWidget *QMainViewer)
    {
    QMainViewer->setObjectName(QString::fromUtf8("QMainViewer"));
    QMainViewer->resize(QSize(559, 476).expandedTo(QMainViewer->minimumSizeHint()));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(5));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(QMainViewer->sizePolicy().hasHeightForWidth());
    QMainViewer->setSizePolicy(sizePolicy);
    QMainViewer->setFocusPolicy(Qt::ClickFocus);
    widget_2 = new QWidget(QMainViewer);
    widget_2->setObjectName(QString::fromUtf8("widget_2"));
    widget_2->setGeometry(QRect(9, 9, 541, 458));
    widget_3 = new QWidget(QMainViewer);
    widget_3->setObjectName(QString::fromUtf8("widget_3"));
    widget_3->setGeometry(QRect(0, 0, 101, 31));
    widget_4 = new QWidget(QMainViewer);
    widget_4->setObjectName(QString::fromUtf8("widget_4"));
    widget_4->setGeometry(QRect(0, 0, 101, 31));
    vboxLayout = new QVBoxLayout(QMainViewer);
    vboxLayout->setSpacing(0);
    vboxLayout->setMargin(9);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(0);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(510, 25, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    mBtnRecord = new QPushButton(QMainViewer);
    mBtnRecord->setObjectName(QString::fromUtf8("mBtnRecord"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(mBtnRecord->sizePolicy().hasHeightForWidth());
    mBtnRecord->setSizePolicy(sizePolicy1);
    mBtnRecord->setMinimumSize(QSize(25, 25));
    mBtnRecord->setMaximumSize(QSize(25, 25));
    mBtnRecord->setFocusPolicy(Qt::NoFocus);
    mBtnRecord->setIcon(QIcon(QString::fromUtf8("../Resources/movie.bmp")));
    mBtnRecord->setCheckable(true);
    mBtnRecord->setFlat(true);

    hboxLayout->addWidget(mBtnRecord);


    vboxLayout->addLayout(hboxLayout);

    mViewerFrame = new QFrame(QMainViewer);
    mViewerFrame->setObjectName(QString::fromUtf8("mViewerFrame"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(mViewerFrame->sizePolicy().hasHeightForWidth());
    mViewerFrame->setSizePolicy(sizePolicy2);
    mViewerFrame->setFrameShape(QFrame::NoFrame);
    mViewerFrame->setFrameShadow(QFrame::Plain);
    mViewerFrame->setLineWidth(0);
    mViewerFrame->setMidLineWidth(0);

    vboxLayout->addWidget(mViewerFrame);

    retranslateUi(QMainViewer);

    QMetaObject::connectSlotsByName(QMainViewer);
    } // setupUi

    void retranslateUi(QWidget *QMainViewer)
    {
    QMainViewer->setWindowTitle(QApplication::translate("QMainViewer", "Main Viewer", 0, QApplication::UnicodeUTF8));
    mBtnRecord->setToolTip(QApplication::translate("QMainViewer", "Record a movie", 0, QApplication::UnicodeUTF8));
    mBtnRecord->setText(QApplication::translate("QMainViewer", "", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(QMainViewer);
    } // retranslateUi

};

namespace Ui {
    class QMainViewer: public Ui_QMainViewer {};
} // namespace Ui

#endif // QMAINVIEWER_H
