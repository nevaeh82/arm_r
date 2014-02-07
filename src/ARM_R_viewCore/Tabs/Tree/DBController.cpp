#include "DBController.h"
#include <QDebug>

Pw::Logger::ILogger* DBController::_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(DBController));


DBController::DBController()
{
    _sdb = QSqlDatabase::addDatabase("QSQLITE");
    _set_db();
    connect(this, SIGNAL(signalSet(QMap<QString,QVariant>*)), this, SLOT(_slot_set(QMap<QString,QVariant>*)));
}

DBController::~DBController()
{
    if(_sdb.isOpen())
        _sdb.close();
}

int DBController::_set_db()
{
    QString db_setting_file = QCoreApplication::applicationDirPath();
    db_setting_file.append("/DB/db_model.ini");
    if(_read_settings(db_setting_file) != 0)
    {
        return -1;
    }

    _sdb.setDatabaseName(_db_name);
    if(!_sdb.open())
    {
        QMessageBox::warning(NULL, tr("ВНИМАНИЕ"), _sdb.lastError().text(),QMessageBox::Cancel, QMessageBox::Ok);
//        qDebug() << _sdb.lastError().text();
        _logger->info(_sdb.lastError().text());
    }
}


/// create table based on pid
void DBController::_set_table(QString name)
{
    QString str;
    str.append("CREATE TABLE ");
    str.append(name);
    str.append("(id integer PRIMARY KEY NOT NULL, pid integer, name TEXT, value integer, state integer, editable boolean)");
    QSqlQuery query;
    bool err = query.exec(str);

    if(!err)
    {
//        qDebug() << query.lastError().text();
//        _logger->info(query.lastError().text());

    }
//    _cur_table_name = name;
}

QMap<QString, QVariant>* DBController::get_data(int id, int pid)
{

    /// dynamically forming table name for connecting 1 to many
    QString table_name = "Station";
    table_name += QString::number(pid);
    _set_table(table_name);

    QStringList req_fields("SELECT * FROM ");

    req_fields.append(table_name);
    req_fields.append(" WHERE id=");
    req_fields.append(QString::number(id));


    QSqlQuery query;
    bool err = query.exec(req_fields.join(""));
    QSqlRecord rec = query.record();
    QMap<QString, QVariant>* map_result = new QMap<QString, QVariant>;
    while(query.next())
    {       
        for(int i = 0; i < rec.count(); i++)
        {
            map_result->insert(rec.fieldName(i), query.value(i));
//			qDebug() << rec.fieldName(i);
        }
    }
    return map_result;
}

QMap<QString, QVariant>* DBController::get_data(int pid)
{
    /// dynamically forming table name for connecting 1 to many
    QString table_name = "Station";
    table_name += QString::number(pid);
    _set_table(table_name);

    QStringList req_fields("SELECT * FROM ");

    req_fields.append(table_name);
    QSqlQuery query;
    bool err = query.exec(req_fields.join(""));
    QSqlRecord rec = query.record();
    QMap<QString, QVariant>* map_result = new QMap<QString, QVariant>;
    while(query.next())
    {
        for(int i = 0; i < rec.count(); i++)
        {
            map_result->insert(rec.fieldName(i), query.value(i));
        }
    }
    return map_result;
}

void DBController::set_data(QMap<QString, QVariant> *data)
{
    emit signalSet(data);
}

void DBController::_slot_set(QMap<QString, QVariant> *data)
{
    QString table_name = "Station";
    table_name.append(data->value("pid").toString());

    _set_table(table_name);

    QStringList req_fields("SELECT id FROM ");

    req_fields.append(table_name);
    req_fields.append(" WHERE pid=");
    req_fields.append(data->value("pid").toString());

    QSqlQuery query;
    bool err = query.exec(req_fields.join(""));

    bool err1 = query.next();
    if(err1)
    {
        QString update_string;
        update_string.append("UPDATE ");
        update_string.append(table_name);
        update_string.append(" SET \"");
        QMap<QString, QVariant>::iterator it;
        for( it = data->begin(); it != data->end(); ++it)
        {
            if(it.key() == "id")
            {
                continue;
            }
            update_string.append(it.key());
            update_string.append("\"=\"");
            update_string.append(it.value().toString());
            update_string.append("\", \"");
        }
        update_string.remove(update_string.length() - 3, 3);

        update_string.append(" WHERE ");

        update_string.append("pid=");
        update_string.append(data->value("pid").toString());
        update_string.append(" AND ");
        update_string.append("id=");
        update_string.append(data->value("id").toString());
        err = query.exec(update_string);
    }
    else
    {
        QString insert_string;
        insert_string.append("INSERT INTO ");
        insert_string.append(table_name);
        insert_string.append("(");
        QString values;
        QMap<QString, QVariant>::iterator it;
        for( it = data->begin(); it != data->end(); ++it)
        {
            insert_string.append(it.key());
            insert_string.append(", ");
            values.append("\"");
            values.append(it.value().toString());
            values.append("\", ");
        }
        insert_string.remove(insert_string.length() - 2, 1);
        values.remove(values.length() - 2, 1);
        insert_string.append(") VALUES (");
        insert_string.append(values);
        insert_string.append(")");

        err = query.exec(insert_string);
    }
}

/// read db file location from ini file
int DBController::_read_settings(QString path_to_ini_file)
{
    int error = -1;
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QSettings m_settings(path_to_ini_file, QSettings::IniFormat);

    m_settings.setIniCodec(codec);

    QStringList childKeys = m_settings.childGroups();
    foreach (const QString &childKey, childKeys)
    {
        if(childKey.toLatin1() != "DB_file_name")
        {
            continue;
        }
        m_settings.beginGroup(childKey);

        _db_name.append("DB/");
        _db_name += m_settings.value("file", "db_tree.sqlite").toString();


        error = 0;
        m_settings.endGroup();
    }

    return error;
}
