#ifndef CLIENTGUI_H
#define CLIENTGUI_H

#include <QMainWindow>
#include "include/opcda/OPCClient.h"
#include <QDebug>


namespace Ui {
  class ClientGui;
}

class ClientGui : public QMainWindow
{
  Q_OBJECT

public:
  explicit ClientGui(QWidget *parent = 0);
  ~ClientGui();
  OPCClient *opc;

private slots:
  void on_btn_startopc_clicked();

  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();
  void sl_OnDataChange(OPCHANDLE hGroup,  DWORD dwCount, OPCHANDLE *phClientItems, VARIANT *pvValues, WORD *pwQualities, FILETIME *pftTimeStamps, HRESULT *pErrors);
  void on_pushButton_4_clicked();

private:
  Ui::ClientGui *ui;
};

#endif // CLIENTGUI_H
