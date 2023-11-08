#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

void connectToDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("clinic.db");
    db.open();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось открыть базу данных.");
        QApplication::exit(-1);
    }
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");
    QApplication a(argc, argv);
    connectToDB();
    MainWindow w;
    w.show();

    return a.exec();

}
