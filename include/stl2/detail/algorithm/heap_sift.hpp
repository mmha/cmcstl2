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
#ifndef STL2_DETAIL_ALGORITHM_HEAP_SIFT_HPP
#define STL2_DETAIL_ALGORITHM_HEAP_SIFT_HPP

#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/callable.hpp>

///////////////////////////////////////////////////////////////////////////
// detail::sift_up_n and detail::sift_down_n
// (heap implementation details)
//
STL2_OPEN_NAMESPACE {
  namespace detail {
    template <RandomAccessIterator I, class Proj,
              IndirectCallableStrictWeakOrder<Projected<I, Proj>,
                                              Projected<I, Proj>> Comp>
    void sift_up_n(I first, DifferenceType<I> n, Comp& comp_, Proj& proj_) {
      if (n > 1) {
        auto &&comp = __stl2::as_function(comp_);
        auto &&proj = __stl2::as_function(proj_);
        I last = first + n;
        n = (n - 2) / 2;
        I i = first + n;
        if (comp(proj(*i), proj(*--last))) {
          ValueType<I> v = __stl2::iter_move(last);
          do {
            *last = __stl2::iter_move(i);
            last = i;
            if (n == 0) {
              break;
            }
            n = (n - 1) / 2;
            i = first + n;
          } while(comp(proj(*i), proj(v)));
          *last = std::move(v);
        }
      }
    }

    template <RandomAccessIterator I, class Proj,
              IndirectCallableStrictWeakOrder<Projected<I, Proj>,
                                              Projected<I, Proj>> Comp>
    void sift_down_n(I first, DifferenceType<I> n, I start, Comp& comp_, Proj& proj_)
    {
      // left-child of start is at 2 * start + 1
      // right-child of start is at 2 * start + 2
      auto child = start - first;

      if (n < 2 || (n - 2) / 2 < child) {
        return;
      }

      child = 2 * child + 1;
      I child_i = first + child;

      auto &&comp = __stl2::as_function(comp_);
      auto &&proj = __stl2::as_function(proj_);

      if ((child + 1) < n && comp(proj(*child_i), proj(*(child_i + 1)))) {
        // right-child exists and is greater than left-child
        ++child_i;
        ++child;
      }

      // check if we are in heap-order
      if (comp(proj(*child_i), proj(*start))) {
        // we are, start is larger than its largest child
        return;
      }

      ValueType<I> top = __stl2::iter_move(start);
      do {
        // we are not in heap-order, swap the parent with it's largest child
        *start = __stl2::iter_move(child_i);
        start = child_i;

        if ((n - 2) / 2 < child) {
          break;
        }

        // recompute the child based off of the updated parent
        child = 2 * child + 1;
        child_i = first + child;

        if ((child + 1) < n && comp(proj(*child_i), proj(*(child_i + 1)))) {
          // right-child exists and is greater than left-child
          ++child_i;
          ++child;
        }

        // check if we are in heap-order
      } while (!comp(proj(*child_i), proj(top)));
      *start = std::move(top);
    }
  }
} STL2_CLOSE_NAMESPACE

#endif