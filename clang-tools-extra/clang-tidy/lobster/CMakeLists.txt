add_clang_library(clangTidyLobsterModule
  LobsterTidyModule.cpp
  Tracing.cpp

  LINK_LIBS
  clangTidy
  clangTidyUtils
  )

clang_target_link_libraries(clangTidyLobsterModule
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  )
