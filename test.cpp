#include <sword/swmgr.h>
#include <sword/swmodule.h>
#include <iostream>

int main(int argc, char **argv)
{
	sword::SWMgr library;
	for(sword::ModMap::iterator i = library.Modules.begin(); i != library.Modules.end(); ++i)
	{
		sword::SWBuf name = i->first;
		sword::SWModule *mod = i->second;
		std::cout << name << ':' << ' ' << mod->Type() << std::endl;
	}

	return 0;
}

