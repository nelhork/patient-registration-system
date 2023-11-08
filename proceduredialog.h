#ifndef PROCEDUREDIALOG_H
#define PROCEDUREDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>

class ProcedureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProcedureDialog(QWidget* parent = nullptr);

    QString getName();
    int getDuration();
    int getMinAmount();

    void setName(QString newName);
    void setDuration(int newDuration);
    void setMinAmount(int newMinAmount);

private:
    QLabel* nameLabel;
    QLabel* durationLabel;
    QLabel* minAmountLabel;

    QVBoxLayout* dialogLayout;

    QLineEdit* nameField;
    QSpinBox* durField;
    QSpinBox* minAmountField;

    QDialogButtonBox* buttonBox;

    QString name;
    int duration;
    int minAmount;
};

#endif // PROCEDUREDIALOG_H
