#ifndef ORG_BYKN_ISTANBUL_STLITERATOR
#define ORG_BYKN_ISTANBUL_STLITERATOR

#include <memory>

#include "option.hpp"
#include "iterator.hpp"

namespace org_bykn_istanbul {

template<typename T>
class StlIterator {
  public:
    StlIterator() : _head(), _ptr() { }
    StlIterator(std::unique_ptr<Iterator<T>>&& list): _head(), _ptr() { _ptr.swap(list); }
    StlIterator<T>& operator++() {
      advance(true);
      return *this;
    }
    // This is semantically const, but not actually constant with-respect to memory/sideeffects
    const T& operator*() {
      advance(false);
      return (*_head);
    }

    bool operator!=(const StlIterator<T>& that) const {
      if(this == &that) {
        return false;
      }
      if(finished() && that.finished()) {
        return false;
      }
      
      return (finished() != that.finished()
        || _ptr.get() != that._ptr.get()
        || (_head.get() != that._head.get()));
    }

  private:
    bool headIsEmpty() const { return _head.get() == nullptr; }
    void advance(bool reset) {
      if(headIsEmpty()) {
        _head.reset(new T(_ptr->next()));
      }
      else if(reset) {
        //We already moved:
        _head.reset();
      }
    }
    bool finished() const {
      return (_ptr.get() == nullptr || (headIsEmpty() && !_ptr->hasNext()));
    }
    std::unique_ptr<T> _head;
    std::unique_ptr<Iterator<T>> _ptr;
};

}
#endif
