CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		threescanner.o

LIBS =

TARGET =	threescanner

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
