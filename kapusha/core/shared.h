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
    inline shared(Shareable* t = 0) : t_(t) { if (t_) t_->retain(); }
    inline shared(const shared_base& other)
      : t_(other.get()) { if (t_) t_->retain(); }
    inline ~shared() { if (t_) t_->release(); }
    void reset(Shareable* r = 0) {
      if (r == t_) return;
      if (t_) t_->release();
      if ((t_ = r)) t_->retain();
    }
    inline shared& operator=(const shared& right) {
      reset(right.get());
      return *this;
    }
  protected:
    Shareable* t_;
  }

  //! a useful container for automagically controlling ownership
  //! \warning also thread-unsafe by design!
  template <typename T> class shared : public shared_base {
  public:
    inline explicit operator bool() const { return t_; }
    inline T *get() const { return static_cast<T*>(t_); }
    inline T &operator*() const { return *static_cast<T*>(t_); }
    inline T *operator->() const { return static_cast<T*>(t_); }
  };
  typedef shared<Shareable> SAnything;
} // namespace kapusha
