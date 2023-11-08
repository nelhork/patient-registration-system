#include "patientwidget.h"

PatientWidget::PatientWidget(QWidget *parent)
    : QWidget{parent}
{
    patientsVBoxLayout = new QVBoxLayout(this);
    registerButton = new QPushButton("Зарегистрироваться");
    registerButton->setEnabled(false);

    widget = new QListWidget(this);

    this->setLayout(patientsVBoxLayout);
    patientsVBoxLayout->addWidget(widget);
    patientsVBoxLayout->addWidget(registerButton);

    connect(registerButton, &QPushButton::clicked, this, &PatientWidget::registerPatient);
    connect(widget, &QListWidget::itemSelectionChanged, this, [=]() {
       if (widget->selectedItems().isEmpty()) {
           registerButton->setEnabled(false);
       } else {
           auto item = widget->selectedItems()[0];
           if (item->data(Qt::UserRole + 1).isNull()) {
               registerButton->setEnabled(true);
           }
       }
    });
}

void PatientWidget::setPatientId(int value)
{
    patientId = value;
    retrieveAssignments();
}

void PatientWidget::retrieveAssignments()
{
    QSqlQuery query;
    query.prepare("SELECT registered_at, assignments.id AS id, procedures.name, assignments.appointed_at, assignments.time FROM assignments JOIN procedures ON procedure_id = procedures.id WHERE appointed_at = :appointed_at AND patient_id = :patient_id");
    query.bindValue(":appointed_at", QDate::currentDate().toString("yyyy-MM-dd"));
    query.bindValue(":patient_id", patientId);
    query.exec();
    qDebug() << "last error " << query.lastError().text();

    widget->clear();

    while(query.next()) {
        QString name = query.value("name").toString() + ", " + query.value("appointed_at").toString() + ", " + query.value("time").toString();
        if (!query.value("registered_at").isNull()) {
            name+= " (Зарегистрирован)";
        }
        QListWidgetItem* item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, query.value("id").toInt());
        item->setData(Qt::UserRole + 1, query.value("registered_at"));
        widget->addItem(item);
    }
}

void PatientWidget::registerPatient()
{
    auto item = widget->selectedItems()[0];

    QSqlQuery query;
    query.prepare("UPDATE assignments SET registered_at = :registered_at WHERE id = :id");
    query.bindValue(":registered_at", QTime::currentTime().toString("hh:mm"));
    query.bindValue(":id", item->data(Qt::UserRole).toInt());
    query.exec();
    qDebug() << "id " << query.boundValue(":id");
    qDebug() << "last error " << query.lastError().text();
    item->setData(Qt::UserRole + 1, true);
    retrieveAssignments();

}
