#include "StdAfx.h"
#include "log_packer.h"
#ifdef OMEGA
#include "third_party/crashrpt/include/CrashRpt.h"
#endif // OMEGA

using namespace logging;
bool LogPacker::LogPackerThreadRunning = false;

void LogPacker::ManageLogFilesFromDir( QDir dir )
{
	if (LogPacker::LogPackerThreadRunning)
	{
		LTRACE_S("LogPacker ��������� ��� �������. ����� �������.");
		return;
	}

	LogPacker::LogPackerThreadRunning = true;
	new LogPacker(dir);
}

//////////////////////////////////////////////////////////

LogPacker::LogPacker(QDir dir): QThread(nullptr)
{
	LSCOPE("LogPacker");
	dir_ = dir;
	if (!dir.exists() || !dir.isReadable()) 
	{
		LOG_S("��������� ������� ��� �������� log ������ ��������. ������� "+dir.absolutePath());
		deleteLater();
		return;
	}
	
	//�������� �� ������-�� ����� ������ ���������� ���� ������� ��� ������
	busyDirFile = new QFile(dir_.absolutePath()+"/LogPacker.lock", this);
	if (busyDirFile->exists())
	{
		if (!busyDirFile->remove())
		{
			LOG_S("������� "+dir_.absolutePath()+" ����� � ��������� ������ �����������. �����.");
			deleteLater();
			return;
		}
	}

	if (!busyDirFile->open(QIODevice::ReadWrite | QIODevice::Truncate))
	{
		LOG_S("������� "+dir_.absolutePath()+" ����� � ��������� ������ �����������. �����.");
		deleteLater();
		return;
	}


	QStringList filter;filter << "*.log";
	filesList = dir_.entryInfoList(filter,QDir::Files | QDir::NoDotAndDotDot);
	if (filesList.count() == 0)
	{
		LOG_S("��������� ������� ��� �������� log ������ �� �������� �������� ����� � ����������� log. ������� "+dir_.absolutePath());
		deleteLater();
		return;
	}

	LTRACE_S(QString("������� %1 log ������. ������ �������� ������ ������� ��� � ��������").arg(filesList.count()));

	start();
}

LogPacker::~LogPacker()
{
	LSCOPE("LogPacker");
	busyDirFile->remove();
	delete busyDirFile;busyDirFile = nullptr;//���� ������� ������ (���� parent �� ���), �� ����� ���� ���� ���������...
	
	LTRACE_S("�������� ������, ����������");
	LogPacker::LogPackerThreadRunning = false;
}

