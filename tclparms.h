#ifndef TCL_PARMS_H
#define TCL_PARMS_H

#include <tcl.h>

/// interface to Tcl parameters like an array
class Parms
{
protected:
   Tcl_Interp     *interp_;
   Tcl_Obj *CONST *objv_;
   unsigned        objc_;

public:
   Parms(Tcl_Interp *interp, Tcl_Obj *CONST* objv, unsigned objc):
      interp_(interp), objv_(objv), objc_(objc) {}

   Tcl_Interp* getInterp(void) { return interp_; }

   unsigned getNumArgs(void) const { return objc_-1; }

   Tcl_WideInt operator[](unsigned i)
   {
      if( i+1 >= objc_ ) return -1;

      Tcl_WideInt ret;
      Tcl_GetWideIntFromObj(interp_, objv_[i+1], &ret);
      return ret;
   }

	const char* getStringParm(unsigned i)
   {
      if( i+1 >= objc_ ) return NULL;
      return Tcl_GetStringFromObj(objv_[i+1], NULL);
   }
};

#endif

