#ifndef CONSTANTS_H
#define CONSTANTS_H

const quint16 RAD_PORT = 155;
const double FREQ_MIN = 0.0;	// ÷ела€ часть частот в ћ√ц
const double FREQ_MAX = 2000.0;//## 16.09.13 теперь в полосу(20 ћ√ц)/(шаг 10 √ц)  //6000.0; // ÷ела€ часть частот в ћ√ц
const double VIEW_LABEL_SCALE = 10.0; //## 16.09.13 дл€ учета в подписи 10 √ц
const double DB_MAX = 10.0;
const double DB_MIN = -110.0;
const unsigned STEP_X_MAX = 100;//## 16.09.13 теперь в 10 √ц  //250;
const unsigned STEP_Y_MAX = 5;
const unsigned MAX_AXES_X = 20; //## 16.09.13 24;
const unsigned MAX_AXES_Y = 24;
const unsigned MIN_AXES_X = 10; //## 16.09.13 12;
const unsigned MIN_AXES_Y = 12;
const unsigned MIN_PX_BWN_AXES = 10; // (px)
const unsigned MAX_PX_BWN_AXES = 100; // (px)
const unsigned FIELD_UP = 50; // (px)
const unsigned FIELD_DN = 50; // (px)
const unsigned FIELD_LF = 50; // (px)
const unsigned FIELD_RT = 50; // (px)
const int MIN_LEVEL = -100;
const int MIN_LEVEL_WIDTH_PX = 4; // Ўирина минимального уровн€ в px
const int MIN_LEVEL_HITSIZE_PX = 12; // Ўирина попадани€ "мышкой" в линию минимального уровн€ в px
const unsigned long MAX_SPECTR_POINTS = 2000000;  //## 16.09.13 12; // “очек с шагом 20M√ц/MAX_SPECTR_POINTS
const int FONT_POINT_SIZE = 8;
const int FONT_STRETCH_SIZE = 100;
const unsigned SCAN_PAUSE = 1000; // mSec
const unsigned SCAN_STEP = 1; //## 16.09.13 минимальный шаг при установке частоты в 1ћ√ц //MNz
const unsigned SCALE_TO_1MHZ = 100; //## 16.09.13  1ћ√ц/10  √ц нового масштаба

#endif // CONSTANTS_H
