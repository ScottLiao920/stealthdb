# use pgxs to build encdb & cstore_fdw together
#
include vars.mk

UNTRUSTED_DIR=untrusted
INTERFACE_DIR=untrusted/interface
EXTENSION_DIR=untrusted/extensions
CSTORE_DIR=untrusted/cstore

MODULE_big = encdb
# a shared library to build from multiple source files (list object files in OBJS)
EXTENSION = encdb # extension name
DATA = encdb--1.0.1.sql #  files to install into prefix/share/$MODULEDIR


C_SRCS := $(wildcard $(EXTENSION_DIR)/*.c) $(wildcard $(CSTORE_DIR)/*.c)
CXX_SRCS := $(wildcard tools/*.cpp) $(wildcard $(INTERFACE_DIR)/*.cpp)
CXX_OBJS := $(CXX_SRCS:.cpp=.o)

OBJS = $(C_SRCS:.c=.o) $(UNTRUSTED_DIR)/enclave_u.o $(CXX_OBJS)

FLAGS = --std=c++11 -m64 -O0 -g -fPIC -Wall -Wextra -Wpedantic -std=c11
SHLIB_LINK = -lsgx_urts -lpthread -lprotobuf-c # will be added to MODULE_big link line
CPPFLAGS := -DTOKEN_FILENAME=\"$(STEALTHDIR)/$(ENCLAVE_NAME).token\" \
			-DENCLAVE_FILENAME=\"$(STEALTHDIR)/$(ENCLAVE_NAME).signed.so\" \
			-DDATA_FILENAME=\"$(STEALTHDIR)/stealthDB.data\" \
			$(addprefix -I, $(CURDIR)/include $(SGX_INCLUDE_PATH) $(CURDIR)/$(UNTRUSTED_DIR) $(CURDIR) \
			$(shell pg_config --includedir-server) $(shell pg_config --includedir))
PG_CPPFLAGS := $(FLAGS) $(CPPFLAGS)
#
# Users need to specify their Postgres installation path through pg_config. For
# example: /usr/local/pgsql/bin/pg_config or /usr/lib/postgresql/9.3/bin/pg_config
#

cstore.pb-c.c: cstore.proto
	protoc-c --c_out=. cstore.proto

tools/%.o: tools/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@ # $@: name of target
	@echo "CXX  <=  $<"

$(UNTRUSTED_DIR)/enclave_u.c: $(SGX_EDGER8R) $(ENCLAVE_DIR)/enclave.edl
	@cd $(UNTRUSTED_DIR) && $(SGX_EDGER8R) --untrusted ../$(ENCLAVE_DIR)/enclave.edl
	@echo "GEN  =>  $@"

$(UNTRUSTED_DIR)/enclave_u.o: $(UNTRUSTED_DIR)/enclave_u.c
	@$(CC) $(CFLAGS) -c $< -o $@ # $<: name of the first prerequisite
	@echo "CC   <=  $<"

$(INTERFACE_DIR)/%.o: $(INTERFACE_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -o $@ -c $^ # $^: names of all the prerequisites
	@echo "CXX interface <=  $<"

cstore.pb-c.c: untrusted/cstore/cstore.proto
	protoc-c --c_out=. untrusted/cstore/cstore.proto
C_SRCS += $(CSTORE_DIR)/cstore.pb-c.c
C_OBJS += $(CSTORE_DIR)/cstore.pb-c.o
	# add them there since protoc-compiler added external .c & .h files

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
# use /usr/local/pgsql/lib/pgxs/src/makefiles/pgxs.mk to build extension


ifndef MAJORVERSION
    MAJORVERSION := $(basename $(VERSION))
endif

ifeq (,$(findstring $(MAJORVERSION), 9.3 9.4 9.5 9.6 10 11 12 13))
	$(shell echo $(MAJORVERSION))
    $(error PostgreSQL 9.3 to 13 is required to compile this extension)
endif

