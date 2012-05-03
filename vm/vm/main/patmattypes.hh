// Copyright © 2011, Université catholique de Louvain
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// *  Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// *  Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __PATMATTYPES_H
#define __PATMATTYPES_H

#include "mozartcore.hh"

#ifndef MOZART_GENERATOR

namespace mozart {

///////////////////
// PatMatCapture //
///////////////////

#include "PatMatCapture-implem.hh"

nativeint Implementation<PatMatCapture>::build(VM vm, GR gr, Self from) {
  return from.get().index();
}

bool Implementation<PatMatCapture>::equals(VM vm, Self right) {
  return index() == right.get().index();
}

void Implementation<PatMatCapture>::printReprToStream(
  Self self, VM vm, std::ostream& out, int depth) {

  out << "<Capture/" << index() << ">";
}

///////////////////////
// PatMatConjunction //
///////////////////////

#include "PatMatConjunction-implem.hh"

Implementation<PatMatConjunction>::Implementation(
  VM vm, size_t count, StaticArray<StableNode> _elements) {

  _count = count;

  // Initialize elements with non-random data
  // TODO An Uninitialized type?
  for (size_t i = 0; i < count; i++)
    _elements[i].make<SmallInt>(vm, 0);
}

Implementation<PatMatConjunction>::Implementation(
  VM vm, size_t count, StaticArray<StableNode> _elements, GR gr, Self from) {

  _count = count;

  for (size_t i = 0; i < count; i++)
    gr->copyStableNode(_elements[i], from[i]);
}

StableNode* Implementation<PatMatConjunction>::getElement(Self self,
                                                          size_t index) {
  return &self[index];
}

bool Implementation<PatMatConjunction>::equals(Self self, VM vm, Self right,
                                               WalkStack& stack) {
  if (_count != right->_count)
    return false;

  stack.pushArray(vm, self.getArray(), right.getArray(), _count);

  return true;
}

OpResult Implementation<PatMatConjunction>::initElement(
  Self self, VM vm, size_t index, RichNode value) {

  self[index].init(vm, value);
  return OpResult::proceed();
}

void Implementation<PatMatConjunction>::printReprToStream(
  Self self, VM vm, std::ostream& out, int depth) {

  out << "<PatMatConjunction>(";

  if (depth <= 1) {
    out << "...";
  } else {
    for (size_t i = 0; i < _count; i++) {
      if (i > 0)
        out << ", ";
      out << repr(vm, self[i], depth);
    }
  }

  out << ")";
}

}

#endif // MOZART_GENERATOR

#endif // __PATMATTYPES_H
