L_CXXFLAGS  := $(CXXFLAGS) -Iinclude -std=c++14
L_LDFLAGS   := $(LDFLAGS)
EXE         := umms.bin
SRCS        := $(shell find umms -name '*.cpp')
OBJS        := $(addsuffix .o,$(basename $(SRCS)))
HEADERS     := $(shell find include -name '*.hpp')

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(L_LDFLAGS)

clean:
	rm -rf $(EXE) $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(L_CXXFLAGS) -o $@ -c $<
