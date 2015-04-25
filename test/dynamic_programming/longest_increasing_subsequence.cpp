//          Copyright Diego Ramírez March 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <djp/dynamic_programming/longest_increasing_subsequence.hpp>
#include <bandit/bandit.h>
#include <vector>
#include <numeric>
using namespace bandit;

go_bandit([]{

  describe("longest increasing subsequence", []{
    it("Computes LIS size for small vectors",[]{
      std::vector<int> seq = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
      AssertThat(djp::calc_lis_size(seq.begin(), seq.end()), Equals(6));
    });

    it("Computes LIS size for long vectors", []{
      std::vector<int> vec(4096);
      std::iota(vec.begin(), vec.end(), 0);
      AssertThat(djp::calc_lis_size(vec.begin(), vec.end()), Equals(vec.size()));
      AssertThat(djp::calc_lis_size(vec.rbegin(), vec.rend()), Equals(1));
    });
  });

});

