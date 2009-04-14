/* Modplug XMMS Plugin
 * Authors: Kenton Varda <temporal@gauge3d.org>
 *
 * This source code is public domain.
 */

//open()
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>

#include "arch_gzip.h"

// Find gunzip on the system
bool arch_Gzip::HasExternalProgram()
{
	struct stat gzipStats;
	int lGzipNotExists = stat("/usr/bin/gunzip", &gzipStats);
	if (lGzipNotExists) {
		return(false);
	} else {
		externalProgramName = "/usr/bin/gunzip";
	}
	return(true);
}
	
arch_Gzip::arch_Gzip(const string& aFileName)
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

	// file exists.       
	string lCommand = externalProgramName + " -l \"" + aFileName + '\"';   //get info
	FILE *f = popen(lCommand.c_str(), "r");
	
	if (f <= 0)
	{
		mSize = 0;
		return;
	}
	
	char line[81];
	fgets(line, 80, f);   // ignore a line.
	fscanf(f, "%u", &mSize); // ignore first number.
	fscanf(f, "%u", &mSize); // keep second number.
	
	pclose(f);
	
	mMap = new char[mSize];
	if(mMap == NULL)
	{
		mSize = 0;
		return;
	}
	
	lCommand = externalProgramName + " -c \"" + aFileName + '\"';  //decompress to stdout
        f = popen(lCommand.c_str(), "r");
	
	if (f <= 0)
	{
		mSize = 0;
		return;
	}

	fread((char *)mMap, sizeof(char), mSize, f);

	pclose(f);
	
}

arch_Gzip::~arch_Gzip()
{
	if(mSize != 0)
		delete [] (char*)mMap;
}

bool arch_Gzip::ContainsMod(const string& aFileName)
{
	if (externalProgramName.empty() && !HasExternalProgram()) {
		return false;
	}

	string lName;
	int lFileDesc = open(aFileName.c_str(), O_RDONLY);
	uint32 num;
	float fnum;

	if(lFileDesc == -1)
		return false;
	
	close(lFileDesc);

	// file exists.       
	string lCommand = externalProgramName + " -l \"" + aFileName + '\"';   //get info
	FILE *f = popen(lCommand.c_str(),"r");
	
	if (f <= 0) {
		pclose(f);
		return false;
	}
	
	char line[300];
	fgets(line, 80, f);   // ignore a line.
	fscanf(f, "%i", &num); // ignore first number
	fscanf(f, "%i", &num); // ignore second number
	fscanf(f, "%f%%", &fnum); // ignore ratio
	fgets(line, 300, f);   // read in correct line safely.
	if (strlen(line) > 1)
		line[strlen(line)-1] = 0;
	lName = line;
	
	pclose(f);
	
	return IsOurFile(lName);
}
