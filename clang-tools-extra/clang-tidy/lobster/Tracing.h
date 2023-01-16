//===--- TracingCheck.h - clang-tidy ----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_LOBSTER_TRACINGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_LOBSTER_TRACINGCHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy {
namespace lobster {

/// Generate tracing data for the LOBSTER tool.
///
/// For the user-facing documentation see:
/// https://github.com/bmw-software-engineering/lobster
class TracingCheck : public ClangTidyCheck {
public:
  TracingCheck(StringRef Name, ClangTidyContext *Context);
  ~TracingCheck();

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  ClangTidyContext *Context;

private:
  class TracingCommentHandler;
  std::unique_ptr<TracingCommentHandler> Handler;
};

} // namespace lobster
} // namespace clang::tidy

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_LOBSTER_TRACINGCHECK_H
