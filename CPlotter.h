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
	FILE *fout; //Файл для записи

	//extra data
	long int plotId_;
} Plot;

void plotSetSize(Plot *plot, int sx, int sy);
void plotSetFunction(Plot *plot, double (*func)(double));
void plotSetAxis(Plot *plot, double xLeft, double xRight, double yDown, double yUp);
void plotSetChar(Plot *plot, char ch);
void plotSetTitle(Plot *plot, const char *title);
void plotDestroy(Plot *plot);
void printInCenter(char *text, int rowLen); //Выравнивает текст 
void plotDraw(Plot *plot);
void plotZoom(Plot *plot, double xZoom, double yZoom); //"Растягивает" (или сужает) график.
void plotAdjustToConsole(Plot *plot);
void plotSetDefault(Plot *plot);


#endif // CPLOTTER