#ifndef SPECTRUM_MATERIAL_H
#define SPECTRUM_MATERIAL_H

#include "../../../modules/gui/multi_graphics/multi_graphics.h"
#include "..\..\core\storage_inl.h"

class QGraphicsItem;
class Q_MG_Style;


struct MaterialFloatData
{
	float* values;
	unsigned int PointCount;
	MaterialFloatData (unsigned int _PointCount = 0)
	{
		values = NULL;
		PointCount = _PointCount;
		if (PointCount == 0) return;
		values = new float [PointCount];
		memset(values,0,PointCount*sizeof(float));
		return;
	}
	~MaterialFloatData()
	{
		DeleteMemory();
	}
	bool Allocate(unsigned int _PointCount)//�������� ������
	{
		if (_PointCount == 0) return false;
		PointCount = _PointCount;
		values = new float [PointCount];
		memset(values,0,PointCount*sizeof(float));
		return IsValid();
	}
	void DeleteMemory()//������� ������
	{
		delete[] values;values = NULL;
		PointCount = 0;
	}
	bool IsValid()//��������� �� ��������� ���� ������
	{
		if ((PointCount == 0) || (values == NULL)) return false;
		return true;
	}
	bool Copy(MaterialFloatData  &_mat)//������� ������, �������� ����� � ��������
	{
		DeleteMemory();
		if (!_mat.IsValid()) return false;
		if (!Allocate(_mat.PointCount)) return false;
		memcpy(values,_mat.values,PointCount*sizeof(float));
		return IsValid();
	}
	bool setPoint(double val,unsigned int pos)//������������� �������� � ������ �� ������� pos. ��������: ������ �� ��������!!!
	{
		if (pos >= PointCount) return false;
		values[pos] = val;
		return (values[pos] == val);
	}
	double operator[](int i)
	{	return values[i]; }

private:
	DISALLOW_COPY_AND_ASSIGN(MaterialFloatData);
};

struct CacheElement
{
	unsigned int fromC;//� �������� ������� � ������� ���������
	unsigned int ToC;//�� �������� ������������
	float minVal;
	float maxVal;

	float additionalLine;
	bool fromMax;

	CacheElement(unsigned int _fromC = 0,unsigned int _ToC = 0,float _minC = 0, float _maxC = 0 )
	{
		fromC = _fromC;ToC = _ToC;minVal = _minC;maxVal = _maxC; additionalLine = 0; fromMax = false;
	}
	bool IsValid() { return ((fromC == 0) && (minVal == 0) && (maxVal == 0) && (ToC == 0)); }
};

struct CacheCluster
{
	QCache<unsigned int,CacheElement> ElListCach;
	unsigned int kf;//������ ����� �������� �� �������
};


class SpectrumMaterial;

class Q_MG_PointItem : public  QGraphicsObject
{
	friend Q_MG_Material;
public:
	Q_MG_PointItem(int _szX,int _szY,SpectrumMaterial *parent,Q_MG_Style* _settings);
	~Q_MG_PointItem() { };

	virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
	QRectF boundingRect() const { return QRectF(0,0,szX,szY); };

	int szX;int szY;//� ��������
	unsigned int PointIndex;//N �������
	SpectrumMaterial *parent_;

private:
	Q_MG_Style* settings;
	DISALLOW_COPY_AND_ASSIGN(Q_MG_PointItem);
};


class SpectrumMaterial : public Q_MG_Material
{
	Q_OBJECT
public:
	SpectrumMaterial(QGraphicsItem *parent = NULL);
	~SpectrumMaterial(){	if (!DataIsInline) DataMemoryStore.DeleteMemory(); };

	bool setData(MaterialFloatData &_mat, bool updateAfterSet = true);
	bool setInlineData(float* spectrum,unsigned int array_size);

	virtual void ExtMaterialReInit();
	virtual bool ExtViewPortChange(Q_MG_MaterialElement* NewEl,unsigned __int64 elXpos,unsigned __int64 elYpos);
	virtual bool PrepareForViewPortChange(qreal key_kf_x,qreal key_kf_y, double StartPixelX, double EndPixelX, double StartPixelY,double EndPixelY);
	virtual void ExtAfterViewPortChange();

	float* GetMaterialDataArrayByPixels(double globP1x,double globP2x,size_t *outLen);//���������� ��������� �� ������ � ���������� ���������, ������������ �����

	bool isFilledSpectrum() {return FilledSpectrum;};
	void SetFillSpectrum(bool fill = true);
	unsigned int PointsCount()	{ return DataMemoryStore.PointCount;}


private:
	bool FilledSpectrum;
	bool DataIsInline;
	MaterialFloatData DataMemoryStore;


	Storage<Q_MG_PointItem*> PointItemList;
	inline void ClearPP();

	QCache<unsigned int,CacheCluster> GlobalCacheC;//����������� ��� � ���� ��������

	bool DrawLineInElement(Q_MG_MaterialElement* el,double p1x,double p1y,double p2x,double p2y,QPolygon* spectrum_fill = NULL);

	//���������� ��� ������ ����� ����������� �  �������, �������� � ���� ��������
	bool find_current_cache_cluster_;
	CacheCluster* CurCachCluster;
	qreal key_kf_x_;
	unsigned __int64 round_key_kf_;
	double StartPixelX_;
	double EndPixelX_;
	double StartPixelY_;
	double EndPixelY_;


    //QPolygon *spectrum_fill;

    //signals:
    //    void signalUpdate();

    //private slots:
    //    void _slotUpdate();

	DISALLOW_COPY_AND_ASSIGN(SpectrumMaterial);
};

#endif // SPECTRUM_MATERIAL_H
