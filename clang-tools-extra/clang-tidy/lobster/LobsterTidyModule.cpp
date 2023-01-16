//===--- LobsterTidyModule.cpp - clang-tidy
//-----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "Tracing.h"

namespace clang {
namespace tidy {
namespace lobster {

class LobsterModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<TracingCheck>("lobster-tracing");
  }
};

} // namespace lobster

// Register the LobsterTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<lobster::LobsterModule>
    X("lobster-module", "Adds LOBSTER tracing tools.");

// This anchor is used to force the linker to link in the generated object file
// and thus register the LobsterModule.
volatile int LobsterModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang
