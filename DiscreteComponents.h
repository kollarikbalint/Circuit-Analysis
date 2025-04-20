#pragma once
#include "Component.h"
#include <complex>
#include <stdexcept>
#include <ginac/ginac.h>
#include "Circuit.h"

using namespace GiNaC;

// Utility function used to resize symbolic matrices

matrix resize_matrix(const GiNaC::matrix& original, size_t new_rows, size_t new_cols);

// Linear Resistor

class Resistor : public Component
{
	ex resistance; // Purely resistive, no impedance or imaginary part

public:
	Resistor() : Component(), resistance(ex(0)) {}
    Resistor(const std::string& sym, const ex& res, std::shared_ptr<Node> input = nullptr,
		std::shared_ptr<Node> output = nullptr)
        : Component("R" + sym, input, output), resistance(res) {}

	ex getResistance() const { return resistance; }
    void setResistance(const ex& res) { resistance = res; }

	void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;
};

// Ideal voltage source

class VoltageSource : public Component
{
	ex voltage;

public:
	VoltageSource() : Component(), voltage(ex(0)) {}
	VoltageSource(const std::string& sym, ex &volt ,std::shared_ptr<Node> input = nullptr,
		std::shared_ptr<Node> output = nullptr)
        : Component("V" + sym, input, output), voltage(volt) {}

	ex getVoltage() const { return voltage; }
	void setVoltage(ex volt) { voltage = volt; }

	void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;

};

class CurrentSource : public Component
{
	ex current;

public:
	CurrentSource() : Component(), current(ex(0)) {}
	CurrentSource(const std::string& sym, ex &curr, std::shared_ptr<Node> input = nullptr,
		std::shared_ptr<Node> output = nullptr)
		: Component("I" + sym, input, output), current(curr) {}
	ex getCurrent() { return current; }
	void setCurrent(ex curr) { current = curr; }

	void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;
};



// Virtual base class for dynamic components

class DynamicComponent : public Component
{
	ex impedance; // s*L, 1/(s*C), where s = j*w

public:
	DynamicComponent() : Component(), impedance(ex(0)) {}
	DynamicComponent(const std::string& sym, const ex& imp, std::shared_ptr<Node> input = nullptr,
		std::shared_ptr<Node> output = nullptr)
		: Component(sym, input, output), impedance(imp) {}
	~DynamicComponent() override = default;

	ex getImpedance() const { return impedance; }
	void setImpedance(const ex& imp) { impedance = imp; }

	void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;
};

class Capacitor : public DynamicComponent
{	
	ex capacitance;

public:
	Capacitor() : DynamicComponent(), capacitance(ex(0)) {}
	Capacitor(const std::string& sym, const ex& C, std::shared_ptr<Node> input = nullptr,
		std::shared_ptr<Node> output = nullptr)
		: DynamicComponent("C" + sym, 1 / (s * C), input, output),
		capacitance(C) {}
	
	ex getCapacitance() { return capacitance; }
	void setCapacitance(ex& C) { capacitance = C; }

	// AC stamping for MNA
	void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;
};

class Inductor : public DynamicComponent
{
	ex inductance;

public:
	Inductor() : DynamicComponent(), inductance(ex(0)) {}
	Inductor(const std::string& sym, const ex& L, std::shared_ptr<Node> input = nullptr,
		std::shared_ptr<Node> output = nullptr)
		: DynamicComponent("L" + sym, s * L, input, output),
		inductance(L) {}

	ex getInductance() { return inductance; }
	void setInductance(ex& ind) { inductance = ind; }

	void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;
};
