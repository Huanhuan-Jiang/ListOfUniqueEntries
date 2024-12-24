#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <compare>
#include <concepts>
#include <numeric>
#include <stdexcept>
#include <unordered_set>
#include <utility>
#include <vector>

#include "vectorofunique.h"

TEST(VectorOfUniqueTest, DefaultConstructor) {
  containerofunique::vector_of_unique<int> vou;
  std::vector<int> emptyvec;
  std::unordered_set<int> emptyset;

  EXPECT_EQ(vou.vector(), emptyvec);
  EXPECT_THAT(vou.set(), ::testing::UnorderedElementsAreArray(emptyset));
}

TEST(VectorOfUniqueTest, ConstructorInitializesFromIterators) {
  std::vector<int> vec1 = {3, 1, 2, 3, 4, 5};
  std::vector<int> vec2 = {3, 1, 2, 4, 5};
  containerofunique::vector_of_unique<int> vou1(vec1.begin(), vec1.end());
  containerofunique::vector_of_unique<int> vou2(vec2.begin(), vec2.end());

  EXPECT_EQ(vou1.vector(), vec2);
  EXPECT_THAT(std::vector<int>(vou1.set().begin(), vou1.set().end()),
              ::testing::UnorderedElementsAreArray(vec2));
  EXPECT_EQ(vou2.vector(), vec2);
  EXPECT_THAT(std::vector<int>(vou2.set().begin(), vou2.set().end()),
              ::testing::UnorderedElementsAreArray(vec2));
}

TEST(VectorOfUniqueTest, ConstructorWithInitializerListChecksVectorAndSet) {
  containerofunique::vector_of_unique<int> vou1 = {1};
  containerofunique::vector_of_unique<int> vou2 = {1, 2};
  containerofunique::vector_of_unique<int> vou3 = {1, 2, 3,
                                                   3}; // duplicate elements

  std::vector<int> vec1 = {1};
  std::vector<int> vec2 = {1, 2};
  std::vector<int> vec3 = {1, 2, 3};

  EXPECT_EQ(vou1.vector(), vec1);
  EXPECT_EQ(vou2.vector(), vec2);
  EXPECT_EQ(vou3.vector(), vec3);

  EXPECT_THAT(std::vector<int>(vou1.set().begin(), vou1.set().end()),
              ::testing::UnorderedElementsAreArray(vec1));
  EXPECT_THAT(std::vector<int>(vou2.set().begin(), vou2.set().end()),
              ::testing::UnorderedElementsAreArray(vec2));
  EXPECT_THAT(std::vector<int>(vou3.set().begin(), vou3.set().end()),
              ::testing::UnorderedElementsAreArray(vec3));
}

TEST(VectorOfUniqueTest, CopyConstructor_EmptyVector) {
  containerofunique::vector_of_unique<int> vou1;
  // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
  containerofunique::vector_of_unique<int> vou2(vou1);
  EXPECT_TRUE(vou2.vector().empty());
  EXPECT_TRUE(vou2.set().empty());
}

TEST(VectorOfUniqueTest, CopyConstructor_SingleElement) {
  containerofunique::vector_of_unique<int> vou1 = {42};
  // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
  containerofunique::vector_of_unique<int> vou2(vou1);
  std::vector<int> vec = {42};
  EXPECT_EQ(vou2.vector(), vec);
  EXPECT_THAT(vou2.set(), ::testing::UnorderedElementsAreArray(vec));
}

TEST(VectorOfUniqueTest, CopyConstructor_Independence) {
  containerofunique::vector_of_unique<int> vou1 = {1, 2, 3};
  containerofunique::vector_of_unique<int> vou2(vou1);

  vou1.push_back(4); // Modify the original
  EXPECT_EQ(vou1.vector(), std::vector<int>({1, 2, 3, 4}));
  EXPECT_EQ(vou2.vector(), std::vector<int>({1, 2, 3}));
}

TEST(VectorOfUniqueTest, CopyConstructor_LargeData) {
  std::vector<int> large_data(1000);
  std::iota(large_data.begin(), large_data.end(), 0);

  // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
  containerofunique::vector_of_unique<int> vou1(large_data.begin(),
                                                large_data.end());
  // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
  containerofunique::vector_of_unique<int> vou2(vou1);

  EXPECT_EQ(vou1.vector(), vou2.vector());
  EXPECT_THAT(vou2.set(), ::testing::UnorderedElementsAreArray(large_data));
}

