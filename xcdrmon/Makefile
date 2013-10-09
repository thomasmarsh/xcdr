CXXBUILD = g++ -O2 $< -o $@ `gnome-config gnomemm --cflags --libs` 
CXXCOMPILE = g++ -O2 -c $< -o $@ `gnome-config gnomemm --cflags` 
CXXLINK = g++ -O2 -o $@ `gnome-config gnomemm --libs` 

MYINC = -I../common -I/usr/local/include/mysqlcppapi-2.0

TARGET = xcdrmon mon

all: $(TARGET)

xcdrmon: application.o xcdrmon.o
	$(CXXLINK) $^ $(LIBS)

monitor.o: monitor.cc
	g++ -c $< -o $@ $(MYINC)

main.o: main.cc
	g++ -c $< -o $@ $(MYINC)

mon: monitor.o main.o
	g++ $^ -o $@ ./libxcdrCommon-0.5-0.5.so -lmysqlcppapi-1.9 -lmysqlclient

xcdrmon.o : xcdrmon.cc
	g++ -O2 -c $< -o $@ `gnome-config gnomemm --cflags` 

application.o : application.cc
	g++ -O2 -c $< -o $@ `gnome-config gnomemm --cflags` 

clean:  
	rm -Rf *.o $(TARGET) .libs
