.PHONY: all tests clean

CXX = g++
CXXFLAGS = -O2 -Wall -I . --std=c++23 -fconcepts-diagnostics-depth=2

OUTDIR = ./build

LIBSRC = parser_combinator/string_parsers/atomic_parsers.cpp parser_combinator/string_parsers/char_sets.cpp \
	parser_combinator/string_parsers/string_slice.cpp

LIBOBJ := $(patsubst %.cpp, $(OUTDIR)/%.o, $(LIBSRC) )
LIBDIRS := $(dir $(LIBOBJ))

TESTSRC_JSON = tests/json_parser/main.cpp tests/json_parser/json_parser.cpp tests/json_parser/json_conversions.cpp
TESTOBJ_JSON := $(LIBOBJ)
TESTOBJ_JSON += $(patsubst %.cpp, $(OUTDIR)/%.o, $(TESTSRC_JSON) )
TESTDIRS_JSON := $(dir $(TESTOBJ_JSON))

$(OUTDIR)/tests/test_json : $(TESTOBJ_JSON)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OUTDIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(OUTDIR) $(LIBOBJ) tests

tests: $(OUTDIR) $(OUTDIR)/tests/test_json

$(OUTDIR) :
	@mkdir $@
	@mkdir $@/tests
	@mkdir -p $(LIBDIRS)
	@mkdir -p $(TESTDIRS_JSON)

clean:
	rm -rf $(OUTDIR)