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
  std::vector<int> vec = {3, 1, 2, 3, 4, 5};
  containerofunique::vector_of_unique<int> vou(vec.begin(), vec.end());

  EXPECT_EQ(vou.vector(), vec);
  EXPECT_THAT(std::vector<int>(vou.set().begin(), vou.set().end()),
              ::testing::UnorderedElementsAreArray(vec));
}

TEST(VectorOfUniqueTest, ConstructorWithInitializerListChecksVectorAndSet) {
  containerofunique::vector_of_unique vou1 = {1};
  containerofunique::vector_of_unique vou2 = {1, 2};
  containerofunique::vector_of_unique vou3 = {1, 2, 3, 3}; // duplicate elements

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

TEST(VectorOfUniqueTest, Iterators) {
  containerofunique::vector_of_unique<int> vou = {1, 2, 3, 4};
  EXPECT_EQ(*vou.cbegin(), 1);
  EXPECT_EQ(*--vou.cend(), 4);
  EXPECT_EQ(*vou.crbegin(), 4);
  EXPECT_EQ(*--vou.crend(), 1);

  EXPECT_TRUE((std::same_as<decltype(*vou.cbegin()), const int &>));
  EXPECT_TRUE((std::same_as<decltype(*vou.cend()), const int &>));
  EXPECT_TRUE((std::same_as<decltype(*vou.crbegin()), const int &>));
  EXPECT_TRUE((std::same_as<decltype(*vou.crend()), const int &>));
}

TEST(VectorOfUniqueTest, ClearAndErase) {
  containerofunique::vector_of_unique<int> vou1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> dq1 = {2, 3, 4, 5, 6};
  std::unordered_set<int> set1 = {2, 3, 4, 5, 6};

  vou1.erase(vou1.cbegin());
  EXPECT_EQ(vou1.vector(), dq1);
  EXPECT_THAT(vou1.set(), ::testing::UnorderedElementsAreArray(set1));

  std::vector<int> dq2 = {4, 5, 6};
  std::unordered_set<int> set2 = {4, 5, 6};

  vou1.erase(vou1.cbegin(), vou1.cbegin() + 2);
  EXPECT_EQ(vou1.vector(), dq2);
  EXPECT_THAT(vou1.set(), ::testing::UnorderedElementsAreArray(set2));
}
