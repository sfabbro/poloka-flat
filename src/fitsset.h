// -*- C++ -*- 
#ifndef FITSSET__H
#define FITSSET__H

#include <string>
#include <vector>

using namespace std;

/*! \file
   \brief sets of fits files with homogeneous sizes, filter, and chip id
   */

//! container for fits files that have same sizes, filter, and come from the same chip within a mosaic.
class FitsSet
{
private  :
  vector<string> fitsNames;
  int nx,ny,nxtot, nytot;
  string all_names;
  
public :
  //! constructor from a file that contains file names (one per line). 
  /*! Those can be actual filenames or DbImage names. */
  FitsSet(const string &FileName, const bool CheckFilter = true, const bool CheckCCD = true);
  
  //! constructor from a vector of filenames 
  FitsSet(const std::vector<string> & fnames, 
	  const bool CheckFilter=true, const bool CheckCCD=true);
  
  //! return filename of the i-th image
  string operator [] (const unsigned int i) const { return fitsNames[i];}
  
  //! Number of file in the set.
  int size() const { return fitsNames.size();}
  
  //! returns a string containing all file names.
  string AllNames() const { return all_names;}
  
  //! x size of images (excluding overscan if any).
  int Nx() const { return nx;}
  
  //! y size of images.
  int Ny() const { return ny;}
  
  //! total x size of images (including overscan)
  int NxTot() const { return nxtot;}
  
  //! same for y.
  int NyTot() const { return nytot;}
  
  //! append another file name - return false if error 
  bool  append(const string & fname, const bool CheckFilter, const bool CheckCCD);
};

#endif /* FITSSET__H */
