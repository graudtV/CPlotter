#ifndef CPLOTTER
#define CPLOTTER

#include <stdio.h>

/*!
 * \brief Тип данных кривой на плоскости, заданной неявно
 * 
 * Кривая задается уравнением f(x, y) = 0;
 * Функция типа Curve_t принимает параметры x, y, подставляет их в левую часть
 *  уравнения кривой (т.е. рассчитывает f(x, y)) и возвращает результат.
 * По возвращаемому значению можно судить, лежит ли точки на кривой или нет
 */
typedef double Curve_t (double x, double y);

/*!
 * \brief Структура с настройками графика
 */
typedef struct
{
	int sx_, sy_; //!< Количество пикселей на графике по осям x, y
	double xLeft_, xRight_, yDown_, yUp_; //!< Границы отображения графика (Отображаемые участки по осям)
	double (*func_)(double); //!< Указатель на изображаемую функцию
	Curve_t *curve_; //!< Указатель на изображаемую кривую (линию)
	char ch_; //!< Символ отображения точек графика
	char *title_; //!< Название графика
	FILE *fout_; //!< Файл для вывода графика

	//extra data
	long int plotId_; //!< Номер графика. Присваивается функцией plotSetDefault()
	char backgroundCh_; //!< Символ фона, по умолчанию - пробел
} Plot;

//Инициализация, копирование, удаление графика
void plotSetDefault(Plot *plot);
void plotCopy(Plot *dst, Plot *src);
void plotDestroy(Plot *plot);

//Настройки графика
void plotSetSize(Plot *plot, int sx, int sy);
void plotSetFunction(Plot *plot, double (*func)(double));
void plotSetCurve(Plot *plot, Curve_t *curve);
void plotSetAxes(Plot *plot, double xLeft, double xRight, double yDown, double yUp);
void plotSetChar(Plot *plot, char ch);
void plotSetBackGroundChar(Plot *plot, char ch);
void plotSetTitle(Plot *plot, const char *title);
void plotSetFout(Plot *plot, FILE *fout);

//Отрисовка графика
void plotDraw(Plot *plot);
void plotZoom(Plot *plot, double xZoom, double yZoom); //"Растягивает" (или сужает) график.
void plotAdjustToConsole(Plot *plot); //Подгоняет размер графика под размер окна консоли

//Эти функции используются только внутри CPlotter.c
//void printInCenter(char *text, int rowLen); //Выравнивает текст 
//void fprintInCenter(FILE *fout, char *text, int rowLen); //Выравнивает текст 



#endif // CPLOTTER