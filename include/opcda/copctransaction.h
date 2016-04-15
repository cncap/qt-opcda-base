#ifndef COPCTRANSACTION_H
#define COPCTRANSACTION_H
#include "Opcda.h"
#include <QObject>
#include <QDebug>

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef LONG HRESULT;
#endif
#define STDMETHODIMP HRESULT WINAPI
#define STDMETHODIMP_(type) type WINAPI

class COPCTransaction:public QObject
{
  Q_OBJECT
signals:
  //HANDLE hGroup, HANDLE hItem, VARIANT *pVar, FILETIME timestamp, DWORD quality
  void sg_OnDataChange(OPCHANDLE,DWORD,OPCHANDLE *, VARIANT*, WORD*, FILETIME *,HRESULT*);
  //void sg_OnDataChange(OPCHANDLE hGroup,  DWORD dwCount, OPCHANDLE *phClientItems, VARIANT *pvValues, WORD *pwQualities, FILETIME *pftTimeStamps, HRESULT *pErrors);

public:
  COPCTransaction();
  virtual ~COPCTransaction(void);

  virtual HRESULT  OnDataChange(
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

  virtual HRESULT  OnReadComplete(
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

  virtual HRESULT  OnWriteComplete(
      /* [in] */ DWORD dwTransid,
      /* [in] */ OPCHANDLE hGroup,
      /* [in] */ HRESULT hrMastererr,
      /* [in] */ DWORD dwCount,
      /* [size_is][in] */ OPCHANDLE *pClienthandles,
      /* [size_is][in] */ HRESULT *pErrors) ;

  virtual HRESULT  OnCancelComplete(
      /* [in] */ DWORD dwTransid,
      /* [in] */ OPCHANDLE hGroup);
};
#endif // COPCTRANSACTION_H
