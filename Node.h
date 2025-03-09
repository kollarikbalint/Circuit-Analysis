#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include <ginac/ginac.h>

class Component;

class Node
{
    std::vector<std::weak_ptr<Component>> connections;
    GiNaC::ex potential;
    int index;

public:
    Node() : potential(0.0), index(-1) {}
    
    void addConnection(const std::shared_ptr<Component>& comp);
    void removeConnection(const std::shared_ptr<Component>& comp);
    std::vector<std::shared_ptr<Component>> getConnections() const;

    void setPotential(const GiNaC::ex& newPot) { potential = newPot; }
    GiNaC::ex getPotential() const { return potential; }

    void setIndex(int idx) { index = idx; }
    int getIndex() const { return index; }
};