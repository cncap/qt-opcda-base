#include "OPCClient.h"
#include "copcdatacallback.h"
#include <QDateTime>
#include <QMessageBox>

OPCClient::OPCClient(QString s):
  m_ServerName(qstr2Wstr(s))
{
}
OPCClient::OPCClient()
{
  _pIConnectionPointContainer = NULL;
  _pIConnectionPoint          = NULL;
  _pIOPCServer                = NULL;
  _pIOPCDataCallback          = NULL;
  _pIOpcNamespace             = NULL;
  _pIOpcProperties            = NULL;
  _pIOPCGroupStateMgt         = NULL;
  _pIOPCItemMgt               = NULL;
  _pIOPCSyncIO                = NULL;
  _pIOPCAsyncIO2              = NULL;
  m_GroupHandle               = 0;
  m_ServerHandle              = 0;
  m_OPCDataCallback           = NULL;
  m_COPCTransaction           = NULL;
  m_Cookie                    = 0;
  pErrors                     = NULL;
  //state
  isWriteAble                 = NULL;

  items = new QList<ItemDef*>();
}
/**
 * 建立OPC连接
 * @brief OPCClient::Connect
 * @param s
 * @return
 */
HRESULT OPCClient::Connect(QString s)
{
  this->m_ServerName = qstr2Wstr(s);
  hResult = CoInitialize(0);
  if (FAILED(hResult))
    {
      if (hResult == S_FALSE)
        {
          qDebug()<<"Error CoInitialize():COM Library already initialized";
        }
      else
        {
          qDebug()<< "Error CoInitialize()：Initialisation of COM Library failed. Error Code= " << hResult;
          _pIOPCServer = 0;
          CoUninitialize();
          return hResult;
        }
    }

  hResult = CLSIDFromProgID(this->m_ServerName, &this->m_clsid);
  if (FAILED(hResult))
    {
      qDebug()<< "Error CLSIDFromProgID():Retrival of CLSID failed";
      CoUninitialize();
      return hResult;
    }

  hResult = CoCreateInstance (this->m_clsid, NULL, CLSCTX_LOCAL_SERVER ,IID_IOPCServer, (void**)&_pIOPCServer);
  if (FAILED(hResult))
    {
      qDebug()<< "Error CoCreateInstance():Creation of IOPCServer-Object failed";
      _pIOPCServer = 0;
      CoTaskMemFree(&this->m_clsid);
      CoUninitialize();
      return hResult;
    }
  hResult = _pIOPCServer->QueryInterface(IID_IOPCBrowseServerAddressSpace, (void**)&_pIOpcNamespace);
  if (FAILED(hResult))
    {

      qDebug()<< "Error CoCreateInstance():Creation of IID_IOPCBrowseServerAddressSpace-Object failed";
      CoUninitialize();
      return hResult;
    }
  hResult = _pIOPCServer->QueryInterface(IID_IOPCItemProperties, (void**)&_pIOpcProperties);
  if (FAILED(hResult))
    {
      qDebug()<< "Error CoCreateInstance():Creation of IID_IOPCItemProperties-Object failed";
      CoUninitialize();
      return hResult;
    }
  return hResult;
}

//HRESULT OPCClient::GetBranches(QStringList arrPath, vector<CString>& vBranches)
//{
//	if (!_pIOpcNamespace)	return S_FALSE;

//	int							nCount = 0;
//	int							v = 0;
//	WCHAR						sEmpty[] = {0};
//	WCHAR*						sFullName;
//	WCHAR*						sTmp;
//	ULONG						ulLen;
//#ifndef _UNICODE
//	size_t						sztTmp;
//	TCHAR						szBuffer[MAX_PATH];
//	WCHAR						szChannel[MAX_PATH];
//#endif

//	ATL::CComPtr<IEnumString>	pIEnum;
//	OPCNAMESPACETYPE			nameSpaceType;
//	HRESULT						hResult = _pIOpcNamespace->QueryOrganization(&nameSpaceType);

//	//UP TO TOP
//	do
//	{
//		hResult = _pIOpcNamespace->ChangeBrowsePosition(OPC_BROWSE_UP,L"");
//		nCount++;
//	}  while (SUCCEEDED(hResult) && nCount <100);

