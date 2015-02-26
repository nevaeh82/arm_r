//---------------------------------------------------------------------------

#ifndef nodb_exportsH
#define nodb_exportsH

#include <STDDEF.H> // ��� size_t

//������ �� ��������������� __declspec(dllexport) � __declspec(dllimport)
#ifdef NODB_DLL
#define _NODB_FUNC __declspec(dllexport) _stdcall
#else
#define _NODB_FUNC __declspec(dllimport) _stdcall
#endif

//��������� �������� ��� ����� ������
typedef int 		nodb_result;
typedef wchar_t 	nodb_varchar;
typedef wchar_t* 	nodb_pvarchar;
typedef bool 		nodb_bool;
typedef double 		nodb_double;
typedef double 		nodb_timestamp;
typedef __int16 	nodb_smallint;
typedef int		nodb_int;
typedef __int64 	nodb_int64;
typedef __int64		nodb_time_t;
typedef unsigned char   nodb_byte;
typedef unsigned __int64    nodb_uint64;

enum nodb_dict_type { nodb_maxvalue = 0xffffffff, nodb_apointment = 0, nodb_current_state, nodb_modulation, nodb_multiplexer, nodb_multiservice_access,
     nodb_standard, nodb_noise_resistant_code, nodb_polarization, nodb_processing_status, nodb_scrambler };

enum nodb_table_type { nodb_maxtable = 0xffffffff, nodb_object_table = 0, nodb_source_table, nodb_measure_table, nodb_devicestate_table, nodb_device_table };

//�������� nodb_result:
#define NODB_SQL_ERROR 0
#define NODB_OK 1
#define NODB_NOT_FOUND_ERROR 2
#define NODB_SQL_RETURN_ERROR 3
#define NODB_PARAM_ERROR 4
#define NODB_FUNCTION_PARAMETER_ERROR 5
#define NODB_SQL_EXEC_ERROR 6
#define NODB_CONNECT_ERROR 7
#define NODB_REPEAT_CONNECT_ERROR 8
#define NODB_INIT_ERROR 9
#define NODB_DISCONNECT_ERROR 10
#define NODB_START_TRANSACTION_ERROR 11
#define NODB_TRANSACTION_NUMBER_ERROR 12
#define NODB_NOT_CONNECTED_ERROR 13
#define NODB_COMMIT_TRANSACTION_ERROR 14
#define NODB_ROLLBACK_TRANSACTION_ERROR 15
#define NODB_CREATE_BUFFER_FILE_ERROR 16
#define NODB_DBFORMAT_ERROR 17
#define NODB_MAX_ERROR (NODB_DBFORMAT_ERROR) // ������ ����� ������������� �������� ���� ������ (��� �������� ������������ � ����)

//==============================================================================

//���������������� �������� ��� ������������
//������ ��������� �������
#define nodb_processing_status_unknown 0     //�� ����������
#define nodb_processing_status_new 1     //����� ��������
#define nodb_processing_status_interest 2     //������� � ���������
#define nodb_processing_status_proscription 3     //������ ���������
#define nodb_processing_status_termination 4     //���������� ���������
//������� ��������� �������
#define nodb_current_state_unknown 0    //�� ����������
#define nodb_current_state_active 1    //��������
#define nodb_current_state_reserve 2    //� �������
#define nodb_current_state_out_of_service 3    //������� �� ������������
//���������� �������
#define nodb_apointment_unknown 0	//�� ����������
#define nodb_apointment_military 1	//�������
#define nodb_apointment_commercial 2	//������������
#define nodb_apointment_double 3	//�������� ����������
//��� ����������������� �������
#define nodb_multiservice_access_unknown 0   //�� ����������
//��� �����������
#define nodb_polarization_unknown 0	//�� ����������
//��� ���������
#define nodb_modulation_unknown 0	//�� ����������
//���������������� ���
#define nodb_noise_combating_code_unknown 0	//�� ����������
//�������������������
#define nodb_multiplexer_unknown 0	//�� ����������
//���������������
#define nodb_scrambler_unknown 0	//�� ����������
//��� �������
#define nodb_object_type_unknown 0           //�� ����������
#define nodb_object_type_aircraft 1           //�� ����������
#define nodb_object_type_basestation 2           //�� ����������
#define nodb_object_type_satellite 3           //�� ����������
//��� ���������
#define nodb_measure_type_unknown 0          //�� ����������
#define nodb_measure_type_baring 4          //�� ����������
#define nodb_measure_type_fixing 5          //�� ����������
//��������� �����
#define nodb_source_any_standard 0      //��� ���������

//���������������� �������� ��� ��������
#define nodb_object_without_source_id	0	//� ������� ����� � ���������� ���

//==============================================================================
//���������

//��������� ��� BLOB - �����
#pragma pack(push,1) //bytealignment
struct t_nodb_blob
{
	size_t size;                                    //������
	nodb_byte * data;                                    //�����
};
#pragma pack(pop)

//��������� ��������� ��� ������� "������� ��"
#pragma pack(push,1) //bytealignment
struct t_nodb_object {

const static size_t NAME_LEN = 64;             		//������������ ����� ������������ ������� ��
const static size_t OWNER_LEN = 64;             	//������������ ����� �������������� ������� ��
const static size_t COMMENTS_LEN = 255;         	//������������ ����� ����������� � ������� ��

	nodb_int id;                			//*id!
	nodb_int type_code;               		//��� ������� (��� ������� � �� ����������� �������������, ���������� 0)
	nodb_varchar name[NAME_LEN];             	// "������������ ������� ��"
	nodb_varchar owner[OWNER_LEN];           	// "�������������� ������� ��"
	nodb_timestamp dat;                             // ���� �����������
	nodb_timestamp startup_dat;                     // ���� ����� � ������������
        nodb_int apointment;                            //���������� (���� ����������, �� ���������� 0. ��������� ��������: 0 - ��������������, 1 - �������, 2 - ������������, 3 - �������)
        nodb_int current_state;                         //������� ��������� (���� ����������, �� ���������� 0. ��������� ��������: 0 - ��������������, 1 - ��������, 2 - � �������, 3 - ������� �� ������������)
        t_nodb_blob photo;                              //����
	nodb_varchar comments[COMMENTS_LEN];     	// �����������
        nodb_int source_id;				//����� � ���������� ������� (���� 0, ������ ����� ���)
};
#pragma pack(pop)

