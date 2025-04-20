#include "DiscreteComponents.h"
#include "Component.h"
#include <complex>
#include <optional>
#include <stdexcept>
#include <ginac/ginac.h>

using namespace GiNaC;

// Method to resize symbolic matrix
// This function resizes the matrix to the new dimensions and fills in the new elements with zeros

matrix resize_matrix(const GiNaC::matrix& original, size_t new_rows, size_t new_cols) {
    // Check if we need to resize
    if (original.rows() == new_rows && original.cols() == new_cols) {
        return original; // No need to resize
    }

    // Initialize new matrix with desired parameters
    matrix resized(new_rows, new_cols);

    // Calculate the number of rows and cols to copy
    size_t copy_rows = std::min(static_cast<size_t>(original.rows()), new_rows);
    size_t copy_cols = std::min(static_cast<size_t>(original.cols()), new_cols);

    // Copy
    for (int i = 0; i < copy_rows; i++) {
        for (int j = 0; j < copy_cols; j++) {
            resized(i, j) = original(i, j);
        }
    }

    return resized;
}

// Linear Resistor stamping

void Resistor::stamp(matrix& G, matrix& I, AnalysisType analysis) const {
    int i = getInput()->getIndex(), j = getOutput()->getIndex();

    ex g = 1 / resistance; // Calculate conductance

    // Skip stamping on ground node (idx == -1)
    if (i != -1) G(i, i) += g; // Diagonal stamping
    if (j != -1) G(j, j) += g;
    if (i != -1 && j != -1) {
        G(i, j) -= g; // Off-diagonal stamping
        G(j, i) -= g;
    }
}


// Independent Voltage Source, Table B.6

void VoltageSource::stamp(matrix& G, matrix& I, AnalysisType analysis) const {
    int i = getInput()->getIndex(), j = getOutput()->getIndex();
    int vs_row = G.rows(); // Add a new vairable for current flow

    // Resize the matrices if necessary
    G = resize_matrix(G, vs_row + 1, vs_row + 1);
    I = resize_matrix(I, vs_row + 1, 1);

    // V_i - V_j = voltage
    G(vs_row, i) = 1;
    G(vs_row, j) = -1;
    G(i, vs_row) = 1;
    G(j, vs_row) = -1;
    I(vs_row, 0) = voltage; // Set the voltage source value
}

// Independent Current SourceTable B.8

void CurrentSource::stamp(matrix& G, matrix& I, AnalysisType analysis) const {
    int i = getInput()->getIndex(), j = getOutput()->getIndex();  

    // Skip stamping on ground node (idx == -1)
    if (i != -1) I(i, 0) -= current;  // Current leaves node i
    if (j != -1) I(j, 0) += current;  // Current enters node j
}

// Capacitor (Laplace Domain, Table B.4)

void Capacitor::stamp(matrix& G, matrix& I, AnalysisType analysis) const {
    int i = getInput()->getIndex(), j = getOutput()->getIndex();
    ex Y = GiNaC::symbol("s") * capacitance; // Conductance 1 / Z = s * C where s = j * w is the Laplace variable

    G(i, i) += Y;
    G(j, j) += Y;
    G(i, j) -= Y;
    G(j, i) -= Y;
}

// Inductor (Laplace Domain, Table B.4)

void Inductor::stamp(matrix&G, matrix& I, AnalysisType analysis) const {
    int i = getInput()->getIndex(), j = getOutput()->getIndex();
    ex Y = 1 / (GiNaC::symbol("s") * inductance); // Conductance 1 / Z = 1 / (s * L) where s = j * w is the Laplace variable

    G(i, i) += Y;
    G(j, j) += Y;
    G(i, j) -= Y;
    G(j, i) -= Y;
}