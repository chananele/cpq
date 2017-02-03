#pragma once

namespace cpq {

class INumber {
public:

	enum number_type {
		INT,
		REAL,
	};

	virtual number_type type() const = 0;
};

template <class T>
class Value
{
public:
	Value(T value)
		: m_value(value)
	{}

	T value() const {
		return m_value;
	}

private:
	T m_value;
};

class Int : public INumber, public Value<int> {
public:
	Int(int value)
		: Value(value)
	{}

	virtual number_type type() const override {
		return INT;
	}
};

class Real : public INumber, public Value<float> {
public:
	Real(float value)
		: Value(value)
	{}

	virtual number_type type() const override {
		return REAL;
	}
};

}
