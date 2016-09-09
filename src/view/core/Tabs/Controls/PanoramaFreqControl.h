#pragma once

#include <QObject>
#include <QMap>
#include <QTime>

#define freqStep 20

class PanoramaFreqControl : public QObject
{
	Q_OBJECT

public:
	PanoramaFreqControl(QObject* parent = 0);
	~PanoramaFreqControl() {}

	void initChannelCount(uint channelCount);
	void init(double startFreq, double endFreq);
	void setChannelReady(int channel);

private:
	uint m_channelCount;
	double  m_start;
	double m_end;
	double m_current;
	QTime m_time;

	QMap<uint, bool> m_spectrumReadyMap;

	void changeFreq();

	void clearMap();

	void setFreqCommand(int);

signals:
	void setCommonFreq(int);
};
