#pragma once

#include <string>
#include <memory>

#include "location.hh"

#include "symbol.h"

#include "instructions/Instruction.h"
#include "expressions/Expression.h"

namespace cpq { namespace expressions {

/**
 * @brief Abstract class from which all CPL constant must inherit.
 */
class Constant : public Expression {
public:
	using Expression::Expression;

	/**
	 * @brief Returns the string representation of the constant.
	 */
	virtual std::string str() const = 0;

	virtual void generate(
			instructions::InstructionList& insts,
			const std::shared_ptr<Symbol> symbol
			) const override;
};

/**
 * Helper class used to carry traits of a given type.
 */
template <class T>
class Trait
{};

class Int;
class Real;

/**
 * Traits for the CPL integer constant type.
 */
template <>
class Trait<Int> {
public:
	using type = int;
	static const symbol_type_e symbol_type = symbol_type_e::INT;
};

/**
 * Traits for the CPL real constant type.
 */
template <>
class Trait<Real> {
public:
	using type = float;
	static const symbol_type_e symbol_type = symbol_type_e::REAL;
};

/**
 * Trivial inheritance from the Constant abstract base class.
 * The implicit contact of the type parameter T is:
 *
 *     Trait<T>::type is a valid parameter type to std::to_string.
 *     Trait<T>::symbol_type is a value of type symbol_type_e.  
 */
template <class T>
class Trivial : public Constant
{
public:
	using value_type = typename Trait<T>::type;

	/**
	 * @brief Create a constant of type T.
	 *
	 * @param loc - The location of the constant in the source code.
	 * @param value - The value of the constant.
	 */
	Trivial(const cpq::location& loc, value_type value)
		: Constant(loc)
		, m_value(value)
	{}

	virtual symbol_type_e type() const override {
		return Trait<T>::symbol_type;
	}

	virtual std::string str() const override {
		return std::to_string(m_value);
	}

private:
	value_type m_value;
};

class Int : public Trivial<Int> {
public:
	using Trivial<Int>::Trivial;
};

class Real : public Trivial<Real> {
public:
	 using Trivial<Real>::Trivial;
};

}}
