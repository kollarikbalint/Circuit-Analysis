#pragma once
#include "Node.h"
#include <string>
#include <memory>
#include <ginac/ginac.h>

using namespace GiNaC;

class CircuitElement {
public:
	CircuitElement() = default;
	virtual ~CircuitElement() = default;
	virtual void stamp(matrix &G, matrix I) const = 0;
};

class Component : public CircuitElement
{
	std::shared_ptr<Node> in, out;
	ex voltage;
	std::string symbol;

public:
    Component() : in(nullptr), out(nullptr), voltage(0), symbol("") {}
	Component(std::string const sym,std::shared_ptr<Node> input = nullptr, std::shared_ptr<Node> output = nullptr)
		: in(input), out(output), voltage(ex(0.0)), symbol(sym) {}

	virtual ~Component() = default; // Virtual destructor for polymorphism

	std::shared_ptr<Node> getInput() const  { return in; }
	std::shared_ptr<Node> getOutput() const { return out; }

	void setInput(std::shared_ptr<Node> input) { in = input; }
	void setOutput(std::shared_ptr<Node> output) { out = output; }

	std::string getSym() const { return symbol; }
    void setSym(const std::string& sym) { symbol = sym; }

	ex getVoltage() const { return voltage; }
    void setVoltage(const ex& pot) { voltage = pot; }
	
	// Matrix stamping function as MNA
	virtual void stamp(matrix& G, matrix& I) const = 0;
};