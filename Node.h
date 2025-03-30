#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include <ginac/ginac.h>

using namespace GiNaC;

class Component; // Forward declaration

class Node
{
    std::vector<std::weak_ptr<Component>> connections;
    ex potential;
    int index;
    static int globalIndex; // Keeping global count of nodes for indexing

public:
    // General constructor for new nodes, making sure to assign a unique index
    Node() : potential(ex(0)), index(globalIndex++) {}

    // Ground node constructor
    // Starting point of MNA
    static Node createGroundNode() {
        Node groundNode;
        groundNode.setPotential(ex(0)); // Ground node potential is always 0
        return groundNode;
    }

    void addConnection(const std::shared_ptr<Component>& comp);
    void removeConnection(const std::shared_ptr<Component>& comp);
    std::vector<std::shared_ptr<Component>> getConnections() const;

    void setPotential(const ex& newPot) { potential = newPot; }
    ex getPotential() const { return potential; }

    void setIndex(int idx) { index = idx; }
    int getIndex() const { return index; }
};

int Node::globalIndex = 1; // Initialize global index to 1
