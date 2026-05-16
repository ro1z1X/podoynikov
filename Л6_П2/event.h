#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>

class Event
{
public:
    Event();
    Event(QString name, QString description, QDate date, float cost, QString format);

    QString getName() const;
    QString getDescription() const;
    QDate getDate() const;
    float getCost() const;
    QString getFormat() const;

    void setName(const QString &name);
    void setDescription(const QString &description);
    void setDate(const QDate &date);
    void setCost(float cost);
    void setFormat(const QString &format);

    bool isValid() const;

    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

    bool operator<(const Event &other) const;

private:
    QString name;
    QString description;
    QDate date;
    float cost;
    QString format;
};

#endif // EVENT_H
