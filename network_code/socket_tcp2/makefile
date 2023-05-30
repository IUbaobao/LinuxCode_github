LD=#-DMYSELF
.PHONY:all
all:calclient calserver

calclient:calClient.cc 
	g++ -o $@ $^ -std=c++11 -ljsoncpp ${LD}
calserver:calServer.cc	
	g++ -o $@ $^ -std=c++11 -ljsoncpp ${LD}

.PHONY:clean
clean:
	rm -f calclient calserver
