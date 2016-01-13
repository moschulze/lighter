#include "ValueCalculator.h"
#include "../scene/SceneStep.h"

#include <cmath>

int ValueCalculator::linear(int startValue, int endValue, int totalTime, int currentTime) {
    return (currentTime * ((float)(endValue - startValue) / (float)totalTime) + startValue);
}

int ValueCalculator::sinus(int startValue, int endValue, int totalTime, int currentTime) {
    return (((startValue - endValue) / 2) * sin((M_PIl / (totalTime - 0)) * currentTime + ((M_PIl * totalTime) / (0 - totalTime)) - (M_PIl / 2)) + ((float)(startValue + endValue) / 2));
}

int ValueCalculator::calculate(int startValue, int endValue, int totalTime, int currentTime, int type) {
    switch(type) {
        case SceneStep::FADE_SINUS:
            return ValueCalculator::linear(startValue, endValue, totalTime, currentTime);
        default:
            return ValueCalculator::linear(startValue, endValue, totalTime, currentTime);
    }
}
