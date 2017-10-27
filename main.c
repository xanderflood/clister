#include <stdio.h>
#include <stdlib.h>
#include <clang-c/Index.h>

enum CXChildVisitResult partVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
  // "client_data" is a string prefix, like "arg:", "member:", "value:" for each case
  if (cursor.kind == CXCursor_ParmDecl || cursor.kind == CXCursor_TypeRef)
    printf("%s:%s\n", (char*)client_data,
      clang_getCString(clang_getTypeSpelling(clang_getCursorType(cursor))));

  return CXChildVisit_Continue;
}

enum CXChildVisitResult statementVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
  int i;

  CXSourceLocation cxsl = clang_getCursorLocation(cursor);
  if (!clang_Location_isFromMainFile(cxsl))
    return CXChildVisit_Continue;

  if (cursor.kind == CXCursor_FunctionDecl) {
    printf("function:%s\n", clang_getCString(clang_getCursorSpelling(cursor)));
    printf("return:%s\n", clang_getCString(clang_getTypeSpelling(clang_getCursorResultType(cursor))));

    // CXType fdecl = clang_getCursorType(cursor);
    // for (i = 0; i < clang_getNumArgTypes(fdecl); i++) {
    //   printf("arg:%s:%s\n", clang_getCString(clang_getTypeSpelling(clang_getArgType(fdecl, i))));
    // }

    clang_visitChildren(cursor, partVisitor, "arg");
  }

  // if (cursor.kind == CXCursor_StructDecl) {
  //   printf("struct\n");
  //   clang_visitChildren(cursor, partVisitor, "member");
  // }

  // if (cursor.kind == CXCursor_EnumDecl) {
  //   printf("enum\n");
  //   clang_visitChildren(cursor, partVisitor, "arg");
  // }

  printf("\n");

  // clang_getCursorSpelling(cursor);

  return CXChildVisit_Continue;
}

int main(int argc, char **argv) {
  CXIndex index = clang_createIndex(1, 0);

  // const char *const *cmdArgs = &argv[2];
  CXTranslationUnit tu = clang_parseTranslationUnit(index, argv[1], NULL, argc - 2, NULL, 0, 0);
  CXCursor c = clang_getTranslationUnitCursor(tu);

  clang_visitChildren(c, statementVisitor, NULL);
}

