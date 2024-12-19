#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <compare>
#include <concepts>
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
  std::vector<int> expect_vec = {3, 1, 2, 4, 5};
  containerofunique::vector_of_unique<int> vou(vec.begin(), vec.end());

  EXPECT_EQ(vou.vector(), expect_vec);
  EXPECT_THAT(std::vector<int>(vou.set().begin(), vou.set().end()),
              ::testing::UnorderedElementsAreArray(expect_vec));
}

TEST(DequeOfUniqueTest, ConstructorWithInitializerListChecksVectorAndSet) {
  containerofunique::vector_of_unique dou1 = {1};
  containerofunique::vector_of_unique dou2 = {1, 2};
  containerofunique::vector_of_unique dou3 = {1, 2, 3, 3};  // duplicate elements

  std::vector<int> dq1 = {1};
  std::vector<int> dq2 = {1, 2};
  std::vector<int> dq3 = {1, 2, 3};

  EXPECT_EQ(dou1.vector(), dq1);
  EXPECT_EQ(dou2.vector(), dq2);
  EXPECT_EQ(dou3.vector(), dq3);

  EXPECT_THAT(std::vector<int>(dou1.set().begin(), dou1.set().end()),
              ::testing::UnorderedElementsAreArray(dq1));
  EXPECT_THAT(std::vector<int>(dou2.set().begin(), dou2.set().end()),
              ::testing::UnorderedElementsAreArray(dq2));
  EXPECT_THAT(std::vector<int>(dou3.set().begin(), dou3.set().end()),
              ::testing::UnorderedElementsAreArray(dq3));
}
