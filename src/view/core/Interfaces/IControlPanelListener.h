#ifndef ICONTROLPANELLISTENER_H
#define ICONTROLPANELLISTENER_H

class IControlPanelListener
{
public:
	virtual ~IControlPanelListener(){}

	virtual void onGlobalAutoSearchEnabled(const bool isEnabled) = 0;
	virtual void onGlobalPanoramaEnabled(const bool isEnabled) = 0;
};

#endif // ICONTROLPANELLISTENER_H