//	for (unsigned int i=0;i<arrPath.size();i++)
//	{
//#ifdef _UNICODE
//		hResult = _pIOpcNamespace->ChangeBrowsePosition(OPC_BROWSE_DOWN, arrPath[i]);
//#else
//		mbstowcs_s(&sztTmp,szChannel,arrPath[i].GetLength()+1,arrPath[i],sizeof(szChannel)/sizeof(WCHAR));
//		hResult = _pIOpcNamespace->ChangeBrowsePosition(OPC_BROWSE_DOWN, szChannel);
//#endif
//		//WRONG ORGANIZATION - DUCVD
//		if (FAILED(hResult)) return S_FALSE;
//	}

//	hResult = _pIOpcNamespace->BrowseOPCItemIDs(OPC_BRANCH,sEmpty,VT_EMPTY,0,(&pIEnum));
//	CHECK_VALID_DO(FAILED(hResult),return S_FALSE);

//	while((hResult = pIEnum->Next(1, &sTmp, &ulLen)) == S_OK)
//	{
//		hResult = _pIOpcNamespace->GetItemID(sTmp, &sFullName);
//		if (SUCCEEDED(hResult))
//		{
//#ifdef _UNICODE
//			vBranches.push_back(sFullName);
//#else
//			wcstombs_s(&sztTmp,szBuffer,sFullName,sizeof(szBuffer)/sizeof(TCHAR));
//			vBranches.push_back(szBuffer);
//#endif
//		}
//		_pIMalloc->Free(sFullName);
//		_pIMalloc->Free(sTmp);
//	}
//	return S_OK;
//}
//HRESULT OPCClient::GetLeafs(const vector<CString>& arrPath,vector<CString>& vLeafs,int nMaxLeafToGet, DWORD dwAccessRight)
//{
//	if (!_pIOpcNamespace)	return S_FALSE;

//	int							nCount = 0;
//	int							v = 0;
//	WCHAR						sEmpty[] = {0};
//	WCHAR*						sFullName;
//	WCHAR*						sTmp;
//	ULONG						ulLen;
//#ifndef _UNICODE
//	size_t						sztTmp;
//	TCHAR						szBuffer[MAX_PATH];
//	WCHAR						szChannel[MAX_PATH];
//#endif

//	ATL::CComPtr<IEnumString>	pIEnum;
//	OPCNAMESPACETYPE			nameSpaceType;
//	HRESULT						hResult = _pIOpcNamespace->QueryOrganization(&nameSpaceType);

//	//UP TO TOP
//	do
//	{
//		hResult = _pIOpcNamespace->ChangeBrowsePosition(OPC_BROWSE_UP,L"");
//		nCount++;
//	}  while (SUCCEEDED(hResult) && nCount <100);

//	for (unsigned int i=0;i<arrPath.size();i++)
//	{
//#ifdef _UNICODE
//		hResult = _pIOpcNamespace->ChangeBrowsePosition(OPC_BROWSE_DOWN, arrPath[i]);
//#else
//		mbstowcs_s(&sztTmp,szChannel,arrPath[i].GetLength()+1,arrPath[i],sizeof(szChannel)/sizeof(WCHAR));
//		hResult = _pIOpcNamespace->ChangeBrowsePosition(OPC_BROWSE_DOWN, szChannel);
//#endif
//		//WRONG ORGANIZATION - DUCVD
//		if (FAILED(hResult)) return hResult;
//	}

//	hResult = _pIOpcNamespace->BrowseOPCItemIDs(OPC_LEAF,sEmpty,VT_EMPTY,dwAccessRight,(&pIEnum));
//	nCount	= vLeafs.size();
//	CHECK_VALID_DO(FAILED(hResult),return hResult);

//	while((hResult = pIEnum->Next(1, &sTmp, &ulLen)) == S_OK && nCount<nMaxLeafToGet)
//	{
//		hResult = _pIOpcNamespace->GetItemID(sTmp, &sFullName);
//		if (SUCCEEDED(hResult))
//		{
//#ifdef _UNICODE
//			vLeafs.push_back(sFullName);
//#else
//			wcstombs_s(&sztTmp,szBuffer,sFullName,sizeof(szBuffer)/sizeof(TCHAR));
//			vLeafs.push_back(szBuffer);
//#endif
//			nCount++;
//		}
//		_pIMalloc->Free(sFullName);
//		_pIMalloc->Free(sTmp);
//	}
//	return S_OK;
//}
/**
 * 添加分组
 * @brief OPCClient::AddGroup
 * @param n
 * @param update_rate
 * @return
 */
