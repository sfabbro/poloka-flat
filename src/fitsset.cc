// -*- C++ -*- 
#include <iostream>
#include <fstream>

#include "fitsset.h"
#include "fileutils.h"
#include "fitsimage.h"
#include "fitstoad.h"
#include "frame.h"

using namespace std;

// ______________________________________________________________________
FitsSet::FitsSet(const string & ListName, 
		 const bool CheckFilter, const bool CheckCCD)
  : nx(0), ny(0), nxtot(0), nytot(0)
{
  if (!FileExists(ListName)) 
    {
      cout << "file : " << ListName << " does not exist " << endl;
      return;
    }
  
  ifstream list(ListName.c_str());  
  while (!list.eof())
    {
      string fileName;
      list >> fileName;
      append(fileName, CheckFilter, CheckCCD);
    }
  list.close();

  if (ny==0 || nx==0)
    cout << "[FitsSet::append] No Image opened " << ListName << " --> empty set " << endl;
}

// ______________________________________________________________________
FitsSet::FitsSet(const vector<string> & fnames, 
		 const bool CheckFilter, const bool CheckCCD)
  : nx(0), ny(0), nxtot(0), nytot(0)
{
  vector<string>::const_iterator I;
  for(I=fnames.begin(); I!=fnames.end(); I++)
    append(*I, CheckFilter, CheckCCD);

  if (ny==0 || nx==0)
    cout << "[FitsSet::append] No Image opened  --> empty set " << endl;
}

// ______________________________________________________________________
bool FitsSet::append(const string & fname, const bool CheckFilter, const bool CheckCCD)
{
  string firstFileName;
  string filtRef;
  string ccdRef;
  string objRef;
  
  if (fname.length() == 0) return false;
  FitsHeader loop(fname);
  if (!loop.IsValid())
    {
      cout << "[FitsSet::append] cannot open " << fname << endl;
      return false;
    }
  Frame Illu = TotalIlluRegion(loop);
  int nx_cur = int(Illu.Nx());
  int ny_cur = int(Illu.Ny());
  int nxtot_cur   = loop.KeyVal("NAXIS1");
  int nytot_cur   = loop.KeyVal("NAXIS2");
  string filt_cur = loop.KeyVal("TOADFILT");
  string ccd_cur  = loop.KeyVal("TOADCHIP");
  string obj_cur  = loop.KeyVal("TOADTYPE");
  
  if ((nx==0) && (ny==0) && (filtRef=="") && (ccdRef=="")) 
    {
      nx = nx_cur;
      ny = ny_cur;
      nxtot = nxtot_cur;
      nytot = nytot_cur;
      filtRef = filt_cur;
      ccdRef = ccd_cur;
      objRef = obj_cur;
      firstFileName = fname;
    }
  if((nx_cur != nx)||(ny_cur != ny))
    {
      cout << "[FitsSet::append] Image " << fname 
	   << " does not fit in the set: nx=" << nx_cur << "!=" << nx 
	   << " ny=" << ny << "!=" << ny 
	   << endl;
      return false;
    }
  
  if(obj_cur != objRef)
    {
      cout << "[FitsSet::append] Image " << fname 
	   << " does not fit in the set: obj=" << obj_cur << "!=" << objRef
	   << endl;
      return false;
    }
  
  if(CheckFilter && (filt_cur != filtRef))
    {
      cout << "[FitsSet::append] Image " << fname 
	   << " does not fit in the set: filt=" << filt_cur << "!=" << filtRef 
	   << endl;
      return false;
    }
  
  if(CheckCCD && (ccd_cur != ccdRef))
    {
      cout << "[FitsSet::append] Image " << fname 
	   << " does not fit in the set: CCD="<< ccd_cur << "!=" << ccdRef 
	   << endl; 
      return false;
    }
  
  if ((nxtot_cur != nxtot)||(nytot_cur != nytot))
    {
      cout << "[FitsSet::append] Image " << fname 
	   << " does not fit in the set (overscan region mismatch)" 
	   << endl;
      return false;
    }
  
  fitsNames.push_back(fname);
  
  // could have something more clever depending on what the read line refers to (actual file or DbImage) 
  all_names += fname;  
}
