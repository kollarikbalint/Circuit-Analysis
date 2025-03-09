#include "DiscreteComponents.h"
#include "Component.h"
#include <complex>
#include <optional>
#include <stdexcept>
#include <ginac/ginac.h>

void Resistor::stamp(GiNaC::matrix& G, GiNaC::matrix& I) const {
    const int n1 = getInput()->getIndex();
    const int n2 = getOutput()->getIndex();
        
    if(n1 >= 0) G(n1, n1) += 1/resistance;
    if(n2 >= 0) G(n2, n2) += 1/resistance;
    if(n1 >= 0 && n2 >= 0) {
        G(n1, n2) -= 1/resistance;
        G(n2, n1) -= 1/resistance;
    }
}

void Capacitor::stamp(GiNaC::matrix& G, GiNaC::matrix& I) const {
    GiNaC::ex Z_C = getImpedance();  // 1/(s*C)
    GiNaC::ex Y_C = 1/Z_C;           // Admittance = s*C

    const int n1 = getInput()->getIndex();
    const int n2 = getOutput()->getIndex();

    if (n1 >= 0) G(n1, n1) += Y_C;
    if (n2 >= 0) G(n2, n2) += Y_C;
    if (n1 >= 0 && n2 >= 0) {
        G(n1, n2) -= Y_C;
        G(n2, n1) -= Y_C;
    }
}