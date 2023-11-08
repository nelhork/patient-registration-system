#include "userdialog.h"

UserDialog::UserDialog(QWidget *parent)
    : QDialog{parent}
{
    usernameLabel = new QLabel(this);
    passwordLabel = new QLabel(this);
    fullNameLabel = new QLabel(this);
    typeLabel = new QLabel(this);

    usernameField = new QLineEdit(this);
    passwordField = new QLineEdit(this);
    passwordField->setEchoMode(QLineEdit::EchoMode::Password);
    fullNameField = new QLineEdit(this);
    typeField = new QComboBox(this);

    dialogLayout = new QVBoxLayout(this);

    dialogLayout->addWidget(usernameLabel);
    usernameLabel->setText("Логин:");
    dialogLayout->addWidget(usernameField);

    dialogLayout->addWidget(passwordLabel);
    passwordLabel->setText("Пароль:");
    dialogLayout->addWidget(passwordField);

    dialogLayout->addWidget(fullNameLabel);
    fullNameLabel->setText("ФИО:");
    dialogLayout->addWidget(fullNameField);


    dialogLayout->addWidget(typeLabel);
    typeLabel->setText("Должность:");

    dialogLayout->addWidget(typeField);
    typeField->addItem("Врач");
    typeField->addItem("Администратор");

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    dialogLayout->addStretch(10);
    dialogLayout->addWidget(buttonBox);

    connect(usernameField, &QLineEdit::textChanged, this, [=](QString value) {
        username = value;
    });
    connect(passwordField, &QLineEdit::textChanged, this, [=](QString value) {
        password = value;
    });
    connect(fullNameField, &QLineEdit::textChanged, this, [=](QString value) {
        fullName = value;
    });
    connect(typeField, &QComboBox::activated, this, [=](int value) {
        type = value;
        qDebug() << "value: " << value;
    });
}

QString UserDialog::getUsername()
{
    return username;
}

QString UserDialog::getPassword()
{
    return password;
}

QString UserDialog::getFullName()
{
    return fullName;
}

int UserDialog::getType()
{
    return type;
}

void UserDialog::setUsername(QString value)
{
    username = value;
    usernameField->setText(value);
}

void UserDialog::setPassword(QString value)
{
    password = value;
    passwordField->setText(value);
}

void UserDialog::setFullName(QString value)
{
    fullName = value;
    fullNameField->setText(value);
}

void UserDialog::setType(int value)
{
    type = value;
    typeField->setCurrentIndex(value);
    qDebug() << "value: " << value;
}
