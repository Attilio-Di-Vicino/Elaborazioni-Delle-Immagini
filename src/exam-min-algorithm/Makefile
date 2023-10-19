# Nome dell'eseguibile
TARGET = a.out

# Compiler
CXX = g++

# Opzioni di compilazione
CXXFLAGS = -std=c++11 -Wall

# Directory delle librerie OpenCV
OPENCV_LIBDIR = /usr/local/lib

# Librerie OpenCV
OPENCV_LIBS = -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc

# Directory degli header OpenCV
OPENCV_INCDIR = /usr/local/include/opencv4

# Percorso per cercare le librerie
LDFLAGS = -L$(OPENCV_LIBDIR)

# Percorso per cercare gli header
INCLUDES = -I$(OPENCV_INCDIR)

# Lista dei file sorgente predefiniti
SOURCES = main.cpp

# Lista dei file oggetto generati dalla compilazione
OBJECTS = $(SOURCES:.cpp=.o)

# Regola di compilazione
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS) $(OPENCV_LIBS)

# Definizione di un target personalizzato "clean" per la pulizia
clean:
	rm -f *.o all
	rm -f $(TARGET)
