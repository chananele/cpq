#include "symbol.hh"
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

const std::string Inequality::instruction[symbol_type_e::LAST_TYPE][comparison_e::LAST_COMPARISON] = {
	/* [symbol_type_e::INT] */
	{
		/* [comparison_e::GT]	= */ "IGRT",
		/* [comparison_e::LT]	= */ "ILSS",
		/* [comparison_e::EQ]	= */ "IEQL",
		/* [comparison_e::NEQ]	= */ "INQL",
	},
	/* [symbol_type_e::REAL] */
	{
		/* [comparison_e::GT]	= */ "RGRT",
		/* [comparison_e::LT]	= */ "RLSS",
		/* [comparison_e::EQ]	= */ "REQL",
		/* [comparison_e::NEQ]	= */ "RNQL",
	},
};

}

}
