#pragma once

namespace kapusha {
  //! very basic object that could be shared across many
  //! \warning thread-unsafe by design!
  class Shareable {
  public:
    //! initial retain count should be zero
    //! motivation: 1) most usages are more convenient with this
    //!             2) other usages mostly ok with manual delete
    Shareable() : retain_count_(0) {}
    virtual ~Shareable() {}
    inline void retain() { ++retain_count_; }
    void release() { if(!--retain_count_) delete this; }
    inline int retain_count() const { return retain_count_; }
  private:
    int retain_count_;
  };
  
  //! convenience store
  class shared_base {
  public:
    inline shared_base(Shareable* t = nullptr) : t_(t) { if (t_) t_->retain(); }
    inline shared_base(const shared_base& other)
      : t_(other.t_) { if (t_) t_->retain(); }
    inline ~shared_base() { if (t_) t_->release(); }
    void reset(Shareable* r = nullptr) {
      if (r == t_) return;
      if (t_) t_->release();
      if ((t_ = r)) t_->retain();
    }
    inline bool valid() const { return t_ != nullptr; }
    inline shared_base& operator=(Shareable* t) { reset(t); return *this; }
    inline shared_base& operator=(const shared_base& right) {
      reset(right.t_);
      return *this;
    }
  protected:
    Shareable* t_;
  };

  //! a useful container for automagically controlling ownership
  //! \warning also thread-unsafe by design!
  template <typename T> class shared : public shared_base {
  public:
    inline shared(T *t = nullptr) : shared_base(t) {}
#if !defined(_MSC_VER)
    inline explicit operator bool() const { return t_; }
#endif
    inline T *get() const { return static_cast<T*>(t_); }
    inline T &operator*() const { return *static_cast<T*>(t_); }
    inline T *operator->() const { return static_cast<T*>(t_); }
    inline shared &operator=(T* t) { reset(t); return *this; }
  };
  typedef shared<Shareable> SAnything;
} // namespace kapusha
