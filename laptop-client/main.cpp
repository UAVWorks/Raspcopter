#include "MainWindow.h"

#include <QApplication>
#include <QMessageBox>

#include "netdb.h"
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize the Joystick
    Joystick *joystick = new Joystick("/dev/input/js1"); // Should probably be js0 on your computer
    if (!joystick->isFound())
    {
        QMessageBox q(QMessageBox::Warning, "Joystick failure", "The Joystick can't be found, is it connected ?");
        q.setStandardButtons(QMessageBox::Ok);
        q.exec();
        exit(1);
    }

    // Initialize the Network
    Network *network;
    struct hostent *host = gethostbyname("raspberry-pi");
    if(host != 0) {
        in_addr *addr = (in_addr *)host->h_addr_list[0];
        char *ip = inet_ntoa(*addr);
        network = new Network(ip);
    } else {
        QMessageBox q(QMessageBox::Warning, "Network failure",  "The IP can't be found, is the quadcopter connected ?");
        q.setStandardButtons(QMessageBox::Ok);
        q.exec();
        exit(2);
    }

    // Initialize the interface
    MainWindow window(joystick, network);
    window.showMaximized();

    return a.exec();
}
