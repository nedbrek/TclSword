#include "tclparms.h"

static
int mainCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	return TCL_OK;
}

extern "C" int Tclsword_Init(Tcl_Interp *interp)
{
	Tcl_CreateObjCommand(interp, "sword", mainCmd, NULL, NULL);
	return TCL_OK;
}

