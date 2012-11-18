#ifndef ORG_BYKN_ISTANBUL_LIST
#define ORG_BYKN_ISTANBUL_LIST

#include <memory>
#include <functional>

#include "option.hpp"
#include "iterator.hpp"
#include "stliterator.hpp"
#include "iterable.hpp"

namespace org_bykn_istanbul {

template<typename T>
class List : public Iterable<T> {
  public:

    const T& operator[](uint idx) const {
      List<T> const * thisptr = this;
      while(idx > 0) {
        thisptr = thisptr->_tail.get();
        idx -= 1;
      }
      return thisptr->_head;
    }

    bool operator==(const List<T>& that) const {
      List<T> const * this_ptr = this;
      List<T> const * that_ptr = &that;
      while(true) {
        auto thisE = this_ptr->isEmpty();
        auto thatE = that_ptr->isEmpty();
        if(thisE != thatE) {
          return false;
        }
        if(thisE && thatE) {
          // Both are empty
          return true;
        }
        else {
          //neither are empty:
          if(this_ptr->_head != that_ptr->_head) {
            return false;
          }
          else {
            //Advance and check again:
            this_ptr = this_ptr->_tail.get();
            that_ptr = that_ptr->_tail.get();
          }
        }
      }
    }

    const T& head() const {
      return _head;
    }

    const std::shared_ptr<List<T>>& tail() const {
      return _tail; 
    }

    bool isEmpty() const { return (_tail.get() == nullptr); }
   
    template<typename R>
    R foldRight(R init, std::function<R(T,R)> foldfn) const {
      return reverse()->template foldLeft<std::shared_ptr<List<T>>>(init, [&foldfn](R old, T item) {
        return foldfn(item, old);
      });
    }

    std::unique_ptr<Iterator<T>> iterator() const {
      return std::move(std::unique_ptr<Iterator<T>>(new ListIterator(*this)));
    }

    template<typename R>
    std::shared_ptr<List<R>> map(std::function<R(T)> fn) const {
      return this->template foldRight<std::shared_ptr<List<R>>>(List<R>::empty(),
        [&fn](T vt, std::shared_ptr<List<R>> old) {
          return std::shared_ptr<List<R>>(new List(fn(vt), old));
        });
    }
 
   std::shared_ptr<List<T>> reverse() const {
     // TODO: use a weak_ptr to keep the reverse around if it's called often
     auto empty = List<T>::empty();
     return this->template foldLeft<std::shared_ptr<List<T>>>(empty, [](std::shared_ptr<List<T>> old, T vt) {
         return std::shared_ptr<List<T>>(new List(vt, old));
       });
   }   

 // statics:
    static std::shared_ptr<List<T>> cons(const T& v, const std::shared_ptr<List<T>>& tail) {
      return std::shared_ptr<List<T>>(new List(v, tail));
    }
    // Uses move semantics:
    static std::shared_ptr<List<T>> cons(T&& v, const std::shared_ptr<List<T>>& tail) {
      return std::shared_ptr<List<T>>(new List(v, tail));
    }
    static const std::shared_ptr<List<T>>& empty() { return List<T>::_empty; }

  private:
    List(): _head(), _tail() { }
    List(const List<T>& other): _head(other._head), _tail(other._tail) { }
    List(const T& head, const std::shared_ptr<List<T>>& tail): _head(head), _tail(tail) { }
    List(T&& head, const std::shared_ptr<List<T>>& tail): _head(head), _tail(tail) { }
   
  class ListIterator : public Iterator<T> {
    public:
      // We are jumping through hoops here to avoid doing reference counting arithmetic
      // on each advancement: keep the first tail in scope, then we know the rest is not
      // deleted yet.
      ListIterator(const List<T>& start): _first_head(start.head()), _first_tail(start.tail()), _ptr(nullptr) { }
      bool hasNext() const {
        if(_ptr == nullptr) {
          // The tail is null, which means start was actually empty
          return _first_tail.get() != nullptr;
        }
        else {
          return !_ptr->isEmpty();
        }
      }
      T next() {
        if(_ptr == nullptr) {
          _ptr = _first_tail.get();
          return _first_head;
        }
        else {
          T head = _ptr->head();
          _ptr = _ptr->tail().get();
          return head;
        }
      }
    private:
      const T _first_head;
      const std::shared_ptr<List<T>> _first_tail; 
      List<T>* _ptr; 
  };

   void foreach_ptr(std::function<void(const List<T>*)> fn) const {
      const List<T>* thisptr = this;
      while(!thisptr->isEmpty()) {
        fn(thisptr);
        thisptr = thisptr->_tail.get();
      }
    }
    static std::shared_ptr<List<T>> _empty;
    const T _head;
    const std::shared_ptr<List<T>> _tail;    
};

// Initialize the static member variables:
template<typename T> std::shared_ptr<List<T>> List<T>::_empty = std::shared_ptr<List<T>>(new List<T>());

}
#endif