#define nodb_update_mask_object(id,tc,nm,ow,dt,sd,ap,cs,ph,cm,sid) \
	(id ? (1LL<<0) : 0) | \
	(tc ? (1LL<<1) : 0) | \
	(nm ? (1LL<<2) : 0) | \
	(ow ? (1LL<<3) : 0) | \
	(dt ? (1LL<<4) : 0) | \
	(sd ? (1LL<<5) : 0) | \
	(ap ? (1LL<<6) : 0) | \
	(cs ? (1LL<<7) : 0) | \
	(ph ? (1LL<<8) : 0) | \
	(cm ? (1LL<<9) : 0) | \
	(sid? (1LL<<10): 0)

//��������� ��������� ��� ������� "������� �������"
#pragma pack(push,1) //bytealignment
struct t_nodb_basestation {

const static size_t ADDRESS_LEN = 255;    		//������������ ����� ������
const static size_t BS_ID_STRING_LEN = 64;  		//������������ �����  ������������ ���� ��� "�������" ������� �������
const static size_t TYPE_LEN = 16;  			//������������ ����� ����� ���������� ������������ ���������� �������� ���������
	t_nodb_object object;                           //��������� �������
	nodb_double latitude;                           //������
	nodb_double longitude;                          //�������
        nodb_int elevation;                             //����������
	nodb_varchar address[ADDRESS_LEN];    		//�����
	nodb_bool activated;                            //������� ����������
	nodb_int64 zone_id;    				//�������� ������������� ��������������� ����
        nodb_varchar zone_id_type[TYPE_LEN];            //GSM => LAC, CDMA => SID
	nodb_int64 bs_id;    				//�������� ������������� ������� �������
        nodb_varchar bs_id_type[TYPE_LEN];              //GSM => CID, DECT => RFPI, WIFI => MAC, CDMA => BASEID
	nodb_varchar bs_id_string[BS_ID_STRING_LEN];    //��������� ������������� ������� �������
        nodb_varchar bs_id_string_type[TYPE_LEN];       //
	nodb_int64 sectornum;				//����� �������
        nodb_varchar sectornum_type[TYPE_LEN];          //
        nodb_int64 bs_number;				//����� ������� �������
        nodb_varchar bs_number_type[TYPE_LEN];          //
};
#pragma pack(pop)

#define nodb_update_mask_basestation(la,lo,al,ad,ac,zi,zt,bi,bit,bis,bist,sn,snt,bn,bnt) \
	(la  ? (1LL<<11) : 0) | \
	(lo  ? (1LL<<12) : 0) | \
	(al  ? (1LL<<13) : 0) | \
	(ad  ? (1LL<<14) : 0) | \
	(ac  ? (1LL<<15) : 0) | \
	(zi  ? (1LL<<16) : 0) | \
	(zt  ? (1LL<<17) : 0) | \
	(bi  ? (1LL<<18) : 0) | \
	(bit ? (1LL<<19) : 0) | \
	(bis ? (1LL<<20) : 0) | \
	(bist? (1LL<<21) : 0) | \
	(sn  ? (1LL<<22) : 0) | \
	(snt ? (1LL<<23) : 0) | \
	(bn  ? (1LL<<24) : 0) | \
	(bnt ? (1LL<<25) : 0)

//��������� ��������� ��� ������� "�����-��������� ��������"
#pragma pack(push,1) //bytealignment
struct t_nodb_aircraft {

const static size_t MODEL_LEN = 64;           //������������ ����� ������ ����-���������� ��������
	t_nodb_object object;                           //��������� �������
	nodb_varchar model[MODEL_LEN];            //������ ����-���������� ��������
};
#pragma pack(pop) //bytealignment

#define nodb_update_mask_aircraft(mo) \
	(mo  ? (1LL<<11) : 0)

//��������� ��������� ��� ������� "��������"
#pragma pack(push,1) //bytealignment
struct t_nodb_satellite {
	const static size_t TLE_LEN = 165;             	//������������ ����� ����������� ������� ��������� TLE
        const static size_t BF_LEN = 16;             	//���-�� ������ ������

	t_nodb_object object;                           //��������� �������
        nodb_int norad_id;				//����� �������� �� ���� �����
	nodb_double major_semiaxis;                     //������� �������
	nodb_double eccentricity;			//��������������
	nodb_double inclination;                        //����������
	nodb_double longitude_asc_node;                 //������� ����������� ����
	nodb_double argu_of_periapsis;                  //�������� ����������
	nodb_double true_anomaly;                       //������� ��������
        nodb_varchar tle[TLE_LEN];                      //����������� ����� ��������� TLE
	nodb_double latitude;                           //������
	nodb_double longitude;                          //�������
        nodb_int elevation;                             //����������
        nodb_timestamp launch_date;                    //���� ������� �������������� �������� �����
	nodb_timestamp service_life;                    //���������������� ������
        nodb_int64 beacon_freq[BF_LEN];                 //������� ������
};
#pragma pack(pop)

#define nodb_update_mask_satellite(ni,ms,ec,in,lan,aop,ta,tle,la,lo,al,ld,sl,bf) \
	(ni  ? (1LL<<11) : 0) | \
	(ms  ? (1LL<<12) : 0) | \
	(ec  ? (1LL<<13) : 0) | \
	(in  ? (1LL<<14) : 0) | \
	(lan ? (1LL<<15) : 0) | \
	(aop ? (1LL<<16) : 0) | \
	(ta  ? (1LL<<17) : 0) | \
	(tle ? (1LL<<18) : 0) | \
	(la  ? (1LL<<19) : 0) | \
	(lo  ? (1LL<<20) : 0) | \
	(al  ? (1LL<<21) : 0) | \
	(ld  ? (1LL<<22) : 0) | \
	(sl  ? (1LL<<23) : 0) | \
	(bf  ? (1LL<<24) : 0)

