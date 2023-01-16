//===--- TracingCheck.cpp - clang-tidy ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Tracing.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/Support/Debug.h"

#include <iostream>
#include <list>
#include <utility>

using namespace clang::ast_matchers;

namespace clang::tidy {
namespace lobster {

class TracingCheck::TracingCommentHandler : public CommentHandler {
public:
  TracingCommentHandler(TracingCheck &Check) : Check(Check) {}

  bool HandleComment(Preprocessor &PP, SourceRange Range) override {
    static const std::string trace_prefix = "// lobster-trace:";
    static const std::string exclude_prefix = "// lobster-exclude:";

    StringRef Text =
        Lexer::getSourceText(CharSourceRange::getCharRange(Range),
                             PP.getSourceManager(), PP.getLangOpts());

    if (Text.startswith(trace_prefix) || Text.startswith(exclude_prefix)) {
      comments.push_back
	(std::pair<SourceRange, std::string>(Range,
					     Text.str().substr(11)));
    }
    return false;
  }

  std::list<std::pair<SourceRange, std::string>> comments;
private:
  TracingCheck &Check;
};

TracingCheck::TracingCheck(StringRef Name,
			   ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), Context(Context),
      Handler(std::make_unique<TracingCommentHandler>(*this))
{}

TracingCheck::~TracingCheck() = default;

void TracingCheck::registerPPCallbacks(const SourceManager &SM,
				       Preprocessor *PP,
				       Preprocessor *ModuleExpanderPP)
{
 PP->addCommentHandler(Handler.get());
}

void TracingCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl().bind("x"), this);
}

void TracingCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("x");
  const auto *MatchedDestDecl = Result.Nodes.getNodeAs<CXXDestructorDecl>("x");

  if (!MatchedDecl->doesThisDeclarationHaveABody())
    return;

  std::string kind;
  if (MatchedDecl->isMain()) {
    kind = "main function";
  } else if (MatchedDecl->isCXXClassMember()) {
    if (MatchedDestDecl != nullptr) {
      // Skip default destructors that do not appear in the source
      const auto *The_Class = MatchedDestDecl->getParent();
      if (!The_Class->hasUserDeclaredDestructor()) {
	return;
      }
    }
    kind = "method";
  } else {
    kind = "function";
  }

  const Stmt *body = MatchedDecl->getBody();
  bool has_tracing = false;
  for (auto cpair : Handler.get()->comments) {
    if (body->getSourceRange().fullyContains(cpair.first)) {
      if (cpair.second[0] == 't') {
	  diag(MatchedDecl->getLocation(),
	       "%0 %1 traces to %2")
	    << kind
	    << MatchedDecl->getQualifiedNameAsString()
	    << cpair.second.substr(6);
      } else {
	  diag(MatchedDecl->getLocation(),
	       "%0 %1 exempt from tracing: %2")
	    << kind
	    << MatchedDecl->getQualifiedNameAsString()
	    << cpair.second.substr(8);
      }
      has_tracing = true;
    }
  }

  if (!has_tracing) {
    diag(MatchedDecl->getLocation(), "%0 %1 has no tracing tags")
      << kind
      << MatchedDecl->getQualifiedNameAsString();
  }
}

} // namespace lobster
} // namespace clang::tidy
