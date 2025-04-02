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

    static std::shared_ptr<Node> groundNode; // Singleton ground node

public:
    static std::shared_ptr<Node> getGround() {
        if (!groundNode) {
            groundNode = std::shared_ptr<Node>(new Node()); // Initialize ground node
            groundNode->index = 0; // Give ground node idx 0 
        }
        return groundNode;
    }

    // Regular node constructor
    Node() : potential(ex(0)) {
        if (globalIndex == 0) {
            index = 0; // Redundancy
            throw std::logic_error("Use Node::getGround() to get the ground node.");
        } else {
            index = globalIndex++; // Assign a unique index to each node
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

// Static member initialization

std::shared_ptr<Node> Node::groundNode = nullptr; // Initialize ground node to nullptr
int Node::globalIndex = 1; // Initialize global index to 1
