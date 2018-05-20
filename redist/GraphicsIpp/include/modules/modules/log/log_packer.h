#ifndef LOG_PACKER_H
#define LOG_PACKER_H

#include <QThread>
#include "third_party/xzip/XZip.h"
#include "modules/log/log.h"

namespace logging
{

class LogPacker : public QThread
{
	Q_OBJECT

public:
	static void ManageLogFilesFromDir(QDir dir);//запускает отдельный поток, не лочит программу, возвращаетс€ сразу


private:
	static bool LogPackerThreadRunning;

	LogPacker( QDir dir );
	~LogPacker();

	virtual void	run ();

	QFileInfoList filesList;
	QDir dir_;
	QFile* busyDirFile;

	struct ZIPFile 
	{
		HZIP zip_handle;
		bool need_merge;
		QString dstFileName;
		QString srcFileName;
		QDate lastFileDate;
		bool PossibleMerge()
		{
			if (!need_merge) return false;
			if (dstFileName.isEmpty() || srcFileName.isEmpty()) return false;
			return true;
		}
		ZIPFile(): zip_handle(0), need_merge(false) {}
	};

	void ChangeFileDate(QString fileName,QDate date);
	bool ZipOpenAdd(ZIPFile& zipFile,QString ZipFileName,QFileInfo f_inf);
	bool IsFileOlderThen2Days(QFileInfo f_inf);
	bool IsFileOlderThen2Month(QFileInfo f_inf);
	int  monthDif(QFileInfo f_inf) const;//остчитывает скоко мес€цев от текущего, с расчет год не беретс€
	
	bool MergeZips(QString dst, QString src);
};

}


#endif // LOG_PACKER_H
