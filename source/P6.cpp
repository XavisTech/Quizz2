//--------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                      Include                                                                      //
//--------------------------------------------------------------------------------------------------------------------------------------------------//
#include "P6.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                      Definition                                                                  //
//--------------------------------------------------------------------------------------------------------------------------------------------------//

cv::Mat P6ImageProcessing(const cv::Mat inputImage, cv::Rect ROI) {
    //================================================================ Load Image ==================================================================//
    if (inputImage.empty()) {
        std::cerr << "Error loading image: " << std::endl;
    }
    cv::Mat resultImg = inputImage.clone();
    cv::cvtColor(resultImg, resultImg, cv::COLOR_GRAY2BGR);

    //================================================================ Setting ROI ==================================================================//
    ROI = refindROI(ROI, inputImage.size());
    cv::rectangle(resultImg, ROI, cv::Scalar(0, 255, 0), 2);
    cv::Mat srcImg = inputImage(ROI).clone();

    //================================================================ Find 2 edges to measure the distance =========================================//
    //--------------- Preprocessing ---------------------------------//
    cv::Mat claheImg;
    CLAHEOpenCV(srcImg, claheImg, 3.0, cv::Size(8, 8));

    // Denoising
    cv::Mat denoisedImg;
    cv::fastNlMeansDenoising(claheImg, denoisedImg, 15, 7, 21);

    cv::Mat blurImg;
    cv::bilateralFilter(denoisedImg, blurImg, 9, 75, 150);

    //---------------- Calculate Vertical Profile -------------------//
    // Vertical Profile
    std::vector<double> verticalProfile(blurImg.cols, 0.0);
    cv::reduce(blurImg, verticalProfile, 0, cv::REDUCE_AVG, CV_64F);

    // 1st derivative
    std::vector<double> derivativeProfile(verticalProfile.size(), 0.0);
    for (size_t i = 1; i < verticalProfile.size(); ++i) {
        derivativeProfile[i] = verticalProfile[i] - verticalProfile[i - 1];
    }

    //----------------- Find Peaks and Valleys --------------------//
    // Find the Distance
    FindPeak findPeak(FindPeak::Mode::PEAK);
    std::vector<FindPeak::PeakInfo> peaks;
    findPeak.setMinDistance(20);
    findPeak.setMinProminence(5);
    peaks = findPeak.run(derivativeProfile);

    FindPeak findValley(FindPeak::Mode::VALLEY);
    std::vector<FindPeak::PeakInfo> valleys;
    findValley.setMinDistance(20);
    findValley.setMinProminence(4);
    valleys = findValley.run(derivativeProfile);

    //---------------------- Draw Results -------------------------//
    // First peak from the left
    if (!peaks.empty()) {
        cv::line(resultImg(ROI),
            cv::Point(peaks.front().position, 0),
            cv::Point(peaks.front().position, resultImg.rows),
            cv::Scalar(0, 255, 255),
            4,
            cv::LINE_AA
        );
    }

    // First valley from the right
    if (!valleys.empty()) {
        cv::line(resultImg(ROI),
            cv::Point(valleys.back().position, 0),
            cv::Point(valleys.back().position, resultImg.rows),
            cv::Scalar(255, 0, 255),
            4,
            cv::LINE_AA
        );

        //Addition: Draw first valley from the left
        cv::Rect ROI2 = cv::Rect(cv::Point(valleys.front().position, 0),
            ROI.br());
        cv::rectangle(resultImg(ROI), ROI2, cv::Scalar(255, 0, 0), 4, cv::LINE_AA);
    }

    // Draw double arrow
    if (!peaks.empty() && !valleys.empty()) {
        drawDoubleArrow(resultImg(ROI),
            cv::Point(peaks.front().position, ROI.height / 2),
            cv::Point(valleys.back().position, ROI.height / 2),
            cv::Scalar(0, 0, 255)
        );
    }

    //================================================================ DEBUG ==================================================================//
#ifdef _DEBUG
    cv::Mat debugImg = claheImg.clone();
    cv::cvtColor(debugImg, debugImg, cv::COLOR_GRAY2BGR);

    //plot signal. scale the signal to fit the image width
    double maxVal = *std::max_element(derivativeProfile.begin(), derivativeProfile.end());
    double minVal = *std::min_element(derivativeProfile.begin(), derivativeProfile.end());

    for (size_t i = 1; i < derivativeProfile.size(); ++i) {
        cv::line(debugImg,
            cv::Point(static_cast<int>(i - 1), static_cast<int>(debugImg.rows - (derivativeProfile[i - 1] - minVal) / (maxVal - minVal) * debugImg.rows)),
            cv::Point(static_cast<int>(i), static_cast<int>(debugImg.rows - (derivativeProfile[i] - minVal) / (maxVal - minVal) * debugImg.rows)),
            cv::Scalar(0, 255, 0),
            1
        );
    }

    // Draw peaks and valleys
    for (const auto& peak : peaks) {
        cv::line(debugImg,
            cv::Point(static_cast<int>(peak.position), static_cast<int>(0)),
            cv::Point(static_cast<int>(peak.position), static_cast<int>(debugImg.rows)),
            cv::Scalar(0, 0, 255),
            2
        );
    }

    for (const auto& valley : valleys) {
        cv::line(debugImg,
            cv::Point(static_cast<int>(valley.position), static_cast<int>(0)),
            cv::Point(static_cast<int>(valley.position), static_cast<int>(debugImg.rows)),
            cv::Scalar(255, 0, 0),
            2
        );
    }
#endif

    return resultImg;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                   End of file                                                                    //
//--------------------------------------------------------------------------------------------------------------------------------------------------//