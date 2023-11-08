#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMenuBar>
#include "adminwidget.h"
#include "doctorwidget.h"
#include "userwidget.h"
#include "authorizedialog.h"
#include "authpatientdialog.h"
#include "doctorcalendar.h"
#include "patientwidget.h"
#include <queuewidget.h>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "welcomewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    WelcomeWidget* welcomeWidget;
    AdminWidget *adminWidget = nullptr;
    DoctorWidget *doctorWidget = nullptr;
    UserWidget *userWidget;
    AuthorizeDialog* authDocAdmDialog;
    AuthPatientDialog* authPatientDialog;
    DoctorCalendar* doctorCalendar = nullptr;
    PatientWidget* patientWidget = nullptr;
    QueueWidget* queue = nullptr;
    QMenu *manageMenu;
    QMenu *patientMenu;

    QStackedWidget* stackedWidget;

    void setupMenuBar();
    void retrieveDocAdmData();
    void retrievePatientData();
    void showWidget(QWidget* widget);
};
#endif // MAINWINDOW_H