void logging::LogPacker::run()
{
	LSCOPE("LogPackerThread");

#ifdef OMEGA
    CrThreadAutoInstallHelper crash_rpt_helper;
#endif // OMEGA

	ZIPFile curZipFile_;
	ZIPFile prev_1_ZipFile_;
	ZIPFile prev_2_ZipFile_;

	//��������� ��� ����� "log_*.zip"������ ��� 2 ������
	QStringList filter;filter << "log_*.zip";
	QFileInfoList ZIPfilesList = dir_.entryInfoList(filter,QDir::Files | QDir::NoDotAndDotDot);
	int ZipsDeleteCount = 0;//����� ��� �������
	for (int file_i = 0;file_i < ZIPfilesList.count();file_i++)
	{
		if (IsFileOlderThen2Month(ZIPfilesList[file_i]))
		{
			LOG_S("��� ���������� ����� zip: "+ZIPfilesList[file_i].absoluteFilePath());
			if (QFile::exists(ZIPfilesList[file_i].absoluteFilePath()))
			{
				QFile::remove(ZIPfilesList[file_i].absoluteFilePath());
				ZipsDeleteCount++;
			}
		}
	}
	if (ZipsDeleteCount > 0) LOG_S("������� ������, ����� 2 �������� ��������, ������� � ���������� "+QString::number(ZipsDeleteCount)+" �� "+QString::number(ZIPfilesList.count()));


	ZipsDeleteCount = 0;
	for (int file_i = 0;file_i < filesList.count();file_i++)
	{
		//�������� �� ������� ������ log � 2 �������, �� ��������
		if (IsFileOlderThen2Month(filesList[file_i]))
		{
			LTRACE_S("��� ���������� ����� log: "+filesList[file_i].absoluteFilePath());
			if (QFile::exists( filesList[file_i].absoluteFilePath() )) QFile::remove( filesList[file_i].absoluteFilePath() );
			continue;
		}
				
		QString fname = dir_.absolutePath()+"/log_"+filesList[file_i].lastModified().toString("MMMM.yyyy")+".zip";
		//�������� �� ������� ������ log � ������� �������, �� ���������� � �������
		int m_dif = monthDif(filesList[file_i]);
		if ((m_dif == 1) || (m_dif == -11))
		{
			ZipOpenAdd(prev_1_ZipFile_,fname,filesList[file_i]);
			ZipsDeleteCount++;
			continue;
		}

		if (( m_dif == 2) || (m_dif == -10))
		{
			ZipOpenAdd(prev_2_ZipFile_,fname,filesList[file_i]);
			ZipsDeleteCount++;
			continue;
		}

		//�������� �� ������� ������ log �� ������ ����� 2 ����, �� ���������� � ��������
		if (IsFileOlderThen2Days(filesList[file_i]))
		{
			ZipOpenAdd(curZipFile_,fname,filesList[file_i]);
			ZipsDeleteCount++;
		}
				
	}
	

	CloseZip(curZipFile_.zip_handle);
	CloseZip(prev_1_ZipFile_.zip_handle);
	CloseZip(prev_2_ZipFile_.zip_handle);	
	
	//������� �� ������� ����� � �����������
	//���������� � ������� ��������� ����� src
	if (curZipFile_.PossibleMerge())
	{
		if (MergeZips(curZipFile_.dstFileName,curZipFile_.srcFileName)) QFile::remove(curZipFile_.srcFileName);
	}
	if (prev_1_ZipFile_.PossibleMerge())
	{
		if (MergeZips(prev_1_ZipFile_.dstFileName,prev_1_ZipFile_.srcFileName)) QFile::remove(prev_1_ZipFile_.srcFileName);
	}
	if (prev_2_ZipFile_.PossibleMerge())
	{
		if (MergeZips(prev_2_ZipFile_.dstFileName,prev_2_ZipFile_.srcFileName)) QFile::remove(prev_2_ZipFile_.srcFileName);
	}
	
	//�������� ��� ��������� �������
	auto CheckArcDate = [=](QString l_fileName,QDate l_lastFileDate)
	{
		QFileInfo z_inf(l_fileName);
		if ((z_inf.lastModified().date().month() != l_lastFileDate.month()) || (z_inf.lastModified().date().year() != l_lastFileDate.year()))
		{
			//��������� �������
			ChangeFileDate(l_fileName,l_lastFileDate);
		}
		else
		{
			if (z_inf.lastModified().date().day() < l_lastFileDate.day())
			{
				//��������� �������
				ChangeFileDate(l_fileName,l_lastFileDate);
			}
		}
	};


	if (!curZipFile_.dstFileName.isEmpty())
	{
		CheckArcDate(curZipFile_.dstFileName,curZipFile_.lastFileDate);
	}
	if (!prev_1_ZipFile_.dstFileName.isEmpty())
	{
		CheckArcDate(prev_1_ZipFile_.dstFileName,prev_1_ZipFile_.lastFileDate);
	}
	if (!prev_2_ZipFile_.dstFileName.isEmpty())
	{
		CheckArcDate(prev_2_ZipFile_.dstFileName,prev_2_ZipFile_.lastFileDate);
	}

	//���������� ������
	LTRACE_S(QString("���������. ��������� %1 log ������ �� %2").arg(ZipsDeleteCount).arg(filesList.count()));
	deleteLater();
	return;
}

int logging::LogPacker::monthDif( QFileInfo f_inf ) const
{
	QDateTime f_dt = f_inf.lastModified();
	QDateTime cur_dt = QDateTime::currentDateTime();
	int dif_m = cur_dt.date().month() - f_dt.date().month();
	return dif_m;
}

