#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Информационная система");
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    welcomeWidget = new WelcomeWidget(this);
    stackedWidget->addWidget(welcomeWidget);

    authDocAdmDialog = new AuthorizeDialog(this);
    connect(authDocAdmDialog, &AuthorizeDialog::accepted, this, &MainWindow::retrieveDocAdmData);
    authPatientDialog = new AuthPatientDialog(this);
    connect(authPatientDialog, &AuthPatientDialog::accepted, this, &MainWindow::retrievePatientData);

    setMinimumWidth(854);
    setMinimumHeight(480);

    setupMenuBar();
}

void MainWindow::retrieveDocAdmData() {
    QSqlQuery query;
    query.prepare("SELECT type FROM users WHERE username = :username");
    query.bindValue(":username", authDocAdmDialog->getLogin());
    query.exec();
    query.next();
    qDebug() << "result " << query.value("type");

    patientMenu->setEnabled(false);
    if (query.value("type") == 0) {
        if (doctorWidget == nullptr) {
            doctorWidget = new DoctorWidget(this);
        }
        manageMenu->setEnabled(true);
        showWidget(doctorWidget);
    } else {
        if (adminWidget == nullptr) {
            adminWidget = new AdminWidget(this);
        }
        manageMenu->setEnabled(false);
        showWidget(adminWidget);
    }
}

void MainWindow::retrievePatientData()
{
    if (patientWidget == nullptr) {
        patientWidget = new PatientWidget(this);
    }
    int value = authPatientDialog->getId();
    patientWidget->setPatientId(value);
    patientMenu->setEnabled(true);
    manageMenu->setEnabled(false);
    showWidget(patientWidget);
}

void MainWindow::showWidget(QWidget *widget)
{
    auto index = stackedWidget->indexOf(widget);
    if (index == -1) {
        index = stackedWidget->addWidget(widget);
    }
    stackedWidget->setCurrentIndex(index);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupMenuBar()
{
    QMenu *menu = menuBar()->addMenu(tr("&Вход"));
    manageMenu = menuBar()->addMenu(tr("&Управление"));
    manageMenu->setEnabled(false);
    patientMenu = menuBar()->addMenu(tr("&Пациент"));
    patientMenu->setEnabled(false);
    QMenu* helpMenu = menuBar()->addMenu(tr("&Помощь"));

    menu->addAction(tr("Вход для персонала"), authDocAdmDialog, &AuthorizeDialog::exec);
    menu->addAction(tr("Вход для пациентов"), authPatientDialog, &AuthPatientDialog::exec);

    manageMenu->addAction(tr("Календарь"), this, [=]() {
        if (doctorCalendar == nullptr) {
            doctorCalendar = new DoctorCalendar(this);
        }
        showWidget(doctorCalendar);
    });
    manageMenu->addAction(tr("Пациенты"), this, [=]() {
        showWidget(doctorWidget);
    });

    patientMenu->addAction(tr("Запись"), this, [=]() {
        showWidget(patientWidget);
    });
    patientMenu->addAction(tr("Очередь"), this, [=]() {
        if (queue == nullptr) {
            queue = new QueueWidget();
        }
        showWidget(queue);
        queue->showQueue(QDate::currentDate(), true);
    });
    helpMenu->addAction("О программе", this, [=]() {
        QMessageBox::about(this, "О программе", "АГТУ, ИИТиК, Немышева Н., гр. ДИПРб-21");
    });
}
