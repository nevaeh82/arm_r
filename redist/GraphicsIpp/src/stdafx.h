#include "modules/core/preprocessor.h"
#include "modules/core/storage_inl.h"

#include <QtCore>
#include <QtGui>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QCache>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QHash>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QPixmapCache>
#include <QColor>
#include <QDesktopWidget>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QGraphicsObject>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QFontMetrics>
#include <QPointer>
#include <QSettings>
#include <QMutex>
#include <QPainter>
#include <QColor>
#include <QFile>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSettings>
#include <QApplication>
#include <QLayout>
#include <QLinearGradient>
#include <QBitmap>
#include <QPlainTextEdit>
#include <QTransform>

#include <math.h>
#include <Windows.h>
#include <ipp.h>

#include <tbb/tbb.h>
//#include <tbb/tbbmalloc_proxy.h>