#include <QApplication>

#include "visualizer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Visualizer w(DatasetFormat::KITTI);
    w.show();
    return a.exec();
}
