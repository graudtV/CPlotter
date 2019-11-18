/*!
 * \file
 * \brief Реализация функций из CPlotter.h
 */
#include "CPlotter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h> 
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ - Для определения размера окна консоли

void printInCenter(char *text, int rowLen); //Описание - ниже
void fprintInCenter(FILE *fout, char *text, int rowLen);


/*!
 * \brief Инициализирует график значениями по умолчанию
 *
 * Удобно сначала проинициализировать график, а уже потом выбрать функцию, подогнать размеры графика, 
 *  сменить название и т.п., так как иначе не удастатся увидеть график, пока есть неустановленные значения
 */
void plotSetDefault(Plot *plot)
{
	static int nOfPlots = 0;
	assert(plot != NULL);

	if (plot->plotId_ == 0)
		plot->plotId_ = ++nOfPlots;
	plotAdjustToConsole(plot);
	plotSetAxes(plot, -10, 10, -1.3, 1.3);
	plotSetFunction(plot, sin);
	plotSetChar(plot, 'x');
	char title[40] = "Graph № ";
	char buffer[20] = "";
	sprintf(buffer, "%ld", plot->plotId_);
	plotSetTitle(plot, strcat(title, buffer));
	plot->fout_ = stdout;	
}

/*!
 * \brief Копирует все настройки одного графика в другой
 *
 * Автоматически выделяет память под нужные поля (строку названия графика)
 * По сути, является конструктором копирования.
 */
void plotCopy(Plot *dst, Plot *src)
{
	assert(dst != NULL);
	assert(src != NULL);
	*dst = *src; //Но это еще не все! title_ указывают на одну и ту же строку!
	if (dst->title_ != NULL) //Если title_ не заполнен (нулевой), то все ок
	{
		dst->title_ = strdup(dst->title_); //title_ не нулевой -> создаем копию
		assert(dst->title_ != NULL); //Удалось выделить место
	}
}
/*!
 * \brief Освобождает выделенную под график память
 */
void plotDestroy(Plot *plot)
{
	plot->sx_ = 0;
	plot->sy_ = 0;
	plot->xLeft_ = 0;
	plot->xRight_ = 0; 
	plot->yDown_ = 0;
	plot->yUp_ = 0;
	plot->func_ = NULL;
	plot->ch_ = '\0';
	if (plot->title_ != NULL)
		free(plot->title_);
	plot->title_ = NULL;
}

//Настройки графика
/*!
 * \brief Настраивает отображаемый размер графика (количество пикселей)
 * \param sx Ширина
 * \param sy Высота
 */
void plotSetSize(Plot *plot, int sx, int sy)
{
	plot->sx_ = sx;
	plot->sy_ = sy;
}
/*!
 * \brief Настраивает функцию, график которой нужно построить
 * \param double (*func)(double) - Указатель на функцию
 * Пример использования:
 * \code
 * #include <math.h>
 *
 * Plot plot = {};
 * plotSetFunction(&plot, cos); //Функция cos определена в math.h
 * ... // Инициализация остальных значений
 * plotDraw(&plot);
 * plotDestroy(&plot);
 * \endcode
 * Чтобы нарисовать свою функцию, нужно определить её и точно также передать указатель на неё.
 * Пример:
 * \code
 * double someFunc(double x)
 * {
 *		return 1 / x;
 * }
 * plotSetFunction(&plot, someFunc);
 * \endcode
 */
void plotSetFunction(Plot *plot, double (*func)(double))
{
	plot->func_ = func;
}
/*!
 * \brief Настраивает отображаемый участок графика
 *
 * [xLeft; yLeft] - отображаемый отрезок оси x
 *
 * [yDown, yUp] - отображаемый отрезок оси y
 * То есть:
 * \param xLeft минимальное значение по оси x, которое может быть видно на графике
 * \param xRight максимальное значение по оси x, которое может быть видно на графике
 * \param yDown минимальное значение по оси y, которое может быть видно на графике
 * \param yUp максимальное значение по оси y, которое может быть видно на графике
 */
void plotSetAxes(Plot *plot, double xLeft, double xRight, double yDown, double yUp)
{
	plot->xLeft_ = xLeft;
	plot->xRight_ = xRight;
	plot->yDown_ = yDown;
	plot->yUp_ = yUp;
}

/*!
 * \brief Настраивает символ, которым будут отображаться точки на графике
 *
 * Например, 'x', '.', '~'.
 */
void plotSetChar(Plot *plot, char ch)
{
	plot->ch_ = ch;
}
/*!
 * \brief Настраивает символ фона графика
 * \code
 * plotSetBackGroundChar(&plot, '\0'); //Устанавливает символ по умолчанию
 * \endcode
 * По умолчанию - пробел.
 */
void plotSetBackGroundChar(Plot *plot, char ch)
{
	plot->backgroundCh_ = ch;
}
/*!
 * \brief Настраивает название графика
 * \param title[] - указатель на строку имени или NULL, если название отображать не нужно
 */
