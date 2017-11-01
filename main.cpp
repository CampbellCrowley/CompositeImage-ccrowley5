#include <iostream>
#include <limits>
#include <vector>
#include "CampbellLib/CampbellLib.h"
#include "bitmap_wrapper.h"
using namespace std;

// Forenote: The limit to the number of images that can be processed is
// MAX_NUM_IMAGES. This is because I DO NOT load all images at the same time as
// this would fill RAM very quickly. Only 2 (3, sigh...) images are loaded at a
// time, one from the disk and one is the matrix of all previously loaded
// images. This matrix stores the total sum of all images previously loaded
// which will be averaged once all images have been entered.
// (The third image is the buffer that the bitmap library has.)

// Prototypes (Ugh, this is ugly)
// TODO: Move stuff into a header file.
bool Main(vector<string> &, string &);
void averagePixels(PixelMatrix &, int);
PixelMatrix ProcessImages(vector<string> &inPath);
bool isValidOutPath(const string &);
void printHelp(const char *);

enum Options { UNSET, ALLOW_RESIZE, DISALLOW_RESIZE };
Options option = UNSET;
const unsigned int MAX_NUM_IMAGES = numeric_limits<int>::max() / MAX_RGB;
// 8421504

// Entry
int main(int argc, const char *argv[]) {
  // cout << "MAX_NUM_IMAGES: " << MAX_NUM_IMAGES << endl;

  // argc is guaranteed to be at least 1.
  vector<string> inputs;
  string outPath;
  // argv[0] is binary name. argv[1] is first argument.
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
    } else if (string(argv[i]) == "-a") {
      option = ALLOW_RESIZE;
      continue;
    } else if (string(argv[i]) == "-d") {
      option = DISALLOW_RESIZE;
      continue;
    } else if (string(argv[i]) == "-h" || string(argv[i]) == "--help") {
      printHelp(argv[0]);
      return 0;
    }
    inputs.push_back(string(argv[i]));
  }
  if (argc == 1) {
    printHelp(argv[0]);
  }
  // Loop program until user decides to exit.
  while (Main(inputs, outPath)) {
    inputs.clear();
    outPath.clear();
    option = UNSET;
  }
  return 0;
}

// Definitions

// Main
bool Main(vector<string> &inPath, string &outPath) {
  Bitmap bmp;
  bmp.fromPixelMatrix(ProcessImages(inPath));
  cout << Campbell::Color::green << "Processing complete!"
       << Campbell::Color::reset << endl;
  if (bmp.toPixelMatrix() == PixelMatrix()) {
    cerr << Campbell::Color::yellow << "Image is empty. I can't save nothing."
         << Campbell::Color::reset << endl;
  } else {
    // Determine output location.
    while (true) {
      if (outPath.empty()) {
        cout << "Please enter the place to save the image (nothing for "
                "./composite-ccrowley5.bmp): ";
        getline(cin, outPath);
      }
      if (outPath.empty()) {
        outPath = "composite-ccrowley5.bmp";
      }
      cout << Campbell::Color::green << "File saving to " << outPath << "...\r"
           << Campbell::Color::reset << flush;
      if (isValidOutPath(outPath)) {
        break;
      } else {
        cerr << Campbell::Color::red << outPath
             << " could not be opened for editing. Is it already open by "
                "another program or is it read-only? (Who knows? Anything's "
                "possible!)\n."
             << Campbell::Color::reset;
        outPath.clear();
      }
    }

    cout << Campbell::Color::green << "File saving to " << outPath << "...\r"
         << Campbell::Color::reset << flush;
    // Save file.
    // bitmap.h does not provide an interface for checking whether this succeeds
    // or not (still). The best I can do is assume my previous checks were good
    // enough to verify the result will succeed.
    bmp.save(outPath);

    cout << Campbell::Color::green << "File saved to " << outPath << "     "
         << Campbell::Color::reset << endl;
  }

  if (inPath.empty()) {
    cout << "Would you like to create another photo? (Y/n): ";
    return Campbell::Strings::getYesNo();
  }

  return false;
}  // bool Main

