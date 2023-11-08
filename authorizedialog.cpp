#include "authorizedialog.h"

AuthorizeDialog::AuthorizeDialog(QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle("Авторизация");
    loginLabel = new QLabel(this);
    passwordLabel = new QLabel(this);

    loginField = new QLineEdit(this);
    passwordField = new QLineEdit(this);
    passwordField->setEchoMode(QLineEdit::EchoMode::Password);

    dialogLayout = new QVBoxLayout(this);

    dialogLayout->addWidget(loginLabel);
    loginLabel->setText("Логин: ");
    dialogLayout->addWidget(loginField);

    dialogLayout->addWidget(passwordLabel);
    passwordLabel->setText("Пароль: ");
    dialogLayout->addWidget(passwordField);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AuthorizeDialog::authorize);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    dialogLayout->addStretch(10);
    dialogLayout->addWidget(buttonBox);

    connect(loginField, &QLineEdit::textChanged, this, [=](QString value) {
        login = value;
    });
    connect(passwordField, &QLineEdit::textChanged, this, [=](QString value) {
        password = value;
    });
}

void AuthorizeDialog::authorize()
{
    QSqlQuery query;
    query.prepare("SELECT EXISTS (SELECT * FROM users WHERE username = :login AND password = :password) as result");
    query.bindValue(":login", login);
    query.bindValue(":password", password);
    query.exec();
    query.next();
    if (query.value("result") == 1) {
        accept();
    } else {
        QMessageBox::critical(this,"Ошибка", "Неправильный логин или пароль");
    }
    qDebug() << "executed query: " << query.value("result");
}

QString AuthorizeDialog::getLogin()
{
    return login;
}
