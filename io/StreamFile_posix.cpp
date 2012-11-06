#include "../sys/Log.h"
#include "StreamFile_posix.h"
#include <fcntl.h> // ::open
#include <sys/stat.h> // ::stat
#include <sys/mman.h> // ::mmap
#include <unistd.h> // ::close

namespace kapusha {
  
  StreamFile::~StreamFile()
  {
    close();
  }
  
  Stream::Error StreamFile::open(const char* filename)
  {
    file_ = ::open(filename, O_RDONLY);
    
#if !SIZE_OPTIMIZE
    if (file_ == -1)
    {
      L("Error opening file \"%s\"", filename);
      fail(this, ErrorCorrupted);
      return error_;
    }
#endif
    
    struct ::stat st;
    ::fstat(file_, &st);
    void *ptr = ::mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, file_, 0);

#if !SIZE_OPTIMIZE
    if (ptr == MAP_FAILED)
    {
      L("Error mmapping file \"%s\"", filename);
      close();
      fail(this, ErrorTruncated);
      return error_;
    }
#endif
    
    useMemory(ptr, st.st_size);

	  L("Opened file \"%s\" stream @%p size %ld", filename, ptr, st.st_size);
    
    return error_;
  }
  
  void StreamFile::close()
  {
    //! \fixme unmap, set empty
    ::close(file_);
    file_ = 0;
  }

} // namespace kapusha
