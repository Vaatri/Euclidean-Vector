#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>

TEST_CASE("Utility Functions") {
	SECTION("Euclidean Norms") {
		auto expecting_exception = comp6771::euclidean_vector(0);
		REQUIRE_THROWS_WITH(comp6771::euclidean_norm(expecting_exception),
		                    "euclidean_vector with no dimensions does not have a norm");
		auto t1 = comp6771::euclidean_vector(3, 1);
		CHECK(comp6771::euclidean_norm(t1) == std::sqrt(3));
		CHECK(t1.mag_cache() == std::sqrt(3));
		t1 *= 3;
		CHECK(t1.mag_cache() == -1);
		CHECK(comp6771::euclidean_norm(t1) == std::sqrt(27));
		t1[0] = 1.5;
		t1[1] = -3;
		t1[2] = 0;
		CHECK(comp6771::euclidean_norm(t1) == std::sqrt(11.25));
	}
	SECTION("Unit Vectors") {
		auto expecting_exception = comp6771::euclidean_vector(0);
		auto expecting_exception2 = comp6771::euclidean_vector(2, 0);
		REQUIRE_THROWS_WITH(comp6771::unit(expecting_exception),
		                    "euclidean_vector with no dimensions does not have a unit vector");
		REQUIRE_THROWS_WITH(comp6771::unit(expecting_exception2),
		                    "euclidean_vector with zero euclidean normal does not have a unit "
		                    "vector");

		auto a = comp6771::euclidean_vector(2);
		a[0] = 1;
		a[1] = 2;
		auto b = comp6771::euclidean_vector(2);
		b[0] = 1;
		b[1] = 2;

		auto c = comp6771::unit(a);
		auto d = comp6771::unit(b);
		REQUIRE(c == d);
	}
}