void plotSetTitle(Plot *plot, const char *title)
{
	if (title == NULL)
		plot->title_ = NULL;
	else
		plot->title_ = strdup(title);
}
/*!
 * \brief Настраивает файл, в который будет выведен результат
 *
 * Например:
 * \code
 * plotSetFout(&plot, stdout); //Отображение графика в консоли
 * \endcode
 * Или:
 * \code
 * FILE *fout = fopen("result.txt", "w");
 * plotSetFout(&plot, fout);
 * fclose(fout);
 * \endcode
 */
void plotSetFout(Plot *plot, FILE *fout)
{
	plot->fout_ = fout;
}


/*!
 * \brief Отображает график
 *
 * Все настройки графика должны быть заранее заданы.
 * Если часть обязательных полей не определена, возможно неопределенное поведение
 * Поэтому рекомендуется использовать функцию plotSetDefault() сразу после
 *  создания объекта структуры Plot, чтобы избежать ошибок
 */
void plotDraw(Plot *plot)
{
	assert(plot != NULL);
	if (plot->backgroundCh_ == '\0')
		plot->backgroundCh_ = ' ';
	putc('\n', plot->fout_);
	if (plot->fout_ == NULL)
		printInCenter(plot->title_, plot->sx_ + 2);
	else
		fprintInCenter(plot->fout_, plot->title_, plot->sx_ + 2);
	int *yValues = calloc(plot->sx_, sizeof(int));
	//Заполняем yValues
	for (int xGraph = 0; xGraph < plot->sx_; ++xGraph)
	{
		double xReal = xGraph * (plot->xRight_ - plot->xLeft_) / plot->sx_ + plot->xLeft_;
		double yReal = plot->func_(xReal);
		yValues[xGraph] = round((yReal - plot->yUp_) * (plot->sy_) / (plot->yDown_ - plot->yUp_));
		//Результаты, выходящие за пределы графика, не рисуются автоматически
	}

	//Координаты нуля в системе отсчета графика
	int xNull = round((plot->sx_ * plot->xLeft_) / (plot->xLeft_ - plot->xRight_));
	int yNull = round((plot->sy_ * plot->yUp_) / (plot->yUp_ - plot->yDown_)); 

	for (int i = -2; i < plot->sx_; ++i) //Верхняя рамка графика
		putc('*', plot->fout_);
	putc('\n', plot->fout_);
	for (int j = 0; j < plot->sy_; ++j)
	{
		putc('*', plot->fout_);  //левая рамка графика
		for (int i = 0; i < plot->sx_; ++i)
			if (yValues[i] == j)
				putc(plot->ch_, plot->fout_);
			else if (i == xNull && j == yNull)
				putc('0', plot->fout_);
			else if (i == xNull)
				putc('|', plot->fout_);
			else if (j == yNull)
				putc('-', plot->fout_);
			else
				putc(plot->backgroundCh_, plot->fout_);
		putc('*', plot->fout_); //правая рамка графика
		putc('\n', plot->fout_);
	}
	for (int i = -2; i < plot->sx_; ++i) //Нижняя рамка графика
		putc('*', plot->fout_);
	putc('\n', plot->fout_);	
}

/*!
 * \brief Уменьшает или увеличивает масштаб графика
 * \param xZoom, yZoom - количество раз, в которое нужно "приблизить" график
 *
 * Например,
 * \code
 * plotZoom(plot, 2, 2);
 * \endcode
 * "увеличит" картинку в два раза.
 */
void plotZoom(Plot *plot, double xZoom, double yZoom) //"Растягивает" (или сужает) график.
{
	plot->xLeft_ /= xZoom;
	plot->xRight_ /= xZoom;
	plot->yUp_ /= yZoom;
	plot->yDown_ /= yZoom;
}

/*!
 * \brief Подгоняет размер графика под размеры окна консоли
 *
 * Ширина графика берется равной ширине консоли, высота - такой, что в консоли уместится 3 таких графика
 */
void plotAdjustToConsole(Plot *plot)
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	plot->sx_ = size.ws_col - 2;
	plot->sy_ = size.ws_row / 3 - 4;
}


/*!
 * \brief Печатает строчку в консоль по центру столбца
 * \param text[] -  отображаемая строчка
 * \param rowLen - размер столбца
 */
void printInCenter(char *text, int rowLen) //Выравнивает текст 
{
	fprintInCenter(stdout, text, rowLen);
}

/*!
 * \brief Аналогично функции printInCenter(), только вывод в произвольный файл
 */
void fprintInCenter(FILE *fout, char *text, int rowLen) //Выравнивает текст 
{
	if (text == NULL)
		return;
	int textLen = strlen(text);
	if (textLen <= rowLen) //Текст вмещается на одну строчку
	{
		int spaces = (rowLen - textLen) / 2;
		for (int i = 0; i < spaces; ++i)
			putc(' ', fout);
		fprintf(fout, "%s\n", text);		
	}
	else
	{ //TODO: Доделать нормальный вывод строки в этом случае
		fprintf(fout, "%s\n", text);
	}
}


