#ifndef ROOT_THaAnalysisObject
#define ROOT_THaAnalysisObject

//////////////////////////////////////////////////////////////////////////
//
// THaAnalysisObject
//
//////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "THaGlobals.h"
#include "TDatime.h"
#include "VarDef.h"

#include <vector>
#include <cstdio>

class THaEvData; //needed by derived classes
class TList;
class TVector3;
class THaRunBase;
class THaOutput;

class THaAnalysisObject : public TNamed {
  
public:
  enum EStatus { kOK, kNotinit, kInitError, kFileError };
  enum EType   { kVarDef, kRVarDef };
  enum EMode   { kDefine, kDelete };

  static const Double_t kBig; // = 1.e38; // default junk value
  
  THaAnalysisObject();  // only for ROOT I/O
  
  virtual ~THaAnalysisObject();
  
  virtual Int_t        Begin( THaRunBase* r=0 );
  virtual void         Clear( Option_t* opt="" ) {}
  virtual Int_t        End( THaRunBase* r=0 );
  virtual const char*  GetDBFileName() const;
          const char*  GetConfig() const         { return fConfig.Data(); }
          Int_t        GetDebug() const          { return fDebug; }
          const char*  GetPrefix() const         { return fPrefix; }
          EStatus      Init();
  virtual EStatus      Init( const TDatime& run_time );
          Bool_t       IsInit() const            { return IsOK(); }
          Bool_t       IsOK() const              { return (fStatus == kOK); }

	  TDatime      GetInitDate() const       { return fInitDate; }

          void         SetConfig( const char* label );
  virtual void         SetDebug( Int_t level );
  virtual void         SetName( const char* name );
  virtual void         SetNameTitle( const char* name, const char* title );
          EStatus      Status() const            { return fStatus; }

  virtual Int_t        InitOutput( THaOutput * );
          Bool_t       IsOKOut()                 { return fOKOut; }

  // Static function to provide easy access to database files
  // from CINT scripts etc.
  static  FILE*   OpenFile( const char* name, const TDatime& date,
			    const char* here = "OpenFile()",
			    const char* filemode = "r", 
			    const int debug_flag = 1);

  // Access functions for reading tag/value pairs from database files
  static  Int_t   LoadDBvalue( FILE* file, const TDatime& date, 
			       const char* tag, Double_t& value );
  static  Int_t   LoadDBvalue( FILE* file, const TDatime& date, 
			       const char* tag, std::string& text );
  static  Int_t   LoadDBvalue( FILE* file, const TDatime& date, 
			       const char* tag, TString& text );
  static  Int_t   LoadDB( FILE* file, const TDatime& date, 
			  const TagDef* tags, const char* prefix="" );
  static  Int_t   SeekDBdate( FILE* file, const TDatime& date,
			      bool end_on_tag = false );
  static  Int_t   SeekDBconfig( FILE* file, const char* tag,
				const char* label = "config",
				bool end_on_tag = false );

  // Geometry utility functions
  static  void    GeoToSph( Double_t  th_geo, Double_t  ph_geo,
			    Double_t& th_sph, Double_t& ph_sph );
  static  void    SphToGeo( Double_t  th_sph, Double_t  ph_sph,
			    Double_t& th_geo, Double_t& ph_geo );

  static  Bool_t  IntersectPlaneWithRay( const TVector3& xax,
					 const TVector3& yax,
					 const TVector3& org,
					 const TVector3& ray_start,
					 const TVector3& ray_vect,
					 Double_t& length,
					 TVector3& intersect );

  static Int_t    DefineVarsFromList( const void* list, 
				      EType type, EMode mode,
				      const char* var_prefix,
				      const TObject* obj, 
				      const char* prefix, 
				      const char* here );

protected:

  enum EProperties { kNeedsRunDB = BIT(0) };

  // General status variables
  char*           fPrefix;    // Name prefix for global variables
  EStatus         fStatus;    // Initialization status flag
  Int_t           fDebug;     // Debug level
  bool            fIsInit;    // Flag indicating that ReadDatabase called.
  bool            fIsSetup;   // Flag indicating that Setup called.
  TString         fConfig;    // Configuration to use from database
  UInt_t          fProperties;// Properties of this object (see EProperties)
  Bool_t          fOKOut;     // Flag indicating object-output prepared

  TDatime         fInitDate;  // Date passed to Init
  
  virtual Int_t        DefineVariables( EMode mode = kDefine );
          Int_t        DefineVarsFromList( const VarDef* list, 
					   EMode mode = kDefine,
					   const char* var_prefix="" ) const;
          Int_t        DefineVarsFromList( const RVarDef* list, 
					   EMode mode = kDefine,
					   const char* var_prefix="" ) const;
          Int_t        DefineVarsFromList( const void* list, 
					   EType type, EMode mode,
					   const char* var_prefix="" ) const;

  THaAnalysisObject* FindModule( const char* name, const char* classname );

  virtual const char*  Here( const char* ) const;
          void         MakePrefix( const char* basename );
  virtual void         MakePrefix() = 0;
  virtual FILE*        OpenFile( const TDatime& date );
  virtual FILE*        OpenRunDBFile( const TDatime& date );
  virtual Int_t        ReadDatabase( const TDatime& date );
  virtual Int_t        ReadRunDatabase( const TDatime& date );
  virtual Int_t        RemoveVariables();

  // Support function for reading database files
  static std::vector<std::string> 
    GetDBFileList( const char* name, const TDatime& date,
		   const char* here = "GetDBFileList()" );
  
  static char* ReadComment( FILE* fp, char* buf, const int len );

  // Only derived classes may construct
  THaAnalysisObject( const char* name, const char* description );

  static TList* fgModules;    // List of all currently existing Analysis Modules

private:
  // Support functions for reading database files
  static Int_t IsDBdate( const std::string& line, TDatime& date, 
			 bool warn=true );
  static Int_t IsDBtag ( const std::string& line, const char* tag, 
			 std::string& text );

  // Prevent default construction, copying, assignment
  THaAnalysisObject( const THaAnalysisObject& );
  THaAnalysisObject& operator=( const THaAnalysisObject& );

  ClassDef(THaAnalysisObject,1)   //ABC for a data analysis object
};

//____________ inline functions _______________________________________________

#endif
