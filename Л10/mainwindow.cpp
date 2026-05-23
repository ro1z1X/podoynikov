#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Регистрация");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadPhotoButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(
        this,
        "Выбрать фото",
        "",
        "Images (*.png *.jpg *.jpeg)"
        );

    if(file.isEmpty())
        return;

    photoPath = file;

    QPixmap pix(file);

    ui->photoLabel->setPixmap(
        pix.scaled(
            ui->photoLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            )
        );
}

bool MainWindow::validateInputs()
{
    QRegularExpression nameRegex("^[A-Za-zА-Яа-я]{2,20}$");
    QRegularExpression nicknameRegex("^[A-Za-z0-9_]{3,20}$");
    QRegularExpression emailRegex("^[\\w\\.]+@[\\w\\.]+\\.[a-zA-Z]{2,}$");
    QRegularExpression passwordRegex("^.{6,20}$");
    QRegularExpression idRegex("^[A-Z][0-9]{3}-[0-9]{2}[A-Z]-[A-Z]$");

    if(!nameRegex.match(ui->nameEdit->text()).hasMatch())
    {
        QMessageBox::warning(this, "Ошибка", "Некорректное имя");
        return false;
    }

    if(!nameRegex.match(ui->surnameEdit->text()).hasMatch())
    {
        QMessageBox::warning(this, "Ошибка", "Некорректная фамилия");
        return false;
    }

    if(!nicknameRegex.match(ui->nicknameEdit->text()).hasMatch())
    {
        QMessageBox::warning(this, "Ошибка", "Некорректный никнейм");
        return false;
    }

    if(!idRegex.match(ui->idEdit->text()).hasMatch())
    {
        QMessageBox::warning(this, "Ошибка", "ID должен быть A111-11A-A");
        return false;
    }

    if(!emailRegex.match(ui->emailEdit->text()).hasMatch())
    {
        QMessageBox::warning(this, "Ошибка", "Некорректная почта");
        return false;
    }

    if(!passwordRegex.match(ui->passwordEdit->text()).hasMatch())
    {
        QMessageBox::warning(this, "Ошибка", "Пароль минимум 6 символов");
        return false;
    }

    if(loginExists(ui->nicknameEdit->text()))
    {
        QMessageBox::warning(this, "Ошибка", "Логин уже занят");
        return false;
    }

    return true;
}

bool MainWindow::loginExists(const QString &nickname)
{
    QString desktop =
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    QDir dir(desktop + "/Accounts");

    return dir.exists(nickname);
}

QString MainWindow::encryptPassword(const QString &password)
{
    QString encrypted;

    for(QChar ch : password)
    {
        encrypted += QChar(ch.unicode() + 3);
    }

    return encrypted;
}

QString MainWindow::addZachet(const QString &text)
{
    QString result;

    for(QChar ch : text)
    {
        result += ch;
        result += "допуск";
    }

    return result;
}

void MainWindow::saveData()
{
    QString desktop =
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    QString folderPath =
        desktop + "/Accounts/" + ui->nicknameEdit->text();

    QDir().mkpath(folderPath);

    // фотка
    QString photoSavePath = folderPath + "/photo.jpg";

    if(photoPath.isEmpty())
    {
        QPixmap defaultPix(200,200);
        defaultPix.fill(Qt::gray);
        defaultPix.save(photoSavePath);
    }
    else
    {
        QFile::copy(photoPath, photoSavePath);
    }

    // JSON
    QJsonObject obj;

    obj["name"] = ui->nameEdit->text();
    obj["surname"] = ui->surnameEdit->text();
    obj["nickname"] = ui->nicknameEdit->text();
    obj["id_key"] = ui->idEdit->text();
    obj["email"] = ui->emailEdit->text();

    QStringList servers;

    if(ui->europeCheck->isChecked())
        servers.append("Europe");

    if(ui->asiaCheck->isChecked())
        servers.append("Asia");

    if(ui->americaCheck->isChecked())
        servers.append("America");

    obj["servers"] = servers.join(", ");

    QFile jsonFile(folderPath + "/data.json");

    if(jsonFile.open(QIODevice::WriteOnly))
    {
        jsonFile.write(QJsonDocument(obj).toJson());
        jsonFile.close();
    }

    // TXT
    QFile txtFile(folderPath + "/credentials.txt");

    if(txtFile.open(QIODevice::WriteOnly))
    {
        QString encrypted = encryptPassword(ui->passwordEdit->text());
        encrypted = addZachet(encrypted);

        QString data =
            ui->emailEdit->text()
            + "/"
            + encrypted;

        txtFile.write(data.toUtf8());
        txtFile.close();
    }
}

void MainWindow::on_registerButton_clicked()
{
    if(!validateInputs())
        return;

    saveData();

    QMessageBox::information(this, "Успех", "Регистрация завершена");
}