TEST(VectorOfUniqueTest, MoveConstructor) {
  containerofunique::vector_of_unique<int> vou1 = {1, 2, 3, 4};
  containerofunique::vector_of_unique<int> vou2(std::move(vou1));
  std::vector<int> vec = {1, 2, 3, 4};
  EXPECT_EQ(vou2.vector(), vec);
}

TEST(VectorOfUniqueTest, CopyAssignmentOperator) {
  containerofunique::vector_of_unique<int> vou1 = {1, 2, 3, 4};
  containerofunique::vector_of_unique<int> vou2 = vou1;
  std::vector<int> vec = {1, 2, 3, 4};

  EXPECT_EQ(vou2.vector(), vou1.vector());
  EXPECT_THAT(std::vector<int>(vou2.set().begin(), vou2.set().end()),
              ::testing::UnorderedElementsAreArray(vec));
  vou1.push_back(
      5); // This is used to suppress warning of
          // [performance-unnecessary-copy-initialization,-warnings-as-errors]
}

TEST(VectorOfUniqueTest, MoveAssignmentOperator) {
  containerofunique::vector_of_unique<int> vou1 = {1, 2, 3, 4};
  containerofunique::vector_of_unique<int> vou2 = std::move(vou1);
  std::vector<int> vec = {1, 2, 3, 4};

  EXPECT_EQ(vou2.vector(), vec);
  EXPECT_THAT(std::vector<int>(vou2.set().begin(), vou2.set().end()),
              ::testing::UnorderedElementsAreArray(vec));
}

TEST(VectorOfUniqueTest, InitializerListAssignmentOperator) {
  containerofunique::vector_of_unique<int> vou = {1, 2, 3, 4};
  std::vector<int> vec = {1, 2, 3, 4};
  EXPECT_EQ(vou.vector(), vec);
  EXPECT_THAT(std::vector<int>(vou.set().begin(), vou.set().end()),
              ::testing::UnorderedElementsAreArray(vec));
}

TEST(VectorOfUniqueTest, ElementAccess) {
  containerofunique::vector_of_unique<int> vou = {1, 2, 3, 4};
  EXPECT_EQ(vou.front(), 1);
  EXPECT_EQ(vou.at(1), 2);
  EXPECT_EQ(vou[2], 3);
  EXPECT_EQ(vou.back(), 4);
}

TEST(VectorOfUniqueTest, At_OutOfRange) {
  containerofunique::vector_of_unique<int> vou = {1, 2, 3, 4};
  const containerofunique::vector_of_unique<std::string> vou_const = {"hello",
                                                                      "world"};
  EXPECT_THROW(vou.at(4), std::out_of_range);
  EXPECT_THROW(vou_const.at(2), std::out_of_range);
}

TEST(VectorOfUniqueTest, ElementAccess_ConstVector) {
  const containerofunique::vector_of_unique<std::string> vou = {"hello",
                                                                "world"};
  EXPECT_EQ(vou.front(), "hello");
  EXPECT_EQ(vou.at(0), "hello");
  EXPECT_EQ(vou.at(1), "world");
  EXPECT_EQ(vou[0], "hello");
  EXPECT_EQ(vou[1], "world");
  EXPECT_EQ(vou.back(), "world");
}

// Test for normal iteration using cbegin() and cend()
TEST(VectorOfUniqueTest, CbeginCend_Iteration) {
  containerofunique::vector_of_unique<int> vou = {1, 2, 3, 4};

  auto it = vou.cbegin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(*it, 4);
  ++it;
  EXPECT_EQ(it, vou.cend()); // Ensure iterator reaches cend()
}

// Test for normal iteration using crbegin() and crend()
TEST(VectorOfUniqueTest, CrbeginCrend_Iteration) {
  containerofunique::vector_of_unique<int> vou = {1, 2, 3, 4};

  auto rit = vou.crbegin();
  EXPECT_EQ(*rit, 4);
  ++rit;
  EXPECT_EQ(*rit, 3);
  ++rit;
  EXPECT_EQ(*rit, 2);
  ++rit;
  EXPECT_EQ(*rit, 1);
  ++rit;
  EXPECT_EQ(rit, vou.crend()); // Ensure reverse iterator reaches crend()
}

