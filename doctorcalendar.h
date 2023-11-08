#ifndef DOCTORCALENDAR_H
#define DOCTORCALENDAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCalendarWidget>
#include <QListWidget>
#include <QLabel>
#include <QGroupBox>
#include <QSqlQuery>
#include <QSqlError>
#include "queuewidget.h"

class DoctorCalendar : public QWidget
{
    Q_OBJECT
public:
    explicit DoctorCalendar(QWidget *parent = nullptr);

private:
    QVBoxLayout* VBoxLayout;
    QHBoxLayout* HBoxLayout;
    QCalendarWidget* calendar;
    QueueWidget* queueWidget;
    QGroupBox* groupBox;

    void loadAssignments(QDate date);
};


#endif // DOCTORCALENDAR_H
