// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "comp6771/euclidean_vector.hpp"

#include <algorithm>
#include <cstddef>
#include <fmt/format.h>
#include <functional>
#include <gsl/gsl-lite.hpp>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <string>
// private helper functions for now
namespace {

	auto check_dimensions(comp6771::euclidean_vector const& a, comp6771::euclidean_vector const& b)
	   -> bool {
		return (a.dimensions() != b.dimensions());
	}

	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	auto initialize_magnitude(int dimensions, double magnitude) -> std::unique_ptr<double[]> {
		// ass2 spec requires we use double[]
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		auto retval = std::make_unique<double[]>(gsl_lite::narrow_cast<size_t>(dimensions));
		ranges::fill(retval.get(), retval.get() + dimensions, magnitude);
		return retval;
	}

	auto cast(int i) -> size_t {
		return gsl_lite::narrow_cast<size_t>(i);
	}

} // namespace

namespace comp6771 {

	euclidean_vector::~euclidean_vector() noexcept {
		magnitude_.reset();
	};

	// constructors
	euclidean_vector::euclidean_vector() noexcept
	: dimension_{1} {
		// ass2 spec requires we use double[]
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		this->magnitude_ = std::make_unique<double[]>(1);
		this->magnitude_[0] = 0.0;
		this->mag_cache_ = -1;
	}

	euclidean_vector::euclidean_vector(int dimension) noexcept
	: dimension_{dimension}
	, magnitude_{initialize_magnitude(dimension, 0)}
	, mag_cache_{-1} {}
	// namespace comp6771

