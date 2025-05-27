#pragma once
#include <cstdint>
/**
 * @file utility.hpp
 * @brief Contains utility classes and functions for the backend module.
 */

/**
 * @class Id
 * @brief A class that provides a unique identifier for objects of type T.
 * @details The Id class generates a unique identifier for each instance of T using a static counter.
 */
template <typename T>
class Id
{
public:
  using id_t = uint32_t;

private:
  inline static id_t Counter = 0; ///< Static counter to generate unique IDs
  id_t id_;                       ///< Unique identifier for the instance of T

protected:
  Id() : id_(Counter++) {}

public:
  static uint32_t GetIdCounter() { return Counter; }                 ///< Returns the current value of the ID counter.
  uint32_t GetId() const { return id_; }                             ///< Returns the unique ID of the instance.
  bool operator<(const Id<T> &other) { return id_ < other.GetId(); } ///< Less-than operator for comparing IDs.
};

/**
 * @brief Computes the square of a number.
 * @tparam T The type of the number (e.g., int, float, double).
 * @param x The number to be squared.
 * @return The square of the number.
 */
template <typename T>
T Sqr(T x)
{
  return x * x;
}