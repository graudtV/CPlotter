#include "CPlotter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h> 
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ - Для определения размера окна консоли


void plotSetSize(Plot *plot, int sx, int sy)
{
	plot->sx_ = sx;
	plot->sy_ = sy;
}
void plotSetFunction(Plot *plot, double (*func)(double))
{
	plot->func_ = func;
}
void plotSetAxis(Plot *plot, double xLeft, double xRight, double yDown, double yUp)
{
	plot->xLeft_ = xLeft;
	plot->xRight_ = xRight;
	plot->yDown_ = yDown;
	plot->yUp_ = yUp;
}

void plotSetChar(Plot *plot, char ch)
{
	plot->ch_ = ch;
}
void plotSetTitle(Plot *plot, const char *title)
{
	plot->title_ = strdup(title);
}

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

void printInCenter(char *text, int rowLen) //Выравнивает текст 
{
	if (text == NULL)
		return;
	int textLen = strlen(text);
	if (textLen <= rowLen) //Текст вмещается на одну строчку
	{
		int spaces = (rowLen - textLen) / 2;
		for (int i = 0; i < spaces; ++i)
			putchar(' ');
		printf("%s\n", text);		
	}
	else
	{
		printf("%s\n", text);
	}

}

void plotDraw(Plot *plot)
{
	printf("\n");
	printInCenter(plot->title_, plot->sx_ + 2);
	int *yValues = calloc(plot->sx_, sizeof(int));
	//Заполняем yValues
	for (int xGraph = 0; xGraph < plot->sx_; ++xGraph)
	{
		double xReal = xGraph * (plot->xRight_ - plot->xLeft_) / plot->sx_ + plot->xLeft_;
		double yReal = plot->func_(xReal);
		yValues[xGraph] = round((yReal - plot->yUp_) * (plot->sy_) / (plot->yDown_ - plot->yUp_));
		//Результаты, выходящие за пределы графика, не рисуются автоматически
	}
	// for (int i = 0; i < plot->sx_; ++i) //DEBUG
	// 	printf("%d ", yValues[i]);
	//Координаты нуля в системе отсчета графика
	int xNull = round((plot->sx_ * plot->xLeft_) / (plot->xLeft_ - plot->xRight_));
	int yNull = round((plot->sy_ * plot->yUp_) / (plot->yUp_ - plot->yDown_)); 

	for (int i = -2; i < plot->sx_; ++i) //Верхняя рамка графика
		printf("*");
	printf("\n");
	for (int j = 0; j < plot->sy_; ++j)
	{
		printf("*");  //левая рамка графика
		for (int i = 0; i < plot->sx_; ++i)
			if (yValues[i] == j)
				printf("%c", plot->ch_);
			else if (i == xNull && j == yNull)
				printf("0");
			else if (i == xNull)
				printf("|");
			else if (j == yNull)
				printf("-");
			else
				printf(" ");
		printf("*\n"); //правая рамка графика
	}
	for (int i = -2; i < plot->sx_; ++i) //Нижняя рамка графика
	printf("*");
	printf("\n");	
}

void plotZoom(Plot *plot, double xZoom, double yZoom) //"Растягивает" (или сужает) график.
{
	plot->xLeft_ /= xZoom;
	plot->xRight_ /= xZoom;
	plot->yUp_ /= yZoom;
	plot->yDown_ /= yZoom;
}

void plotAdjustToConsole(Plot *plot)
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	plot->sx_ = size.ws_col - 2;
	plot->sy_ = size.ws_row / 3 - 4;
}

void plotSetDefault(Plot *plot)
{
	static int nOfPlots = 0;
	assert(plot != NULL);

	if (plot->plotId_ == 0)
		plot->plotId_ = ++nOfPlots;
	plotAdjustToConsole(plot);
	plotSetAxis(plot, -10, 10, -1.3, 1.3);
	plotSetFunction(plot, sin);
	plotSetChar(plot, 'x');
	char title[40] = "Graph № ";
	char buffer[20] = "";
	sprintf(buffer, "%ld", plot->plotId_);
	plotSetTitle(plot, strcat(title, buffer));	
}