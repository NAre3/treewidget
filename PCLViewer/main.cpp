#include "PCLViewer.h"
#include <QtWidgets/QApplication>


int main(int argc, char* argv[])
{
    vtkOutputWindow::SetGlobalWarningDisplay(0);
    QApplication a(argc, argv);
    PCLViewer w;
    w.show();
    return a.exec();
}