#ifndef ORG_BYKN_ISTANBUL
#define ORG_BYKN_ISTANBUL

namespace org_bykn_istanbul {

template<typename T>
class Iterator {
  public:
    virtual bool hasNext() const = 0;
    virtual T next() = 0;
};

}

#endif
