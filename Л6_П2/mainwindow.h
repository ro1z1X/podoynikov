#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "Event.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openFileButton_clicked();

private:
    Ui::MainWindow *ui;
    QVector<Event> validEvents;
    QVector<Event> invalidEvents;

    void loadEventsFromFile(const QString &fileName);
    void displayValidEvents();
    void displayInvalidEvents();
    void saveInvalidEvents();
};

#endif // MAINWINDOW_H
