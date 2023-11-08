#ifndef ASSIGNMENTSDIALOG_H
#define ASSIGNMENTSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>

class AssignmentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AssignmentsDialog(QWidget* parent = nullptr);

    int getPatientId();
    int getProcedureId();
    QString getAppointedDate();

    void setPatientId(int value);
    void setProcedureId(int value);
    void setAppointedDate(QString value);
    void loadProcedures();
    QStringList getAvailableHours(QDate date);
    QString getTime();
    void setTime(QString value);
    void fillHours();

private:
    QLabel* procedureIdLabel;
    QLabel* appointedDateLabel;

    QVBoxLayout* dialogLayout;

    QComboBox* procedureIdField;
    QDateEdit* appointedDateField;

    QDialogButtonBox* buttonBox;

    int procedureId;
    QString appointedDate;

    QComboBox* timeField;
    QLabel* timeLabel;

    QString time;
};

#endif // ASSIGNMENTSDIALOG_H
