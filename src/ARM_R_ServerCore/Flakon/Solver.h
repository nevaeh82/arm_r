#ifndef SOLVERGENERAL_H
#define SOLVERGENERAL_H



///////////////////////////////   S O L V E R   ///////////////////////////////

#include "RadiolocationInterface.h"
#include "solver_global.h"
#include <tuple>
#include <set>

/* �������������� ���������� ������� */
class SolverImpl;

/**
 * @enum SolverType
 * ��������� ���� ��������
 * @param AUTO_HEIGH - �������� ����������� ���������� � �������������� ������������
					   ������, ���������� ������� signal_sendDataFromRadioLocation
 * @param MANUAL_HEIGH - �������� ����������� ���������� � ������ ������������ ������,
                         ���������� ������� signal_sendDataFromRadioLocationManualHeigh
 * @param ONE_DATA - �������� ����������� ������������ ������� � ������������, 
                     ���������� ������� signal_sendOneDataFromRadioLocation
 */
enum SolverType { AUTO_HEIGH, MANUAL_HEIGH, ONE_DATA };


/**
 *@class Solver
 * ����� �������� ������ ���������������� �� ���������-������������� ������
 * @note ��������� �� ���� ������ ���� DataFromFlacon ����� ���� GetData
 * @note ������ ���������� �������� � �� ��������� �������� 
		 signal_sendDataFromRadioLocation, signal_sendDataFromRadioLocationManualHeigh �
		 signal_sendOneDataFromRadioLocation
 * @note ��, ����� ������� ����� ������������ �������� �������� � ������� SolverType 
 */
class SOLVERSHARED_EXPORT Solver : public QObject
{
    Q_OBJECT
public:

	/**
	 * ����������� �� ���������
	 * @note ������������ ��� ��������� ���� ���������
	 */
	Solver();
	/**
	 * �����������
	 * @param types - ������ ����� ���������, ������� ����� ��������������
	 */
	Solver(const std::set<SolverType>& types);

	/**
	 * ������ ���� ���������������
	 * @param bottomLeft - ����� ������ ����� ���� �������������� [Lat, Lon]
	 * @param topRight - ������ ������� ����� ���� �������������� [Lat, Lon]
	 * @param H - ����������� ��������� ������ ���� ��� ������� ���� [�]
	 */
    void SetAreaOfResponsibility(const QPointF& bottomLeft,
                                 const QPointF& topRight,const double H=9000.0);

	/* �������� �����-���� ��� �������� */
	void AddSolverType(const SolverType& type);

	/* ������ �����-���� ��������� ��� �������� */
	void RemoveSolverType(const SolverType& type);

	/**
	 * ������ ����� �������� ������
	 * @param length - ������������ ������ ����������� � DataFromRadioLocation
	 */
    void SetOutDataLength(const size_t length);

	/**
	 * ������ ������, �� ������� ������ ����
	 * @param heigh - ������� ��������� ������ ���� ��� ������� ���� [�]
	 */
    void SetHeighApriori(const double heigh);

    /* �������� ������� ���������� ������� */
    void Clear();

	/* ���������� */
	~Solver(){}
private:
	/* ���������������� ����� */
	void Init();

public slots:

	/**
	 * �������� ������
	 * @param data - ������ � ���������� ����������
	 */
    void GetData(const DataFromFlacon& data);

signals:

	/* �������� ������������ ������ ���������� � �������������� ������������ ������ */
    void signal_sendDataFromRadioLocation(const DataFromRadioLocation& allData);

    /* �������� ������������ ������ ���������� � �������������� ������������ ������ */
    void signal_sendDataFromRadioLocationManualHeigh(const DataFromRadioLocation& allData);

	/* �������� ����� ������� ������������ ������ */
    void signal_sendOneDataFromRadioLocation(const OneDataFromRadioLocation& oneData);

private:
	/* ���������� ������� */
	std::shared_ptr<SolverImpl> solverImpl_;

};

#endif // SOLVER_H
