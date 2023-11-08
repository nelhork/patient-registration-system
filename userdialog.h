#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QComboBox>

class UserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserDialog(QWidget* parent = nullptr);

    QString getUsername();
    QString getPassword();
    QString getFullName();
    int getType();

    void setUsername(QString value);
    void setPassword(QString value);
    void setFullName(QString value);
    void setType(int value);

private:
    QLabel* usernameLabel;
    QLabel* passwordLabel;
    QLabel* fullNameLabel;
    QLabel* typeLabel;

    QVBoxLayout* dialogLayout;

    QLineEdit* usernameField;
    QLineEdit* passwordField;
    QLineEdit* fullNameField;
    QComboBox* typeField;

    QDialogButtonBox* buttonBox;

    QString username;
    QString password;
    QString fullName;
    int type;
};

#endif // USERDIALOG_H
