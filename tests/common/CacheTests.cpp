/*
 * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
 * Copyright (C) 2013  Luke San Antonio
 *
 * You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * \file Cache.h
 * \brief Contains the Cache_Impl class declaration (and aliases).
 */
#include "common/Cache.h"
#include "gtest/gtest.h"

using pong::Cache;

/*!
 * \brief Eases the creation of cache gen functions that don't have any logic
 * and will just return one value.
 */
template <typename T, class... Depends>
struct constant_cache_val
{
  constant_cache_val(const T& t) noexcept : t(t) {}

  const T& t;

  using cache_type = Cache<T, Depends...>;
  typename cache_type::ptr_type
  operator()(typename cache_type::ptr_type&& ptr, ...)
  {
    ptr.reset(new T(t));
    return std::move(ptr);
  }
};

class CacheTest : public ::testing::Test
{
protected:
  Cache<int> cache;
  using ptr_type = decltype(cache)::ptr_type;

  int generates = 0;

  virtual void SetUp()
  {
    // Dummy default counts runs.
    cache.gen_func(
    [&](ptr_type&& p)
    {
      ++generates;
      p.reset(new int());
      return std::move(p);
    });
  }
};

TEST_F(CacheTest, Invalidate)
{
  // We haven't run that lambda have we, yet?
  EXPECT_EQ(0, generates);

  // Our cache should have been successful.
  EXPECT_TRUE(cache.generate());

  // Forget that value.
  cache.invalidate();
  EXPECT_TRUE(cache.generate());

  // We should have generated that string twice now.
  EXPECT_EQ(2, generates);
}
TEST_F(CacheTest, Generate)
{
  cache.generate();
  cache.generate();
  cache.generate();
  EXPECT_EQ(3, generates);
}
TEST_F(CacheTest, Cache)
{
  cache.invalidate();

  // ccache shouldn't regenerate!
  cache.ccache();
  EXPECT_EQ(0, generates);

  // cache *should* generate, especially since we already invalidated the
  // current cache.
  cache.cache();
  EXPECT_EQ(1, generates);

  cache.invalidate();

  // What's our current Cache? What's our current Cache? What's our current
  // Cache?
  cache.ccache();
  cache.ccache();
  cache.ccache();

  // This. This. This! (No generations should have occurred, the "This" should
  // be non conditional.)
  EXPECT_EQ(1, generates);

}
TEST(CacheTestsOther, DependenciesAreSet)
{
  Cache<int, int> cache;

  // Set the int dependency.
  int dependency_expected = 5;
  cache.set_dependency<0>(dependency_expected);

  // Make sure get_dependency works.
  EXPECT_EQ(dependency_expected, cache.get_dependency<0>());

  // Now check whether it's passed to the gen func correctly.
  cache.gen_func(
  [](Cache<int, int>::ptr_type ptr, int x)
  {
    ptr.reset(new int(x));
    return std::move(ptr);
  });

  // The cache value should be equal to the int dependency.
  EXPECT_EQ(dependency_expected, *cache.cache());
}
