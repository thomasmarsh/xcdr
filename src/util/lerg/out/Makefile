CFLAGS = -O3 -pipe -march=athlon-xp -falign-functions=4 -fomit-frame-pointer -mfancy-math-387

SRCS = test.cc
OBJS = $(SRCS:.cc=.o)

test: $(OBJS)
	$(CXX) -o test $(OBJS)

%.o: %.cc node.h
	$(CXX) -c $< -o $@ $(CFLAGS)

