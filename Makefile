CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		features.o

LIBS =

TARGET =	features

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
