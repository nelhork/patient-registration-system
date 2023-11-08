#ifndef AUTHORIZEDIALOG_H
#define AUTHORIZEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QSqlQuery>
#include <QMessageBox>
#include <QHeaderView>

class AuthorizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthorizeDialog(QWidget* parent = nullptr);
    QString getLogin();

private:
    QLabel* loginLabel;
    QLabel* passwordLabel;

    QVBoxLayout* dialogLayout;

    QLineEdit* loginField;
    QLineEdit* passwordField;

    QDialogButtonBox* buttonBox;

    QString login;
    QString password;

    void authorize();
};

#endif // AUTHORIZEDIALOG_H
