CXX = g++
STD = -std=c++23
TEST = -g

with-locks:
	$(CXX) $(STD) $(TEST) -o with-locks Barrier.cpp

no-locks:
	$(CXX) $(STD) $(TEST) -o no-locks BarrierNoLock.cpp

clean:
	rm with-locks no-locks
