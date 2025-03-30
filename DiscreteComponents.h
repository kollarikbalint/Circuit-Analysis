#pragma once
#include "Component.h"
#include <complex>
#include <stdexcept>
#include <ginac/ginac.h>

using namespace GiNaC;

class Resistor : public Component
{
	ex resistance;

public:
    Resistor(const ex& res = 1.0,
            const std::string& sym = "",
            std::shared_ptr<Node> input = nullptr,
            std::shared_ptr<Node> output = nullptr)
        : Component(sym, input, output, 0), resistance(res) {}

	ex getResistance() const { return resistance; }
    void setResistance(const ex& res) { resistance = res; }

	void stamp(matrix& G, matrix& I) const override;
};

class VoltageSource : public Component
{
	ex voltage;

public:
	VoltageSource(ex volt, const std::string& sym = "", std::shared_ptr<Node> input = nullptr, std::shared_ptr<Node> output = nullptr)
        : Component(sym, input, output), voltage(volt) {}

	ex getVoltage() const { return voltage; }
	void setVoltage(ex volt) { voltage = volt; }
};

class CurrentSource : public Component
{
	ex current;

public:
	CurrentSource(ex curr, const std::string sym) : Component(sym), current(curr) {}
	ex getCurrent() { return current; }
	void setCurrent(ex curr) { current = curr; }
};



// Dynamic components, for dynamic circuits and AC analysis

class DynamicComponent : public Component
{
	ex impedance; // s*L, 1/(s*C), where s = j*w

public:
	DynamicComponent(const ex imp, const std::string& sym) 
		: Component(sym), impedance(imp) {}

	ex getImpedance() const { return impedance; }
	void setImpedance(ex& imp) { impedance = imp; }
};

class Capacitor : public DynamicComponent
{	
	ex capacitance;

public:
	Capacitor(const ex& imp, const ex& C, const std::string& sym)
		: DynamicComponent(1/(GiNaC::symbol("s") * C), sym), capacitance(C) {}
	
	ex getCapacitance() { return capacitance; }
	void setCapacitance(ex& C) { capacitance = C; }

	// AC stamping for MNA
	void stamp(matrix& G, matrix& I) const;
};

class Inductor : public DynamicComponent
{
	ex inductance;

public:
	Inductor(const ex& L, const std::string& sym)
        : DynamicComponent(GiNaC::symbol("s") * L, sym), inductance(L) {}

	ex getInductance() { return inductance; }
	void setInductance(ex& ind) { inductance = ind; }
};