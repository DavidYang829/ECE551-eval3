CFLAGS=-std=c++11

all: cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4

cyoa-step%: cyoa-step%.o page.o story.o choice.o
	g++ -o $@ $^

%.o: %.cpp
	g++ $(CFLAGS) -c $<

clean:
	rm -f cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4 choice.o page.o story.o cyoa-step1.o cyoa-step2.o cyoa-step3.o cyoa-step4.o
