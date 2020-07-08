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
#include <string>
#include <string_view>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what) noexcept
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		// default constructor
		euclidean_vector() noexcept;

		explicit euclidean_vector(int dim) noexcept;

		euclidean_vector(int dim, double mag) noexcept;

		// takes start and end of an iterator and works out req dimensions
		// and sets magnitude in each dimension according to iterated values
		euclidean_vector(std::vector<double>::const_iterator start,
		                 std::vector<double>::const_iterator end) noexcept;

		// // takes an initialiser list of doubles to populate vector magnitudes.
		euclidean_vector(std::initializer_list<double> init_list) noexcept;

		euclidean_vector(euclidean_vector const& copy) noexcept;

		euclidean_vector(euclidean_vector&& from) noexcept;
		// // destructor or DESTROYER
		~euclidean_vector() noexcept;

	private:
		size_t dimension_;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitude_;
		mutable double mag_cache_;

	public:
		// operator overloading
		auto operator=(euclidean_vector const&) noexcept -> euclidean_vector&;
		auto operator=(euclidean_vector&&) noexcept -> euclidean_vector&;
		auto operator[](int i) const noexcept -> double;
		auto operator[](int i) noexcept -> double&;
		auto operator+() noexcept -> euclidean_vector;
		auto operator-() noexcept -> euclidean_vector;
		auto operator+=(euclidean_vector const& vector) -> euclidean_vector&;
		auto operator-=(euclidean_vector const& vector) -> euclidean_vector&;
		auto operator*=(double const& mult) noexcept -> euclidean_vector&;
		auto operator/=(double const& divisor) -> euclidean_vector&;
		explicit operator std::vector<double>() const noexcept;
		explicit operator std::list<double>() noexcept;

		// member functions
		[[nodiscard]] auto at(int const& dimension) const -> double;
		auto at(int const& dimension) -> double&;

		[[nodiscard]] auto dimensions() const noexcept -> int;
		[[nodiscard]] auto mag_cache() const -> double&;
		auto set_cache(double&) const -> void;

		[[nodiscard]] auto calculate_norm() const noexcept -> double;
		[[nodiscard]] auto calculate_unit(double& norm) const noexcept -> std::vector<double>;
		auto calculate_dot(euclidean_vector const& y) const -> double;
		// friends
		friend auto operator==(euclidean_vector const&, euclidean_vector const&) noexcept -> bool;
		friend auto operator!=(euclidean_vector const&, euclidean_vector const&) noexcept -> bool;
		friend auto operator+(euclidean_vector const&, euclidean_vector const&) -> euclidean_vector;
		friend auto operator-(euclidean_vector const&, euclidean_vector const&) -> euclidean_vector;
		friend auto operator*(euclidean_vector const&, double) noexcept -> euclidean_vector;
		friend auto operator*(double, euclidean_vector const&) noexcept -> euclidean_vector;
		friend auto operator/(euclidean_vector const&, double) -> euclidean_vector;
		friend auto operator<<(std::ostream& os, euclidean_vector const& vector) noexcept
		   -> std::ostream&;
	};

	auto euclidean_norm(euclidean_vector const& v) -> double;
	auto unit(euclidean_vector const& v) -> euclidean_vector;
	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;

} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
