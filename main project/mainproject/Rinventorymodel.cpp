#include "Rinventorymodel.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QStringList>
#include <QDebug>

RInventoryModel::RInventoryModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

int RInventoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.size();
}

int RInventoryModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4; // name, qty, unit, tags
}

QVariant RInventoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return QVariant();

    const RInventoryItem &item = m_items[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return item.name;
        case 1: return QString::number(item.qty);
        case 2: return item.unit;
        case 3: return item.tags.join(", ");
        }
    }

    return QVariant();
}

QVariant RInventoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return "Name";
        case 1: return "Quantity";
        case 2: return "Unit";
        case 3: return "Tags";
        }
    }
    return QVariant();
}

Qt::ItemFlags RInventoryModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void RInventoryModel::addItem(const RInventoryItem &it)
{
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append(it);
    endInsertRows();
}

void RInventoryModel::removeRowAt(int row)
{
    if (row < 0 || row >= m_items.size())
        return;
    
    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}

void RInventoryModel::updateItem(int row, const RInventoryItem &it)
{
    if (row < 0 || row >= m_items.size())
        return;
    
    m_items[row] = it;
    emit dataChanged(index(row, 0), index(row, columnCount() - 1));
}

const RInventoryItem &RInventoryModel::itemAt(int row) const
{
    return m_items[row];
}

QJsonArray RInventoryModel::toJson() const
{
    QJsonArray array;
    for (const auto &item : m_items) {
        QJsonObject obj;
        obj["name"] = item.name;
        obj["qty"] = item.qty;
        obj["unit"] = item.unit;
        
        QJsonArray tagsArray;
        for (const QString &tag : item.tags) {
            tagsArray.append(tag);
        }
        obj["tags"] = tagsArray;
        
        array.append(obj);
    }
    return array;
}

void RInventoryModel::fromJson(const QJsonArray &arr)
{
    beginResetModel();
    m_items.clear();
    
    for (const QJsonValue &value : arr) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            RInventoryItem item;
            item.name = obj["name"].toString();
            item.qty = obj["qty"].toDouble();
            item.unit = obj["unit"].toString();
            
            QJsonArray tagsArray = obj["tags"].toArray();
            for (const QJsonValue &tagValue : tagsArray) {
                item.tags.append(tagValue.toString());
            }
            
            m_items.append(item);
        }
    }
    
    endResetModel();
}

int RInventoryModel::findByName(const QString &name) const
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].name == name) {
            return i;
        }
    }
    return -1;
}
