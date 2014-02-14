#ifndef ISPECTRUMWIDGET_H
#define ISPECTRUMWIDGET_H

class QString;
class ITabSpectrum;

class ISpectrumWidget
{
	public:
	virtual ~ISpectrumWidget(){}

	virtual void setTab(ITabSpectrum*) = 0;
	virtual void setId(const int) = 0;
	virtual void setSpectrumName(const QString&) = 0;

	virtual QString getSpectrumName() const = 0;
};

#endif // ISPECTRUMWIDGET_H