bool logging::LogPacker::MergeZips( QString dst, QString src)
{
	LTRACE_S("���������� ����������� zip: " + dst + " � "+ src);
	if (dst == src) return false;
	if (!(QFile::exists(dst) && QFile::exists(src)))
	{
		LTRACE_S("���� �� ���������� " + dst + " ��� "+ src);
		return false;
	}

	QFile dst_file(dst);
	if (!dst_file.open( QIODevice::ReadWrite))
	{
		LTRACE_S("���� �� ������� ��� ������ " + dst);
		return false;
	}
	QFile src_file(src);
	if (!src_file.open( QIODevice::ReadOnly))
	{
		LTRACE_S("���� �� ������� ��� ������ " + src);
		return false;
	}

	//78 = ������ 1 ������ + ������ �������� zip, ���������� ��������� ������ zip ����� � ����������� ������������
	if (dst_file.size() < 78)
	{
		LTRACE_S("������ ������� ����� < 78 ���� " + dst);
		return false;
	}
	if (src_file.size() < 78)
	{
		LTRACE_S("������ ������� ����� < 78 ���� " + src);
		return false;
	}

	struct someInfoAboutCetralDir
	{
		int		CentralDirOffset;
		int		CentralDirSize;
		ushort	filesCount;
		uchar*	DirData;
		someInfoAboutCetralDir() : CentralDirOffset(0), CentralDirSize(0), filesCount(0)
		{
			DirData = nullptr;
		}
		~someInfoAboutCetralDir()
		{
			if (DirData)
			{
				delete[] DirData;
				LTRACE_S(QString("���� ������ %1 ���� ��� ������� zip - ����������").arg(CentralDirSize));
			}
		}
	};
	
	someInfoAboutCetralDir dst_central;
	someInfoAboutCetralDir src_central;
	
	auto SearchAndFillInfo = [](QFile* file, someInfoAboutCetralDir* central_info) -> bool
	{
		file->seek(file->size());
		uint lookOffset = 4;
		uint posOfEnd = 0;
		bool founded = false;
		while (file->pos() != 0)
		{
			if (!file->seek(file->size()-lookOffset)) return false;
			uint buff = 0;
			file->read((char*)&buff,4);
			if (buff == 101010256u)//��������� 50 4B 05 06
			{
				posOfEnd = file->size()-lookOffset;
				founded = true;break;
			} 
			lookOffset++;
		}
		if (!founded) return false;

		file->seek(posOfEnd+10);
		file->read((char*)&central_info->filesCount,2); 
		file->read((char*)&central_info->CentralDirSize,4); 
		file->read((char*)&central_info->CentralDirOffset,4);

		//�������� ������������
		file->seek(central_info->CentralDirOffset);
		uint buff = 0;
		file->read((char*)&buff,4);
		if (buff != 33639248u) return false;
		if (central_info->CentralDirOffset+central_info->CentralDirSize != posOfEnd) return false;
		central_info->DirData = new uchar[central_info->CentralDirSize];
		LTRACE_S(QString("�������� %1 ���� �� ����������� ������, ��� ������� zip").arg(central_info->CentralDirSize));
		if (central_info->DirData == nullptr) return false;
		file->seek(central_info->CentralDirOffset);
		file->read((char*)central_info->DirData,central_info->CentralDirSize);
		return true;
	};
	
	if (!SearchAndFillInfo(&dst_file,&dst_central)) return false;
	if (!SearchAndFillInfo(&src_file,&src_central)) return false;

	//����� ���� ������ �� src � dst
	dst_file.seek(dst_central.CentralDirOffset);
	src_file.seek(0);
	//������ � ����� ������� �������� �� 3 �����, ����� �� ����� ������...
	const int MaximumBlockSize = 3000000;
	while (src_file.pos() < src_central.CentralDirOffset)
	{
		qint64 size_for_this = src_central.CentralDirOffset-src_file.pos();
		if (size_for_this <= 0)
		{
			LOG_S("������ � ������� ����� zip, CentralDirOffset");
			return false;
		}
		if (size_for_this > MaximumBlockSize) size_for_this = MaximumBlockSize;
		QByteArray ba = src_file.read(size_for_this);
		dst_file.write(ba);
	}
	//������ � �����
	//������ �� dst
	//������ �� src

	//����� dst ����������� ����������� � ����, ��� ��� ���������
	dst_file.write((char*)dst_central.DirData, dst_central.CentralDirSize );
	//������ � �����
	//������ �� dst
	//������ �� src
	//dst_central

	//������� ����������� src
	//���������� ����� ��������� ������ �������� 42 �� ������ ���������� ��������� � ����������� ����������� 
	for (int i = 0;i < src_central.CentralDirSize;i++)
	{
		if (*(uint*)&src_central.DirData[i] == 33639248u)//���� ���������
		{
			if (i+46 > src_central.CentralDirSize)
			{
				LOG_S("������ � ������� ����� zip. ��������� ��������� �������, CentralDirSize");
				return false;
			}
			uint oldOffset = *(uint*)&src_central.DirData[i+42];
			Q_ASSERT_X(!((i == 0) && (oldOffset != 0)),"LogPacker::MergeZips","������ � ����������� ������� ���������");
			oldOffset += dst_central.CentralDirOffset;
			memcpy(&src_central.DirData[i+42],&oldOffset,4);
			i+=45;//i++ � ��������� ����� ��������
		}
	}
	dst_file.write((char*)src_central.DirData, src_central.CentralDirSize);
	//������ � �����
	//������ �� dst
	//������ �� src
	//dst_central
	//src_central
	
	//�������������� � ����� �������� �����
	char EndOfFileHeader[] = {	0x50, 0x4B, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };//22
	ushort totalFilesCount = dst_central.filesCount+src_central.filesCount;
	memcpy(&EndOfFileHeader[8],&totalFilesCount,2);
	memcpy(&EndOfFileHeader[10],&totalFilesCount,2);
	uint size_of_central_directory = src_central.CentralDirSize+dst_central.CentralDirSize;
	memcpy(&EndOfFileHeader[12],&size_of_central_directory,4);
	uint offset_of_central_directory = src_central.CentralDirOffset+dst_central.CentralDirOffset;
	memcpy(&EndOfFileHeader[16],&offset_of_central_directory,4);
	dst_file.write(EndOfFileHeader,22);
	//������ � �����
	//������ �� dst
	//������ �� src
	//dst central dir
	//src central dir
	//end header
	
	return true;
}

