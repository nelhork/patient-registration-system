#include "doctorwidget.h"

DoctorWidget::DoctorWidget(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QHBoxLayout(this);
    vBoxLayout = new QVBoxLayout(this);

    patientsWidget = new QListWidget(this);
    assignsWidget = new QListWidget(this);

    addButton = new QPushButton("Добавить");
    editButton = new QPushButton("Изменить");
    delButton = new QPushButton("Удалить");
    delButton->setEnabled(false);
    editButton->setEnabled(false);

    createDialog = new PatientDialog(this);
    createDialog->setWindowTitle("Добавление пациента");
    editDialog = new PatientDialog(this);
    editDialog->setWindowTitle("Изменение данных о пациенте");

    hBoxLayout = new QHBoxLayout(this);

    createAssignmentsDialog = new AssignmentsDialog(this);
    createAssignmentsDialog->setWindowTitle("Добавление назначения");
    editAssignmentsDialog = new AssignmentsDialog(this);
    editAssignmentsDialog->setWindowTitle("Изменение назначения");

    assignsVBoxLayout = new QVBoxLayout(this);
    assignsHBoxLayout = new QHBoxLayout(this);

    assignsAddButton = new QPushButton("Добавить");
    assignsEditButton = new QPushButton("Изменить");
    assignsDelButton = new QPushButton("Удалить");
    assignsAddButton->setEnabled(false);
    assignsEditButton->setEnabled(false);
    assignsDelButton->setEnabled(false);

    hBoxLayout->addWidget(addButton);
    hBoxLayout->addWidget(editButton);
    hBoxLayout->addWidget(delButton);

    vBoxLayout->addLayout(hBoxLayout);
    vBoxLayout->addWidget(patientsWidget);

    mainLayout->addLayout(vBoxLayout, 1);
    mainLayout->addLayout(assignsVBoxLayout, 2);
    this->setLayout(mainLayout);

    assignsHBoxLayout->addWidget(assignsAddButton);
    assignsHBoxLayout->addWidget(assignsEditButton);
    assignsHBoxLayout->addWidget(assignsDelButton);
    assignsVBoxLayout->addLayout(assignsHBoxLayout);
    assignsVBoxLayout->addWidget(assignsWidget);

    retrieveData();

    connect(addButton, &QPushButton::clicked,this, &DoctorWidget::showPatientDialog);
    connect(createDialog, &PatientDialog::accepted, this, &DoctorWidget::addPatient);
    connect(createAssignmentsDialog, &AssignmentsDialog::accepted, this, &DoctorWidget::addAssign);

    connect(patientsWidget, &QListWidget::itemSelectionChanged, this,[=]() {
        if (patientsWidget->selectedItems().isEmpty()) {
            delButton->setEnabled(false);
            editButton->setEnabled(false);
            assignsAddButton->setEnabled(false);
        } else {
            delButton->setEnabled(true);
            editButton->setEnabled(true);
            assignsAddButton->setEnabled(true);
            retrieveAssigns();
        }
    });
    connect(delButton, &QPushButton::clicked, this, &DoctorWidget::deleteData);
    connect(editButton, &QPushButton::clicked, this, &DoctorWidget::showEditForm);

    connect(editDialog, &PatientDialog::accepted, this, &DoctorWidget::updateData);
    connect(editAssignmentsDialog, &AssignmentsDialog::accepted, this, &DoctorWidget::updateAssignment);

    connect(assignsAddButton, &QPushButton::clicked, this, &DoctorWidget::showAssignmentsDialog);
    connect(assignsEditButton, &QPushButton::clicked, this, &DoctorWidget::showEditAssignForm);
    connect(assignsDelButton, &QPushButton::clicked, this, &DoctorWidget::delAssign);

    connect(assignsWidget, &QListWidget::itemSelectionChanged, this, [=]() {
        if (assignsWidget->selectedItems().isEmpty()) {
            assignsDelButton->setEnabled(false);
            assignsEditButton->setEnabled(false);
        } else {
            assignsDelButton->setEnabled(true);
            assignsEditButton->setEnabled(true);
        }
    });
}

void DoctorWidget::showPatientDialog()
{
    createDialog->exec();
}

