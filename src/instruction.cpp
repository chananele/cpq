#include "instruction.hh"

namespace cpq {

namespace instructions {
	
const std::string Read::instruction[] = {
	/* [symbol_type_e::INT]  = */ "IINP",
	/* [symbol_type_e::READ] = */ "RINP"
};

const std::string Write::instruction[] = {
	/* [symbol_type_e::INT]  = */ "IPRT",
	/* [symbol_type_e::READ] = */ "RPRT"
};
	
}

const std::string Cast::instruction[] = {
	/* [symbol_type_e::INT]  = */ "RTOI",
	/* [symbol_type_e::READ] = */ "ITOR"
};

const std::string Assign::instruction[] = {
	/* [symbol_type_e::INT]  = */ "IASN",
	/* [symbol_type_e::READ] = */ "RASN"
};

const std::string Add::instruction[] = {
	/* [symbol_type_e::INT]	 = */ "IADD",
	/* [symbol_type_e::REAL] = */ "RADD",
};

const std::string Subtract::instruction[] = {
	/* [symbol_type_e::INT]	 = */ "ISUB",
	/* [symbol_type_e::REAL] = */ "RSUB",
};

const std::string Multiply::instruction[] = {
	/* [symbol_type_e::INT]	 = */ "IMLT",
	/* [symbol_type_e::REAL] = */ "RMLT",
};

const std::string Divide::instruction[] = {
	/* [symbol_type_e::INT]	 = */ "IDIV",
	/* [symbol_type_e::REAL] = */ "RDIV",
};

}
