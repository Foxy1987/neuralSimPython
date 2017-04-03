CC = g++

# location of the Python header files
 
PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
 
PYTHON_CFLAGS = $$(/usr/bin/python2.7-config --cflags)
PYTHON_LIBS=-L/usr/lib64 $$(/usr/bin/python2.7-config --ldflags)

# location of the Boost Python include files and library
 
CVODE_LIBS = -L/usr/local/lib/ -lsundials_cvode -lsundials_nvecserial  
BOOST_INC = /usr/include
BOOST_LIB = -L/usr/lib64 -lboost_python
INCLUDE_DIR = ./include /usr/local/include
INCLUDES  := $(addprefix -I,$(INCLUDE_DIR))
CPPFLAGS = -g -O2 -Wall -fpic -std=c++11 
LDFLAGS = -shared -Wl,--export-dynamic -Wl,-rpath,/usr/lib64 

OBJDIR = ./obj
OBJECTS = $(addprefix $(OBJDIR)/, mymodules.o neuron.o compartment.o compstate.o neuronmodel.o rk65n.o rk4.o)


# compile mesh classes
TARGET = mymodules.so

all: $(TARGET)


${TARGET} : $(OBJECTS)
	${CC} $(LDFLAGS) $(OBJECTS) $(BOOST_LIB) $(PYTHON_LIBS) $(CVODE_LIBS) -o $(TARGET)

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CPPFLAGS) $(INCLUDES) -I$(PYTHON_INCLUDE) -I$(BOOST_INC) $< -o $@  


$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf *.so *.o
	rm -f $(OBJECTS)