//��������� ��������� ��� ������� "������"
#pragma pack(push,1) //bytealignment
struct t_nodb_reserved {
	const static size_t RESERV_LEN = 255;             	//������������ ����� ����������������� ��������� �����
        const static size_t CLASSIFICATOR_LEN = 64;
        const static size_t RECTYPE_LEN = 64;

        nodb_int64 id;                                          //������������� (��������� ����)
        nodb_int64 record_num;                      		//����� ������ �������
	nodb_varchar text1[RESERV_LEN];          		//����������������� ��������� ���� �1
	nodb_varchar text2[RESERV_LEN];          		//����������������� ��������� ���� �2
	nodb_varchar text3[RESERV_LEN];          		//����������������� ��������� ���� �3
	nodb_varchar text4[RESERV_LEN];          		//����������������� ��������� ���� �4
	nodb_int64 number1;              			//����������������� �������� ���� �1
	nodb_int64 number2;              			//����������������� �������� ���� �2
	nodb_int64 number3;              			//����������������� �������� ���� �3
	nodb_int64 number4;              			//����������������� �������� ���� �4
        nodb_varchar classificator[CLASSIFICATOR_LEN];                      //�������������
        nodb_varchar record_type[RECTYPE_LEN];                      //��� ��������� ������
};
#pragma pack(pop)

#define nodb_update_mask_reserved(id,rn,t1,t2,t3,t4,n1,n2,n3,n4,cl,rt) \
	(id ? (1LL<<0) : 0) | \
	(rn ? (1LL<<1) : 0) | \
	(t1 ? (1LL<<2) : 0) | \
	(t2 ? (1LL<<3) : 0) | \
	(t3 ? (1LL<<4) : 0) | \
	(t4 ? (1LL<<5) : 0) | \
	(n1 ? (1LL<<6) : 0) | \
	(n2 ? (1LL<<7) : 0) | \
	(n3 ? (1LL<<8) : 0) | \
	(n4 ? (1LL<<9): 0) | \
	(cl ? (1LL<<10): 0) | \
	(rt ? (1LL<<11): 0)

//��������� ��������� ��� ������� "��������� ��"
#pragma pack(push,1) //bytealignment
struct t_nodb_source {

const static size_t	STANDART_LEN = 30;          	//������������ ����� ��������� ����� ��������� ��"
const static size_t	NET_NAME_LEN = 64;          	//������������ ����� �������� ���� ��������� ��"
const static size_t	NAME_LEN = 128;             	//������������ ����� ������������ ��������� ��"
const static size_t	EQUIPMENT_LEN = 128;        	//������������ ����� ������������ ������������ ��������� ��"
const static size_t	EXTENDED_LEN = 255;	      	//������������ ����� ����������� ���������� ��� ��������� ��"

	nodb_int id;                			//*id!
	nodb_int standart;		     		// �������� ����� ��������� �� (����������� �����������)
	nodb_varchar net_name[NET_NAME_LEN];        	// �������� ���� ��������� ��
	nodb_varchar name[NAME_LEN];     	    	// ������������ ��������� ��
	nodb_varchar equipment[EQUIPMENT_LEN];		// ������������ ������������ ��������� ��
	nodb_int status;     				// ������ ��������� (���� ����������, �� ���������� 0. ��������� ��������: 0 - �� ����������, 1 - ����� ��������, 2 - ������� � ���������, 3 - ������ ���������, 4 - ���������� ���������)
	nodb_smallint priority;                         // ���������
	nodb_timestamp detection_date;                  // ���� �����������
	nodb_timestamp last_confirm_date;               // ���� ���������� �������������
	nodb_int64 frequency;                           // ������� �������
	nodb_int64 bandwidth;                           // ������ ������ ������� ��������� ��
	nodb_int polarisation_type;       		//��� ����������� (����������� �����������)
	nodb_double polarization_angle;                 //���� �����������
	nodb_double power;                              //�������� ���������
	nodb_double azimuth;                          //������
	nodb_double azimuth_accuracy;                          //�������� �������
        nodb_double place_angle;                           //���� �����
        nodb_int place_angle_accuracy;                  //�������� ���� �����
        nodb_int distance;                              //���������� �� ��������� ��
	nodb_smallint position_accuracy;                //�������� ��������������
	nodb_int64 height;                              //������ ������� �������
	nodb_int radius;                                //������ ��������
        nodb_int multiservice_access;                   //��� ����������������� ������� (����, ����, ����, ���, � �.�.) (���� ����������, �� ���������� 0)
	nodb_int modulation_type;         		//��� ��������� (����������� �����������)
	nodb_int64 modulation_speed;                    //������������� ��������
        nodb_int noise_combating_code;                  //���������������� ���	(����������� �����������)
        nodb_int scrambler;                             //���������������	(����������� �����������)
        nodb_int multiplexer;                           //������������������� (����������� �����������)
        t_nodb_blob extended_xml;                       //������������� xml ����
	nodb_varchar extended_info[EXTENDED_LEN];    	//����������� ����������
        nodb_int object_id;                             //������ �� ������ �������������
};
#pragma pack(pop)