HRESULT OPCClient::AddGroup(QString n, DWORD update_rate, int async)
{
  m_Async = async;
  m_GroupName = qstr2Wstr(n);
  m_ServerHandle = 1;
  m_UpdateRate = 100;
  long TimeBias;
  DWORD LanguageCode = 0x416, RevisedUpdateRate;
  float DeadBand; //刷新间隔
  hResult=_pIOPCServer->AddGroup(m_GroupName,  // [in] group name
                                 TRUE,                  // [in] active
                                 update_rate,           // [in] request this Update Rate from Server
                                 m_ServerHandle,        // [in] Client handle
                                 &TimeBias,             // [in] no time interval to system UTC time
                                 &DeadBand,             // [in] no deadband, so all data changes are reported
                                 LanguageCode,          // [in] Server uses English language for text values
                                 &m_GroupHandle,        // [out] Server handle to identify this group in later calls
                                 &RevisedUpdateRate,    // [out] the answer form the Server to the requested update rate
                                 IID_IOPCGroupStateMgt, // [in] requested interface type of the group object
                                 (LPUNKNOWN*)&this->_pIOPCGroupStateMgt); // [out] pointer to the requested interface

  if( hResult ==OPC_E_DUPLICATENAME ) {
      qDebug()<< "1001:分组名称已存在.";
    }
  if( hResult ==E_NOINTERFACE ) {
      qDebug()<< "1002:IOPCServer::AddGroup returned E_NOINTERFACE (IID_IOPCGroupStateMgt)";
    }
  if( update_rate != m_UpdateRate ){
      qDebug()<< "1003: OPC server rejected data refresh interval. Setting it to %1 ms." << update_rate;
    }
  if (hResult == OPC_S_UNSUPPORTEDRATE)
    {
      qDebug()<< "1004:请求的刷新速率与实际的刷新速率不一致";
    }
  if( FAILED(hResult) || _pIOPCGroupStateMgt == NULL) {
      qDebug()<< "1005: 创建分组失败";
      DisConnect();
    }

  hResult = _pIOPCGroupStateMgt->QueryInterface(IID_IOPCItemMgt,(void**)&this->_pIOPCItemMgt);
  if (FAILED(hResult))  {
      qDebug()<<"1006: 项目操作指针(_pIOPCItemMgt)创建失败";
      DisConnect();
    }
  //查询 group 对象的同步接口



  if(m_Async){
      hResult = _pIOPCItemMgt->QueryInterface(IID_IOPCAsyncIO2, (void**)&this->_pIOPCAsyncIO2);
      if (FAILED(hResult))  {
          qDebug()<<"1008: IOPCAsyncIO2 没有发现，错误的查询!";
          DisConnect();
        }

      m_COPCTransaction = new COPCTransaction;
      hResult = _pIOPCItemMgt->QueryInterface(IID_IConnectionPointContainer, (void**)&this->_pIConnectionPointContainer);

      //hResult =  _pIOPCItemMgt->QueryInterface(&_pIConnectionPointContainer);
      if (FAILED(hResult))  {
          qDebug()<<"1009: IConnectionPointContainer 创建失败!";
          DisConnect();
        }
      hResult = _pIConnectionPointContainer->FindConnectionPoint( IID_IOPCDataCallback, &_pIConnectionPoint );
      if( FAILED(hResult) || _pIConnectionPoint == NULL) {
          qDebug()<< "1010: A group of OPC with no IOPCDataCallback. OPC server appears to not conform to the OPC DA 2.0 standard (%s).";
          DisConnect();
        }
      if( m_OPCDataCallback == NULL ) {

          if(!m_COPCTransaction){
              m_COPCTransaction = new COPCTransaction;
            }
          m_OPCDataCallback = new COPCDataCallback(m_COPCTransaction);
          m_OPCDataCallback->AddRef();
          // m_OPCDataCallback->receiver = m_DataReceiver;
        }
      hResult = _pIConnectionPoint->Advise(m_OPCDataCallback, &m_Cookie);
      qDebug()<< hResult;
      if(FAILED(hResult)) {
          qDebug()<< "1011: OPCDataCallback set faild." << m_Cookie;
          _pIConnectionPointContainer->Release();
          DisConnect();
        }
      isWriteAble = true;
    }else{
      hResult = _pIOPCItemMgt->QueryInterface(IID_IOPCSyncIO, (void**)&this->_pIOPCSyncIO);
      if (FAILED(hResult))  {
          qDebug()<<"1007: IOPCSyncIO 没有发现，错误的查询!";
          DisConnect();
        }
      isWriteAble = true;
    }

  return hResult;
}

