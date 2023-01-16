add_clang_library(clangTidyLobsterModule
  LobsterTidyModule.cpp
  Tracing.cpp
  )

clang_target_link_libraries(clangTidyLobsterModule
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  )
