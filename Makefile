SRC = Lab12.cpp
TARGET = Lab12.exe

UNAME_S := $(strip $(shell uname -s))

ifeq ($(UNAME_S), Linux)
	CXX = g++
	CXXFLAGS = -std=c++11 -Wall
	INCLUDE_PATH =
	LIB_PATH =
	LIBS = -lmysqlcppconn
	RM = rm -f
	COMPILE_CMD = $(CXX) $(CXXFLAGS) $(INCLUDE_PATH) $(LIB_PATH) -o $(TARGET) $(SRC) $(LIBS)

else

	CXX = cl
	CXXFLAGS = /EHsc
	INCLUDE_PATH = /I "C:/Program Files/MySQL/MySQL Connector C++ 9.3/include/jdbc"
	LIB_PATH = /LIBPATH:"C:/Program Files/MySQL/MySQL Connector C++ 9.3/lib64/vs14"
	LIBS = mysqlcppconn.lib ws2_32.lib
	RM = del /Q
	COMPILE_CMD = $(CXX) $(CXXFLAGS) $(SRC) $(INCLUDE_PATH) /link $(LIB_PATH) $(LIBS) /out:$(TARGET)
endif

all: $(TARGET)

$(TARGET): $(SRC)
	@echo "--- Compiling for $(if $(filter Linux,$(UNAME_S)),Linux,Windows) ---"
	$(COMPILE_CMD)
	@echo "--- Build finished: $(TARGET) ---"

clean:
	@echo "--- Cleaning up ---"
	$(RM) $(TARGET)
ifeq ($(OS), Windows_NT)
	$(RM) $(TARGET:.exe=.obj)
endif
	@echo "--- Cleanup finished ---"

.PHONY: all clean