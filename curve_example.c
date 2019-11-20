#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "CPlotter.h"

double circle(double x, double y)
{
	return x * x + y * y - 1;
}

double sinusCurve(double x, double y)
{
	return y - sin(x);
}

double someCurve(double x, double y)
{
	return 3 * x + 2 * y - 1;
}

int main()
{
	system("clear");
	FILE *fout = fopen("result.txt", "w");

	Plot circlePlot = {}, sinusCurvePlot = {}, someCurvePlot = {};
	//1) circlePlot
	plotSetDefault(&circlePlot);
	plotSetTitle(&circlePlot, "Circle");
	plotSetCurve(&circlePlot, circle);
	plotSetAxes(&circlePlot, -1.2, 1.2, -1.2, 1.2);
	//2) SinusCurve plot
	plotSetDefault(&sinusCurvePlot);
	plotSetTitle(&sinusCurvePlot, "Sinus curve");
	plotSetCurve(&sinusCurvePlot, sinusCurve);
	plotSetAxes(&sinusCurvePlot, -10, 10, -1.3, 1.3);
	//3)
	plotSetDefault(&someCurvePlot);
	plotSetTitle(&someCurvePlot, "Unknown :(");
	plotSetCurve(&someCurvePlot, someCurve);
	plotSetAxes(&someCurvePlot, -1, 1, -1, 1);

	plotDraw(&circlePlot);
	plotDraw(&sinusCurvePlot);
	plotDraw(&someCurvePlot);

	plotDestroy(&circlePlot);
	plotDestroy(&sinusCurvePlot);
	plotDestroy(&someCurvePlot);


	fclose(fout);
	return 0;
}