// Test for empty container's iterators
TEST(VectorOfUniqueTest, EmptyContainer_Iterators) {
  containerofunique::vector_of_unique<int> empty_vou;

  // For an empty vector, cbegin() should be equal to cend()
  EXPECT_EQ(empty_vou.cbegin(), empty_vou.cend());
  // For an empty vector, crbegin() should be equal to crend()
  EXPECT_EQ(empty_vou.crbegin(), empty_vou.crend());
}

// Test for const-correctness of iterators
TEST(VectorOfUniqueTest, ConstCorrectness_Iterators) {
  containerofunique::vector_of_unique<int> vou = {1, 2, 3, 4};

  EXPECT_TRUE((std::same_as<decltype(*vou.cbegin()), const int &>));
  EXPECT_TRUE((std::same_as<decltype(*vou.cend()), const int &>));
  EXPECT_TRUE((std::same_as<decltype(*vou.crbegin()), const int &>));
  EXPECT_TRUE((std::same_as<decltype(*vou.crend()), const int &>));
}

// Test that iterators do not modify elements (compile-time check)
TEST(VectorOfUniqueTest, Iterator_ModificationNotAllowed) {
  containerofunique::vector_of_unique<int> vou = {1, 2, 3, 4};
  auto const_it = vou.cbegin();
  ASSERT_EQ(*const_it, 1);
  ASSERT_TRUE(std::is_const_v<std::remove_reference_t<decltype(*const_it)>>);
}

TEST(VectorOfUniqueTest, Clear) {
  containerofunique::vector_of_unique<int> vou = {1, 2, 3, 4, 5};
  vou.clear();

  EXPECT_EQ(vou.vector().size(), 0); // Vector should be empty
  EXPECT_THAT(vou.set(),
              ::testing::UnorderedElementsAre()); // Set should be empty
}

TEST(VectorOfUniqueTest, Erase_SingleElement) {
  containerofunique::vector_of_unique<int> vou = {1, 2, 3, 4, 5};
  std::vector<int> expected_vector = {2, 3, 4, 5};
  std::unordered_set<int> expected_set = {2, 3, 4, 5};

  vou.erase(vou.cbegin());
  EXPECT_EQ(vou.vector(), expected_vector);
  EXPECT_THAT(vou.set(), ::testing::UnorderedElementsAreArray(expected_set));
}

TEST(VectorOfUniqueTest, Erase_FromEmptyContainer) {
  containerofunique::vector_of_unique<int> vou;
  EXPECT_NO_THROW(vou.erase(vou.cbegin()));
  EXPECT_EQ(vou.vector().size(), 0);
}

TEST(VectorOfUniqueTest, EraseEmptyRange) {
  containerofunique::vector_of_unique<int> vou1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vec2 = {1, 2, 3, 4, 5, 6};
  std::unordered_set<int> set2 = {1, 2, 3, 4, 5, 6};

  auto result = vou1.erase(vou1.cbegin(), vou1.cbegin());
  EXPECT_EQ(result, vou1.cbegin());
  EXPECT_EQ(vou1.vector(), vec2);
  EXPECT_THAT(vou1.set(), ::testing::UnorderedElementsAreArray(set2));
}

TEST(VectorOfUniqueTest, EraseRangeOfElements) {
  containerofunique::vector_of_unique<int> vou1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vec2 = {4, 5, 6};
  std::unordered_set<int> set2 = {4, 5, 6};

  vou1.erase(vou1.cbegin(), vou1.cbegin() + 3);
  EXPECT_EQ(vou1.vector(), vec2);
  EXPECT_THAT(vou1.set(), ::testing::UnorderedElementsAreArray(set2));
}

TEST(VectorOfUniqueTest, EraseAllElements) {
  containerofunique::vector_of_unique<int> vou1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vec2 = {};
  std::unordered_set<int> set2 = {};

  auto result = vou1.erase(vou1.cbegin(), vou1.cend());
  EXPECT_EQ(result, vou1.cend());
  EXPECT_EQ(vou1.vector(), vec2);
  EXPECT_THAT(vou1.set(), ::testing::UnorderedElementsAreArray(set2));
}

