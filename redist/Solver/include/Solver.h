#ifndef SOLVERGENERAL_H
#define SOLVERGENERAL_H



///////////////////////////////   S O L V E R   ///////////////////////////////

#include "ISolver.h"
#include "ISolverListener.h"
#include "solver_global.h"
#include <tuple>
#include <set>

/* �������������� ���������� ������� */
class SolverImpl;
 
/**
 * @class Solver
 * ����� �������� ������ ���������������� �� ���������-������������� ������
 * @note ��������� �� ���� ������ ���� DataFromFlacon ����� ���� GetData
 * @note ������ ���������� �������� � �� ��������� �������� 
		 signal_sendDataFromRadioLocation, signal_sendDataFromRadioLocationManualHeigh,
		 signal_sendOneDataFromRadioLocation � signal_sendHyperbolesFromRadioLocation
 * @note ��, ����� ������� ����� ������������ �������� �������� � ������� SolverType 
 */
class SOLVERSHARED_EXPORT Solver : public QObject, public ISolver
{
	Q_OBJECT
public:

	/**
	 * ����������� �� ���������
	 * @note ��������� ������ ������
	 */
	Solver(QObject* parent = 0);

	/**
	 * ������ ���� ���������������
	 * @param bottomLeft - ����� ������ ����� ���� �������������� [Lat, Lon]
	 * @param topRight - ������ ������� ����� ���� �������������� [Lat, Lon]
	 * @param H - ����������� ��������� ������ ���� ��� ������� ���� [�]
	 */
    virtual void SetAreaOfResponsibility(const QPointF& bottomLeft,
										 const QPointF& topRight,
										 const double H=9000.0);

	/* �������� �����-���� ��� �������� */
	virtual void AddSolverType(const SolverType& type);

	/* ������ �����-���� ��������� ��� �������� */
	virtual void RemoveSolverType(const SolverType& type);

	/**
	 * ������ ����� �������� ������
	 * @param length - ������������ ������ ����������� � DataFromRadioLocation
	 */
    virtual void SetOutDataLength(const size_t length);

	/**
	 * ������ ������, �� ������� ������ ����
	 * @param heigh - ������� ��������� ������ ���� ��� ������� ���� [�]
	 */
	virtual void SetHeighApriori(const double heigh);

    /* �������� ������� ���������� ������� */
    virtual void Clear();
	
	/**
	 * �������� ������� ��������
	 * @param lat - ������ ������� 
	 * @param lon - ������� ������� 
	 * @param alt - ������ ������� 
	 * @note � ������ �������� ���������� -1
	 * @return id - ���������� ������������� ������� 
	 */
	virtual int AddStation(const double lat, const double lon, const double alt);
	
	/**
	 * ������� ������� ��������
	 * @return ������� ������ �������� 
	 */
	virtual bool RemoveStation(const int id) ;
	
	/**
	 * ���������������� ���������
	 * @param listener - ��������� 
	 * @note ������������ ����� ������������ ������ ���� ���������
	 * @return ������� ������ �������� 
	 */
	virtual bool RegisterListener(ISolverListener* listener);
	
	/**
	 * ���������������� ���������
	 * @param listener - ��������� 
	 * @return ������� ������ �������� 
	 */
	virtual bool DeregisterListener(ISolverListener* listener);

	/**
	 * �������� ������
	 * @param data - ������ � ���������� ����������
	 */
    virtual void GetData(const DataFromFlacon& data);

	/* ���������� */
	~Solver(){}

	/* ������� ��������� �� ���������� */
	const SolverImpl* GetImpl()const { return solverImpl_.get(); }

private:

	/* ���������� ������� */
	std::shared_ptr<SolverImpl> solverImpl_;

};

#endif // SOLVER_H