void DoctorWidget::retrieveData()
{
    QSqlQuery query;
    qDebug() << query.exec("SELECT id, (first_name || ' ' || last_name) AS name, current_diagnosis AS diagnosis, (date('now') - date_of_birth) AS age, date_of_birth FROM patients");
    qDebug() << query.lastError().text();

    QSqlQuery countQuery;
    countQuery.exec("SELECT COUNT (*) AS totalRows FROM patients");
    countQuery.first();
    qDebug() << countQuery.lastError().text();

    patientsWidget->clear();

    int rowCount = 0;

    while(query.next()) {

        QString name = query.value("name").toString() + ", " + query.value("age").toString() + "\n" + query.value("diagnosis").toString() + "\n";
        QString diagnosis = query.value("diagnosis").toString();
        QString dateOfBirth = query.value("date_of_birth").toString();
        QListWidgetItem *item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, query.value("id"));
        item->setData(Qt::UserRole + 1, query.value("name").toString());
        item->setData(Qt::UserRole + 2, query.value("diagnosis").toString());
        item->setData(Qt::UserRole + 3, query.value("date_of_birth").toString());
        patientsWidget->addItem(item);

        rowCount++;
    }
}

void DoctorWidget::execQuery(QSqlQuery &query)
{
    bool result = query.exec();
    qDebug() << query.executedQuery();
    if (result) {
        retrieveData();
    } else {
        QMessageBox::critical(this, "Ошибка", "Ошибка выполнения запроса к базе данных.");
        qDebug() << query.lastError().text();
    }
}

void DoctorWidget::retrieveAssigns()
{
    QSqlQuery query;
    query.prepare("SELECT assignments.id, name, duration_in_min, appointed_at, procedure_id, time FROM assignments JOIN procedures ON procedure_id = procedures.id WHERE patient_id = :patient_id");
    auto selectedItem = patientsWidget->selectedItems()[0];
    query.bindValue(":patient_id", selectedItem->data(Qt::UserRole).toString());
    query.exec();
    qDebug() << "ERROR " << query.lastError().text();
    qDebug() << "selected Item " << selectedItem->data(Qt::UserRole).toInt();
    qDebug() << "boundValue" << query.boundValue(":patient_id");

    assignsWidget->clear();

    while(query.next()) {

        QString name = query.value("name").toString() + ", " + query.value("duration_in_min").toString() + "\n" + query.value("appointed_at").toString() + "\n" + query.value("time").toString();
        int duration = query.value("duration_in_min").toInt();
        QDate appointed = query.value("appointed_at").toDate();
        QListWidgetItem *item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, query.value("id").toInt());
        item->setData(Qt::UserRole + 1, query.value("name").toString());
        item->setData(Qt::UserRole + 2, query.value("duration_in_min").toString());
        item->setData(Qt::UserRole + 3, query.value("appointed_at").toString());
        item->setData(Qt::UserRole + 4, query.value("procedure_id").toString());
        item->setData(Qt::UserRole + 5, query.value("time").toString());
        assignsWidget->addItem(item);
    }
}

void DoctorWidget::addAssign()
{
    int patientId = patientsWidget->selectedItems()[0]->data(Qt::UserRole).toInt();
    int procedureId = createAssignmentsDialog->getProcedureId();
    QString appointedAt = createAssignmentsDialog->getAppointedDate();
    if (checkProceduresCount(patientId, appointedAt) && checkIsIdUnique(patientId, appointedAt, procedureId)) {
        QSqlQuery query;
        query.prepare("INSERT INTO assignments (patient_id, procedure_id, appointed_at, time) VALUES (:patient_id, :procedure_id, :appointed_at, :time)");
        query.bindValue(":patient_id", patientId);
        query.bindValue(":procedure_id", procedureId);
        query.bindValue(":appointed_at", appointedAt);
        query.bindValue(":time", createAssignmentsDialog->getTime());
        execAssignmentQuery(query);
    } else {
        QMessageBox::critical(this, "Ошибка", "Невозможно добавить процедуру.");
    }
}

void DoctorWidget::showEditAssignForm()
{
    auto item = assignsWidget->selectedItems()[0];
    editAssignmentsDialog->setProcedureId(item->data(Qt::UserRole + 4).toInt());
    editAssignmentsDialog->setAppointedDate(item->data(Qt::UserRole + 3).toString());
    editAssignmentsDialog->setTime(item->data(Qt::UserRole + 5).toString());
    editAssignmentsDialog->exec();
}

void DoctorWidget::delAssign()
{
    auto selectedItems = assignsWidget->selectedItems();
    QString ids;
    for (auto i = 0; i < selectedItems.count(); i++) {
        if (selectedItems[i]->data(Qt::UserRole).isValid()) {
            ids += (selectedItems[i]->data(Qt::UserRole).toString());
            ids+= ", ";
        }
    }
    ids.chop(2);

    QSqlQuery query;
    query.prepare("DELETE FROM assignments WHERE id IN (" + ids + ")");
    execAssignmentQuery(query);
}

void DoctorWidget::showAssignmentsDialog()
{
    createAssignmentsDialog->fillHours();
    createAssignmentsDialog->exec();
}

