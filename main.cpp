#include <QApplication>
#include "clientgui.h"

int main(int argc, char *argv[])
{

  QApplication a(argc, argv);
  ClientGui c;
  c.show();


  //opc.InsertItens(items2);

  //Teste.Show(3);

  return a.exec();
}
