#pragma once

// 包含迭代器设计

#include <concepts>
#include <cstddef>
#include <type_traits>

#include "type_traits.hpp"

namespace mystl {

// 五种迭代器类型
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// iterator template
template <typename Category, typename T, typename Distance = ptrdiff_t,
          typename Pointer = T*, typename Reference = T&>
struct iterator {
    using iterator_category = Category;
    using value_type = T;
    using pointer = Pointer;
    using reference = Reference;
    using difference_type = Distance;
};

// iterator traits

template <class T>
concept HasIteratorCategory = requires {
    typename T::iterator_category;
};

template <class T>
struct has_iterator_cat {
    static constexpr bool value = HasIteratorCategory<T>;
};

template <typename Iterator, bool>
struct iterator_traits_impl {};

template <typename Iterator>
struct iterator_traits_impl<Iterator, true> {
    using iterator_category = typename Iterator::iterator_category;
    using value_type = typename Iterator::value_type;
    using pointer = typename Iterator::pointer;
    using reference = typename Iterator::reference;
    using difference_type = typename Iterator::difference_type;
};

template <typename Iterator, bool>
struct iterator_traits_helper {};

template <typename Iterator>
struct iterator_traits_helper<Iterator, true>
    : public iterator_traits_impl<
          Iterator,
          std::is_convertible_v<typename Iterator::iterator_category,
                                input_iterator_tag> ||
              std::is_convertible_v<typename Iterator::iterator_category,
                                    output_iterator_tag>> {};

// 萃取迭代器的特性
template <typename Iterator>
struct iterator_traits
    : public iterator_traits_helper<Iterator,
                                    has_iterator_cat<Iterator>::value> {};

// 针对原生指针的偏特化版本
template <typename T>
struct iterator_traits<T*> {
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = ptrdiff_t;
};

template <typename T>
struct iterator_traits<const T*> {
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using difference_type = ptrdiff_t;
};

template <typename T, typename U,
          bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of
    : public m_bool_constant<std::is_convertible_v<
          typename iterator_traits<T>::iterator_category, U>> {};

// 萃取某种迭代器
template <typename T, typename U>
struct has_iterator_cat_of<T, U, false> : public m_false_type {};

template <typename Iter>
struct is_input_iterator
    : public has_iterator_cat_of<Iter, input_iterator_tag> {};

template <typename Iter>
struct is_output_iterator
    : public has_iterator_cat_of<Iter, output_iterator_tag> {};

template <typename Iter>
struct is_forward_iterator
    : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

template <typename Iter>
struct is_bidirectional_iterator
    : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

template <typename Iter>
struct is_random_access_iterator
    : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

template <typename Iterator>
struct is_iterator
    : public m_bool_constant<is_input_iterator<Iterator>::value ||
                             is_output_iterator<Iterator>::value> {};

// 萃取某个迭代器的 category
template <typename Iterator>
typename iterator_traits<Iterator>::iterator_category iterator_category(
    const Iterator&) {
    using Category = typename iterator_traits<Iterator>::iterator_category;
    return Category{};
}

// 萃取某个迭代器的 distance_type
template <typename Iterator>
typename iterator_traits<Iterator>::difference_type* distance_type(
    const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 萃取某个迭代器的 value_type

template <typename Iterator>
typename iterator_traits<Iterator>::value_type* value_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// 以下函数用于计算迭代器间的距离

// distance 的 input_iterator_tag 的版本
template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type distance_dispatch(
    InputIterator first, InputIterator last, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}





}  // namespace mystl