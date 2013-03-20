#include <iostream>
#include <string>

#include <poloka/fitsimage.h>
#include <poloka/fitstoad.h>
#include <poloka/vutils.h>

static int greatest_common_divider(int a, int b) {
  if (a<b) swap(a,b);
  do {
    int q = a/b;
    int r = a-b*q;
    if (r==0) return b;
    a = b;
    b = r;
  } while (b!= 1);
  return 1;
}

static void usage(const char *progname) {
  cerr << "Usage: " << progname << " NPIX FITS...\n"
       << "Compute pixel correlations coefficient of length NPIX for FITS images\n\n";
  exit(EXIT_FAILURE);
}  

int main(int argc, char **argv) {

  if (argc <3) usage(argv[0]);

  int l = atoi(argv[1]);

  bool ok = true;

  for (int a=2; a< argc; ++a) {

    string imagename = argv[a];

    if (l == 0) {
      cout << argv[0] << ": " << imagename << " rho 0\n";
      continue;
    }

    FitsImage image(imagename);
    if (!image.IsValid()) {
      cerr << argv[0] << ": invalid file "  << imagename << endl;
      ok = false;
      continue;
    }
    
    Pixel mean_tot, sigma_tot;
    image.SkyLevel(&mean_tot, &sigma_tot);
    Pixel cutoff = sigma_tot*4;
    
    int nx = image.Nx();
    int ny = image.Ny();
    long npix = nx*ny;
    long nval = 100000;
    int step = npix/nval;
    
    if (npix < nval) {
      step = 1;
      nval = npix;
    }

    while (greatest_common_divider(nx, step) != 1)
      step--;
    nval = (npix/step);

    vector<double> pixel(nval); // P(i)
    vector<double> pixelprod(nval*2); // P(i)*P(i+l)
    
    long count = 0;
    long count_prod = 0;
    
    int i = 0;
    int j = 0;

    while (true)  {
      float val = image(i,j);
      if (fabs(val-mean_tot) < cutoff) {
	pixel[count++] = val;
	if (j+l < ny)
	  pixelprod[count_prod++] = val*image(i,j+l);
	if (i+l < nx)
	  pixelprod[count_prod++] = val*image(i+l,j);
      }
      int toto = i+step;
      j += toto/nx;
      i = toto%nx;
      if (j >= ny-l) break;
    }
    
    double mad;
    double med = median_mad(pixel, mad);
    double rho = (median(pixelprod) - med*med)/(mad*mad);
    cout << argv[0] << ": " << imagename 
	 << " rho " << rho
	 << endl;
    ok = fabs(rho) > 1 ;
  }

  return ok? EXIT_SUCCESS : EXIT_FAILURE;
}
