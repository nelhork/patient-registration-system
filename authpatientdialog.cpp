#include "authpatientdialog.h"

AuthPatientDialog::AuthPatientDialog(QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle("Авторизация");
    nameLabel = new QLabel(this);
    nameField = new QLineEdit(this);

    dialogLayout = new QVBoxLayout(this);

    dialogLayout->addWidget(nameLabel);
    nameLabel->setText("Фамилия и имя:");
    dialogLayout->addWidget(nameField);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AuthPatientDialog::authorize);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    dialogLayout->addStretch(10);
    dialogLayout->addWidget(buttonBox);

    connect(nameField, &QLineEdit::textChanged, this, [=](QString value) {
        name = value;
    });
}

int AuthPatientDialog::getId()
{
    return id;
}

void AuthPatientDialog::authorize()
{
    QSqlQuery query;
    query.prepare("SELECT id, (first_name || ' ' || last_name) AS name FROM patients WHERE name = :name");
    query.bindValue(":name", name);
    query.exec();
    query.next();
    id = query.value("id").toInt();
    if (query.value("name") == name) {
        accept();
    } else {
         QMessageBox::critical(this,"Ошибка", "ФИО не зарегистрировано в системе.");
    }
}