#define nodb_update_mask_source(id,std,nn,nm,eq,st,pr,dd,lcd,fr,bw,pzt,pza,pw,az,aza,pla,plaa,ds,pa,he,ra,ma,mt,ms,ncc,sc,mux,xml,ei,oid) \
	(id  ? (1LL<<0)  : 0) | \
	(std ? (1LL<<1)  : 0) | \
	(nn  ? (1LL<<2)  : 0) | \
	(nm  ? (1LL<<3)  : 0) | \
	(eq  ? (1LL<<4)  : 0) | \
	(st  ? (1LL<<5)  : 0) | \
	(pr  ? (1LL<<6)  : 0) | \
	(dd  ? (1LL<<7)  : 0) | \
	(lcd ? (1LL<<8)  : 0) | \
	(fr  ? (1LL<<9)  : 0) | \
	(bw  ? (1LL<<10) : 0) | \
	(pzt ? (1LL<<11) : 0) | \
	(pza ? (1LL<<12) : 0) | \
	(pw  ? (1LL<<13) : 0) | \
	(az  ? (1LL<<14) : 0) | \
	(aza ? (1LL<<15) : 0) | \
	(pla ? (1LL<<16) : 0) | \
	(plaa? (1LL<<17) : 0) | \
	(ds  ? (1LL<<18) : 0) | \
	(pa  ? (1LL<<19) : 0) | \
	(he  ? (1LL<<20) : 0) | \
	(ra  ? (1LL<<21) : 0) | \
	(ma  ? (1LL<<22) : 0) | \
	(mt  ? (1LL<<23) : 0) | \
	(ms  ? (1LL<<24) : 0) | \
	(ncc ? (1LL<<25) : 0) | \
	(sc  ? (1LL<<26) : 0) | \
	(mux ? (1LL<<27) : 0) | \
	(xml ? (1LL<<28) : 0) | \
	(ei  ? (1LL<<29) : 0) | \
	(oid ? (1LL<<30) : 0)

//��������� ��������� ��� ������� "����������"
#pragma pack(push,1) //bytealignment
struct t_nodb_device {

const static size_t NAME_LEN = 64;           //������������ ����� ������������ �������������� ����������"
const static size_t COMMENTS_LEN = 255;      //������������ ����� ������������� ������

	nodb_int64 id;                			 //*id!
	nodb_varchar name[NAME_LEN];             // "������������"
	nodb_varchar mission[COMMENTS_LEN];     // ������������� ������
};
#pragma pack(pop)

#define nodb_update_mask_device(id,nm,cm) \
	(id ? 1LL : 0) | \
	(nm ? 2LL : 0) | \
	(cm ? 4LL : 0)

//��������� ��������� ��� ������� "��������� �����������"
#pragma pack(push,1) //bytealignment
struct t_nodb_device_state {

	nodb_int64 id;                					//*id!
	nodb_int64 device_id;                           // id ����������
	nodb_timestamp dat;                             //����
	nodb_double latitude;                           //������
	nodb_double longitude;                          //�������
	nodb_int altitude;                              //����������
	nodb_int velocity;                              //��������
	nodb_double roll;                               //����
	nodb_double tangage;                            //������
	nodb_int course;                                //����
	nodb_double azimuth;                            //������, ����
	nodb_double diagram;                            //������ �� �� �������, ����
	nodb_double place_angle;                        //���� �����, ����
	nodb_int place_angle_accuracy;                  //������ �� �� ���� �����, ����
	nodb_int polarisation_type;		       //��� ����������� (����������� �����������)
};
#pragma pack(pop)

#define nodb_update_mask_device_state(id,d_id,dt,la,lo,ht,sp,rl,tg,hd,az,dg,pla,plaa,pt) \
	(id   ? (1LL<<0)  : 0) | \
	(d_id ? (1LL<<1)  : 0) | \
	(dt   ? (1LL<<2)  : 0) | \
	(la   ? (1LL<<3)  : 0) | \
	(lo   ? (1LL<<4)  : 0) | \
	(ht   ? (1LL<<5)  : 0) | \
	(sp   ? (1LL<<6)  : 0) | \
	(rl   ? (1LL<<7)  : 0) | \
	(tg   ? (1LL<<8)  : 0) | \
	(hd   ? (1LL<<9)  : 0) | \
	(az   ? (1LL<<10) : 0) | \
	(dg   ? (1LL<<11) : 0) | \
	(pla  ? (1LL<<12) : 0) | \
	(plaa ? (1LL<<13) : 0) | \
	(pt   ? (1LL<<14) : 0)

//��������� ��������� ��� ������� "���������"
#pragma pack(push,1) //bytealignment
struct t_nodb_measure {

const static size_t	NAME_LEN = 64;          		//������������ ����� ������������ ���������"

	nodb_int64 id;                					//*id!
	nodb_int type;                    				//��� ��������� (��� ������� � �� ����������� �������������, ���������� 0)
	nodb_int source_id;                             //id ���������
	nodb_int64 devstate_id;                         //id ��������� ����������
	nodb_varchar name[NAME_LEN];            	//������������ ���������
	nodb_timestamp measure_date;                    //���� ���������� ���������
	nodb_int64 duration;                        	//������������
	nodb_int64 min_frequency;                       //����������� ���������� �������
	nodb_int64 max_frequency;                       //������������ ���������� �������
	nodb_int signal_level;                          //������� �������
	nodb_double noise_ratio;                        //������/���
	nodb_int power;                                 //�������� ���������
        nodb_int bandwidth_measurment_level;            //������� ��������� ������ ������ �������
        nodb_int64 reference_band;                      //��������� ������ ��� ��������� ��������� ������ �������(��������)
        nodb_double energy_flux_density;                //���������� ��������� ������ �������(��������)
	t_nodb_blob signal;                        //������ ������� (BLOB)
	t_nodb_blob track_description;             //��������� ���������-������� ����������� (BLOB)
};
#pragma pack(pop)

#define nodb_update_mask_measure(id,tp,s_id,ds_id,nm,mdt,dur,min_fr,max_fr,sl,nr,pw,bml,rb,efd,sig,tr_d) \
	(id    ? (1LL<<0)  : 0) | \
	(tp    ? (1LL<<1)  : 0) | \
	(s_id  ? (1LL<<2)  : 0) | \
	(ds_id ? (1LL<<3)  : 0) | \
	(nm    ? (1LL<<4)  : 0) | \
	(mdt   ? (1LL<<5)  : 0) | \
	(dur   ? (1LL<<6)  : 0) | \
	(min_fr? (1LL<<7)  : 0) | \
	(max_fr? (1LL<<8)  : 0) | \
	(sl    ? (1LL<<9)  : 0) | \
	(nr    ? (1LL<<10) : 0) | \
	(pw    ? (1LL<<11) : 0) | \
	(bml   ? (1LL<<12) : 0) | \
	(rb    ? (1LL<<13) : 0) | \
	(efd   ? (1LL<<14) : 0) | \
	(sig   ? (1LL<<15) : 0) | \
	(tr_d  ? (1LL<<16) : 0)

