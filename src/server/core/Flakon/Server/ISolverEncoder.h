#pragma once

#include <QByteArray>
#include "MessageSP.h"

class ISolverEncoder
{
public:
	virtual ~ISolverEncoder(){}

	virtual QByteArray encode( const QByteArray& data ) = 0;
	virtual QByteArray decode(const MessageSP message) = 0;
};

