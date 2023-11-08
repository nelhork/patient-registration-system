#ifndef PATIENTDIALOG_H
#define PATIENTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QVBoxLayout>
#include <QDialogButtonBox>

class PatientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PatientDialog(QWidget* parent = nullptr);

    QString getFirstName();
    QString getLastName();
    QString getDiagnosis();
    QString getDateOfBirth();

    void setFirstName(QString value);
    void setLastName(QString value);
    void setDiagnosis(QString value);
    void setDateOfBirth(QString value);

private:
    QLabel* firstNameLabel;
    QLabel* lastNameLabel;
    QLabel* diagnosisLabel;
    QLabel* dateOfBirthLabel;

    QVBoxLayout* dialogLayout;

    QLineEdit* firstNameField;
    QLineEdit* lastNameField;
    QLineEdit* diagnosisField;
    QDateEdit* dateOfBirthField;

    QDialogButtonBox* buttonBox;

    QString firstName;
    QString lastName;
    QString diagnosis;
    QString dateOfBirth;
};

#endif // PATIENTDIALOG_H
