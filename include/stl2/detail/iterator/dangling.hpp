// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Eric Niebler 2015
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_ITERATOR_DANGLING_HPP
#define STL2_DETAIL_ITERATOR_DANGLING_HPP

#include <type_traits>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/meta.hpp>
#include <stl2/detail/concepts/object.hpp>
#include <stl2/detail/range/concepts.hpp>

STL2_OPEN_NAMESPACE {
	template <ext::CopyConstructibleObject T> // Not to spec
	class dangling {
		T value = {};
	public:
		constexpr dangling() requires DefaultConstructible<T> = default;
		constexpr dangling(T t)
		noexcept(std::is_nothrow_move_constructible<T>::value)
		: value(std::move(t)) {}

		constexpr T get_unsafe() const
		noexcept(std::is_nothrow_copy_constructible<T>::value) {
			return value;
		}
	};

	template <Range R>
	using safe_iterator_t =
		meta::if_<std::is_lvalue_reference<R>, iterator_t<R>, dangling<iterator_t<R>>>;
} STL2_CLOSE_NAMESPACE

#endif
