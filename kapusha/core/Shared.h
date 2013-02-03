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
    inline int getRetainCount() const { return retain_count_; }
  private:
    volatile int retain_count_;
  };
  
  //! a useful container for automagically controlling ownership
  //! \warning also thread-unsafe by design!
  //! this R/T shit is me trying to avoid copies of exactly the same code all over the place (possibly without any success)
  template <typename R, typename T = Shareable> class shared {
  public:
    inline shared(R* r = 0) : t_(static_cast<T*>(r)) { if (t_) t_->retain(); }
    inline shared(const shared& other)
      : t_(other.get()) { if (t_) t_->retain(); }
    inline ~shared() { if (t_) t_->release(); }
    void reset(R* r = 0) {
      T* t = static_cast<T*>(r);
      if (t == t_) return;
      if (t_) t_->release();
      if ((t_ = t)) t_->retain();
    }
    inline R *get() const { return static_cast<R*>(t_); }
    inline R &operator*() const { return *static_cast<R*>(t_); }
    inline R *operator->() const { return static_cast<R*>(t_); }
    inline shared& operator=(const shared& right) {
      reset(right.get());
      return *this;
    }
    inline explicit operator bool() const { return t_; }
  private:
    T* t_;
  };
  typedef shared<Shareable> SAnything;
}