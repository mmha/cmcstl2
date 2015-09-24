// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_ALGORITHM_MIN_ELEMENT_HPP
#define STL2_DETAIL_ALGORITHM_MIN_ELEMENT_HPP

#include <initializer_list>
#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/callable.hpp>

///////////////////////////////////////////////////////////////////////////
// min_element [alg.min.max]
//
STL2_OPEN_NAMESPACE {
  template <ForwardIterator I, Sentinel<I> S, class Proj = identity,
            IndirectCallableStrictWeakOrder<Projected<I, Proj>> Comp = less<>>
  I min_element(I first, S last, Comp comp_ = Comp{}, Proj proj_ = Proj{}) {
    if (first != last) {
      auto&& comp = __stl2::as_function(comp_);
      auto&& proj = __stl2::as_function(proj_);
      for (auto i = __stl2::next(first); i != last; ++i) {
        if (comp(proj(*i), proj(*first))) {
          first = i;
        }
      }
    }
    return first;
  }

  template <ForwardRange Rng, class Proj = identity,
            IndirectCallableStrictWeakOrder<Projected<IteratorType<Rng>, Proj>> Comp = less<>>
  safe_iterator_t<Rng>
  min_element(Rng&& rng, Comp&& comp = Comp{}, Proj&& proj = Proj{}) {
    return __stl2::min_element(__stl2::begin(rng), __stl2::end(rng),
                               __stl2::forward<Comp>(comp),
                               __stl2::forward<Proj>(proj));
  }
} STL2_CLOSE_NAMESPACE

#endif