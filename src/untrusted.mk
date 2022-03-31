include vars.mk

UNTRUSTED_DIR=untrusted
INTERFACE_DIR=untrusted/interface
EXTENSION_DIR=untrusted/extensions
LZ4_DIR=untrusted/lz4
PSQL_PKG_LIBDIR = $(shell pg_config --pkglibdir)
PSQL_SHAREDIR = $(shell pg_config --sharedir)/extension
PSQL_LIBDIR = $(shell pg_config --libdir)


EXTENSION = $(EXTENSION_DIR)/encdb        # the extension's name
DATA = $(EXTENSION_DIR)/encdb--1.0.1.sql  # scripts to install

C_SRCS := $(wildcard $(EXTENSION_DIR)/*.c)
VECTORIZED=yes
ifeq ($(VECTORIZED), yes)
	CSTORE_DIR=untrusted/postgres_vectorization_test
else
	CSTORE_DIR=untrusted/cstore
endif
C_SRCS += $(wildcard $(CSTORE_DIR)/*.c)
C_OBJS := $(C_SRCS:.c=.o)

CXX_SRCS := $(wildcard tools/*.cpp) $(wildcard $(INTERFACE_DIR)/*.cpp)
CXX_OBJS := $(CXX_SRCS:.cpp=.o)

PSQL_CPPFLAGS := $(addprefix -I, $(CURDIR) $(shell pg_config --includedir-server) $(shell pg_config --includedir))

CPPFLAGS := -DTOKEN_FILENAME=\"$(STEALTHDIR)/$(ENCLAVE_NAME).token\" \
			-DENCLAVE_FILENAME=\"$(STEALTHDIR)/$(ENCLAVE_NAME).signed.so\" \
			-DDATA_FILENAME=\"$(STEALTHDIR)/stealthDB.data\" \
			$(addprefix -I, $(CURDIR)/include $(SGX_INCLUDE_PATH) $(UNTRUSTED_DIR) $(CSTORE_DIR) $(LZ4_DIR)/lib)

FLAGS := -m64 -O0 -g -fPIC -Wall -Wextra -Wpedantic
CFLAGS := $(FLAGS) $(CPPFLAGS)
CXXFLAGS := $(FLAGS) $(CPPFLAGS) -std=c++11
LDFLAGS := -lsgx_urts -lpthread -lprotobuf-c -llz4 -Wl,--as-needed -Wl,-rpath $(PSQL_LIBDIR) --enable-new-dtags

.PHONY: all
all: cstore.pb-c.c lz4 $(UNTRUSTED_DIR)/encdb.so cstore

lz4:
	cd $(LZ4_DIR) && $(MAKE)
	cd $(LZ4_DIR) && $(MAKE) install


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


cstore.pb-c.c: $(CSTORE_DIR)/cstore.proto
	protoc-c --c_out=. $(CSTORE_DIR)/cstore.proto

C_SRCS += $(CSTORE_DIR)/cstore.pb-c.c
C_OBJS += $(CSTORE_DIR)/cstore.pb-c.o
$(info $(C_OBJS) $(wildcard $(CSTORE_DIR)/*.c))

$(EXTENSION_DIR)/%.o: $(EXTENSION_DIR)/%.c
	@$(CC) $(CFLAGS) $(PSQL_CPPFLAGS) -o $@ -c $^
	@echo "CC extension <=  $<"

cstore: $(CSTORE_DIR)/cstore.pb-c.c
$(CSTORE_DIR)/%.o: $(CSTORE_DIR)/%.c
	@$(CC) $(CFLAGS) $(PSQL_CPPFLAGS) -o $@ -c $^
	@echo "CC cstore <=  $<"
#		cd $(CSTORE_DIR) && $(MAKE)
#		cd $(CSTORE_DIR) && $(MAKE) install

$(UNTRUSTED_DIR)/encdb.so: $(UNTRUSTED_DIR)/enclave_u.o $(CXX_OBJS) $(C_OBJS)
	@$(CC) -shared -L$(PSQL_LIBDIR) $^ -o $@ $(LDFLAGS)
	@echo "CC extension <=  $<"
	@mkdir -p $(BUILD_DIR)
	@mv $(UNTRUSTED_DIR)/encdb.so $(BUILD_DIR)
	@cp $(EXTENSION_DIR)/*.control $(BUILD_DIR)
	@cp $(EXTENSION_DIR)/*.sql $(BUILD_DIR)


.PHONY: install
install: | $(STEALTHDIR)
	cp $(BUILD_DIR)/encdb.so $(PSQL_PKG_LIBDIR)
	cp $(BUILD_DIR)/*.control $(PSQL_SHAREDIR)
	cp $(BUILD_DIR)/*.sql $(PSQL_SHAREDIR)
	@test -e $(STEALTHDIR)/stealthDB.data || touch $(STEALTHDIR)/stealthDB.data
	@#chown postgres:postgres $(STEALTHDIR)/stealthDB.data

$(STEALTHDIR):
	mkdir -p $@

.PHONY: uninstall
uninstall:
	$(RM) $(PSQL_PKG_LIBDIR)/encdb.so \
          $(PSQL_SHAREDIR)/encdb* \
#          $(PSQL_SHAREDIR)/encdb*.sql \
          -r $(STEALTHDIR) $(BUILD_DIR)

.PHONY: clean
clean:
	@$(RM) $(CXX_OBJS) $(C_OBJS) $(UNTRUSTED_DIR)/enclave_u.*
ifeq ($(VECTORIZED), yes)
	cd $(CSTORE_DIR) && $(MAKE) clean
	cd $(LZ4_DIR) && $(MAKE) clean
else
	cd $(LZ4_DIR) && $(MAKE) clean
endif