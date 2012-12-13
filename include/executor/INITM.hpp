#pragma once
/** @file INITM.hpp
 * @brief Dispatch methods for the initializing a multipole expansion
 *
 */

#include "KernelTraits.hpp"
#include <type_traits>

class INITM
{
  /** If no init_multipole dispatcher matches */
  template <typename... Args>
  inline static void eval(Args...) {
    // Do nothing
  }

  template <typename Kernel>
  inline static
  typename std::enable_if<ExpansionTraits<Kernel>::has_init_multipole>::type
  eval(const Kernel& K,
       typename Kernel::multipole_type& M,
       double box_size) {
    K.init_multipole(M, box_size);
  }

 public:

  template <typename Kernel, typename Context>
  inline static void eval(Kernel& K,
                          Context& bc,
                          const typename Context::box_type& b)
  {
#ifdef DEBUG
    printf("initM: %d\n", b.index());
#endif

    INITM::eval(K, bc.multipole_expansion(b), bc.box_size(b));
  }
};