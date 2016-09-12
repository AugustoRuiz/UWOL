#ifndef FIXEDENDIAN_H
#define FIXEDENDIAN_H
 
#include <stddef.h>         // for size_t
  
// Test the endianness of the processor by "storing" 1 as a multi-byte
// number and "retrieving" its first byte.  Since the compiler knows
// everything about such an object and about its use, the object and the
// test on it will collapse completely at compile-time.
//
// Example:
//
//  if (HostEndianness().isBig())
//    // code for big-endian systems
//  else
//    // code for little-endian systems
 
union HostEndianness
{
  int i;
  char c[sizeof(int)];
 
  HostEndianness() : i(1) { }
 
  bool isBig() const { return c[0] == 0; }
  bool isLittle() const { return c[0] != 0; }
};
 
 
// Store an object of type 'T' in memory in the byte order indicated by
// 'wantBig' -- big-endian if 'wantBig' is true, else little-endian. 
// Provide access to the object via a conversion constructor and a
// conversion operator, swapping bytes on input and output if the
// processor's byte order differs from the desired byte order.
 
template <typename T, bool wantBig>
class FixedEndian
{
  // Object contents, in specified byte order.
 
  T rep;
 
  // Swap bytes if the processor byte order differs from the specified
  // memory order.
 
  static T swap(const T& arg)
  {
    // If the processor byte order is the same as the desired byte
    // order, simply return the argument unchanged.
 
    if (HostEndianness().isBig() == wantBig)
      return arg;
 
    // The processor byte order differs from the specified memory
    // order, so swap the bytes of the argument before returning it.
 
    else
    {
      T ret;
 
      char* dst = reinterpret_cast<char*>(&ret);
      const char* src = reinterpret_cast<const char*>(&arg + 1);
 
      for (size_t i = 0; i < sizeof(T); i++)
        *dst++ = *--src;
 
      return ret;
    }
  }
 
public:
  FixedEndian() { }
  FixedEndian(const T& t) : rep(swap(t)) { }

  const char* GetChars() { return reinterpret_cast<char*>(&rep); }
  void SetChars(char* chars) { rep = *(reinterpret_cast<T*>(chars)); }

  operator T() const { return swap(rep); }
};
 
// Maintain an object in big-endian order, regardless of the host
// machine's native byte order.
 
template <typename T> 
class BigEndian : public FixedEndian<T, true>
{
public:
  BigEndian() { }
  BigEndian(const T& t) : FixedEndian<T, true>(t) { }
};
 
// Maintain an object in little-endian order, regardless of the host
// machine's native byte order.
 
template <typename T> 
class LittleEndian : public FixedEndian<T, false>
{
public:
  LittleEndian() { }
  LittleEndian(const T& t) : FixedEndian<T, false>(t) { }
};
 
#endif // FIXEDENDIAN_H