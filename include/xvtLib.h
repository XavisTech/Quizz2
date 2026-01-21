//--------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                  xvtLib.h                                                                        //
//--------------------------------------------------------------------------------------------------------------------------------------------------//
#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <vector>

//--------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                   Declaration                                                                    //
//--------------------------------------------------------------------------------------------------------------------------------------------------//

/// <summary>
/// Load an image from a file.
/// </summary>
/// <param name="filePath">Path to the image file.</param>
/// <returns>The loaded image. (8UC1)</returns>
cv::Mat loadImage(const std::string& filePath);


/// <summary>
/// Refind the ROI of the image. Set the ROI to the image size if the ROI is out of the image size.
/// </summary>
/// <param name="ROI">The ROI to be refinded.</param>
/// <param name="imageSize">The size of the image.</param>
/// <returns>The refinded ROI.</returns>
cv::Rect refindROI(const cv::Rect& ROI, const cv::Size& imageSize);

/// <summary>
/// Restore the 8-bit image to 16-bit image.
/// </summary>
/// <param name="src8">The 8-bit image to be restored.</param>
/// <param name="dst16">The 16-bit image to be restored.</param>
/// <param name="minVal">The minimum value of the image.</param>
/// <param name="maxVal">The maximum value of the image.</param>
void restore8To16bit(const cv::Mat& src8,
                    cv::Mat& dst16,
                    double minVal,
                    double maxVal);

/// <summary>
/// Restore the 16-bit image to 8-bit image.
/// </summary>
/// <param name="src16">The 16-bit image to be restored.</param>
/// <param name="dst8">The 8-bit image to be restored.</param>
/// <param name="minVal">The minimum value of the image.</param>
/// <param name="maxVal">The maximum value of the image.</param>
/// <returns>True if the restoration is successful, false otherwise.</returns>
bool convert16To8bit(const cv::Mat& src16,
                    cv::Mat& dst8,
                    double& minVal,
                    double& maxVal);

/// <summary>
/// Apply CLAHE to the image.
/// </summary>
/// <param name="src">The image to be processed.</param>
/// <param name="dst">The image after CLAHE.</param>
/// <param name="clipLimit">The clip limit of the CLAHE.</param>
/// <param name="tileGridSize">The tile grid size of the CLAHE.</param>
void CLAHEOpenCV(const cv::Mat& src,
                cv::Mat& dst,
                double clipLimit = 3.0,
                cv::Size tileGridSize = cv::Size(8, 8));

/// <summary>
/// Draw double-headed arrow
/// </summary>
/// <param name="img">The image on which to draw the arrow.</param>
/// <param name="p1">The starting point of the arrow.</param>
/// <param name="p2">The ending point of the arrow.</param>
/// <param name="color">The color of the arrow.</param>
/// <param name="thickness">The thickness of the arrow lines (default = 2).</param>
/// <param name="tipLength">The length of the arrow tips as a fraction of the arrow length (default = 0.05).</param>
void drawDoubleArrow(const cv::Mat& img,
                    cv::Point p1,
                    cv::Point p2,
                    cv::Scalar color,
                    int thickness = 4,
                    double tipLength = 0.1);

//--------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                   Class Definition                                                               //
//--------------------------------------------------------------------------------------------------------------------------------------------------//

/// <summary>
/// Find the peaks/valleys of the signal.
/// </summary>
class FindPeak
{
public:
    /// <summary>
    /// The mode of the FindPeak class.
    /// </summary>
    enum class Mode
    {
        PEAK,   // Find the peaks.
        VALLEY  // Find the valleys.
    };
    
    /// <summary>
    /// The peak information.
    /// </summary>
    struct PeakInfo
    {
        int position; // The position of the peak/valley.
        double value; // The value of the peak/valley.
    };

    /// <summary>
    /// Constructor of the FindPeak class.
    /// </summary>
    /// <param name="mode">The mode of the FindPeak class.</param>
    /// <param name="minDistance">The minimum distance between the peaks/valleys.</param>
    /// <param name="minProminence">The minimum prominence of the peaks/valleys.</param>
    FindPeak(Mode mode = Mode::PEAK, double minDistance = 0.0, int minProminence = 1) : m_mode(mode), m_minDistance(static_cast<int>(minDistance)), m_minProminence(minProminence) {}

    /// <summary>
    /// Set the mode of the FindPeak class.
    /// </summary>
    /// <param name="mode">The mode of the FindPeak class.</param>
    void setMode(Mode mode) { m_mode = mode; }

    /// <summary>
    /// Set the minimum distance between the peaks/valleys.
    /// </summary>
    /// <param name="minDistance">The minimum distance between the peaks/valleys.</param>
    void setMinDistance(int minDistance) { m_minDistance = minDistance; }

    /// <summary>
    /// Set the minimum prominence of the peaks/valleys.
    /// </summary>
    /// <param name="minProminence">The minimum prominence of the peaks/valleys.</param>
    void setMinProminence(double minProminence) { m_minProminence = minProminence; }

    /// <summary>
    /// Find the peaks/valleys of the signal.
    /// </summary>
    /// <param name="signal"></param>
    /// <returns>The peaks/valleys of the signal.</returns>
    std::vector<PeakInfo> run(const std::vector<double>& signal);

private:
    Mode m_mode;
    int m_minDistance;
    double m_minProminence;

    /// <summary>
    /// Find the local extrema of the signal.   
    /// </summary>
    /// <param name="signal">The signal to be processed.</param>
    /// <returns>The local extrema of the signal.</returns>
    std::vector<PeakInfo> findLocalExtrema(const std::vector<double>& signal);

    /// <summary>
    /// Apply the prominence filter to the signal.
    /// </summary>
    /// <param name="extrema">The local extrema of the signal.</param>
    /// <param name="signal">The signal to be processed.</param>
    void applyProminenceFilter(std::vector<PeakInfo>& extrema, const std::vector<double>& signal);

    /// <summary>
    /// Apply the distance filter to the signal.
    /// </summary>
    /// <param name="extrema">The local extrema of the signal.</param>
    void applyDistanceFilter(std::vector<PeakInfo>& extrema);

    /// <summary>
    /// Sort the local extrema by position.
    /// </summary>
    /// <param name="extrema">The local extrema of the signal.</param>
    void sortByPosition(std::vector<PeakInfo>& extrema);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                   End of file                                                                    //
//--------------------------------------------------------------------------------------------------------------------------------------------------//