	euclidean_vector::euclidean_vector(int dimension, double magnitude) noexcept
	: dimension_{dimension}
	, magnitude_{initialize_magnitude(dimension, magnitude)}
	, mag_cache_{-1} {}

	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator start,
	                                   std::vector<double>::const_iterator end) noexcept {
		this->dimension_ = gsl_lite::narrow_cast<int>(std::distance(start, end));
		// ass2 spec requires we use double[]
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		this->magnitude_ = std::make_unique<double[]>(cast(this->dimension_));
		std::transform(start, end, this->magnitude_.get(), [](auto const& a) -> double { return a; });
		this->mag_cache_ = -1;
	}

	euclidean_vector::euclidean_vector(std::initializer_list<double> init_list) noexcept {
		this->dimension_ = gsl::narrow<int>(std::size(init_list));
		// ass2 spec requires we use double[]
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		this->magnitude_ = std::make_unique<double[]>(cast(this->dimension_));
		std::transform(init_list.begin(),
		               init_list.end(),
		               this->magnitude_.get(),
		               [](auto const& a) -> double { return a; });
		this->mag_cache_ = -1;
	}

	euclidean_vector::euclidean_vector(euclidean_vector const& copy_from) noexcept
	: dimension_{copy_from.dimension_} {
		// ass2 spec requires we use double[]
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		this->magnitude_ = std::make_unique<double[]>(cast(copy_from.dimension_));
		auto size = copy_from.dimensions();
		std::transform(copy_from.magnitude_.get(),
		               copy_from.magnitude_.get() + size,
		               this->magnitude_.get(),
		               [](auto const& a) -> double { return a; });
		this->mag_cache_ = -1;
	}

	euclidean_vector::euclidean_vector(euclidean_vector&& move_from) noexcept
	: dimension_{std::exchange(move_from.dimension_, 0)}
	, magnitude_{std::move(move_from.magnitude_)}
	, mag_cache_{move_from.mag_cache_} {}

	// operator overloading

	auto euclidean_vector::operator=(euclidean_vector const& copy_from) noexcept -> euclidean_vector& {
		auto copy = copy_from;
		std::swap(copy, *this);
		return *this;
	}

	auto euclidean_vector::operator=(euclidean_vector&& source) noexcept -> euclidean_vector& {
		std::swap(this->dimension_, source.dimension_);
		std::swap(this->magnitude_, source.magnitude_);
		std::swap(this->mag_cache_, source.mag_cache_);
		// to clear it i guess just set it to 0/empty
		source.dimension_ = 0;
		source.magnitude_.reset(nullptr);

		return *this;
	}

	auto euclidean_vector::operator[](int i) const noexcept -> double {
		assert(i >= 0 and this->dimension_ >= i);
		return this->magnitude_[cast(i)];
	}

	auto euclidean_vector::operator[](int i) noexcept -> double& {
		assert(i >= 0 and euclidean_vector::dimensions() >= i);
		this->mag_cache_ = -1;
		return this->magnitude_[cast(i)];
	}

	auto euclidean_vector::operator+() const noexcept -> euclidean_vector {
		auto return_vector = euclidean_vector(*this);
		return return_vector;
	}

	auto euclidean_vector::operator-() noexcept -> euclidean_vector {
		auto return_vector = euclidean_vector(*this);
		std::for_each (return_vector.magnitude_.get(),
		               return_vector.magnitude_.get() + return_vector.dimension_,
		               [](double& a) -> double { return -a; });

		return return_vector;
	}

	auto euclidean_vector::operator+=(euclidean_vector const& vector) -> euclidean_vector& {
		if (this->dimension_ != vector.dimension_) {
			throw std::logic_error(fmt::format("Dimensions of LHS({}) and RHS({}) do not match",
			                                   this->dimensions(),
			                                   vector.dimensions()));
		}
		for (auto i = 0; i < this->dimension_; ++i) {
			this->magnitude_[cast(i)] = this->magnitude_[cast(i)] + vector.magnitude_[cast(i)];
		}

		this->mag_cache_ = -1;
		return *this;
	}

	auto euclidean_vector::operator-=(euclidean_vector const& vector) -> euclidean_vector& {
		if (this->dimension_ != vector.dimension_) {
			throw std::logic_error(fmt::format("Dimensions of LHS({}) and RHS({}) do not match",
			                                   this->dimensions(),
			                                   vector.dimensions()));
		}
		for (auto i = 0; i < this->dimension_; ++i) {
			this->magnitude_[cast(i)] = this->magnitude_[cast(i)] - vector.magnitude_[cast(i)];
		}
		this->mag_cache_ = -1;
		return *this;
	}

	auto euclidean_vector::operator*=(double const& mult) noexcept -> euclidean_vector& {
		std::transform(this->magnitude_.get(),
		               this->magnitude_.get() + this->dimension_,
		               this->magnitude_.get(),
		               [&mult](double& x) -> double { return x * mult; });
		this->mag_cache_ = -1;
		return *this;
	}

	auto euclidean_vector::operator/=(double const& divisor) -> euclidean_vector& {
		if (divisor == 0) {
			throw std::logic_error("Invalid vector division by 0");
		}

		std::transform(this->magnitude_.get(),
		               this->magnitude_.get() + this->dimension_,
		               this->magnitude_.get(),
		               [&divisor](double& x) -> double { return x / divisor; });
		this->mag_cache_ = -1;
		return *this;
	}

	euclidean_vector::operator std::vector<double>() const noexcept {
		auto vec = std::vector<double>();
		vec.reserve(cast(this->dimension_));
		for (auto i = 0; i < this->dimensions(); ++i) {
			vec.emplace_back(this->magnitude_[cast(i)]);
		}
		// std::fill()
		return vec;
	}

	euclidean_vector::operator std::list<double>() const noexcept {
		auto list = std::list<double>();
		for (auto i = 0; i < dimensions(); ++i) {
			list.emplace_back(euclidean_vector::magnitude_[cast(i)]);
		}
		return list;
	}

	auto euclidean_vector::at(const int& dimension) const -> double {
		if (dimension < 0 and dimension >= this->dimensions()) {
			throw std::out_of_range(
			   fmt::format("Index {} is not Valid for this euclidean_vector object", dimension));
		}

		return this->magnitude_[gsl_lite::narrow_cast<size_t>(dimension)];
	}

	auto euclidean_vector::at(int const& dimension) -> double& {
		if (dimension < 0 and dimension >= this->dimensions()) {
			throw std::out_of_range(
			   fmt::format("Index {} is not Valid for this euclidean_vector object", dimension));
		}

		return this->magnitude_[gsl_lite::narrow_cast<size_t>(dimension)];
	}

	auto operator==(euclidean_vector const& a, euclidean_vector const& b) noexcept -> bool {
		if (check_dimensions(a, b)) {
			return false;
		}

		auto size = a.dimensions();
		return std::equal(a.magnitude_.get(),
		                  a.magnitude_.get() + size,
		                  b.magnitude_.get(),
		                  b.magnitude_.get() + size);
	}
	auto operator!=(euclidean_vector const& a, euclidean_vector const& b) noexcept -> bool {
		if (check_dimensions(a, b)) {
			return true;
		}

		auto size = a.dimensions();
		return !(std::equal(a.magnitude_.get(),
		                    a.magnitude_.get() + size,
		                    b.magnitude_.get(),
		                    b.magnitude_.get() + size));
	}

	auto operator+(euclidean_vector const& a, euclidean_vector const& b) -> euclidean_vector {
		if (check_dimensions(a, b)) {
			throw std::logic_error(fmt::format("Dimensions of LHS({}) and RHS({}) do not match",
			                                   a.dimensions(),
			                                   b.dimensions()));
		}
		auto ret_vec = euclidean_vector(a.dimensions());

		auto size = a.dimension_;
		std::transform(a.magnitude_.get(),
		               a.magnitude_.get() + size,
		               b.magnitude_.get(),
		               ret_vec.magnitude_.get(),
		               [](double& a, double& b) -> double { return a + b; });

		return ret_vec;
	}

	auto operator-(euclidean_vector const& a, euclidean_vector const& b) -> euclidean_vector {
		if (check_dimensions(a, b)) {
			throw std::logic_error(fmt::format("Dimensions of LHS({}) and RHS({}) do not match",
			                                   a.dimensions(),
			                                   b.dimensions()));
		}
		auto ret_vec = euclidean_vector(a.dimensions());
		auto size = a.dimension_;
		std::transform(a.magnitude_.get(),
		               a.magnitude_.get() + size,
		               b.magnitude_.get(),
		               ret_vec.magnitude_.get(),
		               [](double& a, double& b) -> double { return a - b; });
		return ret_vec;
	}

	auto operator*(euclidean_vector const& a, double multiplier) noexcept -> euclidean_vector {
		auto ret_vec = euclidean_vector(a.dimensions());

		auto size = a.dimension_;
		std::transform(a.magnitude_.get(),
		               a.magnitude_.get() + size,
		               ret_vec.magnitude_.get(),
		               [&multiplier](double& a) -> double { return a * multiplier; });
		return ret_vec;
	}

	auto operator*(double multiplier, euclidean_vector const& a) noexcept -> euclidean_vector {
		return operator*(a, multiplier);
	}

	auto operator/(euclidean_vector const& a, double divisor) -> euclidean_vector {
		if (divisor == 0) {
			throw std::logic_error("Invalid vector division by 0");
		}

		auto ret_vec = euclidean_vector(a.dimensions());
		for (auto i = 0; i < a.dimension_; ++i) {
			ret_vec.magnitude_[cast(i)] = a.magnitude_[cast(i)] / divisor;
		}

		auto size = a.dimension_;
		std::transform(a.magnitude_.get(),
		               a.magnitude_.get() + size,
		               ret_vec.magnitude_.get(),
		               [&divisor](double& a) -> double { return a / divisor; });
		return ret_vec;
	}

	auto operator<<(std::ostream& os, euclidean_vector const& vector) noexcept -> std::ostream& {
		auto const& magnitudes = vector.magnitude_;
		auto last = vector.dimension_ - 1;
		os << "[";
		for (auto i = 0; i < vector.dimension_; i++) {
			os << magnitudes[cast(i)];
			if (i != last) {
				os << " ";
			}
		}
		os << "]";
		return os;
	}

	auto euclidean_vector::dimensions() const noexcept -> int {
		return gsl_lite::narrow_cast<int>(this->dimension_);
	}

	auto euclidean_norm(euclidean_vector const& v) -> double {
		if (v.dimensions() == 0) {
			throw std::logic_error("euclidean_vector with no dimensions does not have a norm");
		}
		// check cache if we have
		// auto norm = v.mag_cache();
		// if (norm == -1) {
		// 	norm = v.calculate_norm();
		// 	v.set_cache(norm);
		// }
		return v.calculate_norm();
	}

	auto euclidean_vector::calculate_norm() const noexcept -> double {
		auto norm = this->mag_cache_;
		if (norm == -1) {
			norm = 0.0;
			std::for_each (this->magnitude_.get(),
			               this->magnitude_.get() + this->dimension_,
			               [&norm](double& mag) -> void { norm += mag * mag; });
			norm = std::sqrt(norm);
			this->mag_cache_ = norm;
		}

		return norm;
	}

	auto unit(euclidean_vector const& v) -> euclidean_vector {
		if (v.dimensions() == 0) {
			throw std::logic_error("euclidean_vector with no dimensions does not have a unit vector");
		}
		auto norm = euclidean_norm(v);
		if (norm == 0) {
			throw std::logic_error("euclidean_vector with zero euclidean normal does not have a unit "
			                       "vector");
		}
		auto unit_mags = v.calculate_unit(norm);
		return euclidean_vector(unit_mags.begin(), unit_mags.end());
	}

	auto euclidean_vector::calculate_unit(double& norm) const noexcept -> std::vector<double> {
		auto unit_mags = std::vector<double>();
		std::for_each (this->magnitude_.get(),
		               this->magnitude_.get() + this->dimension_,
		               [&unit_mags, &norm](double& mag) -> void { unit_mags.emplace_back(mag / norm); });
		return unit_mags;
	}

	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double {
		if (check_dimensions(x, y)) {
			throw std::logic_error(fmt::format("Dimensions of LHS({}) and RHS({}) do not match",
			                                   x.dimensions(),
			                                   y.dimensions()));
		}

		return x.calculate_dot(y);
	}

	auto euclidean_vector::calculate_dot(euclidean_vector const& y) const -> double {
		auto size = this->dimension_;
		return std::inner_product(this->magnitude_.get(),
		                          this->magnitude_.get() + size,
		                          y.magnitude_.get(),
		                          0.0);
	}

} // namespace comp6771
