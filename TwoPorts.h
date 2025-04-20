#pragma once
#include "Node.h"
#include "Component.h"
#include <memory>
#include <ginac/ginac.h>

using namespace GiNaC;

class TwoPort : public CircuitElement
{
	std::string symbol;
	std::shared_ptr<Node> pri_in, sec_in, pri_out, sec_out;
	ex v1, v2, i1, i2;

public:
	TwoPort() : symbol(""), pri_in(nullptr), pri_out(nullptr), sec_in(nullptr), sec_out(nullptr), v1(0.0), v2(0.0), i1(0.0), i2(0.0) {}
    TwoPort(const std::string& sym, std::shared_ptr<Node> inputA, std::shared_ptr<Node> outputA,
        std::shared_ptr<Node> inputB, std::shared_ptr<Node> outputB)
    : symbol(sym), pri_in(inputA), pri_out(outputA), sec_in(inputB), sec_out(outputB) {}

    virtual ~TwoPort() = default;

	ex getVoltage1() const { return v1; }
	ex getVoltage2() const { return v2; }

	ex  getCurrent1() const { return i1; }
	ex  getCurrent2() const { return i2; }

	virtual void setVoltage1(ex volt) { v1 = volt; }
	virtual void setVoltage2(ex volt) { v2 = volt; }

	virtual void setCurrent1(ex curr) { i1 = curr; }
	virtual void setCurrent2(ex curr) { i2 = curr; }

	std::shared_ptr<Node> getPrimaryInput() const { return pri_in; }
	std::shared_ptr<Node> getSecondaryInput() const { return sec_in; }
	std::shared_ptr<Node> getPrimaryOutput() const { return pri_out; }
	std::shared_ptr<Node> getSecondaryOutput() const { return sec_out; }

	virtual void stamp(matrix& G, matrix& I, AnalysisType analysis) const = 0;
};

class Transformer : public TwoPort
{
	// voltage2 = n * voltage1
	// current2 = -current1 / n
	ex ratio;

public:
	Transformer(ex n) : TwoPort(), ratio(n) {}
	Transformer(const std::string& sym, ex n, std::shared_ptr<Node> pri_in,std::shared_ptr<Node> pri_out,
		std::shared_ptr<Node> sec_in, std::shared_ptr<Node> sec_out) {}

	ex getRatio() { return ratio; }
	void setRatio(ex newRatio) {ratio = newRatio; }

	virtual void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;
};

class Girator : public TwoPort
{
	// voltage2 = r * current 1
	// voltage1 = -r * current 2
	ex gyResistance;

public:
	Girator(ex r) : TwoPort(), gyResistance(r) {}
	Girator(const std::string& sym, ex r, std::shared_ptr<Node> pri_in,std::shared_ptr<Node> pri_out,
		std::shared_ptr<Node> sec_in, std::shared_ptr<Node> sec_out) {}

	ex getResistance() { return gyResistance; }
	void setResistance(ex r) { gyResistance = r; }

	virtual void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;
};

// Controlled sources
// A ports are the in & out connectors, B ports are the two poles of the control probe

class ControlledSource : public TwoPort
{
	ex gain;

public:
	ControlledSource(ex control) : TwoPort(), gain(control) {}
	ControlledSource(const std::string& sym, std::shared_ptr<Node> in, std::shared_ptr<Node> out,
		std::shared_ptr<Node> c_in, std::shared_ptr<Node> c_out, ex control) {}
	ex getControlValue() const { return gain; }
	void setControlValue(ex control) { gain = control; }

	virtual ex calculateControlValue();

	virtual void stamp(matrix& G, matrix& I, AnalysisType analysis) const = 0;
};

class VCVS : public ControlledSource
{
	ex outputVoltage;

public:
	VCVS(ex control) : ControlledSource(control) {}
	VCVS(const std::string& sym, std::shared_ptr<Node> in, std::shared_ptr<Node> out,
		std::shared_ptr<Node> c_in, std::shared_ptr<Node> c_out, ex control)
		: ControlledSource("VCVS" + sym, in, out, c_in, c_out, control) {};

	ex getOutputVoltage();
	void setOutputVoltage(ex voltage);

	ex calculateControlValue() override;

	void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;
};

class CCVS : public ControlledSource
{
	ex outputVoltage;

public:
	CCVS(ex control) : ControlledSource(control) {}
	CCVS(const std::string& sym, std::shared_ptr<Node> in, std::shared_ptr<Node> out,
		std::shared_ptr<Node> c_in, std::shared_ptr<Node> c_out, ex control)
		: ControlledSource("CCVS" + sym, in, out, c_in, c_out, control) {};

	ex getOutputVoltage();
	void setOutputVoltage(ex voltage);

	ex calculateControlValue() override;

	virtual void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;
};

class CCCS : public ControlledSource
{
	ex outputCurrent;

public:
	CCCS(ex control) : ControlledSource(control) {}
	CCCS(const std::string& sym, std::shared_ptr<Node> in, std::shared_ptr<Node> out,
		std::shared_ptr<Node> c_in, std::shared_ptr<Node> c_out, ex control)
		: ControlledSource("CCCS" + sym, in, out, c_in, c_out, control) {};

	ex getOutputCurrent();
	void setOutputCurrent(ex current);

	ex calculateControlValue() override;
	
	virtual void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;
};

class VCCS : public ControlledSource
{
	ex outputCurrent;

public:
	VCCS(ex control) : ControlledSource(control) {}
	VCCS(const std::string& sym, std::shared_ptr<Node> in, std::shared_ptr<Node> out,
		std::shared_ptr<Node> c_in, std::shared_ptr<Node> c_out, ex control)
		: ControlledSource("VCCS" + sym, in, out, c_in, c_out, control) {};

	ex getOutputCurrent();
	void setOutputCurrent(ex current);

	ex calculateControlValue() override;

	virtual void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;
};

// Operational Amplifier
// U1 = I1 = 0

class OperationalAmplifier : public TwoPort {
public:
	OperationalAmplifier() : TwoPort() {
		setVoltage1(0.0);
		setCurrent1(0.0);
	}

	OperationalAmplifier(const std::string& sym, std::shared_ptr<Node> inA, std::shared_ptr<Node> outA,
		std::shared_ptr<Node> inB, std::shared_ptr<Node> outB) : TwoPort("A" + sym, inA, outA, inB, outB) {
		setVoltage1(0.0);
		setCurrent1(0.0);
	}

	// We don't want anything modifying these values
	// Override setters to do nothing
	void setVoltage1(ex volt) override {}
	void setCurrent1(ex curr) override {}

	virtual void stamp(matrix& G, matrix& I, AnalysisType analysis) const override;
};
