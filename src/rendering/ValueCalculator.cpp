#include "ValueCalculator.h"

#include <cmath>

int ValueCalculator::linear(int startValue, int endValue, int totalTime, int currentTime) {
    return (currentTime * ((float)(endValue - startValue) / (float)totalTime) + startValue);
}

int ValueCalculator::sinus(int startValue, int endValue, int totalTime, int currentTime) {
    return (((startValue - endValue) / 2) * sin((M_PIl / (totalTime - 0)) * currentTime + ((M_PIl * totalTime) / (0 - totalTime)) - (M_PIl / 2)) + ((float)(startValue + endValue) / 2));
}