#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application metadata
    app.setApplicationName("Death Riddle");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Death Riddle Studios");
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}
