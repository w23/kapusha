#pragma once

namespace kapusha {
  //! very basic object that could be shared across many
  //! \warning thread-unsafe by design!
  class Shareable {
  public:
    //! initial retain count should be zero
    //! motivation: 1) most usages are more convenient with this
    //!             2) other usages mostly ok with manual delete
    Shareable() : retainCount_(0) {}
    virtual ~Shareable() {}
    inline void retain() { ++retainCount_; }
    void release() { if(!--retainCount_) delete this; }
    inline int getRetainCount() const { return retainCount_; }
  private:
    volatile int retainCount_;
  };
  
  //! convenience store
  class shared_base {
  public:
    inline shared_base(Shareable* t = 0) : t_(t) { if (t_) t_->retain(); }
    inline shared_base(const shared_base& other)
      : t_(other.t_) { if (t_) t_->retain(); }
    inline ~shared_base() { if (t_) t_->release(); }
    void reset(Shareable* r = 0) {
      if (r == t_) return;
      if (t_) t_->release();
      if ((t_ = r)) t_->retain();
    }
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
    inline shared(T *t = 0) : shared_base(t) {}
    inline explicit operator bool() const { return t_; }
    inline T *get() const { return static_cast<T*>(t_); }
    inline T &operator*() const { return *static_cast<T*>(t_); }
    inline T *operator->() const { return static_cast<T*>(t_); }
    shared &operator=(T *t) { reset(t); return *this; }
  };
  typedef shared<Shareable> SAnything;
} // namespace kapusha
