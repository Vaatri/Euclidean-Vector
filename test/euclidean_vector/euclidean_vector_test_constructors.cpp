#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <initializer_list>
#include <iostream>
#include <iterator>

TEST_CASE("Testing constructors") {
	SECTION("Testing default constructor and << operator overload") {
		auto const t1 = comp6771::euclidean_vector();
		std::cout << t1;
		CHECK(fmt::format("{}", t1) == "[0]");
		CHECK(t1.dimensions() == 1);
	}
	SECTION("Testing Single argument constructor") {
		auto const t1 = comp6771::euclidean_vector(3);
		std::cout << t1;
		CHECK(fmt::format("{}", t1) == "[0 0 0]");
		CHECK(t1.dimensions() == 3);
		auto const large_dimension = comp6771::euclidean_vector(100);
		CHECK(large_dimension.dimensions() == 100);
	}
	SECTION("Testing double argument constructor") {
		auto const t1 = comp6771::euclidean_vector(10, 6.9);
		CHECK(fmt::format("{}", t1) == "[6.9 6.9 6.9 6.9 6.9 6.9 6.9 6.9 6.9 6.9]");
		CHECK(t1.dimensions() == 10);
	}
	SECTION("Testing iterator constructor") {
		auto test_vector = std::vector<double>{4.2, 6.9, 1.3};
		auto begin = std::vector<double>::const_iterator{test_vector.begin()};
		auto end = std::vector<double>::const_iterator{test_vector.end()};
		auto const t1 = comp6771::euclidean_vector(begin, end);
		CHECK(fmt::format("{}", t1) == "[4.2 6.9 1.3]");
		CHECK(t1.dimensions() == 3);
	}
	SECTION("Testing initializer_list constructor") {
		auto const t1 = comp6771::euclidean_vector({1.2, 2.3, 3.4, 4.5});
		CHECK(fmt::format("{}", t1) == "[1.2 2.3 3.4 4.5]");
		CHECK(t1.dimensions() == 4);
	}

	SECTION("Testing copy constructor") {
		auto t1 = comp6771::euclidean_vector();
		auto const t2 = comp6771::euclidean_vector(t1);
		REQUIRE(t1 == t2);
		CHECK(t1.dimensions() == 1);
		CHECK(t1.dimensions() == t2.dimensions());
		t1[0] = 3.2;
		REQUIRE(t1 != t2);
	}
	SECTION("Moving vectors around") {
		auto t1 = comp6771::euclidean_vector(4, 4.2);
		auto t2 = std::move(t1);
		REQUIRE(fmt::format("{}", t2) == "[4.2 4.2 4.2 4.2]");
	}
}
