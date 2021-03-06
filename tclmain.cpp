#include "tclparms.h"
#include <sword/swmgr.h>
#include <sword/swmodule.h>
#include <sword/versekey.h>

struct SwordData
{
	sword::SWMgr library;

public: // methods
	sword::SWModule* findMod(const char *modName)
	{
		sword::ModMap::const_iterator i = library.Modules.find(modName);
		if (i == library.Modules.end())
			return NULL;

		return i->second;
	}
};

static
int modulesCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	sword::SWMgr &library = reinterpret_cast<SwordData*>(cdata)->library;

	Tcl_Obj *ret = Tcl_NewListObj(0, NULL);

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

static
void addERef(const sword::ListKey &l, Tcl_Interp *interp, Tcl_Obj *d)
{
	// new dictionary for ERef
	Tcl_Obj *eref = Tcl_NewDictObj();

	// convert the current verse reference to an expanded reference
	sword::VerseKey vKey(l);

	// pull components of the expanded reference
	const char *tmp;
	const char *tmpKey;
	int tmpInt;

	tmpKey = "Book";
	tmp = vKey.getBookAbbrev();
	Tcl_DictObjPut(interp, eref, Tcl_NewStringObj(tmpKey, strlen(tmpKey)), Tcl_NewStringObj(tmp, strlen(tmp)));

	tmpKey = "Chapter";
	tmpInt = vKey.getChapter();
	Tcl_DictObjPut(interp, eref, Tcl_NewStringObj(tmpKey, strlen(tmpKey)), Tcl_NewIntObj(tmpInt));

	tmpKey = "Verse";
	tmpInt = vKey.getVerse();
	Tcl_DictObjPut(interp, eref, Tcl_NewStringObj(tmpKey, strlen(tmpKey)), Tcl_NewIntObj(tmpInt));

	// add the ERef to the output dictionary
	tmpKey = "ERef";
	Tcl_DictObjPut(interp, d, Tcl_NewStringObj(tmpKey, strlen(tmpKey)), eref);
}

static
int getVerseCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	if (objc < 3)
	{
		Tcl_AddErrorInfo(interp, "Usage: sword::get <module> <verse>");
		return TCL_ERROR;
	}

	sword::SWMgr &library = reinterpret_cast<SwordData*>(cdata)->library;
	Parms p(interp, objv, unsigned(objc));

	sword::ModMap::const_iterator modI = library.Modules.find(p.getStringParm(0));
	if (modI == library.Modules.end())
	{
		Tcl_AddErrorInfo(interp, "Module not found");
		return TCL_ERROR;
	}

	sword::SWModule *mod = modI->second;
	Tcl_Obj *ret = Tcl_NewListObj(0, NULL);

	const char *keyRef = "Ref";
	const char *keyTxt = "Text";
	sword::ListKey l = sword::VerseKey().ParseVerseList(p.getStringParm(1), 0, true);
	for(; !l.Error(); l++)
	{
		const char *valRef = l.getText();
		mod->setKey(valRef);
		const char *valTxt = mod->RenderText();

		Tcl_Obj *entry = Tcl_NewDictObj();
		Tcl_DictObjPut(interp, entry, Tcl_NewStringObj(keyRef, strlen(keyRef)),
		    Tcl_NewStringObj(valRef, strlen(valRef)));
		addERef(l, interp, entry);
		Tcl_DictObjPut(interp, entry, Tcl_NewStringObj(keyTxt, strlen(keyTxt)),
		    Tcl_NewStringObj(valTxt, strlen(valTxt)));

		Tcl_ListObjAppendElement(interp, ret, entry);
	}

	Tcl_SetObjResult(interp, ret);

	return TCL_OK;
}

static
int getDictCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	if (objc < 3)
	{
		Tcl_AddErrorInfo(interp, "Usage: sword::dict <module> <entry>");
		return TCL_ERROR;
	}

	sword::SWMgr &library = reinterpret_cast<SwordData*>(cdata)->library;
	Parms p(interp, objv, unsigned(objc));

	sword::ModMap::const_iterator modI = library.Modules.find(p.getStringParm(0));
	if (modI == library.Modules.end())
	{
		Tcl_AddErrorInfo(interp, "Module not found");
		return TCL_ERROR;
	}

	sword::SWModule *mod = modI->second;

	mod->setKey(p.getStringParm(1));

	const char *valTxt = mod->RenderText();
	Tcl_SetObjResult(interp, Tcl_NewStringObj(valTxt, strlen(valTxt)));

	return TCL_OK;
}

static
int setDictCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	if (objc < 3)
	{
		Tcl_AddErrorInfo(interp, "Usage: sword::dictWrite <module> <newText>");
		return TCL_ERROR;
	}

	SwordData *const data = reinterpret_cast<SwordData*>(cdata);
	Parms p(interp, objv, unsigned(objc));

	sword::SWModule *mod = data->findMod(p.getStringParm(0));
	if (!mod)
	{
		Tcl_AddErrorInfo(interp, "Module not found");
		return TCL_ERROR;
	}

	mod->setEntry(p.getStringParm(1));

	return TCL_OK;
}

extern "C" int Tclsword_Init(Tcl_Interp *interp)
{
	//Tcl_Namespace *ns = Tcl_CreateNamespace(interp, "sword", NULL, NULL);

	SwordData *data = new SwordData;

	Tcl_CreateObjCommand(interp, "sword::modules", modulesCmd, data, NULL);
	Tcl_CreateObjCommand(interp, "sword::get", getVerseCmd, data, NULL);
	Tcl_CreateObjCommand(interp, "sword::dict", getDictCmd, data, NULL);
	Tcl_CreateObjCommand(interp, "sword::dictWrite", setDictCmd, data, NULL);

	return TCL_OK;
}

