#pragma once
#include "Node.h"
#include "Component.h"
#include <memory>
#include <ginac/ginac.h>

using namespace GiNaC;

class TwoPort : public CircuitElement
{
	std::shared_ptr<Node> in1, in2, out1, out2;

	ex v1, v2, i1, i2;

public:
	TwoPort() : in1(nullptr), out1(nullptr), in2(nullptr), out2(nullptr), v1(0.0), v2(0.0), i1(0.0), i2(0.0) {}
    TwoPort(std::shared_ptr<Node> inputA, std::shared_ptr<Node> outputA,
        std::shared_ptr<Node> inputB, std::shared_ptr<Node> outputB)
    : in1(inputA), out1(outputA), in2(inputB), out2(outputB) {}

    virtual ~TwoPort() = default;

	ex getVoltage1() const { return v1; }
	ex getVoltage2() const { return v2; }

	ex  getCurrent1() const { return i1; }
	ex  getCurrent2() const { return i2; }

	virtual void setVoltage1(ex volt) { v1 = volt; }
	virtual void setVoltage2(ex volt) { v2 = volt; }

	virtual void setCurrent1(ex curr) { i1 = curr; }
	virtual void setCurrent2(ex curr) { i2 = curr; }

	std::shared_ptr<Node> getInA() const { return in1; }
    std::shared_ptr<Node> getInB() const { return in2; }
    std::shared_ptr<Node> getOutput1() const { return out1; }
    std::shared_ptr<Node> getOutput2() const { return out2; }

	virtual ex getParameter() const = 0;
};

class Transformer : public TwoPort
{
	// voltage2 = n * voltage1
	// current2 = -current1 / n
	ex ratio;

public:
	Transformer() : ratio(1) { }
	Transformer(ex n) : ratio(n) { }
	ex getRatio() { return ratio; }
	void setRatio(ex newRatio) {ratio = newRatio; }
};

class Girator : public TwoPort
{
	ex gyResistance; 	 // voltage2 = r * current1
						 		 // voltage1 = -r * current2
public:
	Girator(ex r) : gyResistance(r) {}
	ex getResistance() { return gyResistance; }
	void setResistance(ex r) { gyResistance = r; }
};

// Controlled sources
// A ports are the in & out connectors, B ports are the two poles of the control probe
class ControlledSource : public TwoPort
{
	ex controlValue; // We need to calculate this for each crcuit on the fly
							// Needs to be symbolical, can become convoluted depending on the circuit

public:
	ControlledSource(ex control);
	ex getControlValue();
	void setControlValue(ex control);

	virtual ex calculateControlValue();
};

class VCVS : public ControlledSource
{
	ex outputVoltage;

public:
	VCVS(ex control, ex output);
	ex getOutputVoltage();
	void setOutputVoltage(ex voltage);

	ex calculateControlValue() override;
};

class CCVS : public ControlledSource
{
	ex outputVoltage;

public:
	CCVS(ex control, ex output);
	ex getOutputVoltage();
	void setOutputVoltage(ex voltage);

	ex calculateControlValue() override;
};

class CCCS : public ControlledSource
{
	ex outputCurrent;

public:
	CCCS(ex control, ex output);
	ex getOutputCurrent();
	void setOutputCurrent(ex current);

	ex calculateControlValue() override;
};

class VCCS : public ControlledSource
{
	ex outputCurrent;

public:
	VCCS(ex control, ex output);
	ex getOutputCurrent();
	void setOutputCurrent(ex current);

	ex calculateControlValue() override;
};

class OperationalAmplifier : public TwoPort {
public:
	OperationalAmplifier() {
		setVoltage1(0.0);
		setCurrent1(0.0);
	}

	// We don't want anything modifying these values
	// Override setters to do nothing
	
	void setVoltage1(ex volt) override {}
	void setCurrent1(ex curr) override {}
};
