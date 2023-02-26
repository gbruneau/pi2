all: pi searchPi

pi: pi.cpp
	g++ pi.cpp -o pi -lgmpxx -lgmp

searchPi: searchPi.cpp
	g++ searchPi.cpp -o searchPi


clean:
	rm -f pi
	rm -f searchPi