TEST(VectorOfUniqueTest, InsertLvalueRvalue) {
  std::cout << "Test inserting a unique element" << '\n';
  containerofunique::vector_of_unique<int> vou1 = {1};
  std::vector<int> vec1 = {1};
  auto result1 = vou1.insert(vou1.cbegin(), 2);
  auto expected_result1 = vec1.insert(vec1.cbegin(), 2);
  EXPECT_EQ(*result1.first, *expected_result1);
  EXPECT_TRUE(result1.second);

  std::cout << "Test inserting a duplicate element" << '\n';
  containerofunique::vector_of_unique<int> vou2 = {1};
  std::vector<int> vec2 = {1, 2};
  auto result2 = vou2.insert(vou2.cbegin(), 1);
  EXPECT_EQ(*result2.first, *vou2.cbegin());
  EXPECT_FALSE(result2.second);

  std::cout << "Test inserting a unique rvalue string element" << '\n';
  containerofunique::vector_of_unique<std::string> vou3 = {"hello", "world"};
  std::vector<std::string> vec3 = {"hello", "world"};
  std::string str1 = "good";
  auto expected_result3 = vec3.insert(vec3.cbegin(), std::move("good"));
  auto result3 = vou3.insert(vou3.cbegin(), std::move(str1));
  EXPECT_EQ(vou3.vector(),
            (std::vector<std::string>{"good", "hello", "world"}));
  EXPECT_EQ(*result3.first, *expected_result3);
  EXPECT_TRUE(result3.second);

  std::cout << "Test inserting a duplicate rvalue string element" << '\n';
  containerofunique::vector_of_unique<std::string> vou4 = {"hello", "world"};
  std::vector<std::string> vec4 = {"hello", "world"};
  std::string str2 = "hello";
  auto result4 = vou4.insert(vou4.cbegin(), std::move(str2));
  EXPECT_EQ(vou4.vector(), vec4);
  EXPECT_EQ(*result4.first, *vou4.cbegin());
  EXPECT_FALSE(result4.second);
}

TEST(VectorOfUniqueTest, InsertRangeTest) {
  containerofunique::vector_of_unique<std::string> vou5_1 = {"hello", "world"};
  containerofunique::vector_of_unique<std::string> vou5_2 = {"good", "morning"};
  containerofunique::vector_of_unique<std::string> vou5_3 = {"hello", "world"};
  std::vector<std::string> vec5 = {"good", "morning", "hello", "world"};
  auto result5_1 =
      vou5_1.insert(vou5_1.cbegin(), vou5_2.cbegin(), vou5_2.cbegin() + 2);
  EXPECT_EQ(vou5_1.vector(), vec5);
  EXPECT_EQ(*result5_1, *vou5_1.cbegin());
  auto result5_2 =
      vou5_1.insert(vou5_1.cbegin(), vou5_3.cbegin(), vou5_3.cbegin() + 2);
  EXPECT_EQ(vou5_1.vector(), vec5);
  EXPECT_EQ(*result5_2, *(vou5_1.cbegin()));

  containerofunique::vector_of_unique<std::string> vou6 = {"hello", "world"};
  std::vector<std::string> vec6 = {"good", "morning", "hello", "world"};
  auto result6_1 = vou6.insert(vou6.cbegin(), {"good", "morning"});
  EXPECT_EQ(vou6.vector(), vec6);
  EXPECT_EQ(*result6_1, *vou6.cbegin());
  auto result6_2 = vou6.insert(vou6.cbegin(), {"good", "morning"});
  EXPECT_EQ(vou6.vector(), vec6);
  EXPECT_EQ(*result6_2, *vou6.cbegin());
}

TEST(VectorOfUniqueTest, InsertEmptyRange) {
  containerofunique::vector_of_unique<std::string> vou1 = {"existing"};
  std::vector<std::string> vec1 = {"existing"};
  std::vector<std::string> vec2 = {"hello", "world", "apple", "fruit"};

  auto result1 = vou1.insert(vou1.cbegin(), vec2.begin(), vec2.begin());
  EXPECT_EQ(result1, vou1.cbegin());
  EXPECT_EQ(vou1.vector(), vec1);

  auto result2 = vou1.insert(vou1.cbegin(), {});
  EXPECT_EQ(result2, vou1.cbegin());
  EXPECT_EQ(vou1.vector(), vec1);
}

TEST(VectorOfUniqueTest, InsertAtEnd) {
  containerofunique::vector_of_unique<std::string> vou = {"hello"};
  auto result = vou.insert(vou.cend(), "world");
  EXPECT_EQ(*result.first, vou.back());
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(vou.vector(), (std::vector<std::string>{"hello", "world"}));
}