/**
 * 添加监视项
 * @brief OPCClient::AddItems
 * @param inames
 * @return
 */
QStringList OPCClient::AddItems(QStringList inames)
{
  QStringList r_inames;

  if(!_pIOPCItemMgt) return r_inames;

  pErrors = NULL;
  DWORD iCount=items->count();
  OPCITEMRESULT * pResults;
  qDebug()<<"1--- begin add item -----------";
  for(int i=0;i<inames.count();i++) {
      QString iname = inames[i];
      if(getItemByName(iname)){
          qDebug() << "    already exsits."<< iname;
          continue;
        }
      r_inames.append(iname);
    }
  int nCount=r_inames.count();
  if(nCount>0){
      OPCITEMDEF idef[nCount];
      for(int i=0;i<nCount;i++) {
          QString iname = r_inames[i];
          qDebug()<< "    build item def"<< iname;
          idef[i].szAccessPath= SysAllocString(L"");           // path SysAllocString(qstr2Wstr(name))
          idef[i].szItemID    = SysAllocString(qstr2Wstr(iname));       // name
          idef[i].bActive     = TRUE;
          idef[i].hClient     = iCount+i;
          idef[i].dwBlobSize  = 0;
          idef[i].pBlob       = NULL;
          idef[i].vtRequestedDataType = 0;
        }
      qDebug()<<"2--- end of add item-----------";

      hResult = _pIOPCItemMgt->AddItems(nCount,  // [in] add items
                                        idef,        // [in] see above
                                        &pResults,   // [out] array with additional information about the item
                                        &pErrors);   // [out] tells which of the items was successfully added.

      if(hResult==S_OK && hResult!=S_FALSE){
          for(int i=0;i < nCount; i++) {
              QString iname = r_inames[i];
              DWORD cItemHandle = iCount+i;
              ItemDef *item = new ItemDef;
              item->name = iname;
              item->accessRights = pResults[i].dwAccessRights;
              item->hClientHandle = cItemHandle;
              item->type = pResults[i].vtCanonicalDataType;
              item->hServerHandle = pResults[i].hServer;
              items->append(item);
              //    检测 Item 的可读写性
              //            if (pResults[i].dwAccessRights != (OPC_READABLE +  OPC_WRITEABLE))   {
              //                qDebug()<<"Item 不可读，也不可写,请检查服务器配置";
              //              }
            }
        }

     if(pResults) CoTaskMemFree( pResults );
      if(pErrors)  CoTaskMemFree( pErrors );
      ClearOPCITEMDEF(idef, nCount);
    }
   return r_inames;
}
/**
 * 同步读取指定项
 * @brief OPCClient::ReadItem
 * @param item
 * @return
 */
bool OPCClient::ReadItem(ItemDef *item)
{
  bool result = false;
  OPCITEMSTATE *pItemValue;
  hResult = _pIOPCSyncIO->Read(OPC_DS_DEVICE, 1, &item->hServerHandle,  &pItemValue, &pErrors);

  if(SUCCEEDED(hResult)){
      item->value = pItemValue[0].vDataValue;
      item->quality = pItemValue[0].wQuality;
      item->time = pItemValue[0].ftTimeStamp;
      item->hClientHandle = pItemValue[0].hClient;
      result = true;
      VariantClear(&pItemValue->vDataValue);
    }else{
      qDebug()<< "同步读取项目失败" << hResult;
    }

  CoTaskMemFree(pErrors);
  CoTaskMemFree(pItemValue);

  return result;
}
/**
 * 写入值
 * @brief OPCClient::WriteValue
 * @param cHandle
 * @param time
 * @param value
 * @param Quality
 * @return
 */
