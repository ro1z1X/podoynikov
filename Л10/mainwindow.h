#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_loadPhotoButton_clicked();

    void on_registerButton_clicked();

private:

    Ui::MainWindow *ui;

    QString photoPath;

    bool validateInputs();

    bool loginExists(const QString &nickname);

    QString encryptPassword(const QString &password);

    QString addZachet(const QString &text);

    void saveData();
};

#endif
