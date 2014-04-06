#ifndef IRPCCONFIGCLIENT_H
#define IRPCCONFIGCLIENT_H

class QString;

class IRpcConfigClient
{

public:
	virtual ~IRpcConfigClient(){}

	virtual void requestGetStationList(const QString& filename);
	virtual void requestGetAtlantConfiguration(const QString& filename);
};

#endif // IRPCCONFIGCLIENT_H