bool OPCClient::WriteValue( DWORD cHandle, FILETIME &time, VARIANT &value, WORD Quality )
{
	ItemDef * item = getItemByHandle( cHandle );
	if( !item )
		return false;

	item->quality = Quality;
	item->value = value;
	item->time = time;

	if( m_Async )
		return SUCCEEDED( WriteValue_Async(item) );
	else
		return SUCCEEDED( WriteValue_Sync( item ) );
}
bool OPCClient::WriteValue(QString &n, QString &v )
{
	ItemDef * item = getItemByName(n);
	if(!item )
		return false;
	StringVariant(item, v);
	if( m_Async )
		return SUCCEEDED( WriteValue_Async(item) );
	else
		return SUCCEEDED( WriteValue_Sync( item ) );
}
HRESULT OPCClient::WriteValue_Async(ItemDef * item)
{
          QString str;
          pErrors    = NULL;
          if( m_COPCTransaction == NULL ) return E_FAIL;
          if( !isConnected() ) return E_FAIL;

         // m_Group->QueryInterface( &AsyncIO );
          if( _pIOPCAsyncIO2 == NULL ) {
                  qDebug()<< "Failed to get interface IOPCAsyncIO2";
                  return E_FAIL;
          }
          DWORD cancelID = 0;

          hResult = _pIOPCAsyncIO2->Write( 1, &item->hServerHandle, &item->value, rand(), &cancelID, &pErrors);
          if( FAILED(hResult) /*|| FAILED(pErrors[0])*/) {
                  qDebug()<< "Parameter [%s] is not passed" << item->name;
                  hResult = E_FAIL;

          }
          if( pErrors )
          {
                  switch( pErrors[0] ) {
                          case OPC_S_CLAMP:
                                  qDebug()<< "AsyncIO->Write(%s) -> [OPC_S_CLAMP] The value was accepted but was clamped."<<item->name;
                                  break;
                          case OPC_E_RANGE: qDebug()<< "AsyncIO->Write(%s) -> [OPC_E_RANGE] The value was out of range."<<item->name;
                                  break;
                          case OPC_E_BADTYPE:
                                  str=QString("AsyncIO->Write(%1) -> [OPC_E_BADTYPE] The passed data type (%2) cannot be accepted for this item.").arg(item->name, item->value.vt);
                                  qDebug()<< str;
                                  break;
                          case OPC_E_BADRIGHTS:
                                  qDebug()<< "AsyncIO->Write(%s) -> [OPC_E_BADRIGHTS] The item is not writeable."<<item->name;
                                  break;
                          case OPC_E_INVALIDHANDLE:
                                  qDebug()<< "AsyncIO->Write(%s) -> [OPC_E_INVALIDHANDLE] The passed item handle was invalid."<<item->name;
                                  break;
                          case OPC_E_UNKNOWNITEMID:
                                  qDebug()<< "AsyncIO->Write(%s) -> [OPC_E_UNKNOWNITEMID] The item is no longer available in the server address space."<<item->name;
                                  break;
                  }

          }
          if( pErrors )CoTaskMemFree( pErrors );
          return hResult;

}

HRESULT OPCClient::WriteValue_Sync( ItemDef * item )
{
  pErrors    = NULL;
  if( m_COPCTransaction == NULL ) return E_FAIL;
  if( !isConnected() ) return E_FAIL;

        qDebug() << "Sync Write start (hdl="<< item->hServerHandle << ") value=" << VariantString(item->value);

        hResult = _pIOPCSyncIO->Write( 1, &item->hServerHandle, &item->value, &pErrors);

        qDebug() << "Sync Write finished. hr = " << hResult;

	if( FAILED(hResult) /*|| FAILED(pErrors[0])*/) {
		qDebug() << "Parameter [%s] is not passed"<<item->name;
		hResult = E_FAIL;

		if( pErrors != NULL)
		{
			switch( pErrors[0] ) {
			case OPC_S_CLAMP:
				qDebug() << "SyncIO->Write(%s) -> [OPC_S_CLAMP] The value was accepted but was clamped." << item->name ;
				break;
			case OPC_E_RANGE:
				qDebug() << "SyncIO->Write(%s) -> [OPC_E_RANGE] The value was out of range."<<item->name  ;
				break;
			case OPC_E_BADTYPE:
				qDebug() << "SyncIO->Write(%s) -> [OPC_E_BADTYPE] The passed data type cannot be accepted for this item."<<item->name ;
				break;
			case OPC_E_BADRIGHTS:
				qDebug() << "SyncIO->Write(%s) -> [OPC_E_BADRIGHTS] The item is not writeable."<<item->name ;
				break;
			case OPC_E_INVALIDHANDLE:
				qDebug() << "SyncIO->Write(%s) -> [OPC_E_INVALIDHANDLE] The passed item handle was invalid."<<item->name ;
				break;
			case OPC_E_UNKNOWNITEMID:
				qDebug() << "SyncIO->Write(%s) -> [OPC_E_UNKNOWNITEMID] The item is no longer available in the server address space."<<item->name ;
				break;
			}
		}
	}

	if( pErrors )
		CoTaskMemFree( pErrors );

	return hResult;
}

