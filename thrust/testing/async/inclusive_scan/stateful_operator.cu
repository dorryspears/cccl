#include <cuda/__cccl_config>

_CCCL_SUPPRESS_DEPRECATED_PUSH

#include <thrust/detail/config.h>

#include <async/inclusive_scan/mixin.h>
#include <async/test_policy_overloads.h>

namespace
{

// Custom binary operator for scan:
template <typename T>
struct stateful_operator
{
  T offset;

  __host__ __device__ T operator()(T v1, T v2)
  {
    return v1 + v2 + offset;
  }
};

// Postfix args overload definition that uses a stateful custom binary operator
template <typename value_type>
struct use_stateful_operator
{
  using postfix_args_type = std::tuple< // Single overload:
    std::tuple<stateful_operator<value_type>> // bin_op
    >;

  static postfix_args_type generate_postfix_args()
  {
    return postfix_args_type{std::make_tuple(stateful_operator<value_type>{value_type{2}})};
  }
};

template <typename value_type>
struct invoker
    : testing::async::mixin::input::device_vector<value_type>
    , testing::async::mixin::output::device_vector<value_type>
    , use_stateful_operator<value_type>
    , testing::async::inclusive_scan::mixin::invoke_reference::host_synchronous<value_type>
    , testing::async::inclusive_scan::mixin::invoke_async::simple
    , testing::async::mixin::compare_outputs::assert_almost_equal_if_fp_quiet
{
  static std::string description()
  {
    return "scan with stateful operator";
  }
};

} // namespace

template <typename T>
struct test_stateful_operator
{
  void operator()(std::size_t num_values) const
  {
    testing::async::test_policy_overloads<invoker<T>>::run(num_values);
  }
};
DECLARE_GENERIC_SIZED_UNITTEST_WITH_TYPES(test_stateful_operator, NumericTypes);

_CCCL_SUPPRESS_DEPRECATED_POP
