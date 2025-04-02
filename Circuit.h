#pragma once
#include "Node.h"
#include "DiscreteComponents.h"
#include "TwoPorts.h"
#include <vector>
#include <ginac/ginac.h>

extern GiNaC::symbol s; // Laplace vairable where s = j * w
extern GiNaC::symbol w; // Omega, angular velocity

class Circuit
{
    std::vector<std::shared_ptr<Component>> components;
    std::vector<std::shared_ptr<Node>> nodes;


public:
    Circuit() = default;

    void addComponent(std::shared_ptr<Component>);
    void addNode(std::shared_ptr<Node>);
    
    void solve();
};
