#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>
#include "CPlotter.h"

double funcNull(double x)
{
	return 0.0;
}
double someFunc(double x)
{
	static double bias = 0;
	bias += 0.005;
	return sin(x + bias);
}

double anotherFunc(double x)
{
	static double bias = 0;
	bias += 0.005;
	return sin(x) * cos(x + bias);
}


int main()
{
	system("clear");
	FILE *fout = fopen("result.txt", "w");

	Plot defaultPlot = {}, cosPlot = {}, sqrtPlot = {};
	//1) default
	plotSetDefault(&defaultPlot);
	plotSetTitle(&defaultPlot, "Hello, world! I'm a graph");
	//2) cos
	plotCopy(&cosPlot, &defaultPlot);
	plotSetFunction(&cosPlot, cos);
	plotSetTitle(&cosPlot, "It is cos(x) plot");
	//3) sqrt
	plotCopy(&sqrtPlot, &defaultPlot);
	plotSetFunction(&sqrtPlot, sqrt);
	plotSetTitle(&sqrtPlot, "One more graph. It is sqrt.");
	plotSetChar(&sqrtPlot, '\'');
	plotSetAxis(&sqrtPlot, -0.5, 10, -0.5, 3);

	plotDraw(&defaultPlot);
	plotDraw(&cosPlot);
	plotDraw(&sqrtPlot);

	//Теперь запишем тоже самое в текстовый файл
	plotSetFout(&defaultPlot, fout);
	plotSetFout(&cosPlot, fout);
	plotSetFout(&sqrtPlot, fout);

	plotDraw(&defaultPlot);
	plotDraw(&cosPlot);
	plotDraw(&sqrtPlot);
	// while (1)
	// {
	// 	system("clear");
	// 	plotSetFunction(&plot, someFunc);
	// 	plotDraw(&plot);
	// 	plotSetFunction(&plot, anotherFunc);
	// 	plotDraw(&plot);
	// 	usleep(100000);
	// }

	plotDestroy(&defaultPlot);
	plotDestroy(&cosPlot);
	plotDestroy(&sqrtPlot);

	//fclose(fout);
	return 0;
}