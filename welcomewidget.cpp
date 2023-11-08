#include "welcomewidget.h"
#include <QVBoxLayout>
#include <QLabel>

WelcomeWidget::WelcomeWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel(this);
    layout->addWidget(label);
    setLayout(layout);
    label->setText("Добро пожаловать в информационную систему учета\nи контроля процедурных назначений пациентов 'Парацельс'.\nДля начала работы, пожалуйста, войдите в систему.");
    label->setStyleSheet("text-align: center; font-size: 24px");
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}
