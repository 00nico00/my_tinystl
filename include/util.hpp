#pragma once

// 工具组件，提供一些通用工具，例如 move, forward,
// swap

#include <compare>
#include <cstddef>
#include <type_traits>

#include "type_traits.hpp"

namespace mystl {

// move

template <typename T>
std::remove_reference_t<T>&& move(T&& arg) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(arg);
}

// forward

template <typename T>
T&& forward(std::remove_reference_t<T>& arg) noexcept {
    return static_cast<T&&>(arg);
}

template <typename T>
T&& forward(std::remove_reference_t<T>&& arg) noexcept {
    static_assert(!std::is_lvalue_reference_v<T>, "bad forward");
    return static_cast<T&&>(arg);
}

// swap

template <typename Tp>
void swap(Tp& lhs, Tp& rhs) {
    auto tmp{mystl::move(lhs)};
    lhs = mystl::move(rhs);
    rhs = mystl::move(tmp);
}

template <typename ForwardIter1, typename ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter2 last1,
                        ForwardIter2 first2) {
    for (; first1 != last1; ++first1, (void)++first2) {
        mystl::swap(*first1, *first2);
    }
    return first2;
}

template <typename Tp, size_t N>
void swap(Tp (&a)[N], Tp (&b)[N]) {
    mystl::swap_range(a, a + N, b);
}

// pair

template <typename Ty1, typename Ty2>
struct pair {
    using first_type = Ty1;
    using second_type = Ty2;

    first_type first;
    second_type second;

    // default constructiable
    template <typename Other1 = Ty1, typename Other2 = Ty2>
    requires std::is_default_constructible_v<Other1> &&
        std::is_default_constructible_v<Other2>
    constexpr pair() : first{}, second{} {}

    // implicit constructiable for this type
    template <typename U1 = Ty1, typename U2 = Ty2>
    requires std::is_copy_constructible_v<U1> &&
        std::is_copy_constructible_v<U2> &&
        std::is_convertible_v<const U1&, Ty1> &&
        std::is_convertible_v<const U2&, Ty2>
    constexpr pair(const Ty1& a, const Ty2& b) : first(a), second(b) {}

    // explicit constructible for this type
    template <typename U1 = Ty1, typename U2 = Ty2>
    requires std::is_copy_constructible_v<U2> &&
        std::is_convertible_v<const U1&, Ty1> &&
        (!std::is_convertible_v<const U1&, Ty1> ||
         !std::is_convertible_v<const U2&,
                                Ty2>)explicit constexpr pair(const Ty1& a,
                                                             const Ty2& b)
        : first(a), second(b) {}

    pair(const pair&) = default;
    pair(pair&&) = default;

    // implicit constructiable for other type
    template <typename Other1, typename Other2>
    requires std::is_constructible_v<Ty1, Other1> &&
        std::is_constructible_v<Ty2, Other2> &&
        std::is_convertible_v<Other1&&, Ty1> &&
        std::is_convertible_v<Other2&&, Ty2>
    constexpr pair(Other1&& a, Other2&& b)
        : first(mystl::forward<Other1>(a)), second(mystl::forward<Other2>(b)) {}

    // explicit constructiable for other type
    template <typename Other1, typename Other2>
    requires std::is_constructible_v<Ty1, Other1> &&
        std::is_constructible_v<Ty2, Other2> &&
        (!std::is_convertible_v<Other1, Ty1> ||
         !std::is_convertible_v<Other2, Ty2>)explicit constexpr pair(Other1&& a,
                                                                     Other2&& b)
        : first(mystl::forward<Other1>(a)), second(mystl::forward<Other2>(b)) {}

    // implicit constructiable for other pair
    template <typename Other1, typename Other2>
    requires std::is_constructible_v<Ty1, const Other1&> &&
        std::is_constructible_v<Ty2, const Other2&> &&
        std::is_convertible_v<const Other1&, Ty1> &&
        std::is_convertible_v<const Other2&, Ty2>
    constexpr pair(const pair<Other1, Other2>& other)
        : first(other.first), second(other.second) {}

    // explicit constructiable for other pair
    template <typename Other1, typename Other2>
    requires std::is_constructible_v<Ty1, const Other1&> &&
        std::is_constructible_v<Ty2, const Other2&> &&
        (!std::is_convertible_v<const Other1&, Ty1> ||
         !std::is_convertible_v<
             const Other2&,
             Ty2>)explicit constexpr pair(const pair<Other1, Other2>& other)
        : first(other.first), second(other.second) {}

    // implicit constructiable for other pair
    template <typename Other1, typename Other2>
    requires std::is_constructible_v<Ty1, Other1> &&
        std::is_constructible_v<Ty2, Other2> &&
        std::is_convertible_v<Other1, Ty1> && std::is_convertible_v<Other2, Ty2>
    constexpr pair(pair<Other1, Other2>&& other)
        : first(mystl::forward(other.first)),
          second(mystl::forward(other.second)) {}

    // explicit constructiable for other pair
    template <typename Other1, typename Other2>
    requires std::is_constructible_v<Ty1, Other1> &&
        std::is_constructible_v<Ty2, Other2> &&
        (!std::is_convertible_v<Other1, Ty1> ||
         !std::is_convertible_v<
             Other2, Ty2>)explicit constexpr pair(pair<Other1, Other2>&& other)
        : first(mystl::forward(other.first)),
          second(mystl::forward(other.second)) {}

    // copy assign for this pair
    pair& operator=(const pair& rhs) {
        if (this != &rhs) {
            first = rhs.first;
            second = rhs.second;
        }
        return *this;
    }

    // move assign for this pair
    pair& operator=(pair&& rhs) {
        if (this != &rhs) {
            first = mystl::move(rhs.first);
            second = mystl::move(rhs.second);
        }
        return *this;
    }

    // copy assign for other pair
    template <class Other1, class Other2>
    pair& operator=(const pair<Other1, Other2>& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

    // move assign for other pair
    template <class Other1, class Other2>
    pair& operator=(pair<Other1, Other2>&& other) {
        first = mystl::forward<Other1>(other.first);
        second = mystl::forward<Other2>(other.second);
        return *this;
    }

    ~pair() = default;

    void swap(pair& other) {
        if (this != &other) {
            mystl::swap(first, other.first);
            mystl::swap(second, other.second);
        }
    }
};

// 重载比较操作符
template <class Ty1, class Ty2>
bool operator==(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <class Ty1, class Ty2>
bool operator<(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
    return lhs.first < rhs.first ||
           (lhs.first == rhs.first && lhs.second < rhs.second);
}

template <class Ty1, class Ty2>
bool operator!=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
    return !(lhs == rhs);
}

template <class Ty1, class Ty2>
bool operator>(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
    return rhs < lhs;
}

template <class Ty1, class Ty2>
bool operator<=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
    return !(rhs < lhs);
}

template <class Ty1, class Ty2>
bool operator>=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
    return !(lhs < rhs);
}

template <typename Ty1, typename Ty2>
void swap(pair<Ty1, Ty2>& lhs, pair<Ty1, Ty2>& rhs) {
    lhs.swap(rhs);
}

template <class Ty1, class Ty2>
pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second) {
    return pair<Ty1, Ty2>(mystl::forward<Ty1>(first),
                          mystl::forward<Ty2>(second));
}

}  // namespace mystl