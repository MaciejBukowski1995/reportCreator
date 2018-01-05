/*****************************************************************************
 *	APPLICATION:	fp1a_Bukowski_Maciej
 *	MODULE:			main
 *	PURPOSE:
 *	AUTHOR(S):		user
 *
 *	12/23/2017 8:59:24 PM	Created.
 *****************************************************************************/
#include <stdio.h>
#include <time.h>
#include <float.h>

#define firstName ("Maciej")
#define lastName ("Bukowski")
#define studentsID 350762
#define titleOfReport ("STATE OF GAS IN TECHNOLOGICAL PROCESS")
#define delimeter ("  |  ")
#define dateWidth 10
#define timeWidth 8
#define temperatureWidth 16
#define massWidth 9
#define pressureWidth 14
#define MOLAR_MASS_OF_NITROGEN 14.01 // [kg/kmol]
#define GAS_CONSTANT 8.314472 // [kJ/kmol/K]
#define VOLUME_OF__GAS 10 // [m3]


struct record
{
	int time;// amount of seconds since 1900th
	double temperature;// [°C]
	double mass;// [kg]
};

double amountOfGas(double massOfGas, double molarMassOfGas)// [kg], [kg/kmol]
{
	return massOfGas / molarMassOfGas;// [kmol]
}

double pressureOfGas(double amountOfGas, double temperature, double volume)// [kmol], [K], [m3]
{
	return (amountOfGas * GAS_CONSTANT * temperature) / (100 * volume);// [hPa]
}

int main(int argc, char* argv[])
{
	char outFileName[] = "report.txt";
	char inFileName[] = "measurements.dta";
	FILE * outFile = fopen(outFileName, "w");
	FILE * inFile = fopen(inFileName, "rb");
	struct record record;
	double maximumPressure = -DBL_MAX;// [hPa]
	double minimumPressure = DBL_MAX;// [hPa]

	time_t t = time(0);
	struct tm tm = *localtime(&t);

	if (outFile && inFile)
	{
		fprintf(outFile,"%s %s %d %02d.%02d.%04d %02d:%02d:%02d\n\n\n", firstName, lastName, studentsID, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
			tm.tm_hour, tm.tm_min,tm.tm_sec);
		fprintf(outFile, "%*s\n\n", (dateWidth + strlen(delimeter) + timeWidth + strlen(delimeter) + temperatureWidth + strlen(delimeter) + massWidth
			+ strlen(delimeter) + pressureWidth - strlen(titleOfReport)) / 2 + strlen(titleOfReport), titleOfReport);
		fprintf(outFile, "%-*s%s%-*s%s%-*s%s%-*s%s%-*s\n", dateWidth, "DATE", delimeter, timeWidth, "TIME", delimeter, temperatureWidth, "TEMPERATURE [°C]",
			delimeter, massWidth, "MASS [kg]", delimeter, pressureWidth, "PRESSURE [hPa]");



		while (fread(&record, sizeof(struct record), 1, inFile))
		{
			double amountOfGas_ = amountOfGas(record.mass, MOLAR_MASS_OF_NITROGEN);
			double pressureOfGas_ = pressureOfGas(amountOfGas_, record.temperature + 273.15, VOLUME_OF__GAS);
			if (pressureOfGas_ < minimumPressure) minimumPressure = pressureOfGas_;
			else if (pressureOfGas_ > maximumPressure) maximumPressure = pressureOfGas_;
			tm = *localtime(&record.time);
			fprintf(outFile, "%02d.%02d.%04d%s%02d:%02d:%02d%s%-*.3f%s%-*.1f%s%-*f\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, delimeter, tm.tm_hour,
				tm.tm_min, tm.tm_sec, delimeter, temperatureWidth, record.temperature, delimeter, massWidth, record.mass, delimeter, pressureWidth, pressureOfGas_);
		}
		


		fprintf(outFile, "%s : %-8f\n%s : %-8f\n", "MINIMUM PRESSURE [hPa]", minimumPressure, "MAXIMUM PRESSURE [hPa]", maximumPressure);
	}
	else printf("%s\n", "Files can not be opened!");

	fclose(inFile);
	fclose(outFile);

	system("pause");
	return 0;
}
