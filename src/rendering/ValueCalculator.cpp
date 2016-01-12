#include "ValueCalculator.h"

int ValueCalculator::linear(int startValue, int endValue, int totalTime, int currentTime) {
    return (currentTime * ((float)(endValue - startValue) / (float)totalTime) + startValue);
}