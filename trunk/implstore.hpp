#ifndef IMPLSTORE_HPP
# define IMPLSTORE_HPP
# pragma once

#include <cstddef>

#include <new>

#include <type_traits>

#include <utility>

namespace generic
{

template <class U, ::std::size_t N = 64>
class implstore
{
public:
  static constexpr ::std::size_t buffer_size = N;

  using value_type = U;

  template <typename ...A, typename =
    typename ::std::enable_if<::std::is_constructible<U, A...>{}>::type>
  implstore(A&& ...args)
  {
    static_assert(sizeof(U) <= sizeof(store_),
      "impl too large");
    new (static_cast<void*>(&store_)) U(::std::forward<A>(args)...);
  }

  ~implstore() { get()->~U(); }

  template <::std::size_t M, typename K = U, typename =
    typename ::std::enable_if<::std::is_copy_constructible<K>{}>::type>
  implstore(implstore<U, M> const& other)
  {
    new (static_cast<void*>(&store_)) U(*other);
  }

  template <::std::size_t M, typename K = U, typename =
    typename ::std::enable_if<::std::is_move_constructible<K>{}>::type>
  implstore(implstore<U, M>&& other)
  {
    new (static_cast<void*>(&store_)) U(::std::move(*other));
  }

  template <::std::size_t M, typename K = U, typename =
    typename ::std::enable_if<::std::is_copy_assignable<K>{}>::type>
  implstore& operator=(implstore<U, M> const& other)
  {
    **this = *other;

    return *this;
  }

  template <::std::size_t M, typename K = U, typename =
    typename ::std::enable_if<::std::is_move_assignable<K>{}>::type>
  implstore& operator=(implstore<U, M>&& other)
  {
    **this = ::std::move(*other);

    return *this;
  }

  U const* operator->() const noexcept
  {
    return reinterpret_cast<U*>(&store_);
  }

  U* operator->() noexcept
  {
    return reinterpret_cast<U*>(&store_);
  }

  U const* get() const noexcept
  {
    return reinterpret_cast<U*>(&store_);
  }

  U* get() noexcept
  {
    return reinterpret_cast<U*>(&store_);
  }

  U const& operator*() const noexcept
  {
    return *reinterpret_cast<U*>(&store_);
  }

  U& operator*() noexcept
  {
    return *reinterpret_cast<U*>(&store_);
  }

private:
  typename ::std::aligned_storage<N>::type store_;
};

}

#endif // IMPLSTORE_HPP
