#ifndef ORG_BYKN_ISTANBUL_OPTION
#define ORG_BYKN_ISTANBUL_OPTION

namespace org_bykn_istanbul {

template<typename T>
class Option {
  public:
    Option() : _value(), _is_set(false) { }
    Option(const T& v) : _value(v), _is_set(true) { }

    template<typename R>
    Option<R> flatMap(std::function<Option<R>(T)> fn) const {
      if(!_is_set) {
        return Option<R>();
      }
      else {
        return fn(_value);
      }
    }
    bool isDefined() const { return _is_set; }
    bool isEmpty() const { return _is_set == false; }
    
    const T& get() const { return _value; }

    T getOrElse(T def) const {
      if(!_is_set) {
        return def;
      }
      else {
        return _value;
      }
    }

   bool operator==(Option<T> that) const { return (_is_set == that._is_set && _value == that._value); }
   bool operator!=(Option<T> that) const { return (_is_set != that._is_set || _value != that._value); }

  private:
    T _value;
    bool _is_set;  
};

}

#endif