// Do all of the processing of images including loading from disk.
PixelMatrix ProcessImages(vector<string> &inPath) {
  if (inPath.size() > MAX_NUM_IMAGES) {
    cerr << Campbell::Color::red << "A maximum of " << MAX_NUM_IMAGES
         << " images is supported. Only the first " << MAX_NUM_IMAGES
         << " will be processed.\n"
         << Campbell::Color::reset;
    inPath.resize(MAX_NUM_IMAGES);
  }
  PixelMatrix output;
  int numImages = 0;
  int numImagesLoaded = 0;
  size_t previousWidth = 0;
  size_t previousHeight = 0;
  string nextPath;
  while (true) {
    // Bitmap appends new images to previously loaded images when open() is
    // called. bmp needs to be cleared prior to looping to prevent this.
    Bitmap bmp;
    // Determine file to load.
    if (inPath.size() < 1) {
      if (numImagesLoaded == 10) {
        cout << Campbell::Color::yellow
             << "At this point, I'm supposed to tell you that you have entered "
                "the maximum allowable number of images, but this is not the "
                "limit.\nYou may add up to "
             << MAX_NUM_IMAGES << " images, not the miniscule 10 I'm "
                                  "required.\n"
             << Campbell::Color::reset;
      }
      // If no paths were provided via arguments, ask user for image each time
      // prior to processing.
      cout << "Please enter an image name to load (#" << numImagesLoaded + 1
           << ") or \"DONE\" to stop: ";
      getline(cin, nextPath);
      if (nextPath == "DONE") {
        if (numImagesLoaded == 0) {
          return PixelMatrix();
        } else {
          averagePixels(output, numImagesLoaded);
          return output;
        }
      }
    } else if (numImages < (int)inPath.size()) {
      nextPath = inPath[numImages];
      cout << Campbell::Color::green << "Opening image (" << numImages + 1
           << "/" << inPath.size() << " "
           << (numImages / (float)inPath.size() * 100.0) << "%): " << nextPath
           << Campbell::Color::reset << "          \r" << flush;
      numImages++;
    } else {
      averagePixels(output, numImagesLoaded);
      return output;
    }

    // Attempt to open file. If it fails, allow user to choose new file, or skip
    // if passed in through arguments.
    cout << Campbell::Color::magenta;
    bmp.open(nextPath);
    if (!bmp.isImage()) {
      cerr << Campbell::Color::red << "Invalid image (" << numImagesLoaded + 1
           << ", " << nextPath
           << ").\nEither the file provided wasn't a valid bitmap "
              "(24bit depth Windows BMP), or the path was incorrect.\n"
           << Campbell::Color::reset;
      continue;
    }
    cout << Campbell::Color::green << "Loading image (" << numImages << "/"
         << inPath.size() << " "
         << ((numImages - 0.5) / (float)inPath.size() * 100.0)
         << "%): " << nextPath << Campbell::Color::reset << "          \r"
         << flush;

    numImagesLoaded++;

    // Convert image to 2D vector of Pixels. Resize output to fit the largest
    // image entered.
    PixelMatrix pixels = bmp.toPixelMatrix();
    // It is safe to assume that the image has at least one row and column and
    // is not jagged.
    if (numImagesLoaded > 1 && previousWidth != pixels.size() &&
        previousHeight != pixels[0].size() &&
        (output.size() != pixels.size() ||
         output[0].size() != pixels[0].size())) {
      cerr << Campbell::Color::yellow
           << "Canvas size and image size do not match! Canvas: "
           << output.size() << "x" << output[0].size() << ", Image(#"
           << numImagesLoaded << ", " << nextPath << "): " << pixels.size()
           << "x" << pixels[0].size() << ". Images may not align properly!"
           << Campbell::Color::reset << endl;
      // Check if canvas will need to be resized to fit the new image and inform
      // user.
      if (output.size() < pixels.size() ||
          output[0].size() < pixels[0].size()) {
        if (option == UNSET) {
          cout << "Would you like to allow canvas resizing? (Tip: run with "
                  "--help for options to choose this automatically), (Y/n): ";
          option = Campbell::Strings::getYesNo(true) ? ALLOW_RESIZE
                                                     : DISALLOW_RESIZE;
        }
        if (option == ALLOW_RESIZE) {
          cout << "Canvas will be resized.\n";
        } else {
          cout << "Canvas will NOT be resized.\n";
        }
      }
    } else if (numImagesLoaded <= 1) {
      output.resize(pixels.size());
    }
    for (int i = 0; i < (int)pixels.size(); i++) {
      // Increase the size of the canvas to fit the new image.
      if (i >= (int)output.size()) {
        if (option == ALLOW_RESIZE) {
          output.push_back(vector<Pixel>(output[i - 1].size(), Pixel()));
        } else {
          break;
        }
      }
      for (int j = 0; j < (int)pixels[i].size(); j++) {
        // Increase the size of the canvas to fit the new image.
        if (numImagesLoaded <= 1) {
          output[i].resize(pixels[i].size());
        } else if (j >= (int)output[i].size()) {
          if (option == ALLOW_RESIZE) {
            output[i].push_back(Pixel());
          } else {
            break;
          }
        }
        // Sum canvas and new image values.
        output[i][j] += pixels[i][j];
      }
    }
    previousWidth = pixels.size();
    previousHeight = pixels[0].size();
    if ((unsigned int)numImagesLoaded >= MAX_NUM_IMAGES) {
      cout << Campbell::Color::yellow
           << "Maximum number of images reached. Finalizing image."
           << Campbell::Color::reset << endl;
      averagePixels(output, numImagesLoaded);
      return output;
    }
  }
}  // PixelMatrix ProcessImages

