.PHONY:all
all:mysignalTest1 mysignal

mysignal:mysignal.cc 
	g++ -o $@ $^ -std=c++11 -g


mysignalTest1:mysignalTest1.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -rf mysignal mysignalTest1 
