#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>
#include <QtWidgets>

#include <QFileInfo>
#include <QDir>
#include <QLineEdit>
#include <QTextEdit>
#include <QtCore>
#include <QCloseEvent>

#include "mainwindow.h"

//![0]
MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent)
  , trayIcon(new QSystemTrayIcon(this))
{
  // Tray icon menu
  auto menu = this->createMenu();
  this->trayIcon->setContextMenu(menu);

  // App icon
  auto appIcon = QIcon(":/icons/server.png");
  this->trayIcon->setIcon(appIcon);
  this->setWindowIcon(appIcon);

  // Timers
  timer1 = new QTimer(this);
  timer2 = new QTimer(this);

  // Displaying the tray icon
  this->trayIcon->show();

  // Interaction
  connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

  // Main window
  setWindowTitle(tr("SysTray"));
  QPushButton *browseButton = new QPushButton(tr("&Browse..."), this);
  connect(browseButton, &QAbstractButton::clicked, this, &MainWindow::browse);
  QPushButton *launchSchedulerButton = new QPushButton(tr("&Launch scheduler"), this);
  connect(launchSchedulerButton, &QAbstractButton::clicked, this, &MainWindow::launchScheduler);

  msgComboBox = createComboBox(tr("Message one"));
  fileComboBox = createComboBox(tr("EXAMPLE.txt"));
  directoryComboBox = createComboBox(QDir::toNativeSeparators(QDir::currentPath()));

  filesFoundLabel = new QLabel;

  QGridLayout *mainLayout = new QGridLayout(this);
  mainLayout->addWidget(new QLabel(tr("Named msg:")), 0, 0);
  mainLayout->addWidget(msgComboBox, 0, 1, 1, 4);
  mainLayout->addWidget(new QLabel(tr("Named file:")), 4, 0);
  mainLayout->addWidget(fileComboBox, 4, 1, 1, 4);
  mainLayout->addWidget(new QLabel(tr("In directory:")), 6, 0);
  mainLayout->addWidget(directoryComboBox, 6, 1, 1, 4);
  mainLayout->addWidget(filesFoundLabel, 8, 0, 1, 4);
  mainLayout->addWidget(browseButton, 8, 4);
  mainLayout->addWidget(launchSchedulerButton, 10, 2);
}
//![0]

//![1] - Systray functions
void MainWindow::launchScheduler()
{
//!
    qDebug() << "Timer" << " Status" << "Interval" << "Remaining time";
    qDebug() << "Timer 1 " << timer1->isActive() << " " << timer1->interval() << " " << timer1->remainingTime();
    qDebug() << "Timer 2 " << timer2->isActive() << " " << timer2->interval() << " " << timer2->remainingTime();
//!

    if(!timer1->isActive())
    {
        timer1 = new QTimer(this);
        connect(timer1,&QTimer::timeout,this,&MainWindow::showMessage);
        timer1->start(10000);
    }
    if(!timer2->isActive())
    {
        timer2 = new QTimer(this);
        connect(timer2,&QTimer::timeout,this,&MainWindow::find);
        timer2->start(30000);
    }

}

QMenu* MainWindow::createMenu()
{
  auto stopTask1 = new QAction(tr("&Stop task1"), this);
  connect(stopTask1, &QAction::triggered, this, &MainWindow::stopTimer1);

  auto stopTask2 = new QAction(tr("&Stop task2"), this);
  connect(stopTask2, &QAction::triggered, this, &MainWindow::stopTimer2);

  auto showPanel = new QAction(tr("&Open control panel"), this);
  connect(showPanel, &QAction::triggered, this, &QWidget::showNormal);

  auto quitAction = new QAction("&Quit", this);
  connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

  auto menu = new QMenu(this);
  menu->addAction(showPanel);
  menu->addAction(stopTask1);
  menu->addAction(stopTask2);
  menu->addSeparator();
  menu->addAction(quitAction);

  return menu;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason_)
{
  switch (reason_) {
  case QSystemTrayIcon::Trigger:
    this->trayIcon->showMessage("Hello", "Click right mouse button and choose action");
    break;
  default:
    ;
  }
}

QComboBox *MainWindow::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

static void updateComboBox(QComboBox *comboBox)
{
    if (comboBox->findText(comboBox->currentText()) == -1)
        comboBox->addItem(comboBox->currentText());
}
//![1] - Systray functions

//![2] - Task1 functions
void MainWindow::showMessage()
{    
    QString msgName = msgComboBox->currentText();
    updateComboBox(msgComboBox);
    auto titleEdit = new QLineEdit(tr("Message"));
    auto appIcon = QIcon(":/icons/message.png");

    trayIcon->showMessage(titleEdit->text(), msgName, appIcon, 500);
    qDebug() << msgName;
}

//![2] - Task1 functions

//![3] - Task2 functions
void MainWindow::browse()
{
    QString directory =
        QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Find Files"), QDir::currentPath()));

    if (!directory.isEmpty()) {
        if (directoryComboBox->findText(directory) == -1)
            directoryComboBox->addItem(directory);
        directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
    }
}

void MainWindow::find()
{
    QString fileName = fileComboBox->currentText();
    QString path = QDir::cleanPath(directoryComboBox->currentText());
    currentDir = QDir(path);

    updateComboBox(fileComboBox);
    updateComboBox(directoryComboBox);

    QStringList filter;
    if (!fileName.isEmpty())
        filter << fileName;
    QDirIterator it(path, filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QStringList files;
    while (it.hasNext())
        files << it.next();
    files.sort();
    showFiles(files);
}

void MainWindow::showFiles(const QStringList &paths)
{
    QString dbgMsg("File not found");

    if(!paths.size())
    {
        qDebug() << dbgMsg;
        QMessageBox::warning(this, tr("Systray: Debug message"),dbgMsg);
    }

    filesFoundLabel->setText(tr("%n file(s) found", nullptr, paths.size()));
    filesFoundLabel->setWordWrap(true);

}
//![3] - Task2 functions

//![4]
void MainWindow::stopTimer1()
{
    timer1->stop();
}

void MainWindow::stopTimer2()
{
    timer2->stop();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (trayIcon->isVisible()) {
        QMessageBox::critical(this, tr("Systray: Request to close tasks"),
                                 tr("The Systray app will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();        
        if(timer1->isActive()) this->stopTimer1();
        if(timer2->isActive()) this->stopTimer2();
    }
}
//![4]

MainWindow::~MainWindow()
{
//!
    QString dbgMsgTimer1("task1 stoped");
    QString dbgMsgTimer2("task2 stoped");

    if(timer1->isActive())
    {
        timer1->stop();
        qDebug() << dbgMsgTimer1;
    }

    if(timer2->isActive())
    {
        timer2->stop();
        qDebug() << dbgMsgTimer2;
    }
//!
}
