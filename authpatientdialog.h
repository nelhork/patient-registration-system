#ifndef AUTHPATIENTDIALOG_H
#define AUTHPATIENTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QMessageBox>

class AuthPatientDialog : public QDialog
{
    Q_OBJECT

public:
    AuthPatientDialog(QWidget *parent = nullptr);

    int getId();
private:
    int id;
    QLabel* nameLabel;
    QLineEdit* nameField;
    QDialogButtonBox* buttonBox;
    QString name;

    QVBoxLayout* dialogLayout;

    void authorize();
};

#endif // AUTHPATIENTDIALOG_H