//��������� ��������� ��� ������� "��������� ���� ������ �� �������� ��"
#pragma pack(push,1) //bytealignment
struct t_nodb_measure_baring {
	t_nodb_measure measure;                         //��������� ���������
	nodb_double azimuth;                            //������, ����
	nodb_double diagram;                            //������ �� �� �������, ����
	nodb_double place_angle;                        //���� �����, ����
	nodb_int place_angle_accuracy;                  //������ �� �� ���� �����, ����
	nodb_int distance;                              //�������� �� ���������, �
	nodb_int distance_quality;                      //�������� ��������� ���������, 0-100%
};
#pragma pack(pop)

#define nodb_update_mask_measure_baring(az,aza,pla,plaa,rm,rma) \
	(az   ? (1LL<<17) : 0) | \
	(aza  ? (1LL<<18) : 0) | \
	(pla  ? (1LL<<19) : 0) | \
	(plaa ? (1LL<<20) : 0) | \
	(rm   ? (1LL<<21) : 0) | \
	(rma  ? (1LL<<22) : 0)

//��������� ��������� ��� ������� "��������� ���� ���������������� ��������� ��"
#pragma pack(push,1) //bytealignment
struct t_nodb_measure_fixing {
	t_nodb_measure measure;                         //��������� ���������
	nodb_double latitude;                           //������
	nodb_double longitude;                          //�������
	nodb_int64 elevation;                           //����������
	nodb_int quality;                               //�������� ���������, 0-100%
};
#pragma pack(pop)

#define nodb_update_mask_measure_fixing(la,lo,el,ac) \
	(la ? (1LL<<17) : 0) | \
	(lo ? (1LL<<18) : 0) | \
	(el ? (1LL<<19) : 0) | \
	(ac ? (1LL<<20) : 0)

//��������� ��������� ��� ������������
#pragma pack(push,1) //bytealignment
struct t_nodb_dictionary {
const static size_t VALUE_30_LEN = 30;          	//������������ �����
const static size_t VALUE_64_LEN = 64;          	//������������ �����
        nodb_dict_type dict_type;                       //��� �����������
	nodb_int id;  					//id ������ � �����������
        //� ����������� �� ���� ����������� - ������ ������ ��������
        //��� nodb_multiservice_access, nodb_standard
        nodb_varchar value_30[VALUE_30_LEN];            //�������� ��� ����������� 30 ��������
        //��� nodb_apointment, nodb_current_state, nodb_modulation, nodb_multiplexer,
        //nodb_noise_resistant_code, nodb_polarization, nodb_processing_status, nodb_scrambler
        nodb_varchar value_64[VALUE_64_LEN];            //�������� ��� ����������� 64 �������
};
#pragma pack(pop)

//��������� ��������� ��� ������� �������� � ��
#pragma pack(push,1) //bytealignment
struct t_nodb_delivery_system {
        nodb_int Inserted;         //���-�� ����������� ������
        nodb_int Loaded;           //������� ������ ��������� � ��
        nodb_int Errors;           //������� ������
        nodb_double WorkTime;         //����� ������ � ������� ����������� (���)
        nodb_double LoadSpeed;     //������� �������� �������� � �� Loaded/WorkTime
        nodb_double InsertSpeed;      //�������� �������� ������  Inserted/WorkTime
        nodb_double EffCurrentSpeed;     //����������� ������� ��������  (�� 1 ����)
        nodb_double EffAverageSpeed;   //����������� ������� ��������
};
#pragma pack(pop) //bytealignment


// callback declaration
typedef bool (__stdcall *nodb_aircraft_callback) (t_nodb_aircraft * air, void * user_data);
typedef bool (__stdcall *nodb_basestation_callback) (t_nodb_basestation * bs, void * user_data);
typedef bool (__stdcall *nodb_satellite_callback) (t_nodb_satellite * sat, void * user_data);
typedef bool (__stdcall *nodb_source_callback) (t_nodb_source * src, void * user_data);
typedef bool (__stdcall *nodb_measure_baring_callback) (t_nodb_measure_baring * baring, void * user_data);
typedef bool (__stdcall *nodb_measure_fixing_callback) (t_nodb_measure_fixing * fixing, void * user_data);
typedef bool (__stdcall *nodb_device_state_callback) (t_nodb_device_state * state, void * user_data);
typedef bool (__stdcall *nodb_device_callback) (t_nodb_device * device, void * user_data);
typedef bool (__stdcall *nodb_reserve_callback) (t_nodb_reserved * reserve, void * user_data);
typedef bool (__stdcall *nodb_dictionary_callback) (t_nodb_dictionary * dictionary, void * user_data);


