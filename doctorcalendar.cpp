#include "doctorcalendar.h"

DoctorCalendar::DoctorCalendar(QWidget *parent)
    : QWidget{parent}
{
    HBoxLayout = new QHBoxLayout(this);
    setLayout(HBoxLayout);
    VBoxLayout = new QVBoxLayout(this);
    calendar = new QCalendarWidget(this);
    calendar->setMaximumHeight(300);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    queueWidget = new QueueWidget(this);
    groupBox = new QGroupBox(this);


    VBoxLayout->addWidget(calendar);
    VBoxLayout->addWidget(groupBox);
    HBoxLayout->addLayout(VBoxLayout, 1);
    HBoxLayout->addWidget(queueWidget, 3);
    groupBox->setMinimumHeight(200);

    loadAssignments(QDate::currentDate());

    connect(calendar, &QCalendarWidget::clicked, this, &DoctorCalendar::loadAssignments);
}

void DoctorCalendar::loadAssignments(QDate date)
{
    queueWidget->showQueue(date, true);
}
