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
        component->stamp(G, I);
    }
}
