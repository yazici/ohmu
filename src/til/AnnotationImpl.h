//===- AnnotationImpl.h ----------------------------------------*- C++ --*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License.  See LICENSE.TXT in the LLVM repository for details.
//
//===----------------------------------------------------------------------===//

#ifndef OHMU_TIL_ANNOTATIONIMPL_H
#define OHMU_TIL_ANNOTATIONIMPL_H

#include "Annotation.h"
#include "TIL.h"

namespace ohmu {
namespace til {

/// Collection of forward declarations to avoid circular includes.
class BytecodeReader;
class BytecodeWriter;
class CFGBuilder;

template <class Self, class StreamType>
class PrettyPrinter;

template <class S>
class Traversal;


/// Sample annotation for storing instruction names.
class InstrNameAnnot : public Annotation {
public:
  InstrNameAnnot(StringRef N) : Annotation(ANNKIND_InstrNameAnnot), Name(N) {}

  static bool classof(const Annotation *A) {
    return A->kind() == ANNKIND_InstrNameAnnot;
  }

  StringRef name() const { return Name; }

  void setName(StringRef N) { Name = N; }

  template <class Trav>
  void traverse(Trav *T) {
    T->self()->template reduceAnnotationT<InstrNameAnnot>(this);
  }

  InstrNameAnnot *copy(CFGBuilder &Builder,
                       const std::vector<SExpr*> &SubExprs);

  void rewrite(const std::vector<SExpr*> &SubExprs) { }

  template <class Printer, class StreamType>
  void print(Printer *P, StreamType &SS) const {
    SS << "InstrName(\"" << Name << "\")";
  }

  void serialize(BytecodeWriter* B);

  static InstrNameAnnot *deserialize(BytecodeReader* B);

  template <class Comp>
  void compare(const InstrNameAnnot *A, Comp *C) const {
    C->compareScalarValues(Name, A->Name);
  }

private:
  StringRef Name;
};


/// Annotation for storing source code positions.
class SourceLocAnnot : public Annotation {
public:
  typedef uint64_t SourcePosition;

  SourceLocAnnot(SourcePosition P) :
    Annotation(ANNKIND_SourceLocAnnot), Position(P) {}

  static bool classof(const Annotation *A) {
    return A->kind() == ANNKIND_SourceLocAnnot;
  }

  SourcePosition position() const { return Position; }

  template <class Trav>
  void traverse(Trav *T) {
    T->self()->template reduceAnnotationT<SourceLocAnnot>(this);
  }

  SourceLocAnnot *copy(CFGBuilder &Builder,
                       const std::vector<SExpr*> &SubExprs);

  void rewrite(const std::vector<SExpr*> &SubExprs) { }

  template <class Printer, class StreamType>
  void print(Printer *P, StreamType &SS) const {
    SS << "SourceLoc(" << Position << ")";
  }

  void serialize(BytecodeWriter *B);

  static SourceLocAnnot *deserialize(BytecodeReader *B);

  template <class Comp>
  void compare(const SourceLocAnnot *A, Comp *C) const {
    C->compareScalarValues(Position, A->Position);
  }

private:
  SourcePosition const Position;
};


/// Annotation for storing preconditions.
class PreconditionAnnot : public Annotation {
public:
  PreconditionAnnot(SExpr *P) :
    Annotation(ANNKIND_PreconditionAnnot),Condition(P) {}

  static bool classof(const Annotation *A) {
    return A->kind() == ANNKIND_PreconditionAnnot;
  }

  SExpr *condition() { return Condition.get(); }

  template <class Trav>
  void traverse(Trav *T) {
    T->self()->traverseArg(Condition.get());
    T->self()->template reduceAnnotationT<PreconditionAnnot>(this);
  }

  PreconditionAnnot *copy(CFGBuilder &Builder,
      const std::vector<SExpr*> &SubExprs);

  void rewrite(const std::vector<SExpr*> &SubExprs) {
    Condition.reset(SubExprs.at(0));
  }

  template <class Printer, class StreamType>
  void print(Printer *P, StreamType &SS) const {
    SS << "Precondition(";
    Printer::print(Condition.get(), SS);
    SS << ")";
  }

  void serialize(BytecodeWriter *B);

  static PreconditionAnnot *deserialize(BytecodeReader* B);

  template <class Comp>
  void compare(const PreconditionAnnot *A, Comp *C) const {
    C->compare(Condition.get(), A->Condition.get());
  }

private:
  SExprRef Condition;
};


/// Just testing storing multiple SExprRefs.
class TestTripletAnnot : public Annotation {
public:
  TestTripletAnnot(SExpr *A, SExpr *B, SExpr *C) :
    Annotation(ANNKIND_TestTripletAnnot), ExpA(A), ExpB(B), ExpC(C) {}

  static bool classof(const Annotation *A) {
    return A->kind() == ANNKIND_TestTripletAnnot;
  }

  template <class Trav>
  void traverse(Trav *T) {
    T->self()->traverseArg(ExpA.get());
    T->self()->traverseArg(ExpB.get());
    T->self()->traverseArg(ExpC.get());
    T->self()->template reduceAnnotationT<TestTripletAnnot>(this);
  }

  TestTripletAnnot *copy(CFGBuilder &Builder,
      const std::vector<SExpr*> &SubExprs);

  void rewrite(const std::vector<SExpr*> &SubExprs) {
    ExpA.reset(SubExprs.at(0));
    ExpB.reset(SubExprs.at(1));
    ExpC.reset(SubExprs.at(2));
  }

  template <class Printer, class StreamType>
  void print(Printer *P, StreamType &SS) const {
    SS << "TestTriples(";
    Printer::print(ExpA.get(), SS);
    SS << ", ";
    Printer::print(ExpB.get(), SS);
    SS << ", ";
    Printer::print(ExpC.get(), SS);
    SS << ")";
  }

  void serialize(BytecodeWriter *B);

  static TestTripletAnnot *deserialize(BytecodeReader *B);

  template <class Comp>
  void compare(const TestTripletAnnot *Ann, Comp *Co) const {
    Co->compare(ExpA.get(), Ann->ExpA.get());
    Co->compare(ExpB.get(), Ann->ExpB.get());
    Co->compare(ExpC.get(), Ann->ExpC.get());
  }

private:
  SExprRef ExpA;
  SExprRef ExpB;
  SExprRef ExpC;
};


}  // end namespace ohmu
}  // end namespace til

#endif  // OHMU_TIL_ANNOTATIONIMPL_H
