// Copyright (C) 2008-2011 by Vincent Falco, All rights reserved worldwide.
// This file is released under the MIT License:
// http://www.opensource.org/licenses/mit-license.php

#ifndef __VF_ALLOCATOR_VFHEADER__
#define __VF_ALLOCATOR_VFHEADER__

#include "vf/vf_List.h"
#include "vf/vf_PageAllocator.h"

#define LOCKFREE_ALLOCATOR_LOGGING 0

// Lock-free allocator for small variably sized memory blocks.
//
// - Any thread may allocate and free concurrently.
//
// - Freed memory is re-used in future allocations after a certain amount
//   of time has passed, in order to prevent the ABA problem for callers.
//
// - When there are no free blocks available, the algorithm becomes a blocking
//   algorithm since we request memory from the standard library.
//
class Allocator
{
public:
  explicit Allocator ();
  ~Allocator ();

  void* allocate (const size_t bytes);
  static void deallocate (void* const p);

private:
  struct Header;

  class Page;

  inline Page* newPage ();
  static inline void deletePage (Page* page);

private:
  class PerThreadData;
  boost::thread_specific_ptr <PerThreadData> m_tsp;

  GlobalPageAllocator m_pages;
};



template <class Tag>
class GlobalAllocator
{
public:
  inline void* allocate (size_t bytes)
  {
    return s_allocator.allocate (bytes);
  }

  static inline void deallocate (void* const p)
  {
    Allocator::deallocate (p);
  }

private:
  static Allocator s_allocator;
};

template <class Tag>
Allocator GlobalAllocator <Tag>::s_allocator;

#endif