CXXFLAGS = -D _DEBUG  -ggdb -g3 -D_FORTIFY_SOURCES=3 -std=c++17 -Og -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=81920 -Wstack-usage=81920 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr


all: akinator tree_compare


akinator: clean_dump main_akinator.o akinator.o logging.o print_error.o
	@g++ $(CXXFLAGS) build/main_akinator.o build/akinator.o build/logging.o build/print_error.o -o akinator

tree_compare: clean_dump main_tree_compare.o tree_compare.o logging.o print_error.o
	@g++ $(CXXFLAGS) build/main_tree_compare.o build/tree_compare.o build/logging.o build/print_error.o -o tree


logging.o: My_lib/Logger/logging.cpp
	@g++ $(CXXFLAGS) -c My_lib/Logger/logging.cpp -o build/logging.o

print_error.o: My_lib/Assert/print_error.cpp
	@g++ $(CXXFLAGS) -c My_lib/Assert/print_error.cpp -o build/print_error.o


main_akinator.o: Akinator/main_akinator.cpp
	@g++ $(CXXFLAGS) -c Akinator/main_akinator.cpp -o build/main_akinator.o

akinator.o: Akinator/akinator.cpp
	@g++ $(CXXFLAGS) -c Akinator/akinator.cpp -o build/akinator.o


main_tree_compare.o: Tree_compare/main_tree_compare.cpp
	@g++ $(CXXFLAGS) -c Tree_compare/main_tree_compare.cpp -o build/main_tree_compare.o

tree_compare.o: Tree_compare/tree_compare.cpp
	@g++ $(CXXFLAGS) -c Tree_compare/tree_compare.cpp -o build/tree_compare.o

clean_dump:
	rm -rf Dump_Files/*

clean:
	rm -rf build/*.o
