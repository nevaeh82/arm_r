#ifndef RESOURCES_H
#define RESOURCES_H
#include <QString>
#include <QDir>
#include <QFile>
#include <QTextCodec>

//invalid maps
#define INVALID_INGIT_SAMPLE Resources::invalidIngitSample()
#define GOOGLECACHE_FAKEMOSCOW Resources::googleCacheFakeMoscow()

//good maps
#define INGIT_SAMPLE Resources::ingitSample()
#define INGIT_WORLD Resources::ingitWorld()
#define PANORAMA_SAMPLE Resources::panoramaSample()
#define PANORAMASXF_SAMPLE Resources::panoramaSxfSample()
#define PANORAMASIT_SAMPLE Resources::panoramaSitSample()
#define MAPX_SPB_GK42 Resources::mapxSPbGK42()
#define MAPX_WORLDMERCATOR41001 Resources::mapxWorldMercator41001()
#define MAPX_UNDEFINEDELLIPSOID Resources::mapxUndefinedEllipsoid()
#define KML_SAMPLE Resources::kmlSample()
#define GOOGLECACHE_WORLD Resources::googleCacheWorld()
#define GOOGLECACHE_MOSCOW910 Resources::googleCacheMoscow910()

#define DEFAULT_MAPREGISTRY_FILE Resources::defaultMapRegistryFile()

#define MapSamplesDir SolutionDir "/MapSamples"

class Resources
{
public:

	//invalid maps
	static QString invalidIngitSample()
	{
		return QDir::toNativeSeparators( MapSamplesDir "/FakeMaps/Ingit/fakeAbakan/ABAKAN.chart" );
	}

	static QString panoramaUndefinedEpplisoidKind()
	{
		return QDir::toNativeSeparators( MapSamplesDir "/Panorama/sxf-sit/RU-BRY.sxf" );
	}

	static QString mapxUndefinedEllipsoid()
	{
		return QDir::toNativeSeparators( MapSamplesDir "/FakeMaps/MapX/VladivostokUndef/Vladivostok.gst" );
	}

	static QString googleCacheFakeMoscow()
	{
		return QDir::toNativeSeparators( MapSamplesDir "/FakeMaps/GoogleCache/fakeMoscow/info.gc" );
	}

	//good maps
	static QString ingitSample()
	{
	  return QDir::toNativeSeparators( MapSamplesDir "/Ingit/abakan/ABAKAN.chart" );
	}

	static QString ingitWorld()
	{
		return QDir::toNativeSeparators( MapSamplesDir "/Ingit/world/World.chart");
	}

	static QString panoramaSample()
	{
#ifdef 	WIN32
		return QDir::toNativeSeparators( MapSamplesDir "/Panorama/podolsk_win32/Podolsk.map" );
#else
		return QDir::toNativeSeparators( MapSamplesDir "/Panorama/podolsk_linux/Podolsk.map" );
#endif
	}

	static QString panoramaSxfSample()
	{
		return QDir::toNativeSeparators( MapSamplesDir "/Panorama/voronej/M3703.SXF" );
	}

	static QString panoramaSitSample()
	{
		return QDir::toNativeSeparators( MapSamplesDir "/Panorama/moscow/moskow.sit" );
	}

	static QString mapxSPbGK42()
	{
		return QDir::toNativeSeparators( MapSamplesDir "/MapX/SaintPetersburgGK42/SaintPetersburgGK42.gst" );
	}

	static QString mapxWorldMercator41001()
	{
		return QDir::toNativeSeparators( MapSamplesDir "/MapX/WorldMercator41001/WorldMercator41001.gst" );
	}

	static QString kmlSample()
	{
		return QDir::toNativeSeparators(MapSamplesDir "/Kml/spb.kml");
	}

	static QString kmlQuasiWorld()
	{
		return QDir::toNativeSeparators( MapSamplesDir "/Kml/quasiWorld.kml" );
	}

	static QString googleCacheWorld()
	{
		return QDir::toNativeSeparators( MapSamplesDir "/GoogleCache/world/info.gc" );
	}

	static QString googleCacheMoscow910()
	{
		return QDir::toNativeSeparators( MapSamplesDir "/GoogleCache/Moscow910/info.gc" );
	}

	static QString varDir()
	{
		QString result = QDir::toNativeSeparators( SolutionDir "/build/var" );
		QDir().mkpath( result );
		return result;
	}
	static QString defaultMapRegistryFile()
	{
		return QDir::toNativeSeparators( varDir() + "/testGisServersManager.csv");
	}

	static QString createMapRegistryFile()
	{
		QFile file(Resources::defaultMapRegistryFile());
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("cp1251"));
		file.open(QIODevice::Text|QIODevice::WriteOnly);
		file.write(QString::fromAscii(
			"1;Ингит. Абакан;" MapSamplesDir "/Ingit/abakan/ABAKAN.chart\n"
			"2;Ингит. Россия;" MapSamplesDir "/Ingit/russia/russia.chart\n"
			"3;Панорама. Подольск. win32;" MapSamplesDir "/Panorama/podolsk/podolsk.map\n"
			"4;MapX. Санкт-Петербург;" MapSamplesDir "/MapX/SaintPetersburgGK42/SaintPetersburgGK42.gst\n"
			"5;Kml. Санкт-Петербург;" MapSamplesDir "/Kml/spb.kml\n"
		).toAscii().data());

		file.close();
		return Resources::defaultMapRegistryFile();
	}
};

#endif // RESOURCES_H
