#ifndef ROOT_THaRawHit
#define ROOT_THaRawHit

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// THaRawHit - for Hall C style hit lists                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include "TObject.h"

class THaRawHit : public TObject {

public:
 THaRawHit(Int_t plane, Int_t counter) :
  fPlane(plane), fCounter(counter) {}
 THaRawHit( const THaRawHit& rhs ) : TObject(rhs) {}
  THaRawHit& operator=( const THaRawHit& rhs )
  { TObject::operator=(rhs); return *this; }

  virtual ~THaRawHit();

  virtual void Clear( Option_t* opt="" )=0;

  //  virtual Bool_t  operator==( const THaRawHit& ) = 0;
  //  virtual Bool_t  operator!=( const THaRawHit& ) = 0;

  virtual void SetData(Int_t signal, Int_t data) {};
  virtual Int_t GetData(Int_t signal) {return 0;};

  // Derived objects must be sortable and supply Compare method
  virtual Bool_t  IsSortable () const {return kFALSE; }
  virtual Int_t   Compare(const TObject* obj) const {return 0;}

  Int_t fPlane;
  Int_t fCounter;

protected:

  ClassDef(THaRawHit,0)      // Track ID abstract base class
};

#endif
