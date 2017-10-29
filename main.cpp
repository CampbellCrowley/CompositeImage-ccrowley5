#include <iostream>
#include <vector>
#include "CampbellLib/CampbellLib.h"
#include "bitmap.h"
using namespace std;

// Prototypes
Pixel addPixels(const Pixel &, const Pixel &);
Pixel dividePixels(const Pixel &, const Pixel &);
Pixel dividePixels(const Pixel &, int);
vector<vector<Pixel> > averagePixels(vector<vector<Pixel> >, int);
vector<vector<Pixel> > ProcessImages(vector<string> inPath = vector<string>(0));
bool isValidOutPath(string);
vector<vector<Pixel> > ProcessImages(vector<string>);

// Main
bool Main(vector<string> inPath, string outPath = "") {
  Bitmap bmp;
  bmp.fromPixelMatrix(ProcessImages(inPath));
  cout << Campbell::Color::green << "Processing complete!"
       << Campbell::Color::reset << endl;

  // Determine output location.
  while (true) {
    if (outPath.length() < 1) {
      cout << "Please enter the place to save the image (nothing for "
              "./composite-ccrowley5.bmp): ";
      getline(cin, outPath);
    }
    if (outPath.length() < 1) {
      outPath = "composite-ccrowley5.bmp";
    }
    if (isValidOutPath(outPath)) {
      break;
    } else {
      cerr << Campbell::Color::red << outPath
           << " could not be opened for editing. Is it already open by another "
              "program or is it read-only?\n"
           << Campbell::Color::reset;
      outPath = "";
    }
  }

  // Save file.
  // bitmap.h does not provide an interface for checking whether this succeeds
  // of not. The best I can do is assume my previous checks were good enough to
  // verify the result will succeed.
  bmp.save(outPath);

  cout << Campbell::Color::green << "File saved to " << outPath << endl
       << Campbell::Color::reset;

  if (inPath.empty()) {
    cout << "Would you like to create another photo? (Y/n): ";
    return Campbell::Strings::getYesNo();
  }

  return false;
}

// Entry
int main(int argc, const char *argv[]) {
  // argc is guaranteed to be at least 1.
  vector<string> inputs;
  string outPath;
  // argv[0] is binary name.
  for (int i = 1; i < argc; i++) {
    if (string(argv[i]) == "-o") {
      if (outPath.empty()) {
        if (i + 1 < argc) {
          outPath = string(argv[i + 1]);
        } else {
          cerr << Campbell::Color::red << "Specified output may not be blank!"
               << Campbell::Color::reset << endl;
        }
      } else {
        cerr << Campbell::Color::red << "Only one output may be specified!"
             << Campbell::Color::reset << endl;
      }
      i++;
      continue;
    }
    inputs.push_back(string(argv[i]));
  }
  if (argc == 1) {
    cout << "Info: You may specify filenames as arguments to the executable "
            "(because tab-complete is nice).\nInfo: use '-o {FILENAME}' to "
            "specify the output file.\nExample: "
         << argv[0] << " image1.bmp image2.bmp -o output.bmp\n\n";
  }
  while (Main(inputs, outPath)) {
    inputs.clear();
    outPath.clear();
  }
  return 0;
}

// Definitions

// Do all of the processing of images including loading from disk.
vector<vector<Pixel> > ProcessImages(vector<string> inPath) {
  vector<vector<Pixel> > output;
  int numImages = 0;
  int numImagesLoaded = 0;
  string nextPath;
  while (true) {
    // Bitmap appends new images to previously loaded images when open() is
    // called. bmp needs to be cleared prior to looping to prevent this.
    Bitmap bmp;
    // Determine file to load.
    if (inPath.size() < 1) {
      // If no paths were provided via arguments, ask user for image each time
      // prior to processing.
      cout << "Please enter an image name to load (#" << numImages + 1
           << ") or \"DONE\" to stop: ";
      getline(cin, nextPath);
      if (nextPath == "DONE") return averagePixels(output, numImagesLoaded);
    } else if (numImages < (int)inPath.size()) {
      nextPath = inPath[numImagesLoaded];
      cout << Campbell::Color::green << "Loading image (" << numImagesLoaded + 1
           << "): " << nextPath << Campbell::Color::reset << endl;

    } else {
      return averagePixels(output, numImagesLoaded);
    }

    numImages++;

    // Attempt to open file. If it fails, allow user to choose new file, or skip
    // if passed in through arguments.
    bmp.open(nextPath);
    if (!bmp.isImage()) {
      cerr << Campbell::Color::red << "Invalid image (" << numImages << ", "
           << nextPath
           << ").\nEither the file provided wasn't a valid bitmap "
              "(24bit depth Windows BMP), or the path was incorrect.\n"
           << Campbell::Color::reset;
      continue;
    }

    numImagesLoaded++;

    // Convert image to 2D vector of Pixels. Resize output to fit the largest
    // image entered.
    vector<vector<Pixel> > pixels = bmp.toPixelMatrix();
    // It is safe to assume that the image has at least one row and column and
    // is not jagged.
    if (numImagesLoaded > 1 && (output.size() != pixels.size() ||
                                output[0].size() != pixels[0].size())) {
      cerr << Campbell::Color::yellow
           << "Canvas size and image size do not match! The canvas may be "
              "resized to accommodate. Canvas: "
           << output.size() << "x" << output[0].size() << ", Image(#"
           << numImages << "): " << pixels.size() << "x" << pixels[0].size()
           << ". Images may not align properly!" << Campbell::Color::reset
           << endl;
    } else if (numImagesLoaded <= 1) {
      output.resize(pixels.size());
    }
    for (int i = 0; i < (int)pixels.size(); i++) {
      if (i >= (int)output.size()) {
        output.push_back(vector<Pixel>(output[i - 1].size(), Pixel(0, 0, 0)));
      }
      for (int j = 0; j < (int)pixels[i].size(); j++) {
        if (numImagesLoaded <= 1) {
          output[i].resize(pixels[i].size());
        } else if (j >= (int)output[i].size()) {
          output[i].push_back(Pixel(0, 0, 0));
        }
        output[i][j] = addPixels(pixels[i][j], output[i][j]);
      }
    }
  }
}

// Add two pixels together.
Pixel addPixels(const Pixel &one, const Pixel &two) {
  return Pixel(one.red + two.red, one.green + two.green, one.blue + two.blue);
}
// Divide one pixel by another.
Pixel dividePixels(const Pixel &one, const Pixel &two) {
  return Pixel(one.red / two.red, one.green / two.green, one.blue / two.blue);
}
// Divide a pixel by a value.
Pixel dividePixels(const Pixel &one, int num) {
  return dividePixels(one, Pixel(num, num, num));
}
// Divide matrix of pixels by a value in order to find the average of the summed
// pixels.
vector<vector<Pixel> > averagePixels(vector<vector<Pixel> > input, int count) {
  vector<vector<Pixel> > output =
      vector<vector<Pixel> >(input.size(), vector<Pixel>(input[0].size()));
  for (int i = 0; i < (int)input.size(); i++) {
    for (int j = 0; j < (int)input[i].size(); j++) {
      output[i][j] = dividePixels(input[i][j], count);
    }
  }
  return output;
}
// Check if file will be able to save to specified path.
bool isValidOutPath(string filename) {
  std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);

  if (file.fail()) {
    file.close();
    return false;
  } else {
    file.close();
    return true;
  }
}
