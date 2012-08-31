CPP = g++
AR = ar
RANLIB = ranlib
FLAGS = -Wall -Werror 
#FLAGS += -O4
FLAGS += -g -DDEBUG

XML_LIB_PATH = ./XmlLib

INCS = -I./
INCS += -I./usr/local/include
INCS += -I$(XML_LIB_PATH)

OBJS = CMemcache.o CUrlParse.o CDnsParse.o CDataParse.o Common.o CKeyCheck.o CUrlData.o CUrlCheck.o CXml.o
XML_OBJS = tinystr.o tinyxml.o tinyxmlparser.o tinyxmlerror.o


TARGET = liburlcheck.a

LIBS = -L/usr/local/lib -lmemcached 
LIBS += -L/usr/local/lib -lcurl
LIBS += -L./ -lkeyrule

LIBKEY = libkeyrule.so

all: $(TARGET)

$(TARGET): $(OBJS) $(XML_OBJS)
	-rm -f $(TARGET)
	$(AR) cru $(TARGET) $(OBJS) $(XML_OBJS)
	$(RANLIB) $(TARGET)

lib: clean $(LIBKEY) $(OBJS)
	$(CPP) -shared -o liburlcheck.so $(OBJS) $(LIBS)

test: clean $(LIBKEY) $(OBJS) $(XML_OBJS)
	cp $(LIBKEY) /usr/lib -f
	$(CPP) -o test test.cpp $(OBJS) $(XML_OBJS) $(INCS) $(LIBS) $(FLAGS) 

%.o: %.cpp
	$(CPP) -c $< -o $@ $(FLAGS) $(INCS)

$(XML_OBJS):
	$(MAKE) -C $(XML_LIB_PATH)
	cd $(XML_LIB_PATH) && cp $(XML_OBJS) .. -f

$(LIBKEY): libkeyrule.o
	$(CPP) -shared -o $(LIBKEY) libkeyrule.o

libkeyrule.o: libkeyrule.cpp
	$(CPP) -c $< -o $@ $(INCS)

libkeyrule.cpp: keyrule.l
	flex -R -o libkeyrule.cpp keyrule.l

.PHONY: clean

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
	rm -f $(LIBKEY)
	make -C $(XML_LIB_PATH) clean
	@rm -f libkeyrule.cpp
	@rm -f libkeyrule.o
	@rm -f liburlcheck.so
	@rm -f test test.o
	@rm -f *.o
