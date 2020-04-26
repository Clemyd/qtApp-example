#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDir>

QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;
class QTableWidgetItem;
QT_END_NAMESPACE

//![0]
class MainWindow : public QWidget
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);  
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent *event) override;

public slots:
  void launchScheduler();

private slots:
  void iconActivated(QSystemTrayIcon::ActivationReason);
  void showMessage();
  void browse();
  void find();

private:
  void showFiles(const QStringList &paths);
  QComboBox *createComboBox(const QString &text = QString());
  void stopTimer1();
  void stopTimer2();

  QTimer *timer1;
  QTimer *timer2;
  QSystemTrayIcon* trayIcon;
  QMenu* trayIconMenu;

  QMenu* createMenu();

  QComboBox *msgComboBox;
  QComboBox *fileComboBox;
  QComboBox *directoryComboBox;
  QLabel *filesFoundLabel;

  QDir currentDir;
};
//![0]
