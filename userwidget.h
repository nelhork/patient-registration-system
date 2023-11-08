#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "userdialog.h"
#include <QSqlQuery>
#include <QHeaderView>
#include <QSqlError>
#include <QMessageBox>

class UserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserWidget(QWidget *parent = nullptr);

private:
    QTableWidget* tableWidget;
    QVBoxLayout* vBoxLayout;
    QHBoxLayout* hBoxLayout;

    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* delButton;

    UserDialog* createDialog;
    UserDialog* editDialog;

    void showUserDialog();
    void addUser();
    void retrieveData();
    void deleteData();
    void execQuery(QSqlQuery &query);
    void showEditForm();
    void updateData();
};

#endif // USERWIDGET_H