TEST(VectorOfUniqueTest, InsertAtBeginning) {
  containerofunique::vector_of_unique<std::string> vou = {"world"};
  auto result = vou.insert(vou.cbegin(), "hello");
  EXPECT_EQ(*result.first, vou.front());
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(vou.vector(), (std::vector<std::string>{"hello", "world"}));
}

TEST(VectorOfUniqueTest, InsertDuplicateElement) {
  containerofunique::vector_of_unique<std::string> vou = {"hello", "world"};
  auto result = vou.insert(vou.cend(), "hello");
  EXPECT_EQ(result.first, vou.cend());
  EXPECT_EQ(result.second, false);
  EXPECT_EQ(vou.vector(), (std::vector<std::string>{"hello", "world"}));
}

TEST(VectorOfUniqueTest, InsertIntoEmptyVector) {
  containerofunique::vector_of_unique<std::string> vou;
  auto result = vou.insert(vou.cend(), "first");
  EXPECT_EQ(*result.first, vou.front());
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(vou.vector(), (std::vector<std::string>{"first"}));
}

TEST(VectorOfUniqueTest, InsertAtSpecificPosition) {
  containerofunique::vector_of_unique<std::string> vou = {"hello", "world"};
  auto it = vou.cbegin();
  auto result = vou.insert(it + 1, "goodbye");

  EXPECT_EQ(*result.first, *(vou.cbegin() + 1));
  EXPECT_EQ(result.second, true);
  EXPECT_EQ(vou.vector(),
            (std::vector<std::string>{"hello", "goodbye", "world"}));
}

TEST(VectorOfUniqueTest, EmplaceIntoEmpty) {
  containerofunique::vector_of_unique<std::string> vou;
  std::vector<std::string> vec;

  auto result = vou.emplace(vou.cbegin(), "hello");
  vec.emplace(vec.begin(), "hello");

  EXPECT_EQ(vou.vector(), vec);
  EXPECT_EQ(*result.first, *vou.cbegin());
  EXPECT_TRUE(result.second);
}

TEST(VectorOfUniqueTest, EmplaceAtEnd) {
  containerofunique::vector_of_unique<std::string> vou = {"hello", "world"};
  std::vector<std::string> vec = {"hello", "world"};

  auto result = vou.emplace(vou.cend(), "goodbye");
  vec.emplace(vec.end(), "goodbye");

  EXPECT_EQ(vou.vector(), vec);
  EXPECT_EQ(*result.first, *(vou.cend() - 1));
  EXPECT_TRUE(result.second);
}

struct ThrowingType {
  std::string value;
  ThrowingType(const std::string &val) : value(val) {
    if (val == "throw") {
      throw std::runtime_error("Test exception");
    }
  }
  bool operator==(const ThrowingType &other) const {
    return value == other.value;
  }
};

namespace std {
template <> struct hash<ThrowingType> {
  size_t operator()(const ThrowingType &obj) const {
    (void)obj;
    return 0;
  }
};
} // namespace std

TEST(VectorOfUniqueTest, EmplaceExceptionSafety) {
  containerofunique::vector_of_unique<ThrowingType> vou;

  // Normal insertion
  EXPECT_NO_THROW(vou.emplace(vou.cbegin(), "hello"));

  // Exception-throwing insertion
  EXPECT_THROW(vou.emplace(vou.cbegin(), "throw"), std::runtime_error);

  // Ensure the container remains consistent
  EXPECT_EQ(vou.vector().size(), 1);
  EXPECT_EQ(vou.vector().front().value, "hello");
}

TEST(VectorOfUniqueTest, EmplaceNonString) {
  containerofunique::vector_of_unique<int> vou = {1, 2, 3};
  std::vector<int> vec = {1, 2, 3};

  auto result = vou.emplace(vou.cbegin(), 4);
  vec.emplace(vec.begin(), 4);

  EXPECT_EQ(vou.vector(), vec);
  EXPECT_EQ(*result.first, *vou.cbegin());
  EXPECT_TRUE(result.second);

  // Attempt to emplace a duplicate
  result = vou.emplace(vou.cbegin(), 4);
  EXPECT_EQ(vou.vector(), vec); // No change
  EXPECT_FALSE(result.second);
}