/**
 * 移除所有项目
 * @brief OPCClient::RemoveItems
 * @param _items
 * @return
 */
HRESULT OPCClient::RemoveItems(QStringList inames)
{
  int _iCount = inames.count();
  if( _iCount==0) return -1;
  OPCHANDLE _hpSvr[_iCount];
  for(int i=0;i<_iCount ;i++){
      if(getItemByName(inames.value(i))){
          _hpSvr[i] = getItemByName(inames.at(i))->hServerHandle;
        }

    }

  hResult = _pIOPCItemMgt->RemoveItems(_iCount, _hpSvr, &pErrors);
  if(SUCCEEDED(hResult)){
      for(int i=0;i<_iCount ;i++){
          items->removeAll(getItemByName(inames.at(i)));;
        }
    }

  return hResult;
}
/**
 * @brief OPCClient::RemoveAllItems
 * @return
 */
HRESULT OPCClient::RemoveAllItems()
{
  int _iCount = items->count();
  OPCHANDLE _hpSvr[_iCount];
  qDebug()<<"3---begin delete-----------";
  for(int i=0;i<_iCount ;i++){
      qDebug() <<"    next one->"<< items->at(i)->hServerHandle << items->at(i)->name;
      _hpSvr[i] = items->at(i)->hServerHandle;
    }

  hResult = _pIOPCItemMgt->RemoveItems(_iCount, _hpSvr, &pErrors);
  qDebug() <<"4---end delete-----------" << hResult;
  if(SUCCEEDED(hResult)){
      while(items->count()!=0){
          items->removeAt(0);
        }
    }
  return hResult;
}
/**
 * 断开连接
 * @brief OPCClient::DisConnect
 */
void OPCClient::DisConnect()
{
  if(_pIOPCSyncIO){
      _pIOPCSyncIO->Release();
      _pIOPCSyncIO=0;
    }
  if (_pIConnectionPoint)
    {
      _pIConnectionPoint->Unadvise(m_Cookie);
      _pIConnectionPoint->Release();
      _pIConnectionPoint = 0;
    }
  if(_pIOPCItemMgt){
      _pIOPCItemMgt->Release();
      _pIOPCItemMgt = 0;
    }
  if(_pIOPCGroupStateMgt){
      _pIOPCGroupStateMgt->Release();
      _pIOPCGroupStateMgt = 0;
    }
  if(_pIOPCServer){
      _pIOPCServer->RemoveGroup(m_GroupHandle, TRUE);
      _pIOPCServer->Release();
      _pIOPCServer=0;
    }
  if(m_OPCDataCallback){
      m_OPCDataCallback->Release();
      m_OPCDataCallback = 0;
    }
  if(m_COPCTransaction){
      m_COPCTransaction->deleteLater();
      m_COPCTransaction = 0;
    }

  m_GroupHandle = 0;
  m_ServerHandle = 0;
  CoUninitialize();
  delete items;
}

void OPCClient::dataCallback()
{


  //  // 建立异步回调
  //  CComObject<COPCDataCallback>* pCOPCDataCallback;
  //  // 回调对象的指针
  //  // 通过 ATL 模板创建回调对象的实例
  //  CComObject<COPCDataCallback>::CreateInstance(&pCOPCD ataCallback);
  //  // 查询 IUnknown 接口
  //  LPUNKNOWN pCbUnk;
  //  pCbUnk = pCOPCDataCallback->GetUnknown();
  //  // 建立一个服务器的连接点与客户程序接收器之间的连接
  //  HRESULT hRes = AtlAdvise( m_IOPCGroupStateMgt, // [in]  //连接点的 IUnknown 接口
  //                            pCbUnk, // [in] 回调对象的 IUnknown 接口
  //                            IID_IOPCDataCallback,// [in] 连接点 ID
  //                            &m_dwAdvise    // [out] 唯一的标识符
  //                            );
  //    if (hRes != S_OK)    {
  //        AfxMessageBox("Advise 失败!");
  //      }
}

/**
 * 连接状态
 * @brief OPCClient::isConnected
 * @return
 */
bool OPCClient::isConnected()
{
  return ( _pIOPCServer && _pIOPCGroupStateMgt );
}

