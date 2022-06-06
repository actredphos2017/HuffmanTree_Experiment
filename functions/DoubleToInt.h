#pragma once

unsigned int count_of_decimal_digits(double x){
    unsigned int count = 0;
    while(x - (int)x > 0.000001 || x - (int)x < -0.000001){
        x *= 10;
        count ++;
    }
    return count;
}

vector<int> double_to_int(vector<double> v){
    int i = 0;
    while(x - (int)x > 0.000001 || x - (int)x < -0.000001){
        x *= 10;
        i ++;
    }
    return i;
}