TEST(VectorOfUniqueTest, EmplaceBack_NewElement) {
  // Test 1: Emplace a new element "good" to the end of vou
  // Emplace_back "good" to vou1
  containerofunique::vector_of_unique<std::string> vou = {"hello", "world"};
  std::vector<std::string> vec = {"hello", "world"};
  auto result = vou.emplace_back("good");
  vec.emplace_back("good");
  EXPECT_EQ(*(vou.cend() - 1), "good");
  ASSERT_TRUE(result.has_value());
  // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
  EXPECT_EQ(result.value().get(), "good");
  EXPECT_EQ(vou.vector(), vec);
}

TEST(VectorOfUniqueTest, EmplaceBack_DuplicateElement) {
  // Test 2: Try emplacing "good" twice (duplicate value)
  // Expected: No insertion, vector remains unchanged
  containerofunique::vector_of_unique<std::string> vou = {"hello", "world"};
  std::vector<std::string> vec = {"hello", "world"};
  vou.emplace_back("good");
  vec.emplace_back("good");
  auto result = vou.emplace_back("good");
  EXPECT_EQ(*(vou.cend() - 1), "good");
  EXPECT_EQ(result, std::nullopt);
  EXPECT_EQ(vou.vector(), vec);
}

TEST(VectorOfUniqueTest, EmplaceBack_Rvalue) {
  // Test 3: Emplace an rvalue "good" to the back of vou
  // Expected: "good" should be inserted at the end of the vector
  containerofunique::vector_of_unique<std::string> vou = {"hello", "world"};
  std::string str = "good";
  auto result = vou.emplace_back(std::move(str));
  std::vector<std::string> vec = {"hello", "world", "good"};
  EXPECT_EQ(*(vou.cend() - 1), "good");
  ASSERT_TRUE(result.has_value());
  // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
  EXPECT_EQ(result.value().get(), "good");
  EXPECT_EQ(vou.vector(), vec);
}

TEST(VectorOfUniqueTest, EmplaceBack_DuplicateRvalue) {
  // Test 4: Try emplacing the rvalues "good" twice (duplicate value)
  // Expected: No insertion, vector remains unchanged
  containerofunique::vector_of_unique<std::string> vou = {"hello", "world"};
  std::string str1 = "good";
  std::string str2 = "good";
  vou.emplace_back(std::move(str1));
  auto result = vou.emplace_back(std::move(str2));
  std::vector<std::string> vec = {"hello", "world", "good"};
  EXPECT_EQ(*(vou.cend() - 1), "good");
  EXPECT_EQ(result, std::nullopt);
  EXPECT_EQ(vou.vector(), vec);
}

TEST(VectorOfUniqueTest, EmplaceBack_EmptyVector) {
  // Test 5: Emplace a new element "first" to an empty vector
  // Expected: "first" should be at the back of the vector
  containerofunique::vector_of_unique<std::string> vou_empty;
  auto result_empty = vou_empty.emplace_back("first");
  std::vector<std::string> vec = {"first"};
  EXPECT_EQ(*vou_empty.cbegin(), "first");
  ASSERT_TRUE(result_empty.has_value());
  // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
  EXPECT_EQ(result_empty.value().get(), "first");
  EXPECT_EQ(vou_empty.vector(), vec);
}

TEST(VectorOfUniqueTest, EmplaceBack_MultipleElements) {
  // Test 6: Emplace multiple distinct elements to the back of the vector
  // Expected: Elements should be inserted at the back in the order of
  // emplace_back calls
  containerofunique::vector_of_unique<std::string> vou = {"hello"};
  vou.emplace_back("world");
  vou.emplace_back("good");
  vou.emplace_back("morning");

  std::vector<std::string> vec = {"hello", "world", "good", "morning"};

  EXPECT_EQ(*(vou.cend() - 1), "morning");
  EXPECT_EQ(*(vou.cend() - 2), "good");
  EXPECT_EQ(*(vou.cend() - 3), "world");
  EXPECT_EQ(*(vou.cend() - 4), "hello");
  EXPECT_EQ(vou.vector(), vec);
}

