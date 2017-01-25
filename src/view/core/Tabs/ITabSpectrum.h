#ifndef ITABSPECTRUM_H
#define ITABSPECTRUM_H

#include <QVector>
#include <QMap>
#include <QVariant>
#include <QLabel>

#include "Interfaces/IStation.h"
#include "Common/IMessage.h"
#include "Defines.h"

class ITabSpectrum
{
public:
	virtual ~ITabSpectrum(){}

	virtual void setSelectedArea(const SpectrumSelection& selection) = 0; //1 - selection, 2 - central freq, 3 - begin freq, 4 - end freq
	virtual void sendCommand(TypeCommand type, IMessage *msg) = 0;
	virtual void setPointsRpc(QVector<QPointF> points) = 0;
	virtual void setThreshold(double y) = 0;
	virtual void setIndicator(int state) = 0;
	virtual void setShowControlPrm(bool state) = 0;
	virtual void setDoubleClicked(int id, double, double) = 0;
	virtual void setPanorama(bool state) = 0;
	virtual void enableCorrelation(bool enable = true) = 0;

	virtual void recognize() = 0;

	virtual double getCurrentFrequency() = 0;

    virtual void updateDbStationsLists() = 0;
};

#endif // ITABSPECTRUM_H
