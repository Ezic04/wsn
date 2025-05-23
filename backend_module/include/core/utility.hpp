#pragma once
#include <cstdint>
#include <ostream>

template <typename T>
class Id
{
public:
  using id_t = uint32_t;

private:
  inline static id_t Counter = 0;
  id_t id_;

protected:
  Id() : id_(Counter++) {}

public:
  static uint32_t GetIdCounter() { return Counter; }
  uint32_t GetId() const { return id_; }
  bool operator<(const Id<T> &other) { return id_ < other.GetId(); }
};

template <typename T>
T Sqr(T x)
{
  return x * x;
}