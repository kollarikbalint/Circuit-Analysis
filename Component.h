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
	std::string symbol;

public:
    Component() : in(nullptr), out(nullptr), symbol("") {}
	Component(std::string const sym,std::shared_ptr<Node> input = nullptr, std::shared_ptr<Node> output = nullptr)
		: in(input), out(output), symbol(sym) {}

	std::shared_ptr<Node> getInput() const  { return in; }
	std::shared_ptr<Node> getOutput() const { return out; }

	void setInput(std::shared_ptr<Node> input) { in = input; }
	void setOutput(std::shared_ptr<Node> output) { out = output; }

	std::string getSym() const { return symbol; }
    void setSym(const std::string& sym) { symbol = sym; }
	
	// Matrix stamping function as MNA
	virtual void stamp(matrix& G, matrix& I) const = 0;
};