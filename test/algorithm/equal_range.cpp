// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <stl2/detail/algorithm/equal_range.hpp>
#include <vector>
#include <iterator>
#include "../simple_test.hpp"
#include "../test_iterators.hpp"

namespace ranges = __stl2;

struct my_int
{
	int value;
};

bool compare(my_int lhs, my_int rhs)
{
	return lhs.value < rhs.value;
}

void not_totally_ordered()
{
	// This better compile!
	std::vector<my_int> vec;
	ranges::equal_range(vec, my_int{10}, compare);
}

template <class Iter, class Sent, class T, class Proj = ranges::identity>
void
test(Iter first, Sent last, const T& value, Proj proj = Proj{})
{
	auto i = ranges::equal_range(first, last, value, ranges::less<>{}, proj);
	for (Iter j = first; j != i.begin(); ++j)
		CHECK(ranges::invoke(proj, *j) < value);
	for (Iter j = i.begin(); j != last; ++j)
		CHECK(!(ranges::invoke(proj, *j) < value));
	for (Iter j = first; j != i.end(); ++j)
		CHECK(!(value < ranges::invoke(proj, *j)));
	for (Iter j = i.end(); j != last; ++j)
		CHECK(value < ranges::invoke(proj, *j));

	auto res = ranges::equal_range(
		ranges::ext::make_range(first, last), value, ranges::less<>{}, proj);
	for (Iter j = first; j != res.begin().get_unsafe(); ++j)
		CHECK(ranges::invoke(proj, *j) < value);
	for (Iter j = res.begin().get_unsafe(); j != last; ++j)
		CHECK(!(ranges::invoke(proj, *j) < value));
	for (Iter j = first; j != res.end().get_unsafe(); ++j)
		CHECK(!(value < ranges::invoke(proj, *j)));
	for (Iter j = res.end().get_unsafe(); j != last; ++j)
		CHECK(value < ranges::invoke(proj, *j));
}

template <class Iter, class Sent = Iter>
void
test()
{
#if 0
	using namespace ranges::view;
	static constexpr unsigned M = 10;
	std::vector<int> v;
	auto input = ints | take(100) | transform([](int i){return repeat_n(i,M);}) | join;
	ranges::copy(input, ranges::back_inserter(v));
	for (int x = 0; x <= (int)M; ++x)
		test(Iter(v.data()), Sent(v.data()+v.size()), x);
#endif
}

int main()
{
	int d[] = {0, 1, 2, 3};
	for (int* e = d; e <= d+4; ++e)
		for (int x = -1; x <= 4; ++x)
			test(d, e, x);

	test<forward_iterator<const int*> >();
	test<bidirectional_iterator<const int*> >();
	test<random_access_iterator<const int*> >();
	test<const int*>();

	test<forward_iterator<const int*>, sentinel<const int*> >();
	test<bidirectional_iterator<const int*>, sentinel<const int*> >();
	test<random_access_iterator<const int*>, sentinel<const int*> >();

	{
		struct foo { int i; };

		foo some_foos[] = {{1}, {2}, {4}};
		test(some_foos, some_foos + 3, 2, &foo::i);
	}

	return ::test_result();
}
