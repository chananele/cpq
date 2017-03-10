#pragma once

#include <fstream>

#include "symbol.hh"

namespace cpq {

class Instruction {
public:
	virtual void generate(std::ostream& os) const = 0;
};

class Cast : public Instruction {
public:

	static const std::string instruction[];

	Cast(symbol_type_e type, const std::string& var, const std::string val)
		: m_type(type)
		, m_var(var)
		, m_val(val)
	{}

	virtual void generate(std::ostream& os) const override {
		os << instruction[m_type] << " ";
		os << m_var << " " << m_val << std::endl;
	}

private:
	const symbol_type_e m_type;
	const std::string	m_var;
	const std::string	m_val;
};

class Assign : public Instruction {
public:

	static const std::string instruction[];

	Assign(symbol_type_e type, const std::string& var, const std::string& val)
		: m_type(type)
		, m_var(var)
		, m_val(val)
	{}

	virtual void generate(std::ostream& os) const override {
		os << instruction[m_type] << " ";
		os << m_var << " " << m_val << std::endl;
	}

private:
	const symbol_type_e m_type;
	const std::string	m_var;
	const std::string	m_val;
};

template <class T>
class BinaryOperation : public Instruction {
public:
	BinaryOperation(
		symbol_type_e type,
		const std::string& dest,
		const std::string& left,
		const std::string& right)
	: m_type(type)
	, m_dest(dest)
	, m_left(left)
	, m_right(right)
	{}

	virtual void generate(std::ostream& os) const override {
		os << T::instruction[m_type] << " ";
		os << m_dest << " " << m_left << " " << m_right << std::endl;
	}

private:
	const symbol_type_e m_type;
	const std::string m_dest;
	const std::string m_left;
	const std::string m_right;
};

class Add : public BinaryOperation<Add> {
public:
	using BinaryOperation<Add>::BinaryOperation;
	static const std::string instruction[];
};

class Subtract : public BinaryOperation<Subtract> {
public:
	using BinaryOperation<Subtract>::BinaryOperation;
	static const std::string instruction[];
};

class Multiply : public BinaryOperation<Multiply> {
public:
	using BinaryOperation<Multiply>::BinaryOperation;
	static const std::string instruction[];
};

class Divide : public BinaryOperation<Divide> {
public:
	using BinaryOperation<Divide>::BinaryOperation;
	static const std::string instruction[];
};

}
