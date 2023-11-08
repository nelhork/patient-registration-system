#include "queuewidget.h"

QueueWidget::QueueWidget(QWidget *parent)
    : QWidget{parent}
{
    queue = new QTableWidget();
    auto header = queue->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    QVBoxLayout* layout = new QVBoxLayout(this);
    queueTitle = new QLabel();
    queueTitle->setStyleSheet("font-size: 16px; font-weight: 800");
    layout->addWidget(queueTitle);
    layout->addWidget(queue);
    setLayout(layout);
}

void QueueWidget::showQueue(QDate date, bool showOnlyRegistered = true)
{
    queueTitle->setText("Очередь пациентов на " + date.toString("dd.MM.yyyy"));
    QSqlQuery query;
    if (showOnlyRegistered == true) {
        query.prepare("SELECT *, replace (registered_at, ':', '0') AS time_value FROM assignments JOIN patients ON patient_id = patients.id WHERE registered_at IS NOT NULL AND appointed_at = :date ORDER BY cast(time_value AS INTEGER);");
    } else {
        query.prepare("SELECT *, replace (registered_at, ':', '0') AS time_value FROM assignments JOIN patients ON patient_id = patients.id WHERE appointed_at = :date ORDER BY cast(time_value AS INTEGER);");
    }

    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.exec();

    QSqlQuery countQuery;
    if (showOnlyRegistered == true) {
        countQuery.prepare("SELECT COUNT (*) as size FROM assignments WHERE registered_at IS NOT NULL AND appointed_at = :date");
    } else {
        countQuery.prepare("SELECT COUNT (*) as size FROM assignments WHERE appointed_at = :date");
    }
    countQuery.bindValue(":date", date.toString("yyyy-MM-dd"));
    countQuery.exec();

    countQuery.next();
    queue->setRowCount(countQuery.value("size").toInt());
    queue->setColumnCount(4);
    QStringList labels {"Имя", "Назначенное время", "Время регистрации", "Время приема"};
    queue->setHorizontalHeaderLabels(labels);

    int index = 0;
    while(query.next())
    {
        auto name = query.value("first_name").toString() + " " + query.value("last_name").toString();
        auto time = query.value("time").toString();
        auto registeredAt = query.value("registered_at").toString();
        auto actualTime = hours[index++];
        QTableWidgetItem* nameItem = new QTableWidgetItem(name);
        queue->setItem(index - 1, 0, nameItem);
        QTableWidgetItem* appointedAtItem = new QTableWidgetItem(time);
        queue->setItem(index - 1, 1, appointedAtItem);
        QTableWidgetItem* registeredAtItem = new QTableWidgetItem(registeredAt);
        queue->setItem(index - 1, 2, registeredAtItem);
        QTableWidgetItem* actualTimeItem = new QTableWidgetItem(actualTime);
        queue->setItem(index - 1, 3, actualTimeItem);
    }
}