//�������
extern "C"
{
//����������� � ���� ������
nodb_result _NODB_FUNC NODB_ConnectDatabase(const wchar_t *Server,const wchar_t *Database, const wchar_t *User,const wchar_t *Password,const wchar_t *Workdir, const bool UpdateOnConnect);
//��������� ������ �� (major & minor)
nodb_result _NODB_FUNC NODB_GetDatabaseVersion(int *major,int *minor);
//���������� �� ���� ������
nodb_result _NODB_FUNC NODB_DisconnectDatabase();
//��������� ������ ������ ��� ����������� � ����
const nodb_pvarchar _NODB_FUNC NODB_GetErrorString(nodb_result id);
//��������� ���������� �� ������� ��������
nodb_result _NODB_FUNC NODB_GetDeliveryStatistics(t_nodb_delivery_system *ds);
//����������� time_t � TDateTime
nodb_double _NODB_FUNC NODB_Time_t_To_TDateTime(nodb_time_t *value);

//��������� �����-���������� �������� �� id
nodb_result _NODB_FUNC NODB_GetAircraft(t_nodb_aircraft *air);
//��������� ������� ������� �� id
nodb_result _NODB_FUNC NODB_GetBasestation(t_nodb_basestation *bs);
//��������� �������� �� id
nodb_result _NODB_FUNC NODB_GetSatellite(t_nodb_satellite *sat);
//��������� ��������� �� �� id
nodb_result _NODB_FUNC NODB_GetSource(t_nodb_source *src);
//��������� ��������� (������) �� id
nodb_result _NODB_FUNC NODB_GetMeasureBaring(t_nodb_measure_baring *baring);
//��������� ��������� (����������������) �� id
nodb_result _NODB_FUNC NODB_GetMeasureFixing(t_nodb_measure_fixing *fixing);
//��������� ��������� �������������� ���������� �� id
nodb_result _NODB_FUNC NODB_GetDeviceState(t_nodb_device_state *state);
//��������� �������������� ���������� �� id
nodb_result _NODB_FUNC NODB_GetDevice(t_nodb_device *device);
//��������� ��������� ������ �� ���� record_number (������� ����� ���� ���������)
//�������� table ��������� ��� ����� ������� ����� �������� ��������� ������:
//���� table = nodb_object_table, �� ��� ������� NOC_OBJECT (������� ��)
//���� table = nodb_source_table, �� ��� ������� NOC_SOURCE (��������� ��)
//���� table = nodb_measure_table, �� ��� ������� NOC_MEASURE (���������)
//���� table = nodb_devicestate_table, �� ��� ������� NOC_DEVICESTATE (��������� ������������� ���������)
//���� table = nodb_device_table, �� ��� ������� NOC_DEVICE (������������� ����������)
//�������� re�ord_id ������ ��������� ID ������ �������, ��������� ���������� table
nodb_result _NODB_FUNC NODB_GetReserve(nodb_reserve_callback callback, void* user_data, nodb_table_type table, nodb_int64 record_id);

//���������� �����-���������� ��������
nodb_result _NODB_FUNC NODB_SetAircraft(const t_nodb_aircraft *air, nodb_uint64 mask);
//���������� ������� �������
nodb_result _NODB_FUNC NODB_SetBasestation(const t_nodb_basestation *bs, nodb_uint64 mask);
//���������� ��������
nodb_result _NODB_FUNC NODB_SetSatellite(const t_nodb_satellite *sat, nodb_uint64 mask);
//���������� ��������� ��
nodb_result _NODB_FUNC NODB_SetSource(const t_nodb_source *src, nodb_uint64 mask);
//���������� ��������� (������)
nodb_result _NODB_FUNC NODB_SetMeasureBaring(const t_nodb_measure_baring *baring, nodb_uint64 mask);
//���������� ��������� (����������������)
nodb_result _NODB_FUNC NODB_SetMeasureFixing(const t_nodb_measure_fixing *fixing, nodb_uint64 mask);
//���������� ��������� �������������� ����������
nodb_result _NODB_FUNC NODB_SetDeviceState(const t_nodb_device_state *state, nodb_uint64 mask);
//���������� �������������� ����������
nodb_result _NODB_FUNC NODB_SetDevice(const t_nodb_device *device, nodb_uint64 mask);
//���������� ��������� ������ �� id
nodb_result _NODB_FUNC NODB_SetReserve(const t_nodb_reserved *reserve, nodb_uint64 mask);

//�������� �����-���������� ��������
nodb_result _NODB_FUNC NODB_DeleteAircraft(const nodb_int id);
//�������� ������� �������
nodb_result _NODB_FUNC NODB_DeleteBasestation(const nodb_int id);
//�������� ��������
nodb_result _NODB_FUNC NODB_DeleteSatellite(const nodb_int id);
//�������� ��������� ��
nodb_result _NODB_FUNC NODB_DeleteSource(const nodb_int id);
//�������� ��������� (������)
nodb_result _NODB_FUNC NODB_DeleteMeasureBaring(const nodb_int64 id);
//�������� ��������� (����������������)
nodb_result _NODB_FUNC NODB_DeleteMeasureFixing(const nodb_int64 id);
//�������� ��������� �������������� ����������
nodb_result _NODB_FUNC NODB_DeleteDeviceState(const nodb_int64 id);
//�������� �������������� ����������
nodb_result _NODB_FUNC NODB_DeleteDevice(const nodb_int64 id);
//�������� ��������� ������ �� id
nodb_result _NODB_FUNC NODB_DeleteReserve(const nodb_int64 id);

//���������� �����-���������� ��������
nodb_result _NODB_FUNC NODB_InsertAircraft(const t_nodb_aircraft *air, nodb_int *id);
//���������� ������� �������
nodb_result _NODB_FUNC NODB_InsertBasestation(const t_nodb_basestation *bs, nodb_int *id);
//���������� ��������
nodb_result _NODB_FUNC NODB_InsertSatellite(const t_nodb_satellite *sat, nodb_int *id);
//���������� ��������� ��
nodb_result _NODB_FUNC NODB_InsertSource(const t_nodb_source *src, nodb_int *id);
//���������� ��������� (��� ���������)
nodb_result _NODB_FUNC NODB_InsertMeasure(const t_nodb_measure *measure, nodb_int *id);
//���������� ��������� (������)
nodb_result _NODB_FUNC NODB_InsertMeasureBaring(const t_nodb_measure_baring *baring, nodb_int *id);
//���������� ��������� (����������������)
nodb_result _NODB_FUNC NODB_InsertMeasureFixing(const t_nodb_measure_fixing *fixing, nodb_int *id);
//���������� ��������� �������������� ����������
nodb_result _NODB_FUNC NODB_InsertDeviceState(const t_nodb_device_state *state, nodb_int *id);
//���������� �������������� ����������
nodb_result _NODB_FUNC NODB_InsertDevice(const t_nodb_device *device, nodb_int *id);
//���������� ��������� ������,
//�������� table ��������� ��� ����� ������� ����������� ��������� ������:
//���� table = nodb_object_table, �� ��� ������� NOC_OBJECT (������� ��)
//���� table = nodb_source_table, �� ��� ������� NOC_SOURCE (��������� ��)
//���� table = nodb_measure_table, �� ��� ������� NOC_MEASURE (���������)
//���� table = nodb_devicestate_table, �� ��� ������� NOC_DEVICESTATE (��������� ������������� ���������)
//���� table = nodb_device_table, �� ��� ������� NOC_DEVICE (������������� ����������)
nodb_result _NODB_FUNC NODB_InsertReserve(const t_nodb_reserved *reserve, nodb_int *id, nodb_table_type table);
//���������� �������� � ����������
nodb_result _NODB_FUNC NODB_InsertDictionary(const t_nodb_dictionary *dictionary, nodb_int *id);

//��������� ���� �����-��������� �������
// ���� standard_id == NULL, �� ���������� ��� ���� ���������� � ���� ����������
// ���� standard_id = 0, �� ���������� ��� �� ��������� � �����������
// ���� standard_id != 0, �� ���������� ��� ���������� ���������
nodb_result _NODB_FUNC NODB_GetAllAircrafts(nodb_aircraft_callback callback, const nodb_int *standard_id, void* user_data);
//��������� ���� ������� �������
// ���� standard_id == NULL, �� ���������� �� ���� ���������� � ���� ����������
// ���� standard_id = 0, �� ���������� �� �� ��������� � �����������
// ���� standard_id != 0, �� ���������� �� ���������� ���������
nodb_result _NODB_FUNC NODB_GetAllBasestations(nodb_basestation_callback callback, const nodb_int *standard_id, void* user_data);
//��������� ���� ���������
// ���� standard_id == NULL, �� ���������� �������� ���� ���������� � ���� ����������
// ���� standard_id = 0, �� ���������� �������� �� ��������� � �����������
// ���� standard_id != 0, �� ���������� �������� ���������� ���������
nodb_result _NODB_FUNC NODB_GetAllSatellites(nodb_satellite_callback callback, const nodb_int *standard_id, void* user_data);
//��������� ���� ���������� ��
// ���� standard_id = 0, �� ���������� ��� ���������
// ���� standard_id != 0, �� ���������� ��������� ���������� ���������
nodb_result _NODB_FUNC NODB_GetAllSources(nodb_source_callback callback, const nodb_int *standard_id, void* user_data);
//��������� ���� ��������� (������) �� id
nodb_result _NODB_FUNC NODB_GetAllMeasuresBaring(nodb_measure_baring_callback callback, void* user_data);
//��������� ���� ��������� (����������������) �� id
nodb_result _NODB_FUNC NODB_GetAllMeasuresFixing(nodb_measure_fixing_callback callback, void* user_data);
//��������� ���� ��������� ������������� ��������� �� id
nodb_result _NODB_FUNC NODB_GetAllDeviceStates(nodb_device_state_callback callback, void* user_data);
//��������� ���� ������������ ��������� �� id
nodb_result _NODB_FUNC NODB_GetAllDevices(nodb_device_callback callback, void* user_data);
//��������� ���� ��������� �������
nodb_result _NODB_FUNC NODB_GetAllReserve(nodb_reserve_callback callback, void* user_data, nodb_table_type table);
//��������� ���� �������� �� �����������
nodb_result _NODB_FUNC NODB_GetAllDictionary(nodb_dictionary_callback callback, void* user_data, const nodb_dict_type type);

//���������� ������������ ���������
nodb_result _NODB_FUNC NODB_InsertHistory(double datetime, //����+�����
                  __int64 *DevstateId,  //������ �� ��������� ����������, ���� NULL �� �� ������� � ����������
                  int *standard_id,  //�������� - ������ �� ������� ����������, ���� NULL �� �� ������� �� ����������
                  wchar_t *event,     //�������� ���������
                  wchar_t *comment,   //�������������� ���������� � ���������
                  wchar_t *PostId,    //�������� �����
                  wchar_t *authenticateduser, //������������
                  wchar_t *notice      //�������
                  );

}


