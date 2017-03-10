#pragma once

#include <cstddef>

namespace cpq { namespace instructions {

/**
 * @brief Represents a location to which an instruction may branch.
 */
class Label {
public:

	/**
	 * @brief Creates a new label. The target of the label is not set.
	 */
	Label();

	/**
	 * @brief Sets the label to point to instruction after the given position.
	 * @param pos - The instruction after which this label will point.
	 */
	void set(std::size_t pos);
	
	/**
	 * @brief Gets the position to which the label points.
	 */
	unsigned get() const;

private:
	unsigned	m_position;
};

}}
