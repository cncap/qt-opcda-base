#include "clientgui.h"
#include "ui_clientgui.h"
#include <QDateTime>
#include <QUuid>

ClientGui::ClientGui(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ClientGui)
{
  ui->setupUi(this);
  opc = 0;
  for(int i=0;i<6;i++){
      QString uuid = QUuid::createUuid().toString();

      qDebug()<<  uuid.mid(1,uuid.length()-2);
    }

}

ClientGui::~ClientGui()
{
  if(opc){
      opc->DisConnect();
      opc->deleteLater();
    }
  delete ui;
}
void ClientGui::sl_OnDataChange(OPCHANDLE hGroup,  DWORD dwCount, OPCHANDLE *phClientItems, VARIANT *pvValues, WORD *pwQualities, FILETIME *pftTimeStamps, HRESULT *pErrors)
{
  for (DWORD i = 0; i < dwCount; i++) {
      qDebug()<<"---from gui:"
          <<hGroup
          <<dwCount
          <<phClientItems[i]
          <<opc->VariantString(pvValues[i])
          <<opc->QualityString(pwQualities[i])
          <<QString::number(pftTimeStamps[i].dwLowDateTime)
          <<pErrors[i];
    }
}

void ClientGui::on_btn_startopc_clicked()
{
  if(!opc){
      opc=new OPCClient();
    }
  if(!opc->isConnected()){
      QString serverName = "KEPware.KEPServerEx.V4";
      bool result =  opc->Connect(serverName);
      if(result==S_OK){
          qDebug()<< "连接成功";
          QString groupName = "MPI";
          if(opc->AddGroup(groupName,1000)==S_OK){
              qDebug()<< "分组添加成功";

              connect(opc->m_COPCTransaction,SIGNAL(sg_OnDataChange(OPCHANDLE,DWORD,OPCHANDLE*,VARIANT*,WORD*,FILETIME*,HRESULT*)),this,SLOT(sl_OnDataChange(OPCHANDLE,DWORD,OPCHANDLE*,VARIANT*,WORD*,FILETIME*,HRESULT*)));
            }else{
              qDebug()<< "装载失败";
            }
        }
    }

}

void ClientGui::on_pushButton_clicked()
{
    if(opc){
       qDebug()<<"删除所有项:"<< opc->RemoveAllItems();
       qDebug()<<opc->items->count();

      }
}

void ClientGui::on_pushButton_2_clicked()
{
  if(!opc) return;
  QStringList inames;
  inames.append("s7-300.MPI.Tag_1");
  inames.append("s7-300.MPI.Tag_2");
//  inames.append("s7-300.MPI.Tag_3");
//  inames.append("s7-300.MPI.Tag_4");
//  inames.append("s7-300.MPI.Tag_5");
//  inames.append("s7-300.MPI.Tag_6");
//  inames.append("s7-300.MPI.Tag_7");
//  inames.append("s7-300.MPI.Tag_8");
//  inames.append("s7-300.MPI.Tag_9");
//  inames.append("s7-300.MPI.Tag_10");
//  inames.append("s7-300.MPI.Tag_11");
//  inames.append("s7-300.MPI.Tag_12");
//  inames.append("s7-300.MPI.Tag_13");
  QStringList r_inames= opc->AddItems(inames);
  if(r_inames.count()>0){
      qDebug()<<"添加项目监视成功。" ;
//                  for(int i=0;i<opc->items->count();i++){
//                      qDebug()<<"读取指定项目值：" ;
//                      if(opc->ReadItem(opc->items->value(i))){
//                          qDebug()<< opc->items->value(i)->hServerHandle<<opc->items->value(i)->hClientHandle<<opc->QualityString(opc->items->value(i)->quality)<<opc->VariantString(opc->items->value(i)->value);
//                        }
//                    }
    }else{
      qDebug()<<"添加项目监视失败。";
    }
}

void ClientGui::on_pushButton_3_clicked()
{
  if(opc->isWriteAble){
    }
}

void ClientGui::on_pushButton_4_clicked()
{
    if(opc){
        opc->RemoveItems(QStringList()<<ui->lineEdit_2->text());
      }
}
