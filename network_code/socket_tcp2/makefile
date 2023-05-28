.PHONY:all
all:calclient calserver

calclient:calClient.cc 
	g++ -o $@ $^ -std=c++11
calserver:calServer.cc	
	g++ -o $@ $^ -std=c++11 

.PHONY:clean
clean:
	rm -f calclient calserver
