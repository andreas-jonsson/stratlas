CXX = g++
CXXFLAGS = -DWITHOUT_WX -O2 -pipe -Wall -fomit-frame-pointer -frename-registers -fno-strict-aliasing -I. -I/usr/local/include -I./in -I./out -I./hlp
#CXXFLAGS = -pg -I. -I/usr/local/include -I./in -I./out -I./hlp
LDFLAGS = -L/usr/local/lib -lpng -ljpeg
#LDFLAGS = -pg
SOURCES = main_cmd.cpp in/in_warcraft.cpp out/out_png.cpp hlp/hlp_war.cpp hlp/hlp_mix.cpp in/in_cnctd.cpp in/in_war2.cpp hlp/hlp_mpq.cpp in/in_totala.cpp out/out_jpg.cpp out/out_bmp.cpp
OBJECTS=$(SOURCES:.cpp=.o)
NAME = stratlas_cmd

$(NAME): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)
	strip $@

clean:
	rm -f *.o */*.o $(NAME) $(NAME)_cmd

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@
