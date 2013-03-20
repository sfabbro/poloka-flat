#include <list>

#include <poloka/fitsimage.h>
#include <poloka/frame.h>
#include <poloka/fitstoad.h>
#include <poloka/polokaexception.h>

static void usage(const char* progname) {
  cerr << "Usage: " << progname << "[OPTION] FITS... \n" 
       << "Trim the image to save only the illuminated frame\n\n"
       << "    -t '[x0:x1,y0:y1]' : user define trimming section (default: from header)\n\n";
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {

  if (argc <=1) usage(argv[0]);

  // keep it fast, so avoid list<string>
  bool use_header = true;
  int istart = 1;
  Frame illu;
  list<string> imList;

  for (int i=1; i < argc; ++i)  {
    const char *arg = argv[i];
    if (arg[0] != '-') {
      imList.push_back(argv[i]);
      continue;
    }
    switch (arg[1]) {
    case 't':
      int x0,x1,y0,y1;
      i++;
      if (sscanf(argv[i],"[%d:%d,%d:%d]",&x0,&x1,&y0,&y1) == 4) {
	if (x0>x1) swap(x0,x1);
	if (y0>y1) swap(y0,y1);
	illu = Frame(Point(x0-1,y0-1), Point(x1-1, y1-1));
	use_header = false;
      }
      else 
	cerr << argv[0] << ": cannot decode 4 integers in " << argv[i] << endl;
      break;
    default : usage(argv[0]); break;
    }
  }

  bool ok = true;
  for (list<string>::const_iterator it=imList.begin(); it != imList.end(); ++it) {
    try {
      FitsImage image(*it, RW);
      image.EnableWrite(false); // hold it
      if (!image.IsValid()) {
	cerr << argv[0] << ": invalid fits file "  << *it << endl;
	continue;
      }
      if (use_header) illu = TotalIlluRegion(image);
      if (image.Trim(illu)) image.EnableWrite(true);
      // keep the original scaling : 
      image.Write( /*force_bscale = */ true);
      } catch(PolokaException p) {
	p.PrintMessage(cerr);
	ok = false;
    }
  }

  return ok? EXIT_SUCCESS : EXIT_FAILURE;
}
