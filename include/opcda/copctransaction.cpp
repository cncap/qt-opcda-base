#include "copctransaction.h"

COPCTransaction::COPCTransaction()
{

}
COPCTransaction::~COPCTransaction()
{

}
HRESULT  COPCTransaction::OnDataChange(
	/* [in] */ DWORD /*dwTransid*/,
	/* [in] */ DWORD hGroup,
	/* [in] */ HRESULT /*hrMasterquality*/,
	/* [in] */ HRESULT /*hrMastererror*/,
	/* [in] */ DWORD dwCount,
	/* [size_is][in] */ DWORD __RPC_FAR *phClientItems,
	/* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
	/* [size_is][in] */ WORD __RPC_FAR *pwQualities,
	/* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
	/* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
{
//  for (DWORD i = 0; i < dwCount; i++) {
//      qDebug()<<"---from trans:"
//          <<hGroup
//         <<dwCount
//         <<phClientItems[i]
//         <<QString::number(pftTimeStamps[i].dwLowDateTime)
//         <<pErrors[i];
//    }
  emit sg_OnDataChange(hGroup,  dwCount, phClientItems, pvValues, pwQualities, pftTimeStamps, pErrors);

}

HRESULT  COPCTransaction::OnReadComplete(
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

HRESULT  COPCTransaction::OnWriteComplete(
	/* [in] */ DWORD /*dwTransid*/,
	/* [in] */ OPCHANDLE /*hGroup*/,
	/* [in] */ HRESULT /*hrMastererr*/,
	/* [in] */ DWORD /*dwCount*/,
	/* [size_is][in] */ OPCHANDLE __RPC_FAR * /*pClienthandles*/,
	/* [size_is][in] */ HRESULT __RPC_FAR * /*pErrors*/)
{

}

HRESULT  COPCTransaction::OnCancelComplete(
	/* [in] */ DWORD /*dwTransid*/,
	/* [in] */ OPCHANDLE /*hGroup*/)
{

}