void logging::LogPacker::ChangeFileDate( QString fileName,QDate date )
{
	LTRACE_S("����� ����� "+fileName+" �������� �� "+date.toString("dd.MM.yyyy"));
	wchar_t *wch = new wchar_t[fileName.size() + 1];
	memset(wch,0,	(fileName.size()+1) *2);
	fileName.toWCharArray(wch);
	
	HANDLE h = CreateFile(wch, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) {LTRACE_S("Creation file error. INVALID_HANDLE_VALUE"); delete[] wch; return;}
	SYSTEMTIME sytemTime;
	GetLocalTime(&sytemTime);
	sytemTime.wYear			= date.year();
	sytemTime.wMonth		= date.month();
	sytemTime.wDayOfWeek	= 0;	
	sytemTime.wDay			= date.day();
	
	FILETIME fileTime;
	bool err = SystemTimeToFileTime(&sytemTime,&fileTime);
	err = SetFileTime(h,&fileTime,&fileTime,&fileTime);
#if 0
    int lasterr = GetLastError();
#endif // 0
	CloseHandle(h);
	delete[] wch;
}

bool LogPacker::ZipOpenAdd( ZIPFile &zipFile,QString ZipFileName,QFileInfo f_inf )
{
	if (!zipFile.zip_handle)
	{
		zipFile.dstFileName = ZipFileName;
		if (QFile::exists(ZipFileName))
		{
			zipFile.need_merge = true;
			ZipFileName += ".temp";
			zipFile.srcFileName = ZipFileName;
		}
				
		wchar_t *wch = new wchar_t[ZipFileName.size() + 1];
		memset(wch,0,	(ZipFileName.size()+1) *2);
		ZipFileName.toWCharArray(wch);
		zipFile.zip_handle = CreateZip(wch, 0, ZIP_FILENAME);
		delete[] wch;
		if (!zipFile.zip_handle) {zipFile = ZIPFile(); return false;}
	}
	else
	{
		if (zipFile.PossibleMerge()) ZipFileName = zipFile.srcFileName;
		else ZipFileName = zipFile.dstFileName;
	}

	//����� ZipFileName ������������ ������ ��� ������������� �����
	if (f_inf.lastModified().date().daysTo(zipFile.lastFileDate) < 0) zipFile.lastFileDate = f_inf.lastModified().date();

	wchar_t * wch_local = new wchar_t[f_inf.fileName().size() + 1];
	memset(wch_local,0,	(f_inf.fileName().size()+1) *2);
	f_inf.fileName().toWCharArray(wch_local);

	wchar_t * wch_file = new wchar_t[f_inf.absoluteFilePath().size() + 1];
	memset(wch_file,0,	(f_inf.absoluteFilePath().size()+1) *2);
	f_inf.absoluteFilePath().toWCharArray(wch_file);

	if (QFile::exists( f_inf.absoluteFilePath() ))
	{
		LTRACE_S("�������� ����� "+f_inf.absoluteFilePath()+" � "+ZipFileName);
		ZipAdd(zipFile.zip_handle, wch_local, wch_file, 0, ZIP_FILENAME);
		QFile::remove( f_inf.absoluteFilePath() );
	}
	
	delete[] wch_local;
	delete[] wch_file;

	return true;
}

bool LogPacker::IsFileOlderThen2Month( QFileInfo f_inf )
{
	QDateTime f_dt = f_inf.lastModified();
	QDateTime cur_dt = QDateTime::currentDateTime();
	int dif_y = cur_dt.date().year() - f_dt.date().year();
	if (dif_y > 1) return true;
	if (dif_y < 0) return false;
	int dif_m = cur_dt.date().month() - f_dt.date().month();
	if (dif_m > 2) return true;
	if ((dif_m < 0) && (dif_m > -10))	//-10 = 12+dif_m, ���� dif_m < 0
	{
		if (dif_y <= 0) return false;	//�������
		else return true;
	}
	if ((dif_m >= 0) && (dif_y > 0)) return true;
	return false;
}

bool LogPacker::IsFileOlderThen2Days( QFileInfo f_inf )
{
	QDateTime f_dt = f_inf.lastModified();
	int dif = f_dt.daysTo(QDateTime::currentDateTime());
	if (dif > 2) return true;
	return false;
}