TEST(VectorOfUniqueTest, EmplaceBack_NonStringType) {
  // Test 7: Emplace an integer to the back of an integer vector (non-string
  // type) Expected: The integer 4 should be at the back of the vector
  containerofunique::vector_of_unique<int> vou = {1, 2, 3};
  auto result = vou.emplace_back(4);
  std::vector<int> vec = {1, 2, 3, 4};
  EXPECT_EQ(*(vou.cend() - 1), 4);
  ASSERT_TRUE(result.has_value());
  // NOLINTNEXTLINE(bugprone-unchecked-optional-access,-warnings-as-errors)
  EXPECT_EQ(result.value().get(), 4);
  EXPECT_EQ(vou.vector(), vec);
}

TEST(VectorOfUniqueTest, PopBack_EmptyVector) {
  containerofunique::vector_of_unique<std::string> vou;
  EXPECT_NO_THROW(vou.pop_back());
  EXPECT_TRUE(vou.vector().empty());
  EXPECT_TRUE(vou.set().empty());
}

TEST(VectorOfUniqueTest, PopBack_SingleElement) {
  containerofunique::vector_of_unique<std::string> vou = {"hello"};
  vou.pop_back();
  EXPECT_TRUE(vou.vector().empty());
  EXPECT_TRUE(vou.set().empty());
}

TEST(VectorOfUniqueTest, PopBack_MultipleSequential) {
  containerofunique::vector_of_unique<std::string> vou = {"hello", "world",
                                                          "goodbye"};
  vou.pop_back();
  EXPECT_EQ(vou.vector(), (std::vector<std::string>{"hello", "world"}));
  EXPECT_THAT(vou.set(), ::testing::UnorderedElementsAre("hello", "world"));

  vou.pop_back();
  EXPECT_EQ(vou.vector(), (std::vector<std::string>{"hello"}));
  EXPECT_THAT(vou.set(), ::testing::UnorderedElementsAre("hello"));

  vou.pop_back();
  EXPECT_TRUE(vou.vector().empty());
  EXPECT_TRUE(vou.set().empty());
}

TEST(VectorOfUniqueTest, PushBack_NewElement) {
  containerofunique::vector_of_unique<std::string> vou = {"hello", "world"};
  std::vector<std::string> expected = {"hello", "world", "good"};

  // Test pushing a new element to the back
  bool result = vou.push_back("good");
  EXPECT_TRUE(result); // Should return true
  EXPECT_EQ(vou.vector(), expected);
  EXPECT_THAT(vou.set(), ::testing::UnorderedElementsAreArray(expected));
}

TEST(VectorOfUniqueTest, PushBack_DuplicateElement) {
  containerofunique::vector_of_unique<std::string> vou = {"hello", "world"};
  std::vector<std::string> expected = {"hello", "world"};

  // Test pushing a duplicate element
  bool result = vou.push_back("hello");
  EXPECT_FALSE(result); // Should return false
  EXPECT_EQ(vou.size(), 2);
  EXPECT_EQ(vou.vector(), expected);
  EXPECT_THAT(vou.set(), ::testing::UnorderedElementsAreArray(expected));
}

TEST(VectorOfUniqueTest, PushBack_Rvalue) {
  containerofunique::vector_of_unique<std::string> vou = {"hello", "world"};
  std::vector<std::string> expected = {"hello", "world", "good"};

  // Test pushing an rvalue to the back
  std::string str = "good";
  bool result = vou.push_back(std::move(str));
  EXPECT_TRUE(result); // Should return true
  EXPECT_EQ(vou.vector(), expected);
  EXPECT_THAT(vou.set(), ::testing::UnorderedElementsAreArray(expected));
}

TEST(VectorOfUniqueTest, PushBack_EmptyRvalue) {
  containerofunique::vector_of_unique<std::string> vou = {"hello", "world"};
  std::vector<std::string> expected = {"hello", "world", ""};

  // Test pushing an empty string as an rvalue
  std::string str = "";
  bool result = vou.push_back(std::move(str));
  EXPECT_TRUE(result); // Should return true
  EXPECT_EQ(vou.vector(), expected);
  EXPECT_THAT(vou.set(), ::testing::UnorderedElementsAreArray(expected));
}

TEST(VectorOfUniqueTest, PushBack_EmptyContainer) {
  containerofunique::vector_of_unique<std::string> vou;
  std::vector<std::string> expected = {"hello"};

  // Test pushing to an initially empty container
  bool result = vou.push_back("hello");
  EXPECT_TRUE(result); // Should return true
  EXPECT_EQ(vou.vector(), expected);
  EXPECT_THAT(vou.set(), ::testing::UnorderedElementsAreArray(expected));
}

