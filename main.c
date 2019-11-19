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
	return 1 / x;
}

double anotherFunc(double x)
{
	static double bias = 0;
	bias += 0.005;
	return sin(x) * cos(x + bias);
	//return x;
}

double circle(double x, double y)
{
	return x * x + y * y - 1;
	return x * x  - y * y - 1;
}

int main()
{
	system("clear");
	FILE *fout = fopen("result.txt", "w");

	Plot defaultPlot = {}, cosPlot = {}, sqrtPlot = {};
	//1) default
	plotSetDefault(&defaultPlot);
	plotSetBackGroundChar(&defaultPlot, ' ');
	plotSetChar(&defaultPlot, 'x');
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
	plotSetAxes(&sqrtPlot, -0.5, 10, -0.5, 3);

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

	// plotSetFunction(&cosPlot, anotherFunc);
	// plotSetFout(&cosPlot, stdout);
	// plotDraw(&cosPlot);
	// while (1)
	// {
	// 	system("clear");
	// 	plotDraw(&cosPlot);
	// 	usleep(100000);
	// }

	plotDestroy(&defaultPlot);
	plotDestroy(&cosPlot);
	plotDestroy(&sqrtPlot);

	fclose(fout);
	return 0;
}