#include "comp6771/euclidean_vector.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>

TEST_CASE("Testing operator Overloading") {
	SECTION("Subscript Overloading") {
		auto t1 = comp6771::euclidean_vector();
		CHECK(t1[0] == 0);

		auto t2 = comp6771::euclidean_vector(3);
		CHECK(t2[0] == 0);
		CHECK(t2[0] == t2[1]);
		CHECK(t2[0] == t2[2]);

		auto t3 = comp6771::euclidean_vector(3, 6.9);
		CHECK(t3[0] == 6.9);
		CHECK(t3[0] == t3[1]);
		CHECK(t3[0] == t3[2]);

		auto test_vector = std::vector<double>{4.2, 6.9, 1.3};
		auto begin = std::vector<double>::const_iterator{test_vector.begin()};
		auto end = std::vector<double>::const_iterator{test_vector.end()};
		auto t4 = comp6771::euclidean_vector(begin, end);
		CHECK(t4[0] == 4.2);
		CHECK(t4[1] == 6.9);
		CHECK(t4[2] == 1.3);

		auto t5 = comp6771::euclidean_vector({1.2, 2.3, 3.4, 4.5});
		CHECK(t5[0] == 1.2);
		CHECK(t5[1] == 2.3);
		CHECK(t5[2] == 3.4);
		CHECK(t5[3] == 4.5);
	}

	SECTION("Overloading += operator") {
		auto t1 = comp6771::euclidean_vector(3, 3.0);
		auto t2 = comp6771::euclidean_vector({4.0, 4.0, 4.0});
		t1 += t2;
		auto expected1 = comp6771::euclidean_vector(3, 7.0);
		CHECK(t1 == expected1);

		auto t3 = comp6771::euclidean_vector(2, 4.0);
		REQUIRE_THROWS_AS(t1 += t3, std::logic_error);
		REQUIRE_THROWS_AS(t3 += t1, std::logic_error);

		auto t4 = comp6771::euclidean_vector(3, -1.3);
		t1 += t4;
		CHECK(t1[0] == 5.7);

		auto t5 = comp6771::euclidean_vector({1.0, 2.5, -0.3, 0.1, -3.2});
		auto test_vec = std::vector<double>{-1.0, -3.5, -3.0, 0.5, 5.0};
		auto t6 = comp6771::euclidean_vector(test_vec.begin(), test_vec.end());

		t5 = t5 + t6;

		CHECK(t5.at(0) == 0);
		CHECK(t5.at(1) == -1);
		CHECK(t5.at(2) == -3.3);
		CHECK(t5.at(3) == 0.6);
		CHECK_THAT(t5.at(4), Catch::Matchers::WithinAbs(1.8, 0.00001));
	}

	SECTION("Overloading -= operator") {
		auto exception_test = comp6771::euclidean_vector();
		auto t1 = comp6771::euclidean_vector({1, -1, 0, 30.45, -90.2});
		REQUIRE_THROWS_AS(exception_test -= t1, std::logic_error);
		REQUIRE_THROWS_AS(t1 -= exception_test, std::logic_error);
		auto vec = std::vector<double>{5, 5, 0, 30.45, -9.2};
		auto t2 = comp6771::euclidean_vector(vec.begin(), vec.end());
		t1 -= t2;
		CHECK_THAT(t1.at(0), Catch::Matchers::WithinAbs(t1[0], 0.00001));
		CHECK_THAT(t1[1], Catch::Matchers::WithinAbs(t1.at(1), 0.00001));
		CHECK_THAT(t1.at(2), Catch::Matchers::WithinAbs(0, 0.00001));
		CHECK_THAT(t1[3], Catch::Matchers::WithinAbs(0, 0.00001));
		CHECK_THAT(t1.at(4), Catch::Matchers::WithinAbs(-81, 0.00001));
	}

	SECTION("Overloading *= operator") {
		auto t1 = comp6771::euclidean_vector{0, 1, -1, 2.11, -3.2};
		t1 *= 3;
		CHECK_THAT(t1.at(0), Catch::Matchers::WithinAbs(0, 0.00001));
		CHECK_THAT(t1[1], Catch::Matchers::WithinAbs(3, 0.00001));
		CHECK_THAT(t1.at(2), Catch::Matchers::WithinAbs(-3, 0.00001));
		CHECK_THAT(t1[3], Catch::Matchers::WithinAbs(6.33, 0.00001));
		CHECK_THAT(t1.at(4), Catch::Matchers::WithinAbs(-9.6, 0.00001));
		t1 = t1 * -1;
		CHECK_THAT(t1.at(0), Catch::Matchers::WithinAbs(0, 0.00001));
		CHECK_THAT(t1[1], Catch::Matchers::WithinAbs(-3, 0.00001));
		CHECK_THAT(t1.at(2), Catch::Matchers::WithinAbs(3, 0.00001));
		CHECK_THAT(t1[3], Catch::Matchers::WithinAbs(-6.33, 0.00001));
		CHECK_THAT(t1.at(4), Catch::Matchers::WithinAbs(9.6, 0.00001));
	}

	SECTION("Overloading /= operator") {
		auto expecting_exception = comp6771::euclidean_vector();
		REQUIRE_THROWS_WITH(expecting_exception /= 0, "Invalid vector division by 0");

		auto t1 = comp6771::euclidean_vector({0, -1, 2});
		// t1 /= 2;
		t1 = t1 / 2;
		CHECK_THAT(t1.at(0), Catch::Matchers::WithinAbs(0, 0.00001));
		CHECK_THAT(t1[1], Catch::Matchers::WithinAbs(-0.5, 0.00001));
		CHECK_THAT(1, Catch::Matchers::WithinAbs(t1.at(2), 0.00001));

		t1 /= 2;
		CHECK(t1 == comp6771::euclidean_vector({0, -0.25, 0.5}));
	}

	SECTION("Overloading == and != operators") {
		auto t1 = comp6771::euclidean_vector();
		auto t2 = comp6771::euclidean_vector(2);

		REQUIRE(!(t1 == t2));
		REQUIRE(t1 != t2);

		auto t3 = comp6771::euclidean_vector{0, 1};
		CHECK(!(t1 == t2));
		CHECK(t1 != t2);
	}

	// SECTION("Overloading typecast") {
	// 	SECTION("Vector typecast") {
	// 		auto const a = comp6771::euclidean_vector{0.0, 1.0, 2.0};
	// 		auto const vf = static_cast<std::vector<double>>(a);
	// 	}
	// }
}
