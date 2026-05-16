#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->costEdit->setValidator(new QDoubleValidator(0, 1000000, 2, this));

    createEmptyJsonFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createEmptyJsonFile()
{
    QString fileName = "events.json";
    QFile file(fileName);

    // Если файла не существует, создаем пустой файл (0 байт)
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly)) {
            // Не пишем ничего - файл остается пустым
            file.close();
            qDebug() << "Создан пустой файл events.json";
        }
    }
}

void MainWindow::on_addButton_clicked()
{
    QString name = ui->nameEdit->text();
    QString description = ui->descEdit->toPlainText();
    QDate date = ui->dateEdit->date();
    float cost = ui->costEdit->text().toFloat();
    QString format = ui->formatEdit->text();

    Event newEvent(name, description, date, cost, format);

    saveEventToJson(newEvent);
    clearFields();

    QMessageBox::information(this, "Успех", "Мероприятие успешно добавлено!");
}

void MainWindow::on_loadTxtButton_clicked()
{
    loadFromTxtFile();
}

void MainWindow::saveEventToJson(const Event &event)
{
    QString fileName = "events.json";
    QFile file(fileName);

    QJsonArray eventsArray;

    // Читаем существующие данные
    if (file.exists() && file.size() > 0) {
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            file.close();

            if (!data.isEmpty()) {
                QJsonDocument doc = QJsonDocument::fromJson(data);

                // Проверяем, массив это или объект
                if (doc.isArray()) {
                    eventsArray = doc.array();
                } else if (doc.isObject()) {
                    // Если это объект (одна запись), добавляем его в массив
                    eventsArray.append(doc.object());
                }
            }
        }
    }

    // Добавляем новое мероприятие
    eventsArray.append(event.toJson());

    // Сохраняем в файл
    if (file.open(QIODevice::WriteOnly)) {
        // Всегда сохраняем как массив
        QJsonDocument doc(eventsArray);
        file.write(doc.toJson());
        file.close();
        qDebug() << "Сохранено в events.json, всего записей:" << eventsArray.size();
    }
}

void MainWindow::loadFromTxtFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Выберите TXT файл",
                                                    "",
                                                    "Text Files (*.txt)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    file.close();

    QStringList parts = line.split('|');

    if (parts.size() >= 5) {
        ui->nameEdit->setText(parts[0]);
        ui->descEdit->setPlainText(parts[1]);
        ui->dateEdit->setDate(QDate::fromString(parts[2], "yyyy-MM-dd"));
        ui->costEdit->setText(parts[3]);
        ui->formatEdit->setText(parts[4]);

        QMessageBox::information(this, "Успех", "Данные загружены из TXT файла");
    } else {
        QMessageBox::warning(this, "Ошибка",
                             "Неверный формат файла.\n"
                             "Ожидается: название|описание|дата|стоимость|формат\n"
                             "Пример: Концерт|Классическая музыка|2024-12-25|1500|Очный");
    }
}

void MainWindow::clearFields()
{
    ui->nameEdit->clear();
    ui->descEdit->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->costEdit->clear();
    ui->formatEdit->clear();
}
