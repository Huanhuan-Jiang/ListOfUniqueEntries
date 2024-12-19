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