#ifndef CPLOTTER
#define CPLOTTER

#include <stdio.h>
typedef struct
{
	int sx_, sy_; //Пикселей на графике
	double xLeft_, xRight_, yDown_, yUp_; //Границы отображения графика
	double (*func_)(double); //Указатель на изображаемую функцию
	char ch_; //Отображаемый символ
	char *title_; //Имя графика
	FILE *fout_; //Файл для записи

	//extra data
	long int plotId_;
} Plot;

//Создание графика (по-умолчанию, по копии), удаление графика
void plotSetDefault(Plot *plot);
void plotCopy(Plot *dst, Plot *src); //По сути конструктор копирования
void plotDestroy(Plot *plot);

//Настройки графика
void plotSetSize(Plot *plot, int sx, int sy);
void plotSetFunction(Plot *plot, double (*func)(double));
void plotSetAxis(Plot *plot, double xLeft, double xRight, double yDown, double yUp);
void plotSetChar(Plot *plot, char ch);
void plotSetTitle(Plot *plot, const char *title);
void plotSetFout(Plot *plot, FILE *fout);

//Эти функции есть, но используются только внутри
//void printInCenter(char *text, int rowLen); //Выравнивает текст 
//void fprintInCenter(FILE *fout, char *text, int rowLen); //Выравнивает текст 

//Отрисовка графика
void plotDraw(Plot *plot);
void plotZoom(Plot *plot, double xZoom, double yZoom); //"Растягивает" (или сужает) график.
void plotAdjustToConsole(Plot *plot); //Подгоняет размер графика под размер окна консоли



#endif // CPLOTTER