// Divide matrix of pixels by a value in order to find the average of the summed
// pixels.
void averagePixels(PixelMatrix &input, int count) {
  for (int i = 0; i < (int)input.size(); i++) {
    for (int j = 0; j < (int)input[i].size(); j++) {
      input[i][j] = input[i][j] / count;
    }
  }
}  // void averagePixels

// Check if file will be able to save to specified path.
bool isValidOutPath(const string &filename) {
  std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);

  if (file.fail()) {
    file.close();
    return false;
  } else {
    file.close();
    return true;
  }
}  // bool isValidOutPath

void printHelp(const char *argv) {
  cout
      << "Info: You may specify filenames as arguments to the executable "
         "(because tab-complete and wildcards are nice).\n"
         "Info: use '-o {FILENAME}' to specify the output file.\n"
         "Info: use '-a' to automatically allow canvas resizing if necessary.\n"
         "Info: use '-d' to disallow any canvas resizing.\n"
         "Info: use '-h' or '--help' to show this message.\n"
         "Example: "
      << argv
      << " ./image1.bmp ~/image2.bmp ../*.bmp -o output.bmp -a\n\n"
         "Note: If input files are specified as arguments, the program will "
         "not\n"
         "      give the option to make a another photo on completion.\n\n";
}  // void printHelp

// My issues with bitmap library:
// 1) Does not include iostream. Requires parent to do so.
// 2) Gives no feedback as to whether operations fail or succeed.
// 3) Does not contain interface for accessing Pixels as an array making loops
// more complicated.
// 4) Pixels are stored as int instead of space efficient unsigned byte
// (uchar_t), which I see a typedef for...
// 5) Comments are formatted as a Java Doc. (This is C++, I can see what is
// being returned)
// 6) Appends newly opened files to current buffer instead of overwriting and
// does not provide interface for clearing buffer.
// 7) Inefficiently requires 2 copies of an image to be stored at all times. (1
// is its internal buffer, while the user is required to copy this buffer to
// another buffer in order to use it.)
// 8) White space EVERYWHERE.
// 9) Uses cout. Give the includer feedback instead!
// 10) fromPixelMatrix() should check if matrix is valid image and return if
// there is a failure.
// 11) Functions should all be static. (No need for internal buffer, just return
// it on open(). Pass in PixelMatrix for each function instead.)
// 12) Pixels are typedef'd to be "helpful", but do not provide comparative
// operators making inline comparison difficult.
