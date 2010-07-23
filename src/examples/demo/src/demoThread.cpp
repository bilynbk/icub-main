#include <iCub/demoThread.h>

using namespace yarp::os;
using namespace yarp::sig;
using namespace std;

demoThread::demoThread(int *threshold) {   
   thresholdValue = threshold;
}


bool demoThread::threadInit() {
    /* initialize variables and create data-structures if needed */
    
    /* opening ports */
    if (!imagePortIn.open("/demo/image:i")) {
      cout << ": unable to open input port " << endl;
      return false;  // unable to open; let RFModule know so that it won't run
    }

    if (!imagePortOut.open("/demo/image:o")) {
      cout  << ": unable to open output port " <<  endl;
      return false;  // unable to open; let RFModule know so that it won't run
    }

    return true;
}

void demoThread::run() {

    /* 
    * do some work ....
    * for example, convert the input image to a binary image using the threshold provided 
    */ 

    unsigned char value;

    while (isStopping() != true) { // the thread continues to run until isStopping() returns true
        cout << "demoThread: threshold value is " << *thresholdValue << endl;

        image = imagePortIn.read(true);

        if(image!=0){               
            ImageOf<PixelRgb> &binary_image = imagePortOut.prepare();
            binary_image.resize(image->width(),image->height());

            for (x=0; x<image->width(); x++) {
                for (y=0; y<image->height(); y++) {

                    rgbPixel = image->safePixel(x,y);

                    if (((rgbPixel.r + rgbPixel.g + rgbPixel.b)/3) > *thresholdValue) {
                    value = (unsigned char) 255;
                    }
                    else {
                    value = (unsigned char) 0;
                    }

                    rgbPixel.r = value;
                    rgbPixel.g = value;
                    rgbPixel.b = value;

                    binary_image(x,y) = rgbPixel;
                }
            }

            imagePortOut.write();
        }
      
    }//while
}

void demoThread::threadRelease() {
   /* for example, delete dynamically created data-structures */
}

void demoThread::onStop() {
    imagePortIn.interrupt();
    imagePortOut.interrupt();
    imagePortIn.close();
    imagePortOut.close();
}
