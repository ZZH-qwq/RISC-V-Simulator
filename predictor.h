#pragma once
#include "definitions.h"
#include <vector>

class Predictor {
    int data = 1;

   public:
    bool result() { return data & 2; }
    // ++Predictor
    Predictor &operator++() {
        if (data != 3) data++;
        return (*this);
    }
    // Predictor++
    Predictor operator++(int) {
        if (data != 3) data++;
        return (*this);
    }
    // --Predictor
    Predictor &operator--() {
        if (data != 0) data--;
        return (*this);
    }
    // Predictor--
    Predictor operator--(int) {
        if (data != 0) data--;
        return (*this);
    }
};

vector<Predictor> predictors(1024);