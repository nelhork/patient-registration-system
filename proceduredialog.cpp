#include "proceduredialog.h"

ProcedureDialog::ProcedureDialog(QWidget *parent)
    : QDialog{parent}
{
    nameLabel = new QLabel(this);
    durationLabel = new QLabel(this);
    minAmountLabel = new QLabel(this);

    nameField = new QLineEdit(this);
    durField = new QSpinBox(this);
    minAmountField = new QSpinBox(this);

    dialogLayout = new QVBoxLayout(this);

    dialogLayout->addWidget(nameLabel);
    nameLabel->setText("Название:");
    dialogLayout->addWidget(nameField);

    dialogLayout->addWidget(durationLabel);
    durationLabel->setText("Продолжительность:");
    dialogLayout->addWidget(durField);

    dialogLayout->addWidget(minAmountLabel);
    minAmountLabel->setText("Минимальное количество:");
    dialogLayout->addWidget(minAmountField);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    dialogLayout->addStretch(10);
    dialogLayout->addWidget(buttonBox);

    connect(nameField, &QLineEdit::textChanged, this, [=](QString value) {
        name = value;
    });
    connect(durField, &QSpinBox::valueChanged, this, [=](int value) {
        duration = value;
    });
    connect(minAmountField, &QSpinBox::valueChanged, this, [=](int value) {
        minAmount = value;
    });
}

QString ProcedureDialog::getName()
{
    return name;
}

int ProcedureDialog::getDuration()
{
    return duration;
}

int ProcedureDialog::getMinAmount()
{
    return minAmount;
}

void ProcedureDialog::setName(QString newName)
{
    name = newName;
    nameField->setText(newName);
}

void ProcedureDialog::setDuration(int newDuration)
{
    duration = newDuration;
    durField->setValue(newDuration);
}

void ProcedureDialog::setMinAmount(int newMinAmount)
{
    minAmount = newMinAmount;
    minAmountField->setValue(newMinAmount);
}