#ifndef NODB_DLL
//�����
typedef nodb_result (__stdcall *NODB_ConnectDatabaseFunc)(const wchar_t *Server,const wchar_t *Database, const wchar_t *User,const wchar_t *Password,const wchar_t *Workdir, const bool UpdateOnConnect);
typedef nodb_result (__stdcall *NODB_GetDatabaseVersionFunc)(int *major,int *minor);
typedef nodb_result (__stdcall *NODB_DisconnectDatabaseFunc)();
typedef const nodb_pvarchar  (__stdcall *NODB_GetErrorStringFunc)(nodb_result id);
//��������� ���������� �� ������� ��������
typedef nodb_result (__stdcall *NODB_GetDeliveryStatisticsFunc)(t_nodb_delivery_system *ds);
//����������� time_t � TDateTime
typedef nodb_double (__stdcall *NODB_Time_t_To_TDateTimeFunc)(nodb_time_t *value);
//��������� �� id
typedef nodb_result  (__stdcall *NODB_GetAircraftFunc)(t_nodb_aircraft *air);
typedef nodb_result  (__stdcall *NODB_GetBasestationFunc)(t_nodb_basestation *bs);
typedef nodb_result  (__stdcall *NODB_GetSatelliteFunc)(t_nodb_satellite *sat);
typedef nodb_result  (__stdcall *NODB_GetSourceFunc)(t_nodb_source *src);
typedef nodb_result  (__stdcall *NODB_GetMeasureBaringFunc)(t_nodb_measure_baring *baring);
typedef nodb_result  (__stdcall *NODB_GetMeasureFixingFunc)(t_nodb_measure_fixing *fixing);
typedef nodb_result  (__stdcall *NODB_GetDeviceStateFunc)(t_nodb_device_state *state);
typedef nodb_result  (__stdcall *NODB_GetDeviceFunc)(t_nodb_device *device);
typedef nodb_result  (__stdcall *NODB_GetReserveFunc)(nodb_reserve_callback callback, void* user_data, nodb_table_type table, nodb_int64 record_id);
//����������
typedef nodb_result  (__stdcall *NODB_SetAircraftFunc)(const t_nodb_aircraft *air, nodb_uint64 mask);
typedef nodb_result  (__stdcall *NODB_SetBasestationFunc)(const t_nodb_basestation *bs, nodb_uint64 mask);
typedef nodb_result  (__stdcall *NODB_SetSatelliteFunc)(const t_nodb_satellite *sat, nodb_uint64 mask);
typedef nodb_result  (__stdcall *NODB_SetSourceFunc)(const t_nodb_source *src, nodb_uint64 mask);
typedef nodb_result  (__stdcall *NODB_SetMeasureBaringFunc)(const t_nodb_measure_baring *baring, nodb_uint64 mask);
typedef nodb_result  (__stdcall *NODB_SetMeasureFixingFunc)(const t_nodb_measure_fixing *fixing, nodb_uint64 mask);
typedef nodb_result  (__stdcall *NODB_SetDeviceStateFunc)(const t_nodb_device_state *state, nodb_uint64 mask);
typedef nodb_result  (__stdcall *NODB_SetDeviceFunc)(const t_nodb_device *device, nodb_uint64 mask);
typedef nodb_result  (__stdcall *NODB_SetReserveFunc)(const t_nodb_reserved *reserve, nodb_uint64 mask);
//��������
typedef nodb_result  (__stdcall *NODB_DeleteAircraftFunc)(const nodb_int id);
typedef nodb_result  (__stdcall *NODB_DeleteBasestationFunc)(const nodb_int id);
typedef nodb_result  (__stdcall *NODB_DeleteSatelliteFunc)(const nodb_int id);
typedef nodb_result  (__stdcall *NODB_DeleteSourceFunc)(const nodb_int id);
typedef nodb_result  (__stdcall *NODB_DeleteMeasureBaringFunc)(const nodb_int64 id);
typedef nodb_result  (__stdcall *NODB_DeleteMeasureFixingFunc)(const nodb_int64 id);
typedef nodb_result  (__stdcall *NODB_DeleteDeviceStateFunc)(const nodb_int64 id);
typedef nodb_result  (__stdcall *NODB_DeleteDeviceFunc)(const nodb_int64 id);
typedef nodb_result  (__stdcall *NODB_DeleteReserveFunc)(const nodb_int64 id);
//����������
typedef nodb_result  (__stdcall *NODB_InsertAircraftFunc)(const t_nodb_aircraft *air, nodb_int *id);
typedef nodb_result  (__stdcall *NODB_InsertBasestationFunc)(const t_nodb_basestation *bs, nodb_int *id);
typedef nodb_result  (__stdcall *NODB_InsertSatelliteFunc)(const t_nodb_satellite *sat, nodb_int *id);
typedef nodb_result  (__stdcall *NODB_InsertSourceFunc)(const t_nodb_source *src, nodb_int *id);
typedef nodb_result  (__stdcall *NODB_InsertMeasureFunc)(const t_nodb_measure *measure, nodb_int *id);
typedef nodb_result  (__stdcall *NODB_InsertMeasureBaringFunc)(const t_nodb_measure_baring *baring, nodb_int *id);
typedef nodb_result  (__stdcall *NODB_InsertMeasureFixingFunc)(const t_nodb_measure_fixing *fixing, nodb_int *id);
typedef nodb_result  (__stdcall *NODB_InsertDeviceStateFunc)(const t_nodb_device_state *state, nodb_int *id);
typedef nodb_result  (__stdcall *NODB_InsertDeviceFunc)(const t_nodb_device *device, nodb_int *id);
typedef nodb_result  (__stdcall *NODB_InsertReserveFunc)(const t_nodb_reserved *reserve, nodb_int *id, nodb_table_type table);
typedef nodb_result  (__stdcall *NODB_InsertDictionaryFunc)(const t_nodb_dictionary *dictionary, nodb_int *id);
//���������� ������������ ���������
typedef nodb_result (__stdcall *NODB_InsertHistoryFunc)(double datetime, //����+�����
                  __int64 *DevstateId,  //������ �� ��������� ����������, ���� NULL �� �� ������� � ����������
                  int *standard_id,  //�������� - ������ �� ������� ����������, ���� NULL �� �� ������� �� ����������
                  wchar_t *event,     //�������� ���������
                  wchar_t *comment,   //�������������� ���������� � ���������
                  wchar_t *PostId,    //�������� �����
                  wchar_t *authenticateduser, //������������
                  wchar_t *notice      //�������
                  );
