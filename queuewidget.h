#ifndef QUEUEWIDGET_H
#define QUEUEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QLabel>
#include <QDate>
#include <QTableWidget>
#include <QHeaderView>

class QueueWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QueueWidget(QWidget *parent = nullptr);
    void showQueue(QDate date, bool showOnlyRegistered);

private:
    QTableWidget* queue;
    QLabel* queueTitle;
    QStringList hours = {"8:00", "9:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"};
};
#endif // QUEUEWIDGET_H
