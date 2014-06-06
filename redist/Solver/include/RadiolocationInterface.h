#ifndef RADIOLOCATIONINTERFACE_H
#define RADIOLOCATIONINTERFACE_H

#include <QTime>
#include <QVector>

//��������� ��������� �������� ��������: �����, �����, ����������
enum State {FLY,STAND,UNKNOWN};

//�������������� �������� ���������� ������: ������� ��� ������
enum Quality {GOOD,BAD};

///////////////   D A T A  F R O M  R A D I O L O C A T I O N   ///////////////

//��������� ��� �������� ������
struct DataFromRadioLocation {
    //������ ���������
    QVector<QPointF> coordLatLon;
    //������ �����
    QVector<double> heigh;
    //������ ��� ���������
    QVector<QPointF> latLonStdDev;
    //������ ��� ������
    QVector<double> heighStdDev;
    //������ ������� ��������� ���������
    QVector<QTime> timeHMSMs;
    //������ �������
    QVector<double> airspeed;
    //������ ��� ��������
    QVector<double> airSpeedStdDev;
    //������ �������� ������
    QVector<double> relativeBearing;
    //������ ��������� ��������
    QVector<State> StateMassive_;
	//������ ����������� �������� ���������� ������
	QVector<Quality> qualityMassive_;
};


///////////////   D A T A  F R O M  R A D I O L O C A T I O N   ///////////////

//��������� ��� �������� ������ ������� ����� ������ ������
struct OneDataFromRadioLocation {
    //���������� [Lat; Lon], ����
    QPointF coordLatLon;
    //������ ��� ������� ����, �
    double heigh;
    //������������������ ���������� �� ������ � ������� ��������������, [�; �]
    QPointF latLonStdDev;
    //������������������ ���������� �� ������, �
    double heighStdDev;
    //����� ��������� ���������
    QTime timeHMSMs;
};


//////////////////////   D A T A  F R O M  F L A C O N   //////////////////////

//���������� ������
struct DataFromFlacon {
	//������ ��������� ����������
	QVector<double> ranges_;
	//����� �������� �������
	int numOfReferenceDetector_;
	//����� ��������� ���������
	QTime time_;
};

////////   H Y P E R B O L E S   F R O M   R A D I O L O C A T I O N   ////////

//��������� ��� �������� ������ ������� ��������
struct HyperbolesFromRadioLocation {
	//������ ��������, ������� ������������ ��������� ����� [Lat,Lon]
	QList<QVector<QPointF> > hyperboles_list;
	//����� ��������� ���������, ��� ������� ���������� ���������
	QTime timeHMSMs;
};


/**
 * @enum ErrorType
 * ��������� ���� ������ ��������
 * @param SETUP_ERROR - ������, ��������� ��� ��������� �������
 * @param INPUT_DATA_ERROR - ������, ��������� ��� ��������� �������
                             ���������� ������� ������
 * @param INTERNAL_ERROR - ���������� ������ �������
 */
enum ErrorType { SETUP_ERROR, INPUT_DATA_ERROR, INTERNAL_ERROR };

/**
 * @enum SolveResult
 * ��������� ������� 
 * @param SOLVED - ������ ������������ ���������� 
 * @param ERROR_OCCURED - � ���� ���������� ��������� ������
 * @param NOT_ENOUGH_DATA - � ������ ��������� ������������ ������ ��� ������� �� ��������
 * @param THERE_IS_NO_SOLUTION - ������� �� ����������
 * @param CANT_DETERMINE_REAL_TRAJECTORY - ������ �� ����� ���������� �������� ���������� �� ���� ���������
 * @param TOO_FEW_RANGES - ������� ���� ���������� ��������� ���������� ��� ������� 
 * @param TOO_LOW_SOLUTION_ACCURACY - ������� ������ �������� ����������� �������
 * @param TOO_LOW_DATA_ACCURACY - ������� ������ �������� ������� ������
 * @param COORDS_DOES_NOT_HIT_IN_AREA - ������������ ���������� �� �������� � ���� ���������������
 * @param COORDS_DOES_NOT_HIT_IN_STROB -  ������������ ���������� �� �������� � ����� �������
 */
enum SolveResult { SOLVED, ERROR_OCCURED, NOT_ENOUGH_DATA, THERE_IS_NO_SOLUTION,
				   CANT_DETERMINE_REAL_TRAJECTORY, TOO_LOW_DATA_ACCURACY,
	               TOO_FEW_RANGES, TOO_LOW_SOLUTION_ACCURACY, 
				   COORDS_DOES_NOT_HIT_IN_AREA, COORDS_DOES_NOT_HIT_IN_STROB };


#endif // RADIOLOCATIONINTERFACE_H