//��������� ����
typedef nodb_result  (__stdcall *NODB_GetAllAircraftsFunc)(nodb_aircraft_callback callback , const nodb_int *standard_id, void * user_data);
typedef nodb_result  (__stdcall *NODB_GetAllBasestationsFunc)(nodb_basestation_callback callback , const nodb_int *standard_id, void * user_data);
typedef nodb_result  (__stdcall *NODB_GetAllSatellitesFunc)(nodb_satellite_callback callback , const nodb_int *standard_id, void * user_data);
typedef nodb_result  (__stdcall *NODB_GetAllSourcesFunc)(nodb_source_callback callback , const nodb_int *standard_id, void * user_data);
typedef nodb_result  (__stdcall *NODB_GetAllMeasuresBaringFunc)(nodb_measure_baring_callback callback , void * user_data);
typedef nodb_result  (__stdcall *NODB_GetAllMeasuresFixingFunc)(nodb_measure_fixing_callback callback , void * user_data);
typedef nodb_result  (__stdcall *NODB_GetAllDeviceStatesFunc)(nodb_device_state_callback callback , void * user_data);
typedef nodb_result  (__stdcall *NODB_GetAllDevicesFunc)(nodb_device_callback callback , void * user_data);
typedef nodb_result  (__stdcall *NODB_GetAllReserveFunc)(nodb_reserve_callback callback, void* user_data, nodb_table_type table);
typedef nodb_result  (__stdcall *NODB_GetAllDictionaryFunc)(nodb_dictionary_callback callback, void* user_data, const nodb_dict_type type);
#endif




//---------------------------------------------------------------------------
#endif

