#include "assignmentsdialog.h"

void AssignmentsDialog::fillHours()
{
    timeField->clear();
    auto hours = getAvailableHours(appointedDateField->date());
    for (int i = 0; i < hours.count(); i++) {
        timeField->addItem(hours[i]);
    }
}

AssignmentsDialog::AssignmentsDialog(QWidget *parent)
    : QDialog{parent}
{
    procedureIdLabel = new QLabel(this);
    appointedDateLabel = new QLabel(this);

    procedureIdField = new QComboBox(this);
    appointedDateField = new QDateEdit(this);
    appointedDateField->setDate(QDate::currentDate());

    timeLabel = new QLabel(this);
    timeField = new QComboBox(this);

    dialogLayout = new QVBoxLayout(this);

    dialogLayout->addWidget(procedureIdLabel);
    procedureIdLabel->setText("Процедура:");
    loadProcedures();
    procedureId = procedureIdField->itemData(0).toInt();
    dialogLayout->addWidget(procedureIdField);

    dialogLayout->addWidget(appointedDateLabel);
    appointedDateLabel->setText("Дата:");
    dialogLayout->addWidget(appointedDateField);

    dialogLayout->addWidget(timeLabel);
    timeLabel->setText("Время:");
    dialogLayout->addWidget(timeField);

    fillHours();

    time = timeField->itemText(0);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    dialogLayout->addStretch(10);
    dialogLayout->addWidget(buttonBox);

    appointedDate = appointedDateField->date().toString("yyyy-MM-dd");
    connect(procedureIdField, &QComboBox::currentIndexChanged, this, [=](int value) {
        procedureId = procedureIdField->currentData().toInt();
        qDebug() << "id " << procedureIdField->currentData().toInt();
    });
    connect(appointedDateField, &QDateEdit::dateChanged, this, [=](QDate value) {
       appointedDate = value.toString("yyyy-MM-dd");
       fillHours();
    });

    connect(timeField, &QComboBox::currentIndexChanged, this, [=](int value) {
        time = timeField->currentText();
    });
}

int AssignmentsDialog::getPatientId()
{
    return 0;
}

int AssignmentsDialog::getProcedureId()
{
    return procedureId;
}

QString AssignmentsDialog::getAppointedDate()
{
    return appointedDate;
}

void AssignmentsDialog::setPatientId(int value)
{
}

void AssignmentsDialog::setProcedureId(int value)
{
    procedureId = value;
    for (int i = 0; i < procedureIdField->count(); i++) {
        if (procedureIdField->itemData(i).toInt() == value) {
            procedureIdField->setCurrentIndex(i);
        }
    }
}

void AssignmentsDialog::setAppointedDate(QString value)
{
    appointedDate = value;
    QDate date(QDate::fromString(value, "yyyy-MM-dd"));
    appointedDateField->setDate(date);
    fillHours();
}

void AssignmentsDialog::loadProcedures()
{
    QSqlQuery query;
    query.exec("SELECT * FROM procedures");
    procedureIdField->clear();

    while(query.next()) {
        procedureIdField->addItem(query.value("name").toString(), query.value("id").toInt());
    }
}

QStringList AssignmentsDialog::getAvailableHours(QDate date)
{
    QStringList hours;
    QStringList possibleHours;
    QStringList takenHours;

    for (int i = 8; i < 17; i++) {
        hours.append(QString::number(i) + ":00");
    }
    QSqlQuery query;
    query.prepare("SELECT time FROM assignments WHERE appointed_at = :appointed_at");
    query.bindValue(":appointed_at", date.toString("yyyy-MM-dd"));
    query.exec();

    while(query.next()) {
        auto hour = query.value("time").toString();
        if (!takenHours.contains(hour)) {
            takenHours.append(hour);
        }
    }
    for(int i = 0; i < hours.count(); i++) {
        if (!takenHours.contains(hours[i])) {
            possibleHours.append(hours[i]);
        }
    }
    return possibleHours;
}

QString AssignmentsDialog::getTime()
{
    return time;
}

void AssignmentsDialog::setTime(QString value)
{
    time = value;
    for (int i = 0; i < timeField->count(); i++) {
        if (timeField->itemText(i) == value) {
            timeField->setCurrentIndex(i);
        }
    }
}
