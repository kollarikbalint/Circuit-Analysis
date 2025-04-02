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
    // Ensure each new node gets a unique index
    Node() 
        : potential(ex(0)) 
    {
        if (globalIndex == 1) { // Idx = 0 is reserved to the ground node and only one can be in existance
            index = 0; // Actual ground
        } else {
            index = globalIndex++;
        }
    }

    void addConnection(const std::shared_ptr<Component>& comp);
    void removeConnection(const std::shared_ptr<Component>& comp);
    std::vector<std::shared_ptr<Component>> getConnections() const;

    void setPotential(const ex& newPot) { potential = newPot; }
    ex getPotential() const { return potential; }

    void setIndex(int idx) { 
        if (index == 0) {
            throw std::logic_error("Cannot modify the index of the ground node."); // Don't touch the ground
        }
        index = idx; 
    }
    int getIndex() const { return index; }
};

int Node::globalIndex = 1; // Initialize global index to 1
