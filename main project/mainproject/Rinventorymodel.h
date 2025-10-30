#pragma once
#include <QAbstractTableModel>
#include <QVector>
#include <QJsonArray>


struct RInventoryItem {
QString name;
double qty{0.0};
QString unit;
QStringList tags;
};


class RInventoryModel : public QAbstractTableModel {
Q_OBJECT
public:
explicit RInventoryModel(QObject* parent = nullptr);
int rowCount(const QModelIndex &parent = {}) const override;
int columnCount(const QModelIndex &parent = {}) const override;
QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
Qt::ItemFlags flags(const QModelIndex &index) const override;


void addItem(const RInventoryItem &it);
void removeRowAt(int row);
void updateItem(int row, const RInventoryItem &it);
const RInventoryItem &itemAt(int row) const;


QJsonArray toJson() const;
void fromJson(const QJsonArray &arr);
int findByName(const QString &name) const;


private:
QVector<RInventoryItem> m_items;
};
