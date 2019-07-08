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
    case RouterBatteryLevelRole:
        return QVariant(item.routerBatteryLevel);
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
    case BandpassFilterRole:
        item.bandpassFilter = value.toString();
        break;
    case ChannelNumberRole:
        item.channelNumber = value.toInt();
        break;
    case RouterNumberRole:
        item.routerNumber = value.toInt();
        break;
    case SensorNumberRole:
        item.sensorNumber = value.toInt();
        break;
    case BordarRole:
        item.bordar = value.toString();
        break;
    case DescriptionRole:
        item.description = value.toString();
        break;
    case RouterBatteryLevelRole:
        item.routerBatteryLevel = value.toString();
        break;
    case BatteryLevelRole:
        item.batteryLevel = value.toString();
        break;
    case SampleRateRole:
        item.sampleRate = value.toInt();
        break;
    case SavingOnLocalRole:
        item.savingOnLocal = value.toBool();
        break;
    case SavingOnWebRole:
        item.savingOnWeb = value.toBool();
        break;
    case SendingToWebRole:
        item.sendingToWeb = value.toBool();
        break;
    case StoryRole:
        item.story = value.toString();
        break;
    case SensorTypeRole:
        item.sensorType = value.toString();
        break;
        //    case SensorDataRole:
        //       item.sensorData = value.toList();
        //        case DataRole:
        //           item.data = value.toList().toVector().t;
    }

    if(mList->setSensorItem(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }


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
    names[RouterBatteryLevelRole] = "routerBatteryLevel";
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
        connect(mList,&SensorsList::callBatteryUpdate,this,&SensorModel::myBatteryUpdate);
        connect(mList,&SensorsList::callApplyUpdate,this,&SensorModel::myApplyUpdate);

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

void SensorModel::myBatteryUpdate()
{

    const QModelIndex &index=createIndex(mList->sensorNumberForUpdate,mList->sensorNumberForUpdate);

    int role=BatteryLevelRole;
    const QVariant &value=mList->batteryLevelForUpdate;
    setData(index, value, role);

    role=RouterBatteryLevelRole;
    const QVariant &valueRBL=mList->RouterbatteryLevelForUpdate;
    setData(index, valueRBL, role);
}

void SensorModel::myApplyUpdate()
{
    int role;


    const QVariant &value=mList->globalStringForApplyUpdate;
    switch(mList->globalStateforApplyUpdate){
    case SensorsList::Description:
        role=DescriptionRole;
        break;
    case SensorsList::SampleRate:
        role=SampleRateRole;
        break;
    case SensorsList::LBandPass:
        role=BandpassFilterRole;
        break;
    case SensorsList::HBandPass:
        role=BandpassFilterRole;
        break;
    case SensorsList::Story:
        role=StoryRole;
        break;
    }
    if(mList->sensorNumberForApplyUpdate%3==0){
        const QModelIndex &index1=createIndex(mList->sensorNumberForApplyUpdate,mList->sensorNumberForApplyUpdate);
        const QModelIndex &index2=createIndex(mList->sensorNumberForApplyUpdate+1,mList->sensorNumberForApplyUpdate+1);
        const QModelIndex &index3=createIndex(mList->sensorNumberForApplyUpdate+2,mList->sensorNumberForApplyUpdate+2);
        setData(index1,value,role);
        setData(index2,value,role);
        setData(index3,value,role);

    }else if (mList->sensorNumberForApplyUpdate%3==1) {
        const QModelIndex &index1=createIndex(mList->sensorNumberForApplyUpdate,mList->sensorNumberForApplyUpdate);
        const QModelIndex &index2=createIndex(mList->sensorNumberForApplyUpdate+1,mList->sensorNumberForApplyUpdate+1);
        const QModelIndex &index3=createIndex(mList->sensorNumberForApplyUpdate-1,mList->sensorNumberForApplyUpdate-1);
        setData(index1,value,role);
        setData(index2,value,role);
        setData(index3,value,role);
    }else{
        const QModelIndex &index1=createIndex(mList->sensorNumberForApplyUpdate,mList->sensorNumberForApplyUpdate);
        const QModelIndex &index2=createIndex(mList->sensorNumberForApplyUpdate-1,mList->sensorNumberForApplyUpdate-1);
        const QModelIndex &index3=createIndex(mList->sensorNumberForApplyUpdate-2,mList->sensorNumberForApplyUpdate-2);
        setData(index1,value,role);
        setData(index2,value,role);
        setData(index3,value,role);
    }

}

