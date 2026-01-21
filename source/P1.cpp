//--------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                       Include                                                                    //
//--------------------------------------------------------------------------------------------------------------------------------------------------//
#include "P1.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                   Definition                                                                     //
//--------------------------------------------------------------------------------------------------------------------------------------------------//
cv::Mat P1ImageProcessing(const cv::Mat inputImage, cv::Rect ROI)
{
    //================================================================ Load Image ==================================================================//
    
    if (inputImage.empty())
    {
        throw std::invalid_argument("Input image is empty.");
    }
    cv::Mat resultImg = inputImage.clone();
    cv::cvtColor(resultImg, resultImg, cv::COLOR_GRAY2BGR);

    //================================================================ Setting ROI ==================================================================//
    ROI = refindROI(ROI, inputImage.size());
    cv::rectangle(resultImg, ROI, cv::Scalar(0, 255, 0), 2);
    cv::Mat srcImg = inputImage(ROI).clone();

    //================================================================ Find 2 edges to measure the distance =========================================//
    //--------------- Preprocessing ---------------------------------//
    cv::Mat denoisedImg;
    cv::fastNlMeansDenoising(srcImg, denoisedImg, 10, 7, 21);

    //--------------- Calculate Horizontal Profile -----------------//
    cv::Mat1d horizontalProfileImg(denoisedImg.rows, 1);
    cv::reduce(denoisedImg, horizontalProfileImg, 1, cv::REDUCE_AVG, CV_64F);
    std::vector<double> horizontalProfile = cv::Mat_<double>(horizontalProfileImg);

    //1st derivative
    std::vector<double> derivativeProfile(horizontalProfile.size(), 0.0);
    for (size_t i = 1; i < horizontalProfile.size(); ++i) {
        derivativeProfile[i] = horizontalProfile[i] - horizontalProfile[i - 1];
    }

    //----------------- Find Peaks and Valleys --------------------//
    FindPeak findPeak(FindPeak::Mode::PEAK);
    std::vector<FindPeak::PeakInfo> peaks;
    findPeak.setMinDistance(20);
    findPeak.setMinProminence(5);
    peaks = findPeak.run(derivativeProfile);

    FindPeak findValley(FindPeak::Mode::VALLEY);
    std::vector<FindPeak::PeakInfo> valleys;
    findValley.setMinDistance(20);
    findValley.setMinProminence(5);
    valleys = findValley.run(derivativeProfile);

    //---------------------- Draw Results -------------------------//
    // First valley from the bottom
    if (!valleys.empty()) {
        cv::line(resultImg(ROI),
            cv::Point(0, valleys.back().position),
            cv::Point(resultImg.cols, valleys.back().position),
            cv::Scalar(255, 0, 255),
            4
        );
        //Addition: Draw first valley from the top
        cv::Rect ROI2 = cv::Rect(cv::Point(0, valleys.front().position - 2), ROI.br());
        ROI2 = refindROI(ROI2, resultImg.size());
        cv::rectangle(resultImg(ROI), ROI2, cv::Scalar(255, 0, 0), 4);
    }

    // First peak from the bottom
    if (!peaks.empty()) {
        cv::line(resultImg(ROI),
            cv::Point(0, peaks.back().position),
            cv::Point(resultImg.cols, peaks.back().position),
            cv::Scalar(0, 255, 255),
            4
        );
    }

    if(!peaks.empty() && !valleys.empty())
    {
        drawDoubleArrow(resultImg(ROI),
            cv::Point(ROI.width / 2, valleys.back().position),
            cv::Point(ROI.width / 2, peaks.back().position),
            cv::Scalar(0, 0, 255)
        );
    }

    //================================================================ DEBUG: Plot Signal, Peaks and Valleys =========================================//
#ifdef _DEBUG
    cv::Mat debugImg = denoisedImg.clone();
    cv::cvtColor(debugImg, debugImg, cv::COLOR_GRAY2BGR);

    //plot signal. scale the signal to fit the image width
    double maxVal = *std::max_element(derivativeProfile.begin(), derivativeProfile.end());
    double minVal = *std::min_element(derivativeProfile.begin(), derivativeProfile.end());
    for (size_t i = 1; i < derivativeProfile.size(); ++i) {
        cv::line(debugImg,
            cv::Point(static_cast<int>(debugImg.cols - static_cast<int>((derivativeProfile[i - 1] - minVal) / (maxVal - minVal) * debugImg.cols)), static_cast<int>(i - 1)),
            cv::Point(static_cast<int>(debugImg.cols - static_cast<int>((derivativeProfile[i] - minVal) / (maxVal - minVal) * debugImg.cols)), static_cast<int>(i)),
            cv::Scalar(0, 255, 0),
            1
        );
    }

    // Draw peaks and valleys
    for (const auto& peak : peaks) {
        cv::line(debugImg,
            cv::Point(static_cast<int>(0), peak.position),
            cv::Point(static_cast<int>(debugImg.cols), peak.position),
            cv::Scalar(0, 0, 255),
            2
        );
    }

    for (const auto& valley : valleys) {
        cv::line(debugImg,
            cv::Point(static_cast<int>(0), valley.position),
            cv::Point(static_cast<int>(debugImg.cols), valley.position),
            cv::Scalar(255, 0, 0),
            2
        );
    }

#endif // DEBUG

    return resultImg;

}

//--------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                   End of file                                                                    //
//--------------------------------------------------------------------------------------------------------------------------------------------------//