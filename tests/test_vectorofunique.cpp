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

TEST(VectorOfUniqueTest, ConstructorWithInitializerListChecksVectorAndSet) {
  containerofunique::vector_of_unique vou1 = {1};
  containerofunique::vector_of_unique vou2 = {1, 2};
  containerofunique::vector_of_unique vou3 = {1, 2, 3, 3};  // duplicate elements

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

TEST(VectorOfUniqueTest, CopyConstructor) {
  containerofunique::vector_of_unique vou1 = {1, 2, 3, 4};
  containerofunique::vector_of_unique<int> vou2(vou1);
  std::vector<int> dq = {1, 2, 3, 4};
  EXPECT_EQ(vou2.vector(), vou1.vector());
  vou1.push_back(
      5);  // This is used to suppress warning of
           // [performance-unnecessary-copy-initialization,-warnings-as-errors]
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
      5);  // This is used to suppress warning of
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
