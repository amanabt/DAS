#include "mainwindow.h"
#include <QApplication>
#include <chrono>
#include <thread>
#include <unistd.h>

int main(int argc, char *argv[])
{
    using namespace std::chrono_literals;
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

// 	for (auto j = 0; j < 100; ++j) {
		w.clear_dataset();
		for (auto x = 0.0; x < 100; ++x) {
	// 		std::this_thread::sleep_for(0.2s);
			w.add_datapoint (x, j * x * x);
		}
			w.update_plot();
// 			usleep(1000000);
// 	}
	
	
	
	return a.exec();
}
