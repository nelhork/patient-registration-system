#include "adminwidget.h"

AdminWidget::AdminWidget(QWidget *parent)
    : QWidget{parent}
{
    setWindowTitle("Процедуры");
    adminTableWidget = new QTableWidget(this);
    adminTableWidget->horizontalHeader()->setStretchLastSection(true);
    adminTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    adminTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    adminTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    adminTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    adminVBoxLayout = new QVBoxLayout(this);

    addProcButton = new QPushButton("Добавить процедуру");
    editProcButton = new QPushButton("Изменить процедуру");
    delProcButton = new QPushButton("Удалить процедуру");
    delProcButton->setEnabled(false);
    editProcButton->setEnabled(false);

    createDialog = new ProcedureDialog(this);
    createDialog->setWindowTitle("Добавление процедуры");
    editDialog = new ProcedureDialog(this);
    editDialog->setWindowTitle("Изменение процедуры");
    adminHBoxLayout = new QHBoxLayout(this);

    adminHBoxLayout->addWidget(addProcButton);
    adminHBoxLayout->addWidget(editProcButton);
    adminHBoxLayout->addWidget(delProcButton);

    adminVBoxLayout->addLayout(adminHBoxLayout);
    adminVBoxLayout->addWidget(adminTableWidget);

    this->setLayout(adminVBoxLayout);

    adminTableWidget->setColumnCount(3);

    QStringList labels {"Название", "Продолжительность", "Минимальное количество"};
    adminTableWidget->setHorizontalHeaderLabels(labels);

    retrieveData();

    connect(addProcButton, &QPushButton::clicked, this, &AdminWidget::showProcedureDialog);
    connect(createDialog, &ProcedureDialog::accepted, this, &AdminWidget::addProcedure);

    connect(adminTableWidget, &QTableWidget::itemSelectionChanged, this, [=](){
        if (adminTableWidget->selectedItems().isEmpty()) {
            delProcButton->setEnabled(false);
            editProcButton->setEnabled(false);
        } else {
            delProcButton->setEnabled(true);
            editProcButton->setEnabled(true);
        }
    });
    connect(delProcButton, &QPushButton::clicked, this, &AdminWidget::deleteData);

    connect(editProcButton, &QPushButton::clicked, this, &AdminWidget::showEditForm);
    connect(editDialog, &ProcedureDialog::accepted, this, &AdminWidget::updateData);
}

void AdminWidget::retrieveData()
{
    QSqlQuery query;
    qDebug() << query.exec("SELECT id, name, duration_in_min, min_amount FROM procedures");

    QSqlQuery countQuery;
    countQuery.exec("SELECT COUNT (*) AS totalRows FROM procedures");
    countQuery.first();
    qDebug() << countQuery.lastError().text();
    int count = countQuery.value("totalRows").toInt();

    adminTableWidget->clearContents();

    int rowCount = 0;
    adminTableWidget->setRowCount(count);

    while(query.next()) {

        QString name = query.value("name").toString();
        int duration = query.value("duration_in_min").toInt();
        int minAmount = query.value("min_amount").toInt();

        QTableWidgetItem *nameItem = new QTableWidgetItem(name);
        QTableWidgetItem *durItem = new QTableWidgetItem(QString::number(duration));
        QTableWidgetItem *minAmountItem = new QTableWidgetItem(QString::number(minAmount));
        nameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        durItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        minAmountItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        adminTableWidget->setItem(rowCount, 0, nameItem);
        adminTableWidget->setItem(rowCount, 1, durItem);
        adminTableWidget->setItem(rowCount, 2, minAmountItem);
        adminTableWidget->item(rowCount, 0)->setData(Qt::UserRole, query.value("id"));
        rowCount++;
    }
}

void AdminWidget::execQuery(QSqlQuery& query)
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

void AdminWidget::showEditForm()
{
    auto selectedItems = adminTableWidget->selectedItems();
    QTableWidgetItem* nameItem = selectedItems[0];
    QTableWidgetItem* durationItem = selectedItems[1];
    QTableWidgetItem* minAmountItem = selectedItems[2];
    editDialog->setName(nameItem->text());
    editDialog->setDuration(durationItem->text().toInt());
    editDialog->setMinAmount(minAmountItem->text().toInt());
    editDialog->exec();
}

void AdminWidget::updateData()
{
    auto selectedItems = adminTableWidget->selectedItems();
    QTableWidgetItem* nameItem = selectedItems[0];
    int id = nameItem->data(Qt::UserRole).toInt();
    QSqlQuery query;
    qDebug() << "id " << id;
    query.prepare("UPDATE procedures SET name = :name, duration_in_min = :duration, min_amount = :amount WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":name", editDialog->getName());
    query.bindValue(":duration", editDialog->getDuration());
    query.bindValue(":amount", editDialog->getMinAmount());
    execQuery(query);
}

void AdminWidget::deleteData()
{
    auto selectedItems = adminTableWidget->selectedItems();
    QString ids;
    for (auto i = 0; i < selectedItems.size(); i++) {
        if (selectedItems[i]->data(Qt::UserRole).isValid()) {
            ids += (selectedItems[i]->data(Qt::UserRole).toString());
            ids+= ", ";
        }
    }
    ids.chop(2);

    qDebug() << ids;

    //for (auto &item: selectedItems)
    QSqlQuery query;
    query.prepare("DELETE FROM procedures WHERE id IN (" + ids + ")");
    execQuery(query);
}

void AdminWidget::showProcedureDialog()
{
    createDialog->exec();
}

void AdminWidget::addProcedure()
{
    QSqlQuery query;
    query.prepare("INSERT INTO procedures (name, duration_in_min, min_amount) VALUES (:name, :duration, :amount)");
    query.bindValue(":name", createDialog->getName());
    query.bindValue(":duration", createDialog->getDuration());
    query.bindValue(":amount", createDialog->getMinAmount());
    execQuery(query);
}
