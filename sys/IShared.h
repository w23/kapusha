#pragma once

//! basic memory management

namespace kapusha {
  
  //! very basic object that could be shared across many
  class IShared {
  public:
    //! initial retain count should be zero
    //! motivation: 1) most usages are more convenient with this
    //!             2) other usages mostly ok with manual delete
    IShared() : retain_count_(0) {}
    
    //! virtual destructor for us to be deletable from anywhere
    virtual ~IShared() {}
    
    //! increase retain count (gain ownership)
    void retain() {
      //! \todo thread safety: atomics
      ++retain_count_;
    }
    
    //! drop ownership, deletes object if there are no other owner
    void release() {
      //! \todo thread safety: atomics
      if(!--retain_count_)
        delete this;
    }
    
    //! get count of current owners, debug mostly
    int getRetainCount() const { return retain_count_; }
    
  private:
    volatile int retain_count_;
  };
  
  //! a useful container for automagically controlling ownership
  template <typename T>
  class shared {
  public:
    shared() : t_(0) {}
    shared(T* t) : t_(t) { t_->retain(); }
    shared(const shared& other)
      : t_(other.get())
    {
      if (t_)
        t_->retain();
    }
    ~shared() {
      if (t_)
        t_->release();
    }
    
    void reset(T* t = 0)
    {
      if (t == t_) return;
      if (t_)
        t_->release();

      t_ = t;
      if (t_)
        t_->retain();
    }
    
    T *get() const { return t_; }
    T *operator*() { return t_; }
    T *operator->() { return t_; }
    
  private:
    T* t_;
  };
  
}