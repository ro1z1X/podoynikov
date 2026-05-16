#include "Event.h"

Event::Event()
    : name(""), description(""), date(QDate()), cost(0.0f), format("")
{
}

Event::Event(QString name, QString description, QDate date, float cost, QString format)
    : name(name), description(description), date(date), cost(cost), format(format)
{
}

QString Event::getName() const { return name; }
QString Event::getDescription() const { return description; }
QDate Event::getDate() const { return date; }
float Event::getCost() const { return cost; }
QString Event::getFormat() const { return format; }

void Event::setName(const QString &name) { this->name = name; }
void Event::setDescription(const QString &description) { this->description = description; }
void Event::setDate(const QDate &date) { this->date = date; }
void Event::setCost(float cost) { this->cost = cost; }
void Event::setFormat(const QString &format) { this->format = format; }

bool Event::isValid() const
{
    return !name.isEmpty() &&
           date.isValid() &&
           date >= QDate(2020, 1, 1) &&
           cost >= 1 &&
           !format.isEmpty();
}

QJsonObject Event::toJson() const
{
    QJsonObject json;
    json["name"] = name;
    json["description"] = description;
    json["date"] = date.toString("yyyy-MM-dd");
    json["cost"] = cost;
    json["format"] = format;
    return json;
}

void Event::fromJson(const QJsonObject &json)
{
    name = json["name"].toString();
    description = json["description"].toString();
    date = QDate::fromString(json["date"].toString(), "yyyy-MM-dd");
    cost = json["cost"].toDouble();
    format = json["format"].toString();
}

bool Event::operator<(const Event &other) const
{
    return name < other.name;
}
