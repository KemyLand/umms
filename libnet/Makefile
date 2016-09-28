LIB         := libnet.a
SRCS        := $(shell ls | grep .cpp)
TARGET_SRCS := $(shell find $(TARGET) -name '*.cpp')
OBJS        := $(addsuffix .o,$(basename $(SRCS)) $(basename $(TARGET_SRCS)))
HEADER_PATH := $(readlink -f include)
L_CXXFLAGS  := $(CXXFLAGS) -I$(HEADER_PATH) -std=c++14
HEADERS     := $(shell find $(HEADER_PATH) -name '*.hpp')

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^

clean:
	rm -f $(LIB) $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(L_CXXFLAGS) -o $@ -c $<