void OPCClient::ClearOPCITEMDEF( OPCITEMDEF *idef, int count )
{
  if( idef )
    for(int i=0;i<count;i++)
      {
        if( idef[i].szItemID != NULL )
          SysFreeString( idef[i].szItemID );
        if( idef[i].szAccessPath != NULL )
          SysFreeString( idef[i].szAccessPath );
      }
}
/**
 * 获取ItemDef by name
 * @brief OPCClient::getItemByName
 * @param s
 * @return
 */
ItemDef* OPCClient::getItemByName(QString s)
{
  int c = items->count();

  for(int i=0; i<c; i++) {
      if(items->at(i)->name == s){
          return items->at(i);
        }
    }
  return 0;
}
/**
 * 获取ItemDef by Handle
 * @brief OPCClient::getItemByHandle
 * @param s
 * @return
 */
ItemDef* OPCClient::getItemByHandle(DWORD h)
{
  int c = items->count();

  for(int i=0; i<c; i++) {
      if(items->at(i)->hClientHandle == h){
          return items->at(i);
        }
    }
  return 0;
}
/**
 * 析构函数
 * @brief OPCClient::~OPCClient
 */
OPCClient::~OPCClient()
{
  DisConnect();
}
//////////////////////////////////////////////////

/**
 * QString 转换为 const wchar*
 * @brief OPCClient::qstr2Wstr
 * @param s
 * @return
 */
WString OPCClient::qstr2Wstr(QString s)
{
  return reinterpret_cast<const wchar_t*>(s.utf16());
  //wchar_t* tempWide = const_cast< wchar_t* >( tempConstWide );
}
/**
 * 输出项目值字符串
 * @brief OPCClient::VariantString
 * @param pValue
 * @return
 */
QString OPCClient::VariantString(VARIANT &pValue)
{
  QString valueString;
  //qDebug()<< QString(" Format type:%1 ").arg(pValue.vt);
  switch(pValue.vt)
    {
    case VT_I1:
    case VT_UI1:		// BYTE
      {
        int i = pValue.iVal;
        valueString= QString::number(i);
        break;
      }
    case VT_I2:			// SHORT
      valueString= QString::number(pValue.iVal);
      break;
    case VT_UI2:		// UNSIGNED SHORT
      valueString= QString::number(pValue.uiVal);

      break;
    case VT_I4:			// LONG
      valueString= QString::number(pValue.lVal);
      break;
    case VT_UI4:		// UNSIGNED LONG
      valueString= QString::number(pValue.ulVal);
      break;
    case VT_INT:		// INTEGER
      valueString= QString::number(pValue.intVal);
      break;
    case VT_UINT:		// UNSIGNED INTEGER
      valueString= QString::number(pValue.uintVal);

      break;
    case VT_R4:			// FLOAT
      //sprintf	(buf, "%5.2f ", pValue.fltVal );
      valueString= QString::number(pValue.fltVal);
      break;
    case VT_R8:			// DOUBLE
      //sprintf	(buf, "%9.4f ", pValue.dblVal );
      valueString= QString::number(pValue.dblVal);
      break;
    case VT_BSTR:		//BSTR
      {
        //sprintf	(buf, "%ls ", pValue.bstrVal );
        BSTR bstr_str = pValue.bstrVal;
        QString q_str((QChar*)bstr_str, wcslen(bstr_str));
        valueString = q_str;

        break;
      }
    case VT_BOOL:
      {
        if (pValue.boolVal)
          valueString = "TRUE";
        else
          valueString = "FALSE";
        break;
      }
    case VT_DATE:
      {
        QDateTime dt;
        //qDebug()<< pValue.date;
        dt.fromMSecsSinceEpoch(pValue.date);
        valueString =  dt.toString("yyyy-MM-dd hh:mm:ss");
        break;
      }
    default:
      valueString = QString(" unknown type:%1 ").arg(pValue.vt);
      break;
    }
  return valueString;
}
/**
 * 输出项目值质量字符串
 * @brief OPCClient::VariantString
 * @param pValue
 * @return
 */
