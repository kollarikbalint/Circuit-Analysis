#pragma once
#include "Node.h"
#include "Component.h"
#include <memory>
#include <ginac/ginac.h>

using namespace GiNaC;

class TwoPort : CircuitElement
{
	std::shared_ptr<Node> in1, in2, out1, out2;

	ex v1, v2, i1, i2;

public:
	TwoPort() : input1(nullptr), output1(nullptr), input2(nullptr), output2(nullptr), voltage1(0.0), voltage2(0.0), current1(0.0), current2(0.0) {}
    TwoPort(std::shared_ptr<Node> inputA, std::shared_ptr<Node> outputA, std::shared_ptr<Node> inputB, std::shared_ptr<Node> outputB)
        : input1(inputA), output1(outputA), input2(inputB), output2(outputB), voltage1(0.0), voltage2(0.0), current1(0.0), current2(0.0) {}

    virtual ~TwoPort() = default; // Virtual destructor for polymorphism

	ex getVoltage1() const { return v1; }
	ex getVoltage2() const { return v2; }

	ex  getCurrent1() const { return i1; }
	ex  getCurrent2() const { return i2; }

	void setVoltage1(ex volt) { v1 = volt; }
	void setVoltage2(ex volt) { v2 = volt; }

	void setCurrent1(ex curr) { i1 = curr; }
	void setCurrent2(ex curr) { i2 = curr; }

	std::shared_ptr<Node> getInA() const { return in1; }
    std::shared_ptr<Node> getInB() const { return in2; }
    std::shared_ptr<Node> getOutput1() const { return out1; }
    std::shared_ptr<Node> getOutput2() const { return out2; }

	virtual GiNaC::ex getParameter() const = 0;
};

class Transformator : public TwoPort
{				  // ratio = n2 / n1
	double ratio; // voltage2 = n * voltage1
				  // current2 = -current1 / n
public:
	Transformator() : ratio(1) { }
	Transformator(double n) : ratio(n) { }
	double getRatio() { return ratio; }
	void setRatio(double newRatio) {ratio = newRatio; }
};

class Girator : public TwoPort
{
	double gyResistance; // voltage2 = r * current1
						 // voltage1 = -r * current2
public:
	Girator(double r) : gyResistance(r) {}
	double getResistance() { return gyResistance; }
	void setResistance(double r) { gyResistance = r; }
};

// Vezérelt források
// A oldal a forrás ki és bemente, B oldal pedig a vezérlő probe két vége
class ControlledSource : public TwoPort
{
	double controlValue; // A vezérlő érték

public:
	ControlledSource(double control);
	double getControlValue();
	void setControlValue(double control);

	virtual double calculateControlValue();
};

class VCVS : public ControlledSource
{
	double outputVoltage; // Kimeneti feszültség

public:
	VCVS(double control, double output);
	double getOutputVoltage();
	void setOutputVoltage(double voltage);

	double calculateControlValue() override;
};

class CCVS : public ControlledSource
{
	double outputVoltage; // Kimeneti feszültség

public:
	CCVS(double control, double output);
	double getOutputVoltage();
	void setOutputVoltage(double voltage);

	double calculateControlValue() override;
};

class CCCS : public ControlledSource
{
	double outputCurrent; // Kimeneti áram

public:
	CCCS(double control, double output);
	double getOutputCurrent();
	void setOutputCurrent(double current);

	double calculateControlValue() override;
};

class VCCS : public ControlledSource
{
	double outputCurrent; // Kimeneti áram

public:
	VCCS(double control, double output);
	double getOutputCurrent();
	void setOutputCurrent(double current);

	double calculateControlValue() override;
};

class OperationalAmplifier : public TwoPort {
public:
	OperationalAmplifier() {
		setVoltage1(0.0);
		setCurrent1(0.0);
	}
};