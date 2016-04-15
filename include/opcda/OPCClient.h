#ifndef OPCCLIENT_H
#define OPCCLIENT_H
#include <iostream>
#include "opcerror.h"
#include "opccomn.h"
#include "OpcEnum.h"
#include "Opcda.h"
#include "copctransaction.h"
#include "wldef.h"
#include <QObject>
#include <QDebug>
#include "windows.h"
/**
 * Copyright 2016 Chn.Captain (chn.captain@gmail.com)
 */
class COPCDataCallback;
class ItemDef
{
public:
  QString	name;
  DWORD         accessRights;
  VARIANT	value;
  WORD		quality;
  OPCHANDLE	hServerHandle;
  OPCHANDLE	hClientHandle;
  FILETIME	time;
  VARTYPE	type;
  ItemDef()
  {
    type = VT_EMPTY;
    quality = 0;
    hServerHandle = 0;
    hClientHandle = 0;

    //ZeroMemory(&time, sizeof(time));
  }
  ItemDef( const ItemDef& it )
  {
    name =		it.name;
    value =		it.value;
    quality =		it.quality;
    hServerHandle =     it.hServerHandle;
    hClientHandle =     it.hClientHandle;
    time =		it.time;
    type =		it.type;
  }
};

class OPCClient:public QObject
{
  Q_OBJECT
public:
  explicit OPCClient();
  OPCClient(QString s);
  ~OPCClient();
  COPCTransaction *m_COPCTransaction;

  bool isWriteAble;

  bool isConnected();
  HRESULT Connect(QString s);
  void DisConnect();
  HRESULT AddGroup(QString n, DWORD update_rate , int async=1);
  QStringList AddItems(QStringList names);
  bool ReadItem(ItemDef *item);

  bool WriteValue(QString &name, QString &value );
  bool WriteValue( DWORD cHandle, FILETIME &time, VARIANT &value, WORD Quality );
  HRESULT WriteValue_Async(ItemDef * item);
  HRESULT WriteValue_Sync (ItemDef * item);

  HRESULT RemoveItems(QStringList inames);
  HRESULT RemoveAllItems();

  void dataCallback();
  QList<ItemDef*> *items;

  QString VariantString(VARIANT &pValue);
  QString QualityString(UINT qnr);
  QString ValueTypeString(const VARTYPE& v);
  void StringVariant(ItemDef* item, QString v);
  QString AccessTypeString(qint16 accessIdent);
  QString TimeString(FILETIME t);

  HRESULT hResult;
  HRESULT *pErrors;

  WString  m_ServerName;
  WString  m_GroupName;
  DWORD    m_UpdateRate;
  CLSID    m_clsid;
  DWORD	   m_Cookie;
  bool     m_Async;

  IConnectionPointContainer* _pIConnectionPointContainer;
  IConnectionPoint* _pIConnectionPoint;
  IOPCServer *_pIOPCServer;
  IOPCBrowseServerAddressSpace *_pIOpcNamespace;
  IOPCItemProperties *_pIOpcProperties;
  IOPCGroupStateMgt *_pIOPCGroupStateMgt;
  IOPCItemMgt *_pIOPCItemMgt;
  IOPCSyncIO * _pIOPCSyncIO;
  IOPCAsyncIO2 * _pIOPCAsyncIO2;
  IOPCDataCallback * _pIOPCDataCallback;
  COPCDataCallback *m_OPCDataCallback;

  OPCHANDLE  m_GroupHandle;
  OPCHANDLE  m_ServerHandle;
  void       ClearOPCITEMDEF( OPCITEMDEF *idef, int count = 1 );
  ItemDef*   getItemByName(QString s);
  ItemDef*   getItemByHandle(DWORD h);
  WString    qstr2Wstr(QString s);

};

#endif //OPCCLIENT_H
