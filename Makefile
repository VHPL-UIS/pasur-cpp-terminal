GTEST_DIR = /usr/src/googletest/googletest
SRC_DIR = src
TEST_DIR = tests

CPPFLAGS += -isystem $(GTEST_DIR)/include -I$(SRC_DIR)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g -pthread

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)
OBJ_NO_MAIN = $(filter-out $(SRC_DIR)/main.o, $(OBJ))
HEADERS = $(wildcard $(SRC_DIR)/*.hpp)

TARGET = pasur
TEST_SRC = $(TEST_DIR)/test_game.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)
TEST_TARGET = test_game

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
	$(GTEST_DIR)/include/gtest/internal/*.h

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^


test_game.o : $(TEST_DIR)/test_game.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/test_game.cpp

test_game : test_game.o gtest_main.a $(OBJ_NO_MAIN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET) $(TEST_TARGET) gtest.a gtest_main.a *.o
