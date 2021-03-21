#include <stdlib.h>
#include <time.h>

#include "LivelyMath.h"



std::vector<double> livelyRD::linSpace(int numberOfPoints, double lowerBound, double upperBound)
{
    std::vector<double> result;
    if(numberOfPoints == 0)
    {
        result = {lowerBound};
    }
    else if(numberOfPoints == 1)
    {
        result = {upperBound};
    } else
    {
        result.resize(numberOfPoints);
        double stepSize = (upperBound - lowerBound)/(numberOfPoints - 1);
        result[0] = lowerBound;
        for(int i = 1; i<numberOfPoints; i++)
        {
            result[i] = result[i-1] + stepSize;
        }
    }

    return result;
}