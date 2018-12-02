#include "sensormodel.h"
#include "sensorslist.h"

SensorModel::SensorModel(QObject *parent)
    : QAbstractListModel(parent)
    , mList(nullptr)
{
}

//QVariant SensorModel::headerData(int section, Qt::Orientation orientation, int role) const
//{
//    // FIXME: Implement me!
//}

int SensorModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    // FIXME: Implement me!
    return mList->items().size();
}

QVariant SensorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    const Sensor item = mList->items().at(index.row());
    switch (role) {
     case ChannelNumberRole:
        return QVariant(item.channelNumber);
     case RouterNumberRole:
        return QVariant(item.routerNumber);
    case SensorNumberRole:
       return QVariant(item.sensorNumber);
    case BordarRole:
       return QVariant(item.bordar);
    case DescriptionRole:
       return QVariant(item.description);
    case BatteryLevelRole:
       return QVariant(item.batteryLevel);
    case SampleRateRole:
       return QVariant(item.sampleRate);
    case BandpassFilterRole:
       return QVariant(item.bandpassFilter);
    case SavingOnLocalRole:
       return QVariant(item.savingOnLocal);
        case SavingOnWebRole:
           return QVariant(item.savingOnWeb);
        case SendingToWebRole:
           return QVariant(item.sendingToWeb);
        case StoryRole:
           return QVariant(item.story);
        case SensorTypeRole:
           return QVariant(item.sensorType);
//    case SensorDataRole:
//       return QVariant(item.sensorData);
//    case DataRole:
//       return QVariant::fromValue(item.data);
    }

    return QVariant();
}

bool SensorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!mList)
        return false;

    Sensor item = mList->items().at(index.row());
    switch (role) {
     case ChannelNumberRole:
        item.channelNumber = value.toInt();
     case RouterNumberRole:
       item.routerNumber = value.toInt();
     case SensorNumberRole:
       item.sensorNumber = value.toInt();
     case BordarRole:
       item.bordar = value.toString();
     case DescriptionRole:
        item.description = value.toString();
        case BatteryLevelRole:
           item.batteryLevel = value.toString();
        case SampleRateRole:
           item.sampleRate = value.toInt();
        case BandpassFilterRole:
           item.bandpassFilter = value.toString();
        case SavingOnLocalRole:
           item.savingOnLocal = value.toBool();
        case SavingOnWebRole:
           item.savingOnWeb = value.toBool();
        case SendingToWebRole:
           item.sendingToWeb = value.toBool();
        case StoryRole:
           item.story = value.toString();
        case SensorTypeRole:
           item.sensorType = value.toString();
//    case SensorDataRole:
//       item.sensorData = value.toList();
//        case DataRole:
//           item.data = value.toList().toVector().t;
    }

    if(mList->setSensorItem(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
//    if (data(index, role) != value) {
//        // FIXME: Implement me!
//        emit dataChanged(index, index, QVector<int>() << role);
//        return true;
//    }
    return false;
}

QHash<int, QByteArray> SensorModel::roleNames() const
{
  QHash<int, QByteArray> names;
  names[ChannelNumberRole] = "channelNumber";
  names[RouterNumberRole] = "routerNumber" ;
  names[SensorNumberRole] = "sensorNumber" ;
  names[BordarRole] = "bordar" ;
  names[DescriptionRole] = "description" ;
  names[BatteryLevelRole] = "batteryLevel";
  names[SampleRateRole] = "sampleRate" ;
  names[BandpassFilterRole] = "bandpassFilter" ;
  names[SavingOnLocalRole] = "savingOnLocal";
  names[SavingOnWebRole] = "savingOnWeb" ;
  names[SendingToWebRole] = "sendingToWeb" ;
  names[StoryRole] = "story" ;
  names[SensorTypeRole] = "sensorType" ;
//  names[SensorDataRole] = "sensorData" ;
//  names[DataRole] = "data" ;
  return names;
}

Qt::ItemFlags SensorModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

SensorsList *SensorModel::list() const
{
    return mList;
}

void SensorModel::setList(SensorsList *list)
{
    beginResetModel();
    if(mList)
        mList->disconnect(this);

    mList = list;

    if(mList) {
        connect(mList, &SensorsList::preItemAppended, this, [this]() {
            const int index = mList->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &SensorsList::postItemAppended, this, [this]() {
          endInsertRows();
        });
        connect(mList, &SensorsList::preItemRemoved, this, [this](int index) {
          beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &SensorsList::postItemRemoved, this, [this]() {
          endRemoveRows();
        });
    }

    endResetModel();
}

