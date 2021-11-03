/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Open;
    QAction *action_Save;
    QAction *action_detect_plane;
    QAction *action;
    QAction *action_Selection;
    QAction *action_Annotation;
    QAction *action_BoundBox;
    QAction *action_Threshold;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_6;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QGroupBox *groupBox_annotations;
    QVBoxLayout *verticalLayout_2;
    QListWidget *listWidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGroupBox *groupBox_Types;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_filename;
    QVTKWidget *qvtkWidget;
    QLabel *label_message;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menu_Filters;
    QMenu *menumode;
    QMenu *menu_Tool;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *PlaneDetect_dockWidget;
    QWidget *dockWidgetContents_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *distanceThreshold_lineEdit;
    QCheckBox *removePlane_checkBox;
    QPushButton *applyPlaneDetection_pushButton;
    QDockWidget *threshold_dockWidget;
    QWidget *dockWidgetContents;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_4;
    QLineEdit *threshold_lineEdit;
    QPushButton *threshold_pushButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(982, 640);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        action_Save = new QAction(MainWindow);
        action_Save->setObjectName(QString::fromUtf8("action_Save"));
        action_detect_plane = new QAction(MainWindow);
        action_detect_plane->setObjectName(QString::fromUtf8("action_detect_plane"));
        action = new QAction(MainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        action_Selection = new QAction(MainWindow);
        action_Selection->setObjectName(QString::fromUtf8("action_Selection"));
        action_Annotation = new QAction(MainWindow);
        action_Annotation->setObjectName(QString::fromUtf8("action_Annotation"));
        action_BoundBox = new QAction(MainWindow);
        action_BoundBox->setObjectName(QString::fromUtf8("action_BoundBox"));
        action_Threshold = new QAction(MainWindow);
        action_Threshold->setObjectName(QString::fromUtf8("action_Threshold"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        verticalLayout_6 = new QVBoxLayout(centralWidget);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        groupBox_annotations = new QGroupBox(splitter);
        groupBox_annotations->setObjectName(QString::fromUtf8("groupBox_annotations"));
        verticalLayout_2 = new QVBoxLayout(groupBox_annotations);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        listWidget = new QListWidget(groupBox_annotations);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_2->addWidget(listWidget);

        scrollArea = new QScrollArea(groupBox_annotations);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 673, 242));
        groupBox_Types = new QGroupBox(scrollAreaWidgetContents);
        groupBox_Types->setObjectName(QString::fromUtf8("groupBox_Types"));
        groupBox_Types->setGeometry(QRect(10, 10, 651, 221));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(groupBox_Types->sizePolicy().hasHeightForWidth());
        groupBox_Types->setSizePolicy(sizePolicy1);
        groupBox_Types->setMaximumSize(QSize(200000, 200000));
        groupBox_Types->setSizeIncrement(QSize(1, 1));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);

        splitter->addWidget(groupBox_annotations);
        splitter_2->addWidget(splitter);
        layoutWidget = new QWidget(splitter_2);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout_5 = new QVBoxLayout(layoutWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_filename = new QLabel(layoutWidget);
        label_filename->setObjectName(QString::fromUtf8("label_filename"));
        label_filename->setMaximumSize(QSize(16777215, 15));

        verticalLayout_5->addWidget(label_filename);

        qvtkWidget = new QVTKWidget(layoutWidget);
        qvtkWidget->setObjectName(QString::fromUtf8("qvtkWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(qvtkWidget->sizePolicy().hasHeightForWidth());
        qvtkWidget->setSizePolicy(sizePolicy2);
        qvtkWidget->setMinimumSize(QSize(0, 0));

        verticalLayout_5->addWidget(qvtkWidget);

        label_message = new QLabel(layoutWidget);
        label_message->setObjectName(QString::fromUtf8("label_message"));
        label_message->setMaximumSize(QSize(16777215, 15));

        verticalLayout_5->addWidget(label_message);

        splitter_2->addWidget(layoutWidget);

        verticalLayout_6->addWidget(splitter_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 982, 30));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_Filters = new QMenu(menuBar);
        menu_Filters->setObjectName(QString::fromUtf8("menu_Filters"));
        menumode = new QMenu(menuBar);
        menumode->setObjectName(QString::fromUtf8("menumode"));
        menu_Tool = new QMenu(menuBar);
        menu_Tool->setObjectName(QString::fromUtf8("menu_Tool"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        PlaneDetect_dockWidget = new QDockWidget(MainWindow);
        PlaneDetect_dockWidget->setObjectName(QString::fromUtf8("PlaneDetect_dockWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(PlaneDetect_dockWidget->sizePolicy().hasHeightForWidth());
        PlaneDetect_dockWidget->setSizePolicy(sizePolicy3);
        PlaneDetect_dockWidget->setMinimumSize(QSize(130, 120));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        sizePolicy.setHeightForWidth(dockWidgetContents_3->sizePolicy().hasHeightForWidth());
        dockWidgetContents_3->setSizePolicy(sizePolicy);
        groupBox = new QGroupBox(dockWidgetContents_3);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, 10, 128, 111));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        distanceThreshold_lineEdit = new QLineEdit(groupBox);
        distanceThreshold_lineEdit->setObjectName(QString::fromUtf8("distanceThreshold_lineEdit"));

        verticalLayout->addWidget(distanceThreshold_lineEdit);

        removePlane_checkBox = new QCheckBox(groupBox);
        removePlane_checkBox->setObjectName(QString::fromUtf8("removePlane_checkBox"));

        verticalLayout->addWidget(removePlane_checkBox);

        applyPlaneDetection_pushButton = new QPushButton(groupBox);
        applyPlaneDetection_pushButton->setObjectName(QString::fromUtf8("applyPlaneDetection_pushButton"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(applyPlaneDetection_pushButton->sizePolicy().hasHeightForWidth());
        applyPlaneDetection_pushButton->setSizePolicy(sizePolicy4);

        verticalLayout->addWidget(applyPlaneDetection_pushButton);

        PlaneDetect_dockWidget->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), PlaneDetect_dockWidget);
        threshold_dockWidget = new QDockWidget(MainWindow);
        threshold_dockWidget->setObjectName(QString::fromUtf8("threshold_dockWidget"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        groupBox_2 = new QGroupBox(dockWidgetContents);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(0, 20, 121, 101));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_3->addWidget(label_4);

        threshold_lineEdit = new QLineEdit(groupBox_2);
        threshold_lineEdit->setObjectName(QString::fromUtf8("threshold_lineEdit"));

        verticalLayout_3->addWidget(threshold_lineEdit);

        threshold_pushButton = new QPushButton(groupBox_2);
        threshold_pushButton->setObjectName(QString::fromUtf8("threshold_pushButton"));
        sizePolicy4.setHeightForWidth(threshold_pushButton->sizePolicy().hasHeightForWidth());
        threshold_pushButton->setSizePolicy(sizePolicy4);

        verticalLayout_3->addWidget(threshold_pushButton);

        threshold_dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), threshold_dockWidget);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menu_Filters->menuAction());
        menuBar->addAction(menumode->menuAction());
        menuBar->addAction(menu_Tool->menuAction());
        menu_File->addAction(action_Open);
        menu_File->addAction(action_Save);
        menu_Filters->addAction(action_detect_plane);
        menumode->addAction(action);
        menumode->addAction(action_Selection);
        menumode->addAction(action_Annotation);
        menu_Tool->addAction(action_BoundBox);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_Open);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_BoundBox);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_detect_plane);
        mainToolBar->addAction(action_Threshold);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        action_Open->setText(QApplication::translate("MainWindow", "&Open", nullptr));
        action_Save->setText(QApplication::translate("MainWindow", "&Save", nullptr));
        action_detect_plane->setText(QApplication::translate("MainWindow", "&DetectPlane", nullptr));
        action->setText(QApplication::translate("MainWindow", "&Visiualize", nullptr));
        action_Selection->setText(QApplication::translate("MainWindow", "&Selection", nullptr));
        action_Annotation->setText(QApplication::translate("MainWindow", "&Annotation", nullptr));
        action_BoundBox->setText(QApplication::translate("MainWindow", "&BoundBox", nullptr));
        action_Threshold->setText(QApplication::translate("MainWindow", "&Threshold", nullptr));
        groupBox_annotations->setTitle(QApplication::translate("MainWindow", "Annotations", nullptr));
        groupBox_Types->setTitle(QApplication::translate("MainWindow", "Types", nullptr));
        label_filename->setText(QApplication::translate("MainWindow", "filename", nullptr));
        label_message->setText(QApplication::translate("MainWindow", "message", nullptr));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", nullptr));
        menu_Filters->setTitle(QApplication::translate("MainWindow", "&Filters", nullptr));
        menumode->setTitle(QApplication::translate("MainWindow", "&Mode", nullptr));
        menu_Tool->setTitle(QApplication::translate("MainWindow", "&Tool", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "Detect Ground", nullptr));
        label->setText(QApplication::translate("MainWindow", "DistanceThreshold:", nullptr));
        removePlane_checkBox->setText(QApplication::translate("MainWindow", "remove plane", nullptr));
        applyPlaneDetection_pushButton->setText(QApplication::translate("MainWindow", "Apply", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Threshold", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "threshold", nullptr));
        threshold_pushButton->setText(QApplication::translate("MainWindow", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
