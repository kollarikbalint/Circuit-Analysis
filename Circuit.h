#pragma once
#include "Node.h"
#include "DiscreteComponents.h"
#include "TwoPorts.h"
#include <vector>
#include <ginac/ginac.h>


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

static GiNaC::symbol s("s");  // Laplace variable
static GiNaC::symbol w("w");  // Angular frequency