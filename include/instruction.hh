#pragma once

#include <fstream>
#include <memory>

#include "symbol.hh"

namespace cpq {

namespace instructions {

class Label {
public:
	Label() {}

	void set(std::size_t pos) 	{ m_position = pos + 1;	}
	unsigned get() const 		{ return m_position;	}

private:
	unsigned m_position;
};

class Instruction {
public:
	virtual void generate(std::ostream& os) const = 0;
};

class Read : public Instruction {
public:

	static const std::string instruction[];

	Read(symbol_type_e type, const std::string& var)
		: m_type(type)
		, m_var(var)
	{}

	virtual void generate(std::ostream& os) const override {
		os << instruction[m_type] << " " << m_var << std::endl;
	}

private:
	const symbol_type_e m_type;
	const std::string 	m_var;
};

class Write : public Instruction {
public:

	static const std::string instruction[];

	Write(symbol_type_e type, const std::string& var)
		: m_type(type)
		, m_var(var)
	{}

	virtual void generate(std::ostream& os) const override {
		os << instruction[m_type] << " " << m_var << std::endl;
	}

private:
	const symbol_type_e m_type;
	const std::string 	m_var;
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

class Inequality : public Instruction {
public:

	static const std::string instruction[symbol_type_e::LAST_TYPE][comparison_e::LAST_COMPARISON];

	Inequality(
		comparison_e	comp,
		symbol_type_e 	type,
		const std::string& dest,
		const std::string& left,
		const std::string& right
	)
		: m_comp(comp)
		, m_type(type)
		, m_dest(dest)
		, m_left(left)
		, m_right(right)
	{}

	virtual void generate(std::ostream& os) const override {
		bool change = m_comp & 0b100;
		auto left 	= change ? m_left : m_right;
		auto right	= change ? m_right : m_left;

		os << instruction[m_type][m_comp & 0b11] << " ";
		os << m_dest << " " << left << " " << right << std::endl;
	}

private:
	const comparison_e	m_comp;
	const symbol_type_e m_type;
	const std::string	m_dest;
	const std::string	m_left;
	const std::string	m_right;
};

class Branch : public Instruction {
public:
	Branch(const std::string& var, const std::shared_ptr<Label> label)
		: m_var(var)
		, m_label(label)
	{}

	virtual void generate(std::ostream& os) const override {
		os << "JMPZ" << " " << m_label->get() << " " << m_var << std::endl;
	}

private:
	const std::string m_var;
	const std::shared_ptr<Label> m_label;
};

class Jump : public Instruction {
public:
	Jump(const std::shared_ptr<Label> label)
		: m_label(label)
	{}

	virtual void generate(std::ostream& os) const override {
		os << "JUMP" << " " << m_label->get() << std::endl;
	}

private:
	const std::shared_ptr<Label> m_label;
};

class Halt : public Instruction {
public:
	virtual void generate(std::ostream& os) const override {
		os << "HALT" << std::endl;
		os << "copyright Chananel Engelberg, 2017" << std::endl;
	}
};

}

}

