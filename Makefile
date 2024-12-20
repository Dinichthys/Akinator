CXXFLAGS = -D _DEBUG  -ggdb -g3 -D_FORTIFY_SOURCES=3 -std=c++17 -Og -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=81920 -Wstack-usage=81920 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CXX = g++

FESTIVALFLAG = -lfestival

WARNINGS = -Wall -Wextra
# -Wswitch-enum
ifeq ($(CXX), clang++)
  WARNINGS += -pedantic -Wconversion -Wdangling -Wdeprecated -Wdocumentation -Wformat -Wfortify-source -Wgcc-compat -Wgnu -Wignored-attributes -Wignored-pragmas -Wimplicit -Wmost -Wshadow-all -Wthread-safety -Wuninitialized -Wunused -Wformat
  WARNINGS += -Wargument-outside-range -Wassign-enum -Wbitwise-instead-of-logical -Wc23-extensions -Wc11-extensions -Wcast-align -Wcast-function-type -Wcast-qual -Wcomma -Wcomment -Wcompound-token-split -Wconditional-uninitialized -Wduplicate-decl-specifier -Wduplicate-enum -Wduplicate-method-arg -Wduplicate-method-match -Wempty-body -Wempty-init-stmt -Wenum-compare -Wenum-constexpr-conversion -Wextra-tokens -Wfixed-enum-extension -Wfloat-equal -Wloop-analysis -Wframe-address -Wheader-guard -Winfinite-recursion -Wno-gnu-binary-literal -Wint-conversion -Wint-in-bool-context -Wmain -Wmisleading-indentation -Wmissing-braces -Wmissing-prototypes -Wover-aligned -Wundef -Wvla
endif
ifeq ($(CXX), cc)
  WARNINGS += -pedantic -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -Wstack-usage=8192 -Wstack-protector
endif
ifeq ($(CXX), g++)
  WARNINGS += -pedantic -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -Wstack-usage=8192 -Wstack-protector
endif

all: akinator tree_compare

akinator: build build_akin build_stack clean_dump main_akinator.o akinator.o akin_answer.o akin_definition.o akin_dump.o akin_io.o akin_run.o akin_structure.o stack my_stdio.o logging.o print_error.o
	@$(CXX) $(CXXFLAGS) $(FESTIVAL) build/akinator/*.o build/stack/*.o build/my_stdio.o build/logging.o build/print_error.o -o akinator

tree_compare: build build_comp clean_dump main_tree_compare.o tree_compare.o logging.o print_error.o
	@$(CXX) $(CXXFLAGS) build/tree_compare/*.o build/logging.o build/print_error.o -o tree


build:
	mkdir -p build

build_akin:
	mkdir -p build/akinator

build_stack:
	mkdir -p build/stack

build_comp:
	mkdir -p build/tree_compare


stack: stack.o hash.o
	@

stack.o: Stack/stack.cpp
	@$(CXX) $(CXXFLAGS) -c Stack/stack.cpp -o build/stack/stack.o

hash.o: Stack/hash.cpp
	@$(CXX) $(CXXFLAGS) -c Stack/hash.cpp -o build/stack/hash.o


my_stdio.o: My_lib/My_stdio/my_stdio.cpp
	@$(CXX) $(CXXFLAGS) -c My_lib/My_stdio/my_stdio.cpp -o build/my_stdio.o

logging.o: My_lib/Logger/logging.cpp
	@$(CXX) $(CXXFLAGS) -c My_lib/Logger/logging.cpp -o build/logging.o

print_error.o: My_lib/Assert/print_error.cpp
	@$(CXX) $(CXXFLAGS) -c My_lib/Assert/print_error.cpp -o build/print_error.o


main_akinator.o: Akinator/main_akinator.cpp
	@$(CXX) $(CXXFLAGS) -c Akinator/main_akinator.cpp -o build/akinator/main_akinator.o

akinator.o: Akinator/source/akinator.cpp
	@$(CXX) $(CXXFLAGS) -c Akinator/source/akinator.cpp -o build/akinator/akinator.o

akin_answer.o: Akinator/source/akin_answer.cpp
	@$(CXX) $(CXXFLAGS) -c Akinator/source/akin_answer.cpp -o build/akinator/akin_answer.o

akin_definition.o: Akinator/source/akin_definition.cpp
	@$(CXX) $(CXXFLAGS) $(FESTIVAL) -c Akinator/source/akin_definition.cpp -o build/akinator/akin_definition.o

akin_dump.o: Akinator/source/akin_dump.cpp
	@$(CXX) $(CXXFLAGS) -c Akinator/source/akin_dump.cpp -o build/akinator/akin_dump.o

akin_io.o: Akinator/source/akin_io.cpp
	@$(CXX) $(CXXFLAGS) -c Akinator/source/akin_io.cpp -o build/akinator/akin_io.o

akin_run.o: Akinator/source/akin_run.cpp
	@$(CXX) $(CXXFLAGS) -c Akinator/source/akin_run.cpp -o build/akinator/akin_run.o

akin_structure.o: Akinator/source/akin_structure.cpp
	@$(CXX) $(CXXFLAGS) -c Akinator/source/akin_structure.cpp -o build/akinator/akin_structure.o


main_tree_compare.o: Tree_compare/main_tree_compare.cpp
	@$(CXX) $(CXXFLAGS) -c Tree_compare/main_tree_compare.cpp -o build/tree_compare/main_tree_compare.o

tree_compare.o: Tree_compare/tree_compare.cpp
	@$(CXX) $(CXXFLAGS) -c Tree_compare/tree_compare.cpp -o build/tree_compare/tree_compare.o


clean: clean_dump rmdir_build
	rm -rf akinator tree

clean_dump:
	rm -rf Dump_Files/*

rmdir_build: clean_build
	rmdir build

clean_build: rmdir_stack rmdir_akin rmdir_comp
	rm -rf build/*

rmdir_stack: clean_stack
	rmdir build/stack

clean_stack:
	rm -rf build/stack/*

rmdir_akin: clean_akin
	rmdir build/akinator

clean_akin:
	rm -rf build/akinator/*

rmdir_comp: clean_comp
	rmdir build/tree_compare

clean_comp:
	rm -rf build/tree_compare/*
