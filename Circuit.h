#pragma once
#include "Node.h"
#include "DiscreteComponents.h"
#include "TwoPorts.h"
#include <vector>
#include <ginac/ginac.h>

enum class AnalysisType
{
    DC,
    AC,
    Transient
};

extern GiNaC::symbol s; // Laplace vairable where s = j * w
extern GiNaC::symbol w; // Omega, angular velocity

class Circuit
{
    std::vector<std::shared_ptr<Component>> components;
    std::vector<std::shared_ptr<Node>> nodes;
    AnalysisType analysisType = AnalysisType::DC;

public:
    Circuit() = default;

    void addComponent(std::shared_ptr<Component>);
    void addNode(std::shared_ptr<Node>);
    void connect(std::shared_ptr<Component>, std::shared_ptr<Component>);
    void setAnalysisType(AnalysisType type) { analysisType = type; }
    
    void solve();
};
