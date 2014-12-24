#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "canny.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>


using namespace cv;
using namespace std;

Mat main_img , teshert ,main_edge ,teshert_edge ,tmp;
Mat img; Mat templ; Mat result;
char* image_window = (char *)"Source Image";
char* result_window = (char *)"Result window";

int match_method;
int max_Trackbar = 5;

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
  /// Source image to display
  Mat img_display;
  img.copyTo( img_display );

  /// Create the result matrix
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_cols, result_rows, CV_32FC1 );

  /// Do the Matching and Normalize
  matchTemplate( img, templ, result, match_method );
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }

  /// Show me what you got
  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

  imshow( image_window, img_display );
  imshow( result_window, result );

  return;
}


int main(int argc, char** argv)
{
  //argv[1] main image
  //argv[2] teshert

  main_img = imread(argv[1],1);
  teshert = imread(argv[2],1);

  if( !main_img.data )
    { printf("no input");return -1; }

  main_edge = call_canny(70,main_img);

  // resize(teshert ,tmp,tmp.size(),0.40,0.40,INTER_LINEAR);
  tmp = call_canny(30,teshert);
  resize(tmp ,teshert_edge,teshert_edge.size(),0.35,0.35,INTER_LINEAR);

  Size s = main_edge.size();
  printf("hamada \n main_edge ----> height = %d --- width = %d\n",s.height,s.width);
  

  /// Create Trackbar

  imshow("main edged", main_edge);
  imshow("Tshirt edged", teshert_edge);
  img = main_edge;
  templ = teshert_edge;
  /// Create windows
  namedWindow( image_window, CV_WINDOW_AUTOSIZE );
  namedWindow( result_window, CV_WINDOW_AUTOSIZE );
  char* trackbar_label =(char *) "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
  createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );
  MatchingMethod( 0, 0 );
  waitKey(0);
  return 0;
}

