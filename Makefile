HEADER_PATH := $(shell readlink -f include)
L_CXXFLAGS  := $(CXXFLAGS) -I$(HEADER_PATH) -std=c++14
L_LDFLAGS   := $(LDFLAGS)
TARGET      := posix

EXE       := umms.bin
LIB_NAMES := libnet
HEADERS   := $(shell find $(HEADER_PATH) -name '*.hpp')
LIBS      := $(foreach LIB,$(LIB_NAMES),$(LIB)/$(LIB).a)
SRCS      := $(shell find umms -name '*.cpp')
OBJS      := $(addsuffix .o,$(basename $(SRCS)))

all: $(EXE)

$(EXE): $(OBJS)
	for LIB in $(LIB_NAMES); do \
		$(MAKE) -C $$LIB CXXFLAGS="$(CXXFLAGS)" HEADER_PATH="$(HEADER_PATH)" TARGET=$(TARGET); \
	done
	$(CXX) -o $@ $(OBJS) $(LIBS) $(L_LDFLAGS)

clean:
	rm -f $(EXE) $(OBJS)
	for LIB in $(LIB_NAMES); do \
		$(MAKE) clean -C $$LIB; \
	done

%.o: %.cpp $(HEADERS)
	$(CXX) $(L_CXXFLAGS) -o $@ -c $<
