#include "Node.h"
#include "TwoPorts.h"
    
double VCVS::calculateControlValue() {
    return getOutB()->getPotential() - getInB()->getPotential();
}

double VCCS::calculateControlValue() {
    return getOutB()->getPotential() - getInB()->getPotential();
}

double CCVS::calculateControlValue() {
    
}