TEST(VectorOfUniqueTest, MemberFunctionSwap) {
  containerofunique::vector_of_unique<std::string> vou1 = {"hello", "world"};
  containerofunique::vector_of_unique<std::string> vou2 = {"good", "morning"};
  std::vector<std::string> vec1 = {"hello", "world"};
  std::vector<std::string> vec2 = {"good", "morning"};

  vou1.swap(vou2);
  EXPECT_EQ(vou1.vector(), vec2);
  EXPECT_THAT(vou1.set(), ::testing::UnorderedElementsAreArray(vec2));
  EXPECT_EQ(vou2.vector(), vec1);
  EXPECT_THAT(vou2.set(), ::testing::UnorderedElementsAreArray(vec1));
}

TEST(VectorOfUniqueTest, StdSwap) {
  containerofunique::vector_of_unique<std::string> vou1 = {"hello", "world"};
  containerofunique::vector_of_unique<std::string> vou2 = {"good", "morning"};
  std::vector<std::string> vec1 = {"hello", "world"};
  std::vector<std::string> vec2 = {"good", "morning"};

  std::swap(vou1, vou2);
  EXPECT_EQ(vou1.vector(), vec2);
  EXPECT_THAT(vou1.set(), ::testing::UnorderedElementsAreArray(vec2));
  EXPECT_EQ(vou2.vector(), vec1);
  EXPECT_THAT(vou2.set(), ::testing::UnorderedElementsAreArray(vec1));
}

TEST(VectorOfUniqueTest, Empty) {
  containerofunique::vector_of_unique<std::string> vou1;
  EXPECT_TRUE(vou1.empty());
  containerofunique::vector_of_unique<std::string> vou2 = {"good"};
  EXPECT_FALSE(vou2.empty());
  containerofunique::vector_of_unique<std::string> vou3 = {"good", "morning",
                                                           "hello", "world"};
  EXPECT_FALSE(vou3.empty());
}

TEST(VectorOfUniqueTest, Size) {
  // Test 1: Vector with a single element
  containerofunique::vector_of_unique<std::string> vou1 = {"good"};
  EXPECT_EQ(vou1.size(), 1);

  // Test 2: Vector with multiple unique elements
  containerofunique::vector_of_unique<std::string> vou2 = {"good", "morning",
                                                           "hello", "world"};
  EXPECT_EQ(vou2.size(), 4);

  // Adding a new unique element increases the size
  vou2.push_back("new");
  EXPECT_EQ(vou2.size(), 5);

  // Attempting to add a duplicate element does not change the size
  vou2.push_back("morning"); // "morning" is already in the vector
  EXPECT_EQ(vou2.size(), 5);

  // Test 3: Empty vector
  containerofunique::vector_of_unique<std::string> vou3;
  EXPECT_EQ(vou3.size(), 0); // Corrected to check vou3
}

TEST(VectorOfUniqueTest, Operator) {
  // Test 1: Identical vectors
  containerofunique::vector_of_unique<std::string> vou1_1 = {"good"};
  containerofunique::vector_of_unique<std::string> vou1_2 = {"good"};
  EXPECT_EQ(vou1_1 <=> vou1_2, std::strong_ordering::equal);

  // Test 2: Subset case
  containerofunique::vector_of_unique<std::string> vou2 = {"good", "morning"};
  EXPECT_EQ(vou1_1 <=> vou2, std::weak_ordering::less);
  EXPECT_EQ(vou2 <=> vou1_1, std::weak_ordering::greater);

  // Test 3: Different order case (for robustness, even if not expected in
  // unique vectors)
  containerofunique::vector_of_unique<std::string> vou3 = {"morning", "good"};
  EXPECT_EQ(vou2 <=> vou3, std::weak_ordering::less);

  // Test 4: Lexicographical comparison
  containerofunique::vector_of_unique<std::string> vou4 = {"apple", "banana"};
  EXPECT_EQ(vou4 <=> vou1_1, std::weak_ordering::less);

  // Test 5: Empty vectors
  containerofunique::vector_of_unique<std::string> vou_empty1;
  containerofunique::vector_of_unique<std::string> vou_empty2;
  EXPECT_EQ(vou_empty1 <=> vou_empty2, std::strong_ordering::equal);
  EXPECT_EQ(vou_empty1 <=> vou1_1, std::weak_ordering::less);
}
