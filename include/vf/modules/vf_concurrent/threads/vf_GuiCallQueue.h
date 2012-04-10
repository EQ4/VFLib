//==============================================================================
//
// Copyright (C) 2008 by Vinnie Falco, this file is part of VFLib.
// See the file GNU_GPL_v2.txt for full licensing terms.
// 
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 51
// Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
// 
//==============================================================================

#ifndef VF_GUICALLQUEUE_VFHEADER
#define VF_GUICALLQUEUE_VFHEADER

#include "vf_CallQueue.h"

//==============================================================================
/** 
    A CallQueue handled by the Juce message thread (gui).

    To use this, put an instance of the class in your application object
    or other central location. Calls placed in the queue will automatically
    get processed by the Juce message loop, no further action is required
    to make it work.

    \ingroup vf_concurrent
*/
class GuiCallQueue
  : public CallQueue
  , private juce::AsyncUpdater
{
public:
  /** Create a GuiCallQueue */
  GuiCallQueue ();

  /** Close the GuiCallQueue.

      This must be called manually by the application during exit as early
      as possible.
  */
  void close ();

  /** Synchronize the queue.

      This can be called manually if needed
  */
  bool synchronize ();

private:
  void signal ();
  void reset ();
  void handleAsyncUpdate ();
};

#endif
