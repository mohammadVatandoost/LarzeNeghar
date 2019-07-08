#ifndef SENSORMODEL_H
#define SENSORMODEL_H

#include <QAbstractListModel>
class SensorsList;

class SensorModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(SensorsList *list READ list WRITE setList )
public:
    explicit SensorModel(QObject *parent = nullptr);
    enum {
        ChannelNumberRole, RouterNumberRole, SensorNumberRole, BordarRole, DescriptionRole, RouterBatteryLevelRole,
        BatteryLevelRole, SampleRateRole, BandpassFilterRole, SensorDataRole,
        SavingOnLocalRole, SavingOnWebRole, SendingToWebRole, StoryRole, SensorTypeRole, DataRole
    };

    // Header: int routerNumber = 123;
    //    int sensorNumber = 345;
    //    QString bordar = "x";
    //    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    virtual QHash<int, QByteArray> roleNames() const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // for sensorList Q_PROPERTY
    SensorsList *list() const;
    void setList(SensorsList *list);
private:
    SensorsList *mList;

public slots:
    void myBatteryUpdate();
    void myApplyUpdate();
};

#endif // SENSORMODEL_H
