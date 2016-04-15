#include "copcdatacallback.h"
#include <QDateTime>
COPCDataCallback::COPCDataCallback(COPCTransaction* _cOPCTransaction):
  _nRefCount(0),
  m_COPCTransaction(_cOPCTransaction)
{

}

COPCDataCallback::~COPCDataCallback()
{

}

HRESULT STDMETHODCALLTYPE COPCDataCallback::OnDataChange(
	/* [in] */ DWORD dwTransid,
	/* [in] */ OPCHANDLE hGroup,
	/* [in] */ HRESULT hrMasterquality,
	/* [in] */ HRESULT hrMastererror,
	/* [in] */ DWORD dwCount,
	/* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
	/* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
	/* [size_is][in] */ WORD __RPC_FAR *pwQualities,
	/* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
	/* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
{
//  qDebug()<< "---------------OnDataChange---------------";
//  for (DWORD i = 0; i < dwCount; i++) {
//      qDebug()<<"---from callback:"
//          <<hGroup
//         <<dwCount
//         <<phClientItems[i]
//         <<VariantString(pvValues[i])
//         <<QualityString(pwQualities[i])
//         <<QString::number(pftTimeStamps[i].dwLowDateTime)
//         <<pErrors[i];
//    }
  if(m_COPCTransaction){
      m_COPCTransaction->OnDataChange(dwTransid, hGroup, hrMasterquality, hrMastererror, dwCount, phClientItems, pvValues, pwQualities, pftTimeStamps, pErrors);
    }

}

HRESULT STDMETHODCALLTYPE COPCDataCallback::OnReadComplete(
	/* [in] */ DWORD /*dwTransid*/,
	/* [in] */ OPCHANDLE /*hGroup*/,
	/* [in] */ HRESULT /*hrMasterquality*/,
	/* [in] */ HRESULT /*hrMastererror*/,
	/* [in] */ DWORD dwCount,
	/* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
	/* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
	/* [size_is][in] */ WORD __RPC_FAR *pwQualities,
	/* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
	/* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
{
}

HRESULT STDMETHODCALLTYPE COPCDataCallback::OnWriteComplete(
	/* [in] */ DWORD /*dwTransid*/,
	/* [in] */ OPCHANDLE /*hGroup*/,
	/* [in] */ HRESULT /*hrMastererr*/,
	/* [in] */ DWORD /*dwCount*/,
	/* [size_is][in] */ OPCHANDLE __RPC_FAR * /*pClienthandles*/,
	/* [size_is][in] */ HRESULT __RPC_FAR * /*pErrors*/)
{

}

HRESULT STDMETHODCALLTYPE COPCDataCallback::OnCancelComplete(
	/* [in] */ DWORD /*dwTransid*/,
	/* [in] */ OPCHANDLE /*hGroup*/)
{

}

STDMETHODIMP COPCDataCallback::QueryInterface( REFIID iid, LPVOID* ppInterface)
{
	if ( ppInterface == NULL)
		return E_INVALIDARG;
	if ( iid == IID_IUnknown )
		*ppInterface = (IUnknown*)this;
	else if (iid == IID_IOPCDataCallback)
		*ppInterface = (IOPCDataCallback*) this;
	else
	{
		*ppInterface = NULL;
		return E_NOINTERFACE;
	}
	AddRef();
	return S_OK;
}

STDMETHODIMP_(ULONG) COPCDataCallback::AddRef()
{
	return ++_nRefCount;
}

STDMETHODIMP_(ULONG) COPCDataCallback::Release()
{
	--_nRefCount;
	if ( _nRefCount == 0)
		delete this;
	return _nRefCount;
}
/**
 * 输出项目值字符串
 * @brief OPCClient::VariantString
 * @param pValue
 * @return
 */
QString COPCDataCallback::VariantString(VARIANT &pValue)
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
QString COPCDataCallback::QualityString(UINT qnr)
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
