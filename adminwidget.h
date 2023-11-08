#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QPushButton>
#include <QHBoxLayout>
#include <QHeaderView>
#include "proceduredialog.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlResult>


class AdminWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdminWidget(QWidget *parent = nullptr);

private:
    QTableWidget* adminTableWidget;
    QVBoxLayout* adminVBoxLayout;

    QHBoxLayout* adminHBoxLayout;
    QPushButton* addProcButton;
    QPushButton* editProcButton;
    QPushButton* delProcButton;

    ProcedureDialog* createDialog;
    ProcedureDialog* editDialog;

    void showProcedureDialog();
    void addProcedure();
    void retrieveData();
    void deleteData();
    void execQuery(QSqlQuery &query);
    void showEditForm();
    void updateData();
};

#endif // ADMINWIDGET_H
