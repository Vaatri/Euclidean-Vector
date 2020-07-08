#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>

TEST_CASE("Friend arithmetic tests") {
	SECTION("Adding Vectors together") {
		auto t1 = comp6771::euclidean_vector();
		auto expecting_exception = comp6771::euclidean_vector(2);
		REQUIRE_THROWS_WITH(t1 + expecting_exception, "Dimensions of LHS(1) and RHS(2) do not match");

		auto const a1 = comp6771::euclidean_vector(3, 3.0);
		auto const a2 = comp6771::euclidean_vector(3, 3.0);
		auto const a3 = comp6771::euclidean_vector(3, 3.0);
		CHECK(fmt::format("{}", a1 + a2 + a3) == "[9 9 9]");

		auto t2 = comp6771::euclidean_vector({1.2, -1, 40.23, -50});
		auto t3 = comp6771::euclidean_vector{-1, 3, -40, 100};
		CHECK(fmt::format("{}", t2 + t3) == "[0.2 2 0.23 50]");
	}

	SECTION("Subtracting Vectors together") {
		auto t1 = comp6771::euclidean_vector();
		auto expecting_exception = comp6771::euclidean_vector(2);
		REQUIRE_THROWS_WITH(t1 - expecting_exception, "Dimensions of LHS(1) and RHS(2) do not match");

		auto const a1 = comp6771::euclidean_vector(3, 3.0);
		auto const a2 = comp6771::euclidean_vector(3, 3.0);
		auto const a3 = comp6771::euclidean_vector(3, 3.0);
		CHECK(fmt::format("{}", a1 - a2 - a3) == "[-3 -3 -3]");

		auto t2 = comp6771::euclidean_vector({1.2, -1, 40.23, -50});
		auto t3 = comp6771::euclidean_vector{-1, 3, -40, 100};
		CHECK(fmt::format("{}", t2 - t3) == "[2.2 -4 80.23 -150]");
	}

	SECTION("Multiplying a Vector by a Scalar") {
		auto vec = std::vector<double>{1, -1, 2.2, -2};
		auto t1 = comp6771::euclidean_vector{vec.begin(), vec.end()};
		CHECK(fmt::format("{}", t1 * 2) == "[2 -2 4.4 -4]");
		CHECK(fmt::format("{}", 2 * t1) == "[2 -2 4.4 -4]");
	}

	SECTION("Dividing a Vector by a Divisor") {
		auto vec = std::vector<double>{1, -1, 2.2, -2};
		auto t1 = comp6771::euclidean_vector{vec.begin(), vec.end()};
		CHECK(fmt::format("{}", t1 / 2) == "[0.5 -0.5 1.1 -1]");
		CHECK(fmt::format("{}", t1 / -2) == "[-0.5 0.5 -1.1 1]");
		REQUIRE_THROWS_WITH(t1 / 0, "Invalid vector division by 0");
	}
}