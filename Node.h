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
    std::string symbol;
    int index;
    static int globalIndex; // Keeping global count of nodes for indexing

    static std::shared_ptr<Node> groundNode; // Singleton

public:
    static std::shared_ptr<Node> getGround() {
        if (!groundNode) {
            groundNode = std::shared_ptr<Node>(new Node()); // Initialize ground node
            groundNode->index = -1; // Give ground node idx -1
            groundNode->potential = ex(0); // Set potential to 0
        }
        return groundNode;
    }

    // Regular node constructor
    Node() : index(globalIndex++) {
        symbol = "V" + std::to_string(index);
    }

    void addConnection(const std::shared_ptr<Component>& comp);
    void removeConnection(const std::shared_ptr<Component>& comp);
    std::vector<std::shared_ptr<Component>> getConnections() const;

    void setPotential(const ex& newPot) { potential = newPot; }
    ex getPotential() const { return potential; }

    void setIndex(int idx) { 
        if (index == -1) {
            throw std::logic_error("Cannot modify the index of the ground node."); // Don't touch the ground
        }
        index = idx; 
    }
    int getIndex() const { return index; }
};
