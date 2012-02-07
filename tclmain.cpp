#include "tclparms.h"
#include <sword/swmgr.h>
#include <sword/swmodule.h>

static
int modulesCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	Tcl_Obj *ret = Tcl_NewListObj(0, NULL);

	sword::SWMgr library;
	const char *keyName = "Name";
	const char *keyType = "Type";
	for(sword::ModMap::const_iterator i = library.Modules.begin(); i != library.Modules.end(); ++i)
	{
		Tcl_Obj *entry = Tcl_NewDictObj();

		const char *valName = i->first.c_str();
		const char *valType = i->second->Type();
		Tcl_DictObjPut(interp, entry, Tcl_NewStringObj(keyName, strlen(keyName)),
		    Tcl_NewStringObj(valName, strlen(valName)));
		Tcl_DictObjPut(interp, entry, Tcl_NewStringObj(keyType, strlen(keyType)),
		    Tcl_NewStringObj(valType, strlen(valType)));

		Tcl_ListObjAppendElement(interp, ret, entry);
	}

	Tcl_SetObjResult(interp, ret);
	return TCL_OK;
}

extern "C" int Tclsword_Init(Tcl_Interp *interp)
{
	Tcl_Namespace *ns = Tcl_CreateNamespace(interp, "sword", NULL, NULL);
	Tcl_CreateObjCommand(interp, "sword::modules", modulesCmd, NULL, NULL);
	return TCL_OK;
}

