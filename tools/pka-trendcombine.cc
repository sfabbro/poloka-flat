#include <cstdio>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <poloka/fileutils.h>
#include <poloka/fitsimage.h>

#include <poloka/superflat.h>
#include <poloka/fitsset.h>

static void usage(const char *progname) {
  cerr << "Usage: " << progname << "[OPTION]...FITSLIST \n"
       << "Build a median combined image for detrending from a list of FITS images\n\n"
       << "    -b FITS : subtract the bias FITS frame before combining\n"
       << "    -o FITS : define output (default: master.fits)\n"
       << "    -B      : make a master bias frame\n"
       << "    -s FITS : make a master flat frame in 2 steps using a sky flat\n\n";
  exit(EXIT_FAILURE);
}


int main(int argc, char**argv) {

  if (argc < 2) usage (argv[0]);

  string fileList;
  string biasName;
  string skyName;
  string outName = "master.fits";
  bool masterBias = false;

  for (int i=1; i< argc; i++) {
    char *arg = argv[i];
    if (arg[0] != '-') 
      fileList = arg;
    else 
      switch (arg[1]) {
      case 'b': i++ ; biasName = argv[i]; break;
      case 'o': i++ ; outName = argv[i] ; break;
      case 's': i++ ; skyName = argv[i] ; break;
      case 'B': masterBias = true; break;
      default: argc = 1;
      }
  }

  if (fileList.empty()) usage(argv[0]);

  if (masterBias)  {
    FitsSet fitsFileSet(fileList, false);
    Image *MasterBias = MakeRawMedian(fitsFileSet);
    FitsHeader first(fitsFileSet[0]);
    FitsImage masterBiasFits(outName, first, *MasterBias);
    delete MasterBias;
    masterBiasFits.AddOrModKey("OBJECT","MasterBias","see history for more informations");
    masterBiasFits.AddCommentLine("masterbias done using " + fitsFileSet.AllNames());    
    return EXIT_SUCCESS;
  }


  FitsSet fitsFileSet(fileList);
  Image *meanBias = NULL;
  if (!biasName.empty())
    meanBias = new FitsImage(biasName);

  Image *skyFlat = NULL;
  if (!skyName.empty())
    skyFlat = new FitsImage(skyName);

  Image *Flat = MakeSuperFlat(fitsFileSet, meanBias, skyFlat);
  if (meanBias) delete meanBias;
  if (skyFlat) delete skyFlat;  
  if (!Flat) return EXIT_FAILURE;
  
  Image *DeadPixels = DeadPixelImageAndFlatSmoothing(*Flat, 0.6, 1.4);
  
  // write them (and release memory)
  string deadName = AddPrefix("dead_", outName);
  
  FitsHeader first(fitsFileSet[0]);
  FitsImage flatFits(outName, first, *Flat);
  delete Flat;
  FitsImage deadFits(deadName, first, *DeadPixels);
  deadFits.ModKey("BITPIX",8);
  delete DeadPixels;
  
  flatFits.AddOrModKey("OBJECT","MasterFlat","see history for more informations");
  deadFits.AddOrModKey("OBJECT","Dead Pix Mask","see history for more informations");
  flatFits.AddCommentLine("masterflat done using " + fitsFileSet.AllNames());
  deadFits.AddCommentLine("dead pixels mask deduced from " + string(outName));
  
  return EXIT_SUCCESS;
}
