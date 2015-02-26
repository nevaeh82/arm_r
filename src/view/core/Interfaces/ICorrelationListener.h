#ifndef ICORRELATIONLISTENER_H
#define ICORRELATIONLISTENER_H

class ICorrelationListener
{
	public:
	virtual ~ICorrelationListener(){}

	virtual void onCorrelationStateChanged( const bool isEnabled ) = 0;
};

#endif // ICORRELATIONLISTENER_H
