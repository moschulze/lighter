#ifndef LIGHTER_VALUEINTERPOLATOR_H
#define LIGHTER_VALUEINTERPOLATOR_H


class ValueCalculator {
public:
    static int linear(int startValue, int endValue, int totalTime, int currentTime);
    static int sinus(int startValue, int endValue, int totalTime, int currentTime);
};


#endif //LIGHTER_VALUEINTERPOLATOR_H
