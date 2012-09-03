CPP = g++
AR = ar
RANLIB = ranlib
MAKE = make

XML_PATH = ./XmlLib

FLAGS = -Wall -Werror #-O4
FLAGS += -g

INCS = -I.
INCS += -I$(XML_PATH)
INCS += -I/usr/local/include


OBJS = CXml.o Common.o CSocket.o
XMLOBJS = tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o

TARGET = libaddr.a

all: $(TARGET)

$(TARGET): $(OBJS) $(XMLOBJS)
	@rm -f $(TARGET)
	$(AR) cru $(TARGET) $(OBJS) $(XMLOBJS)
	$(RANLIB) $(TARGET)

%.o: %.cpp
	$(CPP) -o $@ -c $< $(INCS) $(FLAGS)

$(XMLOBJS):
	$(MAKE) -C $(XML_PATH)
	cd $(XML_PATH) && cp $(XMLOBJS) .. -f

test: $(OBJS) $(XMLOBJS) test.cpp 
	$(CPP) -o test test.cpp $(OBJS) $(XMLOBJS) $(INCS) $(FLAGS)

format:
	indent *.cpp *.h

.PHONY: clean
clean:
	$(MAKE) -C $(XML_PATH) clean
	-rm -f *.o
	-rm -f *.a
	@-rm -f *~
	@-rm -f test
