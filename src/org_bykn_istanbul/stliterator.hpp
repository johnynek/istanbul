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
    StlIterator(const std::shared_ptr<Iterator<T>>& list): _head(), _ptr(list) { }
    StlIterator<T>& operator++() {
      if(_head.isEmpty()) {
        _head = Option<T>(_ptr->next());
        if(!_ptr->hasNext()) { _ptr.reset(); }
      }
      else {
        //We already moved:
        _head = Option<T>();
      }
      return *this;
    }
    // This is semantically const, but not actually constant with-respect to memory/sideeffects
    const T& operator*() {
      if(_head.isEmpty()) {
        _head = Option<T>(_ptr->next());
        if(!_ptr->hasNext()) { _ptr.reset(); }
      }
      return _head.get();
    }
    bool operator!=(StlIterator<T> that) const {
      return (_ptr.get() != that._ptr.get() || finished() != that.finished() || _head != that._head);
    }

  private:
    bool finished() const {
      return (_ptr.get() == nullptr || (_head.isEmpty() && !_ptr->hasNext()));
    }
    Option<T> _head;
    std::shared_ptr<Iterator<T>> _ptr;
};

}
#endif
