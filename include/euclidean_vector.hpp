#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include <compare>
#include <functional>
#include <list>
#include <memory>
#include <ostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/iterator.hpp>
#include <span>
#include <stdexcept>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::exception {
	public:
		explicit euclidean_vector_error(std::string const& what)
		: what_(what) {}

		auto what() const noexcept -> char const* {
			return what_.c_str();
		}

	private:
		std::string what_;
	};

	class euclidean_vector {
	public:
		// TODO
	private:
		// TODO
	};
} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
