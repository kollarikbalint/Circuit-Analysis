#include "Circuit.h"
#include "ginac/ginac.h"

using namespace GiNaC;

GiNaC::symbol s("s"); // Laplace variable
GiNaC::symbol w("w"); // Angular velocity

void Circuit::addComponent(std::shared_ptr<Component> component) {
    components.push_back(component);
}

void Circuit::addNode(std::shared_ptr<Node> node) {
    nodes.push_back(node);
}

void Circuit::solve() {
    // Initialize conductance matrix and current vector
    size_t nodes_count = nodes.size() - 1; // Exclude ground node
    matrix G(nodes_count, nodes_count);
    matrix I(nodes_count, 1);

    for (const auto& component : components) {
        component->stamp(G, I, analysisType); // Pass analysis type
    }

    // Handle DC (real matrices) or AC (substitute s = jω)
    if (analysisType == AnalysisType::AC) {
        exmap sub_map;
        sub_map[s] = GiNaC::I * w; // Use GiNaC's predefined imaginary unit
        G = ex_to<matrix>(G.subs(sub_map));
        I = ex_to<matrix>(I.subs(sub_map));
    }

    // TODO: Solve G*x = I using GiNaC's linear algebra tools
}
