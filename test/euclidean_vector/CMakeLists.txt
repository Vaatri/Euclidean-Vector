
cxx_test(
   TARGET euclidean_vector_test1
   FILENAME "euclidean_vector_test1.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET euclidean_vector_test_constructors
   FILENAME "euclidean_vector_test_constructors.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)


cxx_test(
   TARGET euclidean_vector_test_overload
   FILENAME "euclidean_vector_test_overload.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET euclidean_vector_test_friend
   FILENAME "euclidean_vector_test_friend.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET euclidean_vector_test_utility
   FILENAME "euclidean_vector_test_utility.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)