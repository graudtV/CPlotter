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
	Plot plot = {};
	printf("\033]2;BLA_BLA\007\n");
	plotSetDefault(&plot);
	system("clear");
	//plotSetSize(&plot, 60, 10);
	plotSetAxis(&plot, -10, 10, -1.3, 1.3);
	plotSetFunction(&plot, someFunc);
	plotSetChar(&plot, 'x');
	plotSetTitle(&plot, "Hello, world! It's my graph");
	plotDraw(&plot);

	Plot plotCos = {};
	plotSetDefault(&plotCos);
	plotSetFunction(&plotCos, cos);
	plotZoom(&plotCos, 1.5, 1);
	plotDraw(&plotCos);
	plotSetDefault(&plot);
	plotSetFunction(&plot, sqrt);
	plotSetAxis(&plot, -1, 10, -1, 3);
	plotDraw(&plot);

	// while (1)
	// {
	// 	system("clear");
	// 	plotSetFunction(&plot, someFunc);
	// 	plotDraw(&plot);
	// 	plotSetFunction(&plot, anotherFunc);
	// 	plotDraw(&plot);
	// 	usleep(100000);
	// }


	return 0;
}