#pragma once
#include "Node.h"
#include <string>
#include <memory>
#include <ginac/ginac.h>

using namespace GiNaC;

class CircuitElement {
public:
	virtual ~CircuitElement() = default;
	virtual void stamp(matrix &G, matrix I) const = 0;
};

class Component : public CircuitElement
{
	std::shared_ptr<Node> in, out;
	ex voltage;
	std::string symbol;

public:
    Component(const std::string& sym = "", 
             std::shared_ptr<Node> input = nullptr,
             std::shared_ptr<Node> output = nullptr,
             const ex& pot = 0.0)
        : symbol(sym), in(input), out(output), voltage(pot) {}

	virtual ~Component() = default; // Virtual destructor for polymorphism

	// Get-ers and Set-ers
	std::shared_ptr<Node> getInput() const  { return in; }
	std::shared_ptr<Node> getOutput() const { return out; }

	void setInput(std::shared_ptr<Node> input) { in = input; }
	void setOutput(std::shared_ptr<Node> output) { out = output; }

	std::string getSym() const { return symbol; }
    void setSym(const std::string& sym) { symbol = sym; }

	ex getVoltage() const { return voltage; }
    void setVoltage(const ex& pot) { voltage = pot; }

	// Automatic derived smybol generation by GiNaC
    GiNaC::symbol getVoltageSymbol() const {
        return GiNaC::symbol(symbol + "_V");
    }
	
	// Matrix stamping black magic
	virtual void stamp(matrix& G, matrix& I) const = 0;
};