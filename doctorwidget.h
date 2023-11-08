#ifndef DOCTORWIDGET_H
#define DOCTORWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "patientdialog.h"
#include <QListWidget>
#include "assignmentsdialog.h"


class DoctorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DoctorWidget(QWidget *parent = nullptr);

private:
    QVBoxLayout* vBoxLayout;
    QHBoxLayout* hBoxLayout;
    QHBoxLayout* mainLayout;

    QVBoxLayout* assignsVBoxLayout;
    QHBoxLayout* assignsHBoxLayout;

    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* delButton;

    QPushButton* assignsAddButton;
    QPushButton* assignsEditButton;
    QPushButton* assignsDelButton;

    PatientDialog* createDialog;
    PatientDialog* editDialog;

    AssignmentsDialog* createAssignmentsDialog;
    AssignmentsDialog* editAssignmentsDialog;

    QListWidget* patientsWidget;
    QListWidget* assignsWidget;

    void showPatientDialog();
    void retrieveData();
    void addPatient();
    void showEditForm();
    void updateData();
    void deleteData();
    void execQuery(QSqlQuery &query);
    void retrieveAssigns();
    void addAssign();
    void showEditAssignForm();
    void delAssign();
    void showAssignmentsDialog();
    void execAssignmentQuery(QSqlQuery &query);
    void updateAssignment();
    bool checkProceduresCount(int patientId, QString appointedAt);
    bool checkIsIdUnique(int patientId, QString appointedAt, int procedureId);
};

#endif // DOCTORWIDGET_H
