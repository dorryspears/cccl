#include <cuda/__cccl_config>

_CCCL_SUPPRESS_DEPRECATED_PUSH

#include <thrust/detail/config.h>

#include <algorithm>
#include <limits>

#include <async/exclusive_scan/mixin.h>
#include <async/test_policy_overloads.h>

template <typename input_value_type,
          typename output_value_type   = input_value_type,
          typename initial_value_type  = input_value_type,
          typename alternate_binary_op = thrust::maximum<>>
struct invoker
    : testing::async::mixin::input::counting_iterator_from_0<input_value_type>
    , testing::async::mixin::output::device_vector<output_value_type>
    , testing::async::exclusive_scan::mixin::postfix_args::all_overloads<initial_value_type, alternate_binary_op>
    , testing::async::exclusive_scan::mixin::invoke_reference::host_synchronous<input_value_type, output_value_type>
    , testing::async::exclusive_scan::mixin::invoke_async::simple
    , testing::async::mixin::compare_outputs::assert_almost_equal_if_fp_quiet
{
  static std::string description()
  {
    return "fancy input iterator (counting_iterator)";
  }
};

template <typename T>
struct test_counting_iterator
{
  void operator()(std::size_t num_values) const
  {
    num_values = unittest::truncate_to_max_representable<T>(num_values);
    testing::async::test_policy_overloads<invoker<T>>::run(num_values);
  }
};
// Use built-in types only, counting_iterator doesn't seem to be compatible with
// the custom_numeric.
DECLARE_GENERIC_SIZED_UNITTEST_WITH_TYPES(test_counting_iterator, UnsignedIntegralTypes);

_CCCL_SUPPRESS_DEPRECATED_POP
