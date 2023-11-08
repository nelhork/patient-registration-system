#include "userwidget.h"

UserWidget::UserWidget(QWidget *parent)
    : QWidget{parent}
{
    tableWidget = new QTableWidget(this);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    vBoxLayout = new QVBoxLayout(this);

    addButton = new QPushButton("Добавить");
    editButton = new QPushButton("Изменить");
    delButton = new QPushButton("Удалить");
    delButton->setEnabled(false);
    editButton->setEnabled(false);

    createDialog = new UserDialog(this);
    editDialog = new UserDialog(this);
    hBoxLayout = new QHBoxLayout(this);

    hBoxLayout->addWidget(addButton);
    hBoxLayout->addWidget(editButton);
    hBoxLayout->addWidget(delButton);

    vBoxLayout->addLayout(hBoxLayout);
    vBoxLayout->addWidget(tableWidget);

    this->setLayout(vBoxLayout);

    tableWidget->setColumnCount(4);
    QStringList labels {"Логин", "Пароль", "ФИО", "Должность"};
    tableWidget->setHorizontalHeaderLabels(labels);

    retrieveData();

    connect(addButton, &QPushButton::clicked,this, &UserWidget::showUserDialog);
    connect(createDialog, &UserDialog::accepted, this, &UserWidget::addUser);

    connect(tableWidget, &QTableWidget::itemSelectionChanged, this, [=](){
        if (tableWidget->selectedItems().isEmpty()) {
            delButton->setEnabled(false);
            editButton->setEnabled(false);
        } else {
            delButton->setEnabled(true);
            editButton->setEnabled(true);
        }
    });
    connect(delButton, &QPushButton::clicked, this, &UserWidget::deleteData);

    connect(editButton, &QPushButton::clicked, this, &UserWidget::showEditForm);
    connect(editDialog, &UserDialog::accepted, this, &UserWidget::updateData);
}

void UserWidget::showUserDialog()
{
    createDialog->exec();
}

void UserWidget::addUser()
{
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, full_name, type) VALUES (:username, :password, :full_name, :type)");
    query.bindValue(":username", createDialog->getUsername());
    query.bindValue(":password", createDialog->getPassword());
    query.bindValue(":full_name", createDialog->getFullName());
    query.bindValue(":type", createDialog->getType());
    execQuery(query);
}

void UserWidget::retrieveData()
{
    QSqlQuery query;
    qDebug() << query.exec("SELECT * FROM users");

    QSqlQuery countQuery;
    countQuery.exec("SELECT COUNT (*) AS totalRows FROM users");
    countQuery.first();
    qDebug() << countQuery.lastError().text();
    int count = countQuery.value("totalRows").toInt();

    tableWidget->clearContents();

    int rowCount = 0;
    tableWidget->setRowCount(count);

    while(query.next()) {

        QString username = query.value("username").toString();
        QString password = query.value("password").toString();
        QString fullName = query.value("full_name").toString();
        QString type = query.value("type").toInt() ? "Администратор" : "Врач";

        QTableWidgetItem *usernameItem = new QTableWidgetItem(username);
        QTableWidgetItem *passwordItem = new QTableWidgetItem(password);
        QTableWidgetItem *fullNameItem = new QTableWidgetItem(fullName);
        QTableWidgetItem *typeItem = new QTableWidgetItem(type);

        usernameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        passwordItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        fullNameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        typeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        tableWidget->setItem(rowCount, 0, usernameItem);
        tableWidget->setItem(rowCount, 1, passwordItem);
        tableWidget->setItem(rowCount, 2, fullNameItem);
        tableWidget->setItem(rowCount, 3, typeItem);

        tableWidget->item(rowCount, 0)->setData(Qt::UserRole, query.value("id"));
        rowCount++;
    }
}

void UserWidget::deleteData()
{
    auto selectedItems = tableWidget->selectedItems();
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
    query.prepare("DELETE FROM users WHERE id IN (" + ids + ")");
    execQuery(query);
}

void UserWidget::execQuery(QSqlQuery &query)
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

void UserWidget::showEditForm()
{
    auto selectedItems = tableWidget->selectedItems();
    QTableWidgetItem* usernameItem = selectedItems[0];
    QTableWidgetItem* passwordItem = selectedItems[1];
    QTableWidgetItem* fullNameItem = selectedItems[2];
    QTableWidgetItem* typeItem = selectedItems[3];

    editDialog->setUsername(usernameItem->text());
    editDialog->setPassword(passwordItem->text());
    editDialog->setFullName(fullNameItem->text());
    editDialog->setType(typeItem->text() == "Врач" ? 0 : 1);
    qDebug() << "set type value " << typeItem->text().toInt();
    editDialog->exec();
}

void UserWidget::updateData()
{
    auto selectedItems = tableWidget->selectedItems();
    QTableWidgetItem* usernameItem = selectedItems[0];
    int id = usernameItem->data(Qt::UserRole).toInt();
    QSqlQuery query;
    qDebug() << "id " << id;
    query.prepare("UPDATE users SET username = :username, password = :password, full_name = :full_name, type = :type WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":username", editDialog->getUsername());
    query.bindValue(":password", editDialog->getPassword());
    query.bindValue(":full_name", editDialog->getFullName());
    query.bindValue(":type", editDialog->getType());
    execQuery(query);
}