QString OPCClient::QualityString(UINT qnr)
{
  QString result;
  switch(qnr)   {
    case OPC_QUALITY_BAD:
      result = "BAD";
      break;
    case OPC_QUALITY_UNCERTAIN:
      result= "UNCERTAIN";
      break;
    case OPC_QUALITY_GOOD:
      result = "GOOD";
      break;
    case OPC_QUALITY_NOT_CONNECTED:
      result = "NOT_CONNECTED";
      break;
    case OPC_QUALITY_DEVICE_FAILURE:
      result = "DEVICE_FAILURE";
      break;
    case OPC_QUALITY_SENSOR_FAILURE:
      result = "SENSOR_FAILURE";
      break;
    case OPC_QUALITY_LAST_KNOWN:
      result = "LAST_KNOWN";
      break;
    case OPC_QUALITY_COMM_FAILURE:
      result = "COMM_FAILURE";
      break;
    case OPC_QUALITY_OUT_OF_SERVICE:
      result = "OUT_OF_SERVICE";
      break;
    case OPC_QUALITY_LAST_USABLE:
      result = "LAST_USABLE";
      break;
    case OPC_QUALITY_SENSOR_CAL:
      result = "SENSOR_CAL";
      break;
    case OPC_QUALITY_EGU_EXCEEDED:
      result = "EGU_EXCEEDED";
      break; case OPC_QUALITY_SUB_NORMAL:
      result = "SUB_NORMAL";
      break;
    case OPC_QUALITY_LOCAL_OVERRIDE:
      result = "LOCAL_OVERRIDE";
      break;
    default:
      result = "UNKNOWN ERROR";
    }
  return result;
}
/**
 * 输出值类型字符串
 * @brief OPCClient::ValueTypeString
 * @param v
 * @return
 */
QString OPCClient::ValueTypeString(const VARTYPE& v)
{
  QString str;

  switch(v) {
    case VT_BSTR:
      str = QString("VT_BSTR");
      break;
    case VT_I1:
      str = QString("VT_I1");
      break;
    case VT_I2:
      str = QString("VT_I2");
      break;
    case VT_I4:
      str = QString("VT_I4");
      break;
    case VT_I8:
      str = QString("VT_I8");
      break;
    case VT_R4:
      str = QString("VT_R4");
      break;
    case VT_R8:
      str = QString("VT_R8");
      break;
    case VT_UI1:
      str = QString("VT_UI1");
      break;
    case VT_UI2:
      str = QString("VT_UI2");
      break;
    case VT_UI4:
      str = QString("VT_UI4");
      break;
    case VT_UI8:
      str = QString("VT_UI8");
      break;
    case VT_INT:
      str = QString("VT_INT");
      break;
    case VT_UINT:
      str = QString("VT_UINT");
      break;
    case VT_BOOL:
      str = QString("VT_BOOL");
      break;
    case VT_DATE:
      str = QString("VT_DATE");
      break;
    default:
      str = QString("unknown");
      break;
    };
  return str;
}
/**
 * 赋值
 * @brief OPCClient::StringVariant
 * @param item
 * @param v
 * @return
 */
void OPCClient::StringVariant(ItemDef* item, QString v)
{
  switch (item->value.vt)
    {
    case VT_UI1:
    case VT_I1:
        //cbRead = sizeof(BYTE);
        break;
    case VT_I2:
    case VT_UI2:
    case VT_BOOL:
        item->value.iVal = v.toShort();
        break;
    case VT_I4:
    case VT_UI4:
    case VT_INT:
    case VT_UINT:
    case VT_ERROR:
      item->value.lVal = v.toLong();
      break;
    case VT_R4:
      item->value.fltVal = v.toFloat();
      break;
    case VT_I8:
    case VT_UI8:
      item->value.llVal = v.toLongLong();
        break;
    case VT_R8:
    case VT_CY:
    case VT_DATE:
      item->value.dblVal = v.toDouble();
        break;
    case VT_BSTR:
      {
        //Value.bstrVal =  SysAllocString(v.utf16());
        //Value.bstrVal = qstringToBstr(v);
        wchar_t *pW = new wchar_t[v.size()+1];
        v.toWCharArray(pW);
        //dialog.m_valueValue.bstrVal = SysAllocString(dialog.m_value.toStdWString().c_str());
        break;
      }
    default:
      break;
    }

  return;
}
/**
 * 输出权限字符串
 * @brief OPCClient::AccessTypeString
 * @param accessIdent
 * @return
 */
QString OPCClient::AccessTypeString(qint16 accessIdent)
{
  QString s;
  switch(accessIdent){
    case OPC_READABLE:
      s = "Read";
      break;
    case OPC_WRITEABLE:
      s = "Write";
      break;
    default:
      s = "Read/Write";
    }
  return s;
}
QString OPCClient::TimeString(FILETIME t)
{
  return QString::number(t.dwLowDateTime);
}
