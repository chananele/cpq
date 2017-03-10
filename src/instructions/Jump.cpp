#include <memory>
#include <string>

#include "instructions/Label.h"
#include "instructions/Jump.h"

namespace cpq { namespace instructions {

Jump::Jump(const std::shared_ptr<Label> label)
	: m_label(label)
{}

std::string Jump::generate() const
{
	return "JUMP " + std::to_string(m_label->get());
}

}}
