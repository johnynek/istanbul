#ifndef ORG_BYKN_ISTANBUL_ITERABLE
#define ORG_BYKN_ISTANBUL_ITERABLE

#include "iterator.hpp"
#include "stliterator.hpp"

namespace org_bykn_istanbul {

template<typename T>
class Iterable {
  public:
    virtual std::unique_ptr<Iterator<T>> iterator() const = 0;
    // STL iteration support
    typedef StlIterator<T> const_iterator;
    const_iterator begin() const { return StlIterator<T>(iterator()); }
    const_iterator end() const { return StlIterator<T>(); }

    void foreach(std::function<void(T)> fn) const {
      auto iter = iterator();
      while(iter->hasNext()) {
        fn(iter->next());
      }
    }

    template<typename R>
    R foldLeft(R init, std::function<R(R,T)> foldfn) const {
      R result = init;
      foreach([&](T valT) { result = foldfn(result, valT); });
      return result;
    }
};

}
#endif
