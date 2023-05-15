.PHONY:all
all:tcpclient tcpserver

tcpclient:tcpClient.cc 
	g++ -o $@ $^ -std=c++11
tcpserver:tcpServer.cc	
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f tcpclient tcpserver
