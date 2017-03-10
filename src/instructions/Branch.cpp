#include <memory>
#include <string>

#include "instructions/Label.h"
#include "instructions/Branch.h"

namespace cpq { namespace instructions {

Branch::Branch(const std::string& var, const std::shared_ptr<Label> label)
	: m_var(var)
	, m_label(label)
{}

std::string Branch::generate() const
{
	return "JMPZ " + std::to_string(m_label->get()) + " " + m_var;
}

}}
