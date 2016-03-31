/* Generated file, do not edit. Do not commit under VCS. */

#include <cxxmock/cxxmock.h>

#ifndef __mocks_85628__generated
#define __mocks_85628__generated
#include "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/CISCommonLib/src/Rpc/RpcPeerAbstract.h"

class CXXMOCK_DECL( RpcPeerAbstract )
{
	virtual bool isServer() const
	{
		return CXXMOCK_GET( bool );
	}
	virtual bool isClient() const
	{
		return CXXMOCK_GET( bool );
	}
	virtual void connect(QHostAddress addr, int port = 80)
	{
		CXXMOCK_SET( addr, port );
	}
	virtual void connect(QString addr, int port = 80)
	{
		CXXMOCK_SET( addr, port );
	}
	virtual bool listen(QHostAddress iface = QHostAddress::Any, int port = 80)
	{
		return CXXMOCK( bool, iface, port );
	}
	virtual void stopListening()
	{
		CXXMOCK_GET( int );
	}
	virtual QList<quint64> clients() const
	{
		return CXXMOCK_GET( QList<quint64> );
	}
	virtual QIODevice* device() const
	{
		return CXXMOCK_GET( QIODevice* );
	}
	virtual void setDevice(QIODevice* dev)
	{
		CXXMOCK_SET( dev );
	}
	virtual QIODevice* takeDevice()
	{
		return CXXMOCK_GET( QIODevice* );
	}
	virtual QxtAbstractSignalSerializer* serializer() const
	{
		return CXXMOCK_GET( QxtAbstractSignalSerializer* );
	}
	virtual void setSerializer(QxtAbstractSignalSerializer* serializer)
	{
		CXXMOCK_SET( serializer );
	}
	virtual QxtAbstractConnectionManager* connectionManager() const
	{
		return CXXMOCK_GET( QxtAbstractConnectionManager* );
	}
	virtual void setConnectionManager(QxtAbstractConnectionManager* manager)
	{
		CXXMOCK_SET( manager );
	}
	virtual bool attachSignal(QObject* sender, const char* signal, const QString& rpcFunction = QString())
	{
		return CXXMOCK( bool, sender, signal, rpcFunction );
	}
	virtual bool attachSlot(const QString& rpcFunction, QObject* recv, const char* slot, Qt::ConnectionType type = Qt::AutoConnection)
	{
		return CXXMOCK( bool, rpcFunction, recv, slot, type );
	}
	virtual void detachSignals(QObject* obj)
	{
		CXXMOCK_SET( obj );
	}
	virtual void detachSlots(QObject* obj)
	{
		CXXMOCK_SET( obj );
	}
	virtual void detachObject(QObject* obj)
	{
		CXXMOCK_SET( obj );
	}
	virtual void disconnectClient(quint64 id)
	{
		CXXMOCK_SET( id );
	}
	virtual void disconnectServer()
	{
		CXXMOCK_GET( int );
	}
	virtual void disconnectAll()
	{
		CXXMOCK_GET( int );
	}
	virtual void call(QString fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(), const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant())
	{
		CXXMOCK_SET( fn, p1, p2, p3, p4, p5, p6, p7, p8 );
	}
	virtual void call(QList<quint64> ids, QString fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(), const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant())
	{
		CXXMOCK_SET( ids, fn, p1, p2, p3, p4, p5, p6, p7, p8 );
	}
	virtual void call(quint64 id, QString fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(), const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant())
	{
		CXXMOCK_SET( id, fn, p1, p2, p3, p4, p5, p6, p7, p8 );
	}
	virtual void callExcept(quint64 id, QString fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(), const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant())
	{
		CXXMOCK_SET( id, fn, p1, p2, p3, p4, p5, p6, p7, p8 );
	}
	virtual void detachSender()
	{
		CXXMOCK_GET( int );
	}
};
CXXMOCK_IMPL(RpcPeerAbstract);



#include "F:/Projects/Qt/Zaviruha2/zaviruha_r/src/CISCommonLib/src/Interfaces/IRpcListener.h"

class CXXMOCK_DECL( IRpcListener )
{
	virtual void onMethodCalled(const QString& method, const QVariant& argument)
	{
		CXXMOCK_SET( method, argument );
	}
};
CXXMOCK_IMPL(IRpcListener);



#endif //__mocks_85628__generated
