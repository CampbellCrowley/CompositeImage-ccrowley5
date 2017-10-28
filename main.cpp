#include <iostream>
#include <vector>
#include "CampbellLib/CampbellLib.h"
#include "bitmap.h"

using namespace std;

Pixel addPixels(const Pixel &one, const Pixel &two) {
  return Pixel(one.red + two.red, one.green + two.green, one.blue + two.blue);
}
Pixel dividePixels(const Pixel &one, const Pixel &two) {
  return Pixel(one.red / two.red, one.green / two.green, one.blue / two.blue);
}
Pixel dividePixels(const Pixel &one, int num) {
  return dividePixels(one, Pixel(num, num, num));
}

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

vector<vector<Pixel> > ProcessImages(
    vector<string> inPath = vector<string>(0)) {
  vector<vector<Pixel> > output;
  Bitmap bmp;
  int numImages = 0;
  int numImagesLoaded = 0;
  string nextPath = "";
  while (true) {
    // Determine file to load.
    if (inPath.size() < 1) {
      // If not paths were provided via arguments, ask user for image each time
      // prior to processing.
      cout << "Please enter an image name to load (#" << numImages + 1
           << ") or \"DONE\" to stop: ";
      getline(cin, nextPath);
      if (nextPath == "DONE") return averagePixels(output, numImagesLoaded);
    } else if (numImages < (int)inPath.size()) {
      nextPath = inPath[numImagesLoaded];
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
    for (int i = 0; i < (int)pixels.size(); i++) {
      if (i > (int)output.size()) {
        output.push_back(vector<Pixel>(output[i - 1].size(), Pixel(0, 0, 0)));
      }
      for (int j = 0; j < (int)pixels[i].size(); j++) {
        if (j > (int)output[i].size()) {
          output[i].push_back(Pixel(0, 0, 0));
        }
        output[i][j] = addPixels(pixels[i][j], output[i][j]);
      }
    }
  }
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

bool Main(vector<string> inPath, string outPath = "") {
  Bitmap bmp;
  bmp.fromPixelMatrix(ProcessImages(inPath));

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

  if (moreOpts) {
    cout << "Would you like to edit another photo? (Y/n): ";
    return Campbell::Strings::getYesNo();
  }

  return false;
}

int main(int argc, const char *argv[]) {
  vector<string> inputs = vector<string>(argc - 1);
  for (int i = 1; i < argc; i++) {
    inputs[i - 1] = argv[i];
  }
  while (Main(inputs)) {
    inputs.clear();
  }
  return 0;
}
