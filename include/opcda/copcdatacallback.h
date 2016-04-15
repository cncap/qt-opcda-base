#ifndef COPCDATACALLBACK
#define COPCDATACALLBACK
#include "Opcda.h"
#include "copctransaction.h"
#include <QDebug>
#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
  typedef LONG HRESULT;
#endif
#define STDMETHODIMP HRESULT WINAPI
#define STDMETHODIMP_(type) type WINAPI
/**
 * 项目值变化的异步回调函数
 * @brief The COPCDataCallback class
 */
class COPCDataCallback:public IOPCDataCallback
{
public:
   COPCDataCallback(COPCTransaction* _cOPCTransaction=0);
   ~COPCDataCallback();
   QString VariantString(VARIANT &pValue);
   QString QualityString(UINT qnr);
   COPCTransaction* m_COPCTransaction;
   /**
   * Functions associated with IUNKNOWN
   */
   STDMETHODIMP QueryInterface( REFIID iid, LPVOID* ppInterface);
   STDMETHODIMP_(ULONG) AddRef();
   STDMETHODIMP_(ULONG) Release();
   HRESULT STDMETHODCALLTYPE OnDataChange(
       /* [in] */ DWORD dwTransid,
       /* [in] */ OPCHANDLE hGroup,
       /* [in] */ HRESULT hrMasterquality,
       /* [in] */ HRESULT hrMastererror,
       /* [in] */ DWORD dwCount,
       /* [size_is][in] */ OPCHANDLE *phClientItems,
       /* [size_is][in] */ VARIANT *pvValues,
       /* [size_is][in] */ WORD *pwQualities,
       /* [size_is][in] */ FILETIME *pftTimeStamps,
       /* [size_is][in] */ HRESULT *pErrors) ;

   HRESULT STDMETHODCALLTYPE OnReadComplete(
       /* [in] */ DWORD dwTransid,
       /* [in] */ OPCHANDLE hGroup,
       /* [in] */ HRESULT hrMasterquality,
       /* [in] */ HRESULT hrMastererror,
       /* [in] */ DWORD dwCount,
       /* [size_is][in] */ OPCHANDLE *phClientItems,
       /* [size_is][in] */ VARIANT *pvValues,
       /* [size_is][in] */ WORD *pwQualities,
       /* [size_is][in] */ FILETIME *pftTimeStamps,
       /* [size_is][in] */ HRESULT *pErrors) ;

   HRESULT STDMETHODCALLTYPE OnWriteComplete(
       /* [in] */ DWORD dwTransid,
       /* [in] */ OPCHANDLE hGroup,
       /* [in] */ HRESULT hrMastererr,
       /* [in] */ DWORD dwCount,
       /* [size_is][in] */ OPCHANDLE *pClienthandles,
       /* [size_is][in] */ HRESULT *pErrors) ;

   HRESULT STDMETHODCALLTYPE OnCancelComplete(
       /* [in] */ DWORD dwTransid,
       /* [in] */ OPCHANDLE hGroup) ;

   DWORD  _nRefCount;
};
#endif // COPCDATACALLBACK

