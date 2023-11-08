#include "patientdialog.h"

PatientDialog::PatientDialog(QWidget *parent)
    : QDialog{parent}
{
    firstNameLabel = new QLabel(this);
    lastNameLabel = new QLabel(this);
    diagnosisLabel = new QLabel(this);
    dateOfBirthLabel = new QLabel(this);

    firstNameField = new QLineEdit(this);
    lastNameField = new QLineEdit(this);
    diagnosisField = new QLineEdit(this);
    dateOfBirthField = new QDateEdit(this);

    dialogLayout = new QVBoxLayout(this);

    dialogLayout->addWidget(firstNameLabel);
    firstNameLabel->setText("Имя:");
    dialogLayout->addWidget(firstNameField);

    dialogLayout->addWidget(lastNameLabel);
    lastNameLabel->setText("Фамилия:");
    dialogLayout->addWidget(lastNameField);

    dialogLayout->addWidget(diagnosisLabel);
    diagnosisLabel->setText("Диагноз:");
    dialogLayout->addWidget(diagnosisField);

    dialogLayout->addWidget(dateOfBirthLabel);
    dateOfBirthLabel->setText("Дата рождения:");
    dialogLayout->addWidget(dateOfBirthField);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    dialogLayout->addStretch(10);
    dialogLayout->addWidget(buttonBox);

    dateOfBirth = dateOfBirthField->date().toString("yyyy-MM-dd");
    connect(firstNameField, &QLineEdit::textChanged, this, [=](QString value) {
        firstName = value;
    });
    connect(lastNameField, &QLineEdit::textChanged, this, [=](QString value) {
       lastName = value;
    });
    connect(diagnosisField, &QLineEdit::textChanged, this, [=](QString value) {
       diagnosis = value;
    });
    connect(dateOfBirthField, &QDateEdit::dateChanged, this, [=](QDate value) {
       dateOfBirth = value.toString("yyyy-MM-dd");
    });
}

QString PatientDialog::getFirstName()
{
    return firstName;
}

QString PatientDialog::getLastName()
{
    return lastName;
}

QString PatientDialog::getDiagnosis()
{
    return diagnosis;
}

QString PatientDialog::getDateOfBirth()
{
    return dateOfBirth;
}

void PatientDialog::setFirstName(QString value)
{
    firstName = value;
    firstNameField->setText(value);
}

void PatientDialog::setLastName(QString value)
{
    lastName = value;
    lastNameField->setText(value);
}

void PatientDialog::setDiagnosis(QString value)
{
    diagnosis = value;
    diagnosisField->setText(value);
}

void PatientDialog::setDateOfBirth(QString value)
{
    dateOfBirth = value;
    QDate date(QDate::fromString(value, "yyyy-MM-dd"));
    dateOfBirthField->setDate(date);
}
