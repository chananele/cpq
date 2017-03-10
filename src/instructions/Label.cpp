#include "instructions/Label.h"

namespace cpq { namespace instructions {

Label::Label()
	: m_position(0)
{}

void Label::set(std::size_t pos)
{
	m_position = pos + 1;
}


unsigned Label::get() const
{
	return m_position;
}

}}
