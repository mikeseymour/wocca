CXX = g++-4.8
LD = g++-4.8
CXXFLAGS += -Isrc/wocca -Isrc
CXXFLAGS += -std=c++0x -O3 -W -Wall -Wextra -Werror -g
#LDLIBS_EXE := obj/wocca/libwocca.a -pthread -lasound

SHELL := /bin/bash

all_srcs    := $(shell find src -name *.cpp)
lib_srcs    := $(shell find src/wocca -name *.cpp)
app_srcs    := $(shell find src/apps -name *.cpp)
ut_srcs     := $(shell find src/test/unit -name *.cpp)
it_srcs     := $(shell find src/test/integration -name *.cpp)

lib_headers := $(shell find src/wocca -name *.h)

ut_srcs_expected := $(lib_headers:src/wocca/%.h=src/test/unit/%.cpp)
ut_srcs_missing := $(filter-out $(ut_srcs),$(ut_srcs_expected))

relver ?= $(shell date +%F)

quick : unittest apps
all : quick release integrationtest
release :

unittest : obj/test/unit/passed 
ifdef ut_srcs_missing
	@echo Missing unit tests: $(ut_srcs_missing)
endif
	@echo Unit tests passed.

integrationtest: obj/test/integration/main

apps : $(app_srcs:src/%.cpp=obj/%)
clean : ; @rm -rf obj release

%_release : reldir = $(patsubst %_release,release/%/$(relver),$@)
%_release : relzip = $(patsubst %_release,release/%-$(relver).tar.bz2,$@)
%_release : relzip2 = $(patsubst %_release,release/%-snapshot.tar.bz2,$@)

dummy_release :
	@echo RELEASE $(relzip)
	@rm -rf $(reldir)
	@mkdir -p $(reldir)
	@cp $^ $(reldir)
	@hg summary > $(reldir)/VERSION
	@(cd $(reldir)/.. && tar cjf $(notdir $(relzip)) $(relver))
	@mv $(reldir)/../$(notdir $(relzip)) $(relzip)
	@cp $(relzip) $(relzip2)

.PHONY : all release clean dummy_release

-include $(all_srcs:src/%.cpp=obj/%.d)

#obj/wocca/libwocca.a : $(lib_srcs:src/%.cpp=obj/%.o)
#	@echo LIB $@
#	@ar rcs $@ $^

obj/% : obj/%.o #obj/wocca/libwocca.a
	@echo EXE $@
	@$(LINK.cpp) $(OUTPUT_OPTION) $^ $(LDLIBS) $(LDLIBS_EXE)

obj/test/unit/main : $(ut_srcs:src/%.cpp=obj/%.o)
obj/test/integration/main : $(it_srcs:src/%.cpp=obj/%.o)

.PRECIOUS : obj/%.o
obj/%.o : src/%.cpp
	@echo C++ $<
	@mkdir -p $(dir $@)
	@$(COMPILE.cpp) $(OUTPUT_OPTION) -MMD $<

obj/test/%/passed : obj/test/%/main
	@echo UNIT TEST
	@ulimit -s 8192; $<
	@touch $@

# This forces dependencies on removed headers to be recompiled.
# Otherwise, with no rule targetting the missing header, its dependencies will be ignored.
src/%.h : ;
