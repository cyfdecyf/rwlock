This rwlock implementation is based on the description of Intel TBB's
[almost fair rwlock](http://software.intel.com/en-us/blogs/2009/04/02/shared-access-with-tbb-readwrite-locks/).

The code relies on GCC's built-in functions for atomic memory access.

I'm not sure about the license and patent issues about this implementation, so use at your own risk.
