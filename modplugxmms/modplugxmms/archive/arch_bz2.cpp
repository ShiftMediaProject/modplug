/* Modplug XMMS Plugin
 * Authors: Kenton Varda <temporal@gauge3d.org>
 *          Colin DeVilbiss <crdevilb@mtu.edu>
 *
 * This source code is public domain.
 */

// BZ2 support added by Colin DeVilbiss <crdevilb@mtu.edu>

//open()
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

#include "arch_bz2.h"

bool arch_Bzip2::HasExternalProgram()
{
	struct stat bz2Stats;
	int lBz2NotExists = stat("/usr/bin/bzcat", &bz2Stats);
	if (lBz2NotExists) {
		return(false);
	} else {
		externalProgramName = "/usr/bin/bzcat";
	}
	return(true);
}
 	
arch_Bzip2::arch_Bzip2(const string& aFileName)
{
	if (!HasExternalProgram()) {
		mSize = 0;
		return;
	}

	//check if file exists
	int lFileDesc = open(aFileName.c_str(), O_RDONLY);
	
	if(lFileDesc == -1)
	{
		mSize = 0;
		return;
	}
	
	close(lFileDesc);
	
	//ok, continue
	string lCommand = externalProgramName + " \'" + aFileName + "\' | wc -c";   //get info
	FILE *f = popen(lCommand.c_str(), "r");

	if (f <= 0)
	{
		mSize = 0;
		return;
	}
	
	fscanf(f, "%u", &mSize); // this is the size.
	
	pclose(f);
	
	mMap = new char[mSize];
	if(mMap == NULL)
	{
		mSize = 0;
		return;
	}
	
	lCommand = externalProgramName + " \'" + aFileName + '\'';  //decompress to stdout
	popen(lCommand.c_str(), "r");

	if (f <= 0)
	{
		mSize = 0;
		return;
	}

	fread((char *)mMap, sizeof(char), mSize, f);

	pclose(f);
}

arch_Bzip2::~arch_Bzip2()
{
	if(mSize != 0)
		delete [] (char*)mMap;
}

bool arch_Bzip2::ContainsMod(const string& aFileName)
{
	// BZIP2 does not require external program to see if mod exists
	string lName;
	int lFileDesc = open(aFileName.c_str(), O_RDONLY);
 	if(lFileDesc == -1)
		return false;
	
	close(lFileDesc);
	
	lName = aFileName.substr(0, aFileName.find_last_of('.'));
	return IsOurFile(lName);
}
