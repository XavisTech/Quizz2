//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                      Include Header Files                                                               //
//-----------------------------------------------------------------------------------------------------------------------------------------//
#include "xvtLib.h"
#include "P1.h"
#include "P6.h"

//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                              Main Function                                                              //
//-----------------------------------------------------------------------------------------------------------------------------------------//
int main() {

    //================================================================ P5 ==================================================================//
    cv::Mat P5 = loadImage("D:/Quizz2/Image/P5.tif");
    cv::Rect ROI_P5(1000, 500, 1000, 1500);
    cv::Mat P5Result = P6ImageProcessing(P5, ROI_P5); // Same processing as P6

    //================================================================ P6 ==================================================================//
    cv::Mat P6 = loadImage("D:/Quizz2/Image/P6.tif");
    cv::Rect ROI_P6(1000, 500, 1000, 1500);
    cv::Mat P6Result = P6ImageProcessing(P6, ROI_P6);

    //================================================================ P7 ==================================================================//
    cv::Mat P7 = loadImage("D:/Quizz2/Image/P7.tif");
    cv::Rect ROI_P7(1000, 500, 1000, 1500);
    // Mirror image P7 to process same with P6
    cv::Mat flippedP7;
    cv::flip(P7, flippedP7, 1);

    // Mirror ROI
    cv::Rect flippedROI7(
        P7.cols - (ROI_P7.x + ROI_P7.width),
        ROI_P7.y,
        ROI_P7.width,
        ROI_P7.height
    );
    cv::Mat P7Result = P6ImageProcessing(flippedP7, flippedROI7);
    cv::flip(P7Result, P7Result, 1); //flip back the result

    //================================================================ P8 ===============================================================//
    cv::Mat P8 = loadImage("D:/Quizz2/Image/P8.tif");
    cv::Rect ROI_P8(1000, 500, 1000, 1500);
    // Mirror image P8 to process same with P6
    cv::Mat flippedP8;
    cv::flip(P8, flippedP8, 1);

    // Mirror ROI
    cv::Rect flippedROI8(
        P8.cols - (ROI_P8.x + ROI_P8.width),
        ROI_P8.y,
        ROI_P8.width,
        ROI_P8.height
    );
    cv::Mat P8Result = P6ImageProcessing(flippedP8, flippedROI8);
    cv::flip(P8Result, P8Result, 1); //flip back the result

    //================================================================ P1 ===============================================================//
    cv::Mat P1 = loadImage("D:/Quizz2/Image/P1.tif");
    cv::Rect ROI_P1(1600, 500, 300, 1500);
    cv::Mat P1Result = P1ImageProcessing(P1, ROI_P1);

    ////================================================================ P2 ===============================================================//
    cv::Mat P2 = loadImage("D:/Quizz2/Image/P2.tif");
    cv::Rect ROI_P2(500, 500, 1000, 1500);
    cv::Mat P2Result = P1ImageProcessing(P2, ROI_P2); // Same processing as P1

    //================================================================ P3 ===============================================================//
    cv::Mat P3 = loadImage("D:/Quizz2/Image/P3.tif");
    cv::Rect ROI_P3(2000, 500, 1000, 1500);

    // Mirror image P3 to process same with P1
    cv::Mat flippedP3;
    cv::flip(P3, flippedP3, 0);
    // Mirror ROI
    cv::Rect flippedROI3(
        ROI_P3.x,
        P3.rows - (ROI_P3.y + ROI_P3.height),
        ROI_P3.width,
        ROI_P3.height
    );
    cv::Mat P3Result = P1ImageProcessing(flippedP3, flippedROI3);
    cv::flip(P3Result, P3Result, 0); //flip back the result

    //================================================================ P4 ===============================================================//
    cv::Mat P4 = loadImage("D:/Quizz2/Image/P4.tif");
    cv::Rect ROI_P4(500, 600, 300, 1300);

    // Mirror image P4 to process same with P1
    cv::Mat flippedP4;
    cv::flip(P4, flippedP4, 0);
    // Mirror ROI
    cv::Rect flippedROI4(
        ROI_P4.x,
        P4.rows - (ROI_P4.y + ROI_P4.height),
        ROI_P4.width,
        ROI_P4.height
    );
    cv::Mat P4Result = P1ImageProcessing(flippedP4, flippedROI4);
    cv::flip(P4Result, P4Result, 0); //flip back the result

    //================================================================ Save Results =====================================================//
    cv::imwrite("D:/Quizz2/Result/P1_Result.png", P1Result);
    cv::imwrite("D:/Quizz2/Result/P2_Result.png", P2Result);
    cv::imwrite("D:/Quizz2/Result/P3_Result.png", P3Result);
    cv::imwrite("D:/Quizz2/Result/P4_Result.png", P4Result);
    cv::imwrite("D:/Quizz2/Result/P5_Result.png", P5Result);
    cv::imwrite("D:/Quizz2/Result/P6_Result.png", P6Result);
    cv::imwrite("D:/Quizz2/Result/P7_Result.png", P7Result);
    cv::imwrite("D:/Quizz2/Result/P8_Result.png", P8Result);

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------//
//                                                            End of File                                                                  //
//-----------------------------------------------------------------------------------------------------------------------------------------//




