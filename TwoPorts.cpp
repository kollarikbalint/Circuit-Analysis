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

    // Primary side: V_pri = r * V_sec
    G(pri_in, pri_in) += 1;
    G(pri_in, sec_in) -= ratio;

    // Secondary side: I_sec = -I_pri / r
    G(sec_out, pri_in) = -1 / ratio;
    G(sec_out, sec_out) += 1;
}

// Operational Amplifier stamping

void OperationalAmplifier::stamp(matrix& G, matrix& I) const {
    int in_p = getPrimaryInput()->getIndex(), in_n = getSecondaryInput()->getIndex();
    int out = getPrimaryInput()->getIndex();

    
    G(out, in_p) = 1;
    G(out, in_n) = -1;
}

// Girator Stamping
// V2 = r * I1, V1 = -r * I2

void Girator::stamp(matrix& G, matrix& I) const {
    int pri_in = getPrimaryInput()->getIndex(), pri_out = getPrimaryOutput()->getIndex();
    int sec_in = getSecondaryInput()->getIndex(), sec_out = getSecondaryOutput()->getIndex();
    int row = G.rows();

    // Resize matricesw to fit I1 I2
    G = resize_matrix(G, row + 2, row + 2);
    I = resize_matrix(I, row + 2, 1);

    // I1: V_sec = r * I1
    G(row, pri_in) = 1;
    G(row, pri_out) = -1;
    G(row, row + 1) = -gyResistance;
    G(sec_in, row) = 1;
    G(sec_out, row) = -1;

    // I2: V_pri = -r * I2
    G(row + 1, sec_in) = 1;
    G(row + 1, sec_out) = -1;
    G(row + 1, row) = gyResistance;
    G(pri_in, row + 1) = 1;
    G(pri_out, row + 1) = -1;
}   

// Voltage Controlled Voltage Source stamping Table B.13 
// V_out = g * V_control

void VCVS::stamp(matrix& G, matrix& I) const {
    int c_in = getSecondaryInput()->getIndex(), c_out = getSecondaryOutput()->getIndex();
    int in = getPrimaryInput()->getIndex(), out = getSecondaryOutput()->getIndex();


    // Resize matrices
    G = resize_matrix(G, G.rows() + 1, G.rows() + 1);
    I = resize_matrix(I, G.rows() + 1, 1);
    int vs_row = G.rows();

    G(vs_row, in) = 1;
    G(vs_row, out) = -1;
    G(vs_row, c_in) = -getControlValue();
    G(vs_row, c_out) = getControlValue();
    G(in, vs_row) = 1;
    G(out, vs_row) = -1;
}

// CCVS stamping
// V_out = h * I_control

void CCVS::stamp(matrix& G, matrix& I) const {
    int c_in = getSecondaryInput()->getIndex(), c_out = getSecondaryOutput()->getIndex();
    int in = getPrimaryInput()->getIndex(), out = getPrimaryOutput()->getIndex();
    int vs_row = G.rows();

    G = resize_matrix(G, vs_row + 1, vs_row + 1);
    I = resize_matrix(I, vs_row + 1, 1);

    // V_out = h * I_control
    G(vs_row, in) = 1;
    G(vs_row, out) = -1;
    G(vs_row, vs_row) = -getControlValue();
    G(c_in, vs_row) = 1;
    G(c_out, vs_row) = -1;
}
