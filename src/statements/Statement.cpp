#include "location.hh"

#include "instructions/Instruction.h"
#include "instructions/Halt.h"

#include "statements/Statement.h"

namespace cpq { namespace statements {	

Statement::Statement(const cpq::location& loc)
	: m_loc(loc)
{}
	
void finish(instructions::InstructionList& insts)
{
	insts.push_back(std::make_unique<instructions::Halt>());
}
	
}}
