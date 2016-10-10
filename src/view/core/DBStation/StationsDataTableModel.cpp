#include "StationsDataTableModel.h"

#include <QSqlQuery>

StationsDataTableModel::StationsDataTableModel(QSqlDatabase database, QObject *parent)
	: QSqlQueryModel( parent )
	, m_database( database )
{
    update();
}

StationsDataTableModel::~StationsDataTableModel()
{
}

void StationsDataTableModel::update()
{
	// ping server before update model query
	QSqlQuery ping( "SELECT 1", m_database );
	ping.exec();

	QString sql = "SELECT sdi.id, st.name AS stationName, " \
			"st.ip AS stationIP, sd.port, cat.name AS CategoryName, " \
			"sdi.frequency, sdi.bandwidth, sigType.name AS signalType, " \
            "sdi.datetime, IF(sdi.checked, 'true', 'false') FROM stationData AS sdi " \
			"INNER JOIN stationDevices as sd ON sdi.deviceID=sd.id " \
			"INNER JOIN station AS st ON st.id=sd.stationID " \
			"INNER JOIN category AS cat ON sdi.categoryID=cat.id " \
			"INNER JOIN signalType AS sigType ON sdi.signalTypeID=sigType.id " \
			"ORDER BY stationName";

    setQuery( sql, m_database );
}

Qt::ItemFlags StationsDataTableModel::flags(const QModelIndex &index) const
{
    if(index.column() == 9)
         return QSqlQueryModel::flags(index) | Qt::ItemIsUserCheckable;
    return QSqlQueryModel::flags(index);
}

QVariant StationsDataTableModel::data(const QModelIndex & index, int role = Qt::DisplayRole) const
{
    if(index.column() == 9)
    {
        if(role == Qt::CheckStateRole)
        {
            bool state = index.data().toBool();
            if(state == false)
                return Qt::Unchecked;
            else
                return Qt::Checked;
        }
    }

    return QSqlQueryModel::data(index, role);
}

bool StationsDataTableModel::setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole)
{
    bool state= false;
    if(index.column() == 9 && role == Qt::CheckStateRole)
    {
        int chk = value.toInt();
        int row = index.row();
        QModelIndex ind = index.sibling(row, 0);
        int id = this->data(ind).toInt();

        query().exec(QString("UPDATE stationdata set checked=%1 where id=%2").arg(chk).arg(id));

        emit updateSignal();
    }
    else
        state = QSqlQueryModel::setData(index, value, role);


    return state;
}
