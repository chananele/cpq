#pragma once

#include <stdexcept>

namespace cpq { namespace instructions {

class PositionSetException : public std::runtime_error {};
class PositionUnsetException : public std::runtime_error {};

}}
