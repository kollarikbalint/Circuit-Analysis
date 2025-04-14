#include "Node.h"
#include "TwoPorts.h"
#include "ginac/ginac.h"
#include "DiscreteComponents.h"
#include "Circuit.h"
#include <string>

// Ideal transformer stamping Table B.11

void Transformer::stamp(matrix& G, matrix& I) const {
    int pri_in = getPrimaryInput()->getIndex(), pri_out = getPrimaryOutput()->getIndex();
    int sec_in = getSecondaryInput()->getIndex(), sec_out = getSecondaryOutput()->getIndex();
    int row = G.rows();

    // Resize matrices to fit I1 I2
    G = resize_matrix(G, row + 1, row + 1);
    I = resize_matrix(I, row + 1, 1);

    // V_pri - r * V_sec = 0
    G(pri_in, pri_in) = 1;
    G(pri_in, sec_out) -= ratio;

    // I_sec + I_pri = 0
    G(sec_out, pri_in) += 1 / ratio;
    G(sec_out, sec_out) += 1;
}

// Operational Amplifier stamping

void OperationalAmplifier::stamp(matrix& G, matrix& I) const {
    int in_p = getPrimaryInput()->getIndex(), in_n = getSecondaryInput()->getIndex();
    int out = getPrimaryOutput()->getIndex();

    
    G(out, in_p) += 1;
    G(out, in_n) -= 1;

    // I_out = 0
}

// Girator Stamping
// V2 = r * I1, V1 = -r * I2

void Girator::stamp(matrix& G, matrix& I) const {
    int pri_in = getPrimaryInput()->getIndex(), pri_out = getPrimaryOutput()->getIndex();
    int sec_in = getSecondaryInput()->getIndex(), sec_out = getSecondaryOutput()->getIndex();

    // Resize matrices to accommodate additional variables
    int row = G.rows();
    G = resize_matrix(G, row + 2, row + 2);
    I = resize_matrix(I, row + 2, 1);

    // V2 = r * I1
    G(row, pri_in) += 1;
    G(row, pri_out) -= 1;
    G(row, row + 1) -= gyResistance;

    // V1 = -r * I2
    G(row + 1, sec_in) += 1;
    G(row + 1, sec_out) -= 1;
    G(row + 1, row) += gyResistance;
}  

// Voltage Controlled Voltage Source stamping Table B.13 
// V_out = g * V_control

void VCVS::stamp(matrix& G, matrix& I) const {
    int c_in = getSecondaryInput()->getIndex(), c_out = getSecondaryOutput()->getIndex();
    int in = getPrimaryInput()->getIndex(), out = getSecondaryOutput()->getIndex();


    // Resize matrices for the new variable
    int vs_row = G.rows();
    G = resize_matrix(G, vs_row + 1, vs_row + 1);
    I = resize_matrix(I, vs_row + 1, 1);

    ex g = getControlValue();

    // Characteristics
    G(vs_row, in) += 1;
    G(vs_row, out) -= 1;
    G(vs_row, c_in) -= g;
    G(vs_row, c_out) += g;

    // Current
    G(in, vs_row) = 1;
    G(out, vs_row) = -1;
}

// CCVS stamping
// V_out = h * I_control

void CCVS::stamp(matrix& G, matrix& I) const {
    int c_in = getSecondaryInput()->getIndex(), c_out = getSecondaryOutput()->getIndex();
    int in = getPrimaryInput()->getIndex(), out = getPrimaryOutput()->getIndex();

    // Resize matrices for the new variable
    int vs_row = G.rows();
    G = resize_matrix(G, vs_row + 1, vs_row + 1);
    I = resize_matrix(I, vs_row + 1, 1);

    ex h = getControlValue();

    // V_out = h * I_control
    G(vs_row, in) += 1;
    G(vs_row, out) -= 1;
    G(vs_row, vs_row) -= h;

    G(c_in, vs_row) += 1;
    G(c_out, vs_row) -= 1;
}

// I_out = g * ( V_c_in - V_c_out )

void VCCS::stamp(matrix& G, matrix& I) const {
    int c_in = getSecondaryInput()->getIndex(), c_out = getSecondaryOutput()->getIndex();
    int in = getPrimaryInput()->getIndex(), out = getPrimaryOutput()->getIndex();

    ex g = getControlValue();

    // Resize matrices
    int vs_row = G.rows();
    G = resize_matrix(G, vs_row + 1, vs_row + 1);
    I = resize_matrix(I, vs_row + 1, 1);

    G(in, c_in) += g;
    G(in, c_out) -= g;
    G(out, c_in) -= g;
    G(out, c_out) += g;
}

// I_out = h * I_control

void CCCS::stamp(matrix& G, matrix& I) const {
    int c_in = getSecondaryInput()->getIndex(), c_out = getSecondaryOutput()->getIndex();
    int in = getPrimaryInput()->getIndex(), out = getPrimaryOutput()->getIndex();

    ex h = getControlValue();
    int aux_row = G.rows();

    G = resize_matrix(G, aux_row + 1, aux_row + 1);
    I = resize_matrix(I, aux_row + 1, 1);

    G(aux_row, c_in) += 1;
    G(aux_row, c_out) -= 1;

    G(in, aux_row) += h;
    G(out, aux_row) -= h;
}
