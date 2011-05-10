// Copyright (C) 2008-2011 by Vincent Falco, All rights reserved worldwide.
// This file is released under the MIT License:
// http://www.opensource.org/licenses/mit-license.php

#include "vf/vf_StandardHeader.h"

BEGIN_VF_NAMESPACE

#include "vf/vf_Atomic.h"
#include "vf/vf_LeakChecked.h"
#include "vf/vf_PerformedAtExit.h"
#include "vf/vf_SharedObject.h"
#include "vf/vf_Static.h"

typedef Static::Storage <Atomic::Pointer <PerformedAtExit>, PerformedAtExit> Head;

class PerformedAtExit::Performer
{
private:
  ~Performer ()
  {
    PerformedAtExit* object = s_head->get ();

    while (object != nullptr)
    {
      PerformedAtExit* next = object->m_next;
      object->performAtExit ();
      object = next;
    }

    PerformedAtExit::performLibraryAtExit ();
  }

public:
  static void push_front (PerformedAtExit* object)
  {
    PerformedAtExit* next;

    do
    {
      next = s_head->get ();
      object->m_next = next;
    }
    while (!s_head->compareAndSet (object, next));
  }

private:
  static Head s_head;
  static Performer s_performer;
};

PerformedAtExit::Performer PerformedAtExit::Performer::s_performer;
Head PerformedAtExit::Performer::s_head;

PerformedAtExit::PerformedAtExit (bool perform)
{
  if (perform)
    Performer::push_front (this);
}

void PerformedAtExit::performLibraryAtExit ()
{
  SharedObject::performLibraryAtExit ();

  LeakCheckedBase::performLibraryAtExit ();
}

END_VF_NAMESPACE
