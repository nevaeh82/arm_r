#include "PanoramaFreqControl.h"

#define TIMEOUT 5

PanoramaFreqControl::PanoramaFreqControl(QObject *parent) :
	QObject(parent),
	m_time(QTime::currentTime())
{

}

void PanoramaFreqControl::initChannelCount(uint channelCount)
{
	m_channelCount = channelCount;
	for(int i = 0; i<channelCount; i++) {
		m_spectrumReadyMap.insert(i, false);
	}
}

void PanoramaFreqControl::init(double startFreq, double endFreq)
{
	m_start = startFreq;
	m_end = endFreq;
	m_current = startFreq;

	clearMap();
	setFreqCommand( m_current );
}

void PanoramaFreqControl::setChannelReady(int channel)
{
	m_spectrumReadyMap.insert(channel, true);

	int tmp = m_time.secsTo( QTime::currentTime() );

	if( !m_spectrumReadyMap.values().contains(false) ) {
		changeFreq();
	} else if( m_spectrumReadyMap.values().contains(true) &&
			   m_time.secsTo( QTime::currentTime() ) > TIMEOUT ) {
		changeFreq();
	}
}

void PanoramaFreqControl::changeFreq()
{
	if(m_current >= m_end) {
			m_current = m_start;
	} else {
		m_current += freqStep;
	}

	clearMap();
	setFreqCommand( m_current );
}

void PanoramaFreqControl::clearMap()
{
	foreach (int key, m_spectrumReadyMap.keys()) {
		m_spectrumReadyMap.insert(key, false);
	}
}

void PanoramaFreqControl::setFreqCommand(int val)
{
	m_time = QTime::currentTime();
	emit setCommonFreq( val );
}
