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
  std::vector<int> emptydq;
  std::unordered_set<int> emptyset;

  EXPECT_EQ(vou.vector(), emptydq);
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

  std::vector<int> dq1 = {1};
  std::vector<int> dq2 = {1, 2};
  std::vector<int> dq3 = {1, 2, 3};

  EXPECT_EQ(vou1.vector(), dq1);
  EXPECT_EQ(vou2.vector(), dq2);
  EXPECT_EQ(vou3.vector(), dq3);

  EXPECT_THAT(std::vector<int>(vou1.set().begin(), vou1.set().end()),
              ::testing::UnorderedElementsAreArray(dq1));
  EXPECT_THAT(std::vector<int>(vou2.set().begin(), vou2.set().end()),
              ::testing::UnorderedElementsAreArray(dq2));
  EXPECT_THAT(std::vector<int>(vou3.set().begin(), vou3.set().end()),
              ::testing::UnorderedElementsAreArray(dq3));
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
  std::vector<int> dq = {1, 2, 3, 4};
  EXPECT_EQ(vou2.vector(), dq);
}

TEST(VectorOfUniqueTest, CopyAssignmentOperator) {
  containerofunique::vector_of_unique<int> vou1 = {1, 2, 3, 4};
  containerofunique::vector_of_unique<int> vou2 = vou1;
  std::vector<int> dq = {1, 2, 3, 4};

  EXPECT_EQ(vou2.vector(), vou1.vector());
  EXPECT_THAT(std::vector<int>(vou2.set().begin(), vou2.set().end()),
              ::testing::UnorderedElementsAreArray(dq));
  vou1.push_back(
      5); // This is used to suppress warning of
          // [performance-unnecessary-copy-initialization,-warnings-as-errors]
}

TEST(VectorOfUniqueTest, MoveAssignmentOperator) {
  containerofunique::vector_of_unique<int> vou1 = {1, 2, 3, 4};
  containerofunique::vector_of_unique<int> vou2 = std::move(vou1);
  std::vector<int> dq = {1, 2, 3, 4};

  EXPECT_EQ(vou2.vector(), dq);
  EXPECT_THAT(std::vector<int>(vou2.set().begin(), vou2.set().end()),
              ::testing::UnorderedElementsAreArray(dq));
}

TEST(VectorOfUniqueTest, InitializerListAssignmentOperator) {
  containerofunique::vector_of_unique<int> vou = {1, 2, 3, 4};
  std::vector<int> dq = {1, 2, 3, 4};
  EXPECT_EQ(vou.vector(), dq);
  EXPECT_THAT(std::vector<int>(vou.set().begin(), vou.set().end()),
              ::testing::UnorderedElementsAreArray(dq));
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
  std::vector<int> dq2 = {1, 2, 3, 4, 5, 6};
  std::unordered_set<int> set2 = {1, 2, 3, 4, 5, 6};

  auto result = vou1.erase(vou1.cbegin(), vou1.cbegin());
  EXPECT_EQ(result, vou1.cbegin());
  EXPECT_EQ(vou1.vector(), dq2);
  EXPECT_THAT(vou1.set(), ::testing::UnorderedElementsAreArray(set2));
}

TEST(VectorOfUniqueTest, EraseRangeOfElements) {
  containerofunique::vector_of_unique<int> vou1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> dq2 = {4, 5, 6};
  std::unordered_set<int> set2 = {4, 5, 6};

  vou1.erase(vou1.cbegin(), vou1.cbegin() + 3);
  EXPECT_EQ(vou1.vector(), dq2);
  EXPECT_THAT(vou1.set(), ::testing::UnorderedElementsAreArray(set2));
}

TEST(VectorOfUniqueTest, EraseAllElements) {
  containerofunique::vector_of_unique<int> vou1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> dq2 = {};
  std::unordered_set<int> set2 = {};

  auto result = vou1.erase(vou1.cbegin(), vou1.cend());
  EXPECT_EQ(result, vou1.cend());
  EXPECT_EQ(vou1.vector(), dq2);
  EXPECT_THAT(vou1.set(), ::testing::UnorderedElementsAreArray(set2));
}
