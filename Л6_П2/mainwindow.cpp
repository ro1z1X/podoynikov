#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTableWidgetItem>
#include <algorithm>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->validTable->setColumnCount(5);
    ui->validTable->setHorizontalHeaderLabels({"Название", "Описание", "Дата", "Стоимость", "Формат"});
    ui->validTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->validTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->invalidTable->setColumnCount(5);
    ui->invalidTable->setHorizontalHeaderLabels({"Название", "Описание", "Дата", "Стоимость", "Формат"});
    ui->invalidTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->invalidTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Выберите JSON файл",
                                                    "",
                                                    "JSON Files (*.json)");

    if (fileName.isEmpty()) return;

    loadEventsFromFile(fileName);
}

void MainWindow::loadEventsFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть JSON файл");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    // Проверка на пустой файл
    if (data.isEmpty()) {
        QMessageBox::information(this, "Информация", "Файл пуст. Добавьте мероприятия в первой программе.");
        validEvents.clear();
        invalidEvents.clear();
        displayValidEvents();
        displayInvalidEvents();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray eventsArray;

    // Определяем, массив это или объект
    if (doc.isArray()) {
        eventsArray = doc.array();
    } else if (doc.isObject()) {
        // Если это один объект, добавляем его в массив
        eventsArray.append(doc.object());
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный формат JSON файла");
        return;
    }

    validEvents.clear();
    invalidEvents.clear();

    for (const auto &item : eventsArray) {
        Event event;
        event.fromJson(item.toObject());

        if (event.isValid()) {
            validEvents.append(event);
        } else {
            invalidEvents.append(event);
        }
    }

    // Сортировка корректных мероприятий в обратном алфавитном порядке по названию
    std::sort(validEvents.begin(), validEvents.end());
    std::reverse(validEvents.begin(), validEvents.end());

    displayValidEvents();
    displayInvalidEvents();

    if (!invalidEvents.isEmpty()) {
        saveInvalidEvents();
    }

    QMessageBox::information(this, "Информация",
                             QString("Загружено мероприятий:\nКорректных: %1\nБитых: %2")
                                 .arg(validEvents.size())
                                 .arg(invalidEvents.size()));
}

void MainWindow::displayValidEvents()
{
    ui->validTable->setRowCount(validEvents.size());

    for (int i = 0; i < validEvents.size(); ++i) {
        const Event &event = validEvents[i];

        ui->validTable->setItem(i, 0, new QTableWidgetItem(event.getName()));
        ui->validTable->setItem(i, 1, new QTableWidgetItem(event.getDescription()));
        ui->validTable->setItem(i, 2, new QTableWidgetItem(event.getDate().toString("yyyy-MM-dd")));
        ui->validTable->setItem(i, 3, new QTableWidgetItem(QString::number(event.getCost(), 'f', 2)));
        ui->validTable->setItem(i, 4, new QTableWidgetItem(event.getFormat()));
    }

    ui->validTable->resizeColumnsToContents();
}

void MainWindow::displayInvalidEvents()
{
    ui->invalidTable->setRowCount(invalidEvents.size());

    for (int i = 0; i < invalidEvents.size(); ++i) {
        const Event &event = invalidEvents[i];

        QTableWidgetItem *nameItem = new QTableWidgetItem();
        if (event.getName().isEmpty()) {
            nameItem->setText("<пусто>");
            nameItem->setBackground(Qt::red);
            nameItem->setForeground(Qt::white);
        } else {
            nameItem->setText(event.getName());
        }
        ui->invalidTable->setItem(i, 0, nameItem);

        QTableWidgetItem *descItem = new QTableWidgetItem();
        if (event.getDescription().isEmpty()) {
            descItem->setText("<пусто>");
        } else {
            descItem->setText(event.getDescription());
        }
        ui->invalidTable->setItem(i, 1, descItem);

        QTableWidgetItem *dateItem = new QTableWidgetItem();
        if (!event.getDate().isValid()) {
            dateItem->setText("<неверная дата>");
            dateItem->setBackground(Qt::red);
            dateItem->setForeground(Qt::white);
        } else if (event.getDate() < QDate(2020, 1, 1)) {
            dateItem->setText(event.getDate().toString("yyyy-MM-dd"));
            dateItem->setBackground(Qt::red);
            dateItem->setForeground(Qt::white);
        } else {
            dateItem->setText(event.getDate().toString("yyyy-MM-dd"));
        }
        ui->invalidTable->setItem(i, 2, dateItem);

        QTableWidgetItem *costItem = new QTableWidgetItem();
        if (event.getCost() < 1) {
            costItem->setText("<некорректная>");
            costItem->setBackground(Qt::red);
            costItem->setForeground(Qt::white);
        } else {
            costItem->setText(QString::number(event.getCost(), 'f', 2));
        }
        ui->invalidTable->setItem(i, 3, costItem);

        QTableWidgetItem *formatItem = new QTableWidgetItem();
        if (event.getFormat().isEmpty()) {
            formatItem->setText("<пусто>");
            formatItem->setBackground(Qt::red);
            formatItem->setForeground(Qt::white);
        } else {
            formatItem->setText(event.getFormat());
        }
        ui->invalidTable->setItem(i, 4, formatItem);
    }

    ui->invalidTable->resizeColumnsToContents();
}

void MainWindow::saveInvalidEvents()
{
    QString fileName = "invalid_events.json";
    QFile file(fileName);

    QJsonArray invalidArray;
    for (const Event &event : invalidEvents) {
        invalidArray.append(event.toJson());
    }

    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(invalidArray);
        file.write(doc.toJson());
        file.close();

        ui->statusBar->showMessage(QString("Битые мероприятия сохранены в файл: %1").arg(fileName), 5000);
    }
}
