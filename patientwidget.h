#ifndef PATIENTWIDGET_H
#define PATIENTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QSqlQuery>
#include <QDate>
#include <QDebug>
#include <QSqlError>


class PatientWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PatientWidget(QWidget *parent = nullptr);
    void setPatientId(int value);

private:
    QVBoxLayout* patientsVBoxLayout;
    QPushButton* registerButton;
    QListWidget* widget;

    int patientId;

    void retrieveAssignments();
    void registerPatient();

};


#endif // PATIENTWIDGET_H