void DoctorWidget::execAssignmentQuery(QSqlQuery &query)
{
    bool result = query.exec();
    qDebug() << query.executedQuery();
    if (result) {
        retrieveAssigns();
    } else {
        QMessageBox::critical(this, "Ошибка", "Ошибка выполнения запроса к базе данных.");
        qDebug() << query.lastError().text();
    }
}

void DoctorWidget::updateAssignment()
{
    auto selectedItems = assignsWidget->selectedItems();
    QListWidgetItem* item = selectedItems[0];
    int id = item->data(Qt::UserRole).toInt();
    QSqlQuery query;
    qDebug() << "id " << id;
    query.prepare("UPDATE assignments SET procedure_id = :procedure_id, appointed_at = :appointed_at, time = :time WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":procedure_id", editAssignmentsDialog->getProcedureId());
    query.bindValue(":appointed_at", editAssignmentsDialog->getAppointedDate());
    query.bindValue(":time", editAssignmentsDialog->getTime());
    execAssignmentQuery(query);
}

void DoctorWidget::addPatient()
{
    QSqlQuery query;
    query.prepare("INSERT INTO patients (first_name, last_name, current_diagnosis, date_of_birth) VALUES (:firstName, :lastName, :diagnosis, :dateOfBirth)");
    query.bindValue(":firstName", createDialog->getFirstName());
    query.bindValue(":lastName", createDialog->getLastName());
    query.bindValue(":diagnosis", createDialog->getDiagnosis());
    query.bindValue(":dateOfBirth", createDialog->getDateOfBirth());
    qDebug() << createDialog->getDateOfBirth();
    execQuery(query);
}

void DoctorWidget::showEditForm()
{
    auto item = patientsWidget->selectedItems()[0];

    QStringList splitName = item->data(Qt::UserRole + 1).toString().split(' ');
    editDialog->setFirstName(splitName[0]);
    editDialog->setLastName(splitName[1]);
    editDialog->setDiagnosis(item->data(Qt::UserRole + 2).toString());
    editDialog->setDateOfBirth(item->data(Qt::UserRole + 3).toString());
    editDialog->exec();
}

void DoctorWidget::updateData()
{
    auto selectedItems = patientsWidget->selectedItems();
    QListWidgetItem* item = selectedItems[0];
    int id = item->data(Qt::UserRole).toInt();
    QSqlQuery query;
    qDebug() << "id " << id;
    query.prepare("UPDATE patients SET first_name = :first_name, last_name = :last_name, current_diagnosis = :current_diagnosis, date_of_birth = :date_of_birth WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":first_name", editDialog->getFirstName());
    query.bindValue(":last_name", editDialog->getLastName());
    query.bindValue(":current_diagnosis", editDialog->getDiagnosis());
    query.bindValue(":date_of_birth", editDialog->getDateOfBirth());
    execQuery(query);
}

void DoctorWidget::deleteData()
{
    auto selectedItems = patientsWidget->selectedItems();
    QString ids;
    for (auto i = 0; i < selectedItems.size(); i++) {
        if (selectedItems[i]->data(Qt::UserRole).isValid()) {
            ids += (selectedItems[i]->data(Qt::UserRole).toString());
            ids+= ", ";
        }
    }
    ids.chop(2);

    qDebug() << ids;

    QSqlQuery query;
    query.prepare("DELETE FROM patients WHERE id IN (" + ids + ")");
    execQuery(query);
    assignsWidget->clear();
}

bool DoctorWidget::checkProceduresCount(int patientId, QString appointedAt)
{

    QSqlQuery checkThreeProceduresQuery;
    checkThreeProceduresQuery.prepare("SELECT count (*) FROM assignments WHERE patient_id = :patient_id AND appointed_at = :appointed_at;");
    checkThreeProceduresQuery.bindValue(":patient_id", patientId);
    checkThreeProceduresQuery.bindValue(":appointed_at", appointedAt);
    checkThreeProceduresQuery.exec();
    checkThreeProceduresQuery.next();

    if (checkThreeProceduresQuery.value(0).toInt() > 2) {
        return false;
    }
    return true;
}

bool DoctorWidget::checkIsIdUnique(int patientId, QString appointedAt, int procedureId) {
    QSqlQuery checkUniqueIdQuery;
    checkUniqueIdQuery.prepare("SELECT count (*) FROM assignments WHERE procedure_id = :procedure_id AND patient_id = :patient_id AND appointed_at = :appointed_at;");
    checkUniqueIdQuery.bindValue(":patient_id", patientId);
    checkUniqueIdQuery.bindValue(":appointed_at", appointedAt);
    checkUniqueIdQuery.bindValue(":procedure_id", procedureId);
    checkUniqueIdQuery.exec();
    checkUniqueIdQuery.next();

    if (checkUniqueIdQuery.value(0).toInt() > 0) {
        return false;
    }
    return true;
}
