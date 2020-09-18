SRC=main.cpp tinyxml.cpp tinystr.cpp tinyxmlerror.cpp tinyxmlparser.cpp
#TARGET=$(patsubst %.cpp, %, $(SRC))
TARGET=demo
CFLAGS=-g

$(TARGET):${SRC:%.cpp=%.o}
	$(CXX) $^ -o $@ $(CFLAGS)
%.o:%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)
.PHONY: clean
clean:
	rm *.o $(TARGET) -rf