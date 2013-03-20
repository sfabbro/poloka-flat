#include <iostream>

#include <poloka/fileutils.h>
#include <poloka/fitsimage.h>
#include <poloka/dbimage.h>
#include <poloka/superflat.h>

static void usage(const char* progname) {
  cerr << "Usage: " << progname << "  DBIMAGE...\n"
       << "Flatfield image using existing flats\n\n"
       << "    -f : force overwrite\n\n";
  exit(EXIT_FAILURE);
}

int main(int argc, char ** argv) {

  if (argc < 2) usage(argv[0]);

  bool overwrite = false;
  list<string> imList;

  for (int i=1; i<argc; ++i) {
    const char *arg = argv[i];
    if (arg[0] != '-') {
      imList.push_back(argv[i]);
      continue;
    }
    switch (arg[1]) {
    case 'f' : overwrite = true; break;
    default : usage(argv[0]); break;
    }
  }

  bool ok = true;

  for (list<string>::const_iterator it=imList.begin(); it != imList.end(); ++it) {
    const string &name = *it;
    DbImage dbimage(name);
    if (!dbimage.IsValid()) {
      cerr << argv[0] << ": cannot find "<< name  << endl;
      continue;
    }
    string outName = dbimage.FitsImageName(Calibrated);
    if (!overwrite && FileExists(outName)) {
      cerr << argv[0] << " flatfielded image already exists for image " << name << endl;
      ok = false;
      continue;
    }

    string inName = dbimage.FitsImageName(Raw);
    if (FileExists(dbimage.FitsFlatName())) {
      cout << argv[0] << ": processing " << name << endl;
      ok = FlatFieldImage(inName, dbimage.FitsFlatName(),
			  dbimage.FitsBiasName(), dbimage.FitsFringeName(),
			  outName);
    } else {
      Image flat; /* empty image -> no flatfielding */
      cerr << argv[0] << ": " << name << " missing flat\n";
      ok = false;
    }
  }
  return ok? EXIT_SUCCESS : EXIT_FAILURE;
}
