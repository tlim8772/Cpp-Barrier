CXX = g++
STD = -std=c++23
TEST = -g -fsanitize=thread

with-locks:
	$(CXX) $(STD) $(TEST) -o with-locks Barrier.cpp

no-locks:
	$(CXX) $(STD) $(TEST) -o no-locks BarrierNoLock.cpp

clean:
	rm with-locks no-locks

disable-aslr:
	echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
