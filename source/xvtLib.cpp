//==============================================================================================================================================//
//                                                            xvtLib.cpp                                                                        //
//==============================================================================================================================================//

#include "xvtLib.h"

//==============================================================================================================================================//
//                                                              Definition                                                                      //
//==============================================================================================================================================//

cv::Mat loadImage(const std::string& filePath) {
    cv::Mat image = cv::imread(filePath, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cerr << "Error loading image: " << filePath << std::endl;
    }

    if (image.type() == CV_16UC1)
    {
        double minVal = 0, maxVal = 0;
        convert16To8bit(image, image, minVal, maxVal);
    }
    return image;
}

cv::Rect refindROI(const cv::Rect& ROI, const cv::Size& imageSize) {
    int x = ROI.x;
    int y = ROI.y;
    int w = ROI.width;
    int h = ROI.height;

    x = std::max(0, x);
    y = std::max(0, y);
    w = std::min(w, imageSize.width - x);
    h = std::min(h, imageSize.height - y);

    if (w < 1 || h < 1) return cv::Rect(); // return empty rectangle if invalid

    return cv::Rect(x, y, w, h);
}

void restore8To16bit(const cv::Mat& src8, cv::Mat& dst16, double minVal, double maxVal)
{
    CV_Assert(src8.type() == CV_8UC1);

    double scale = (maxVal - minVal) / 255.0;
    double shift = minVal;

    src8.convertTo(dst16, CV_16U, scale, shift);
}

bool convert16To8bit(const cv::Mat& src16,
    cv::Mat& dst8,
    double& minVal,
    double& maxVal
) {
    CV_Assert(src16.type() == CV_16UC1);

    cv::minMaxLoc(src16, &minVal, &maxVal);

    if (maxVal - minVal < 1e-6)
        return false;   // flat image

    double scale = 255.0 / (maxVal - minVal);
    double shift = -minVal * scale;

    src16.convertTo(dst8, CV_8U, scale, shift);
    return true;
}

void CLAHEOpenCV(const cv::Mat& src,
    cv::Mat& dst,
    double clipLimit,
    cv::Size tileGridSize)
{
    cv::Mat src8, dst8;
    double minVal = 0.0, maxVal = 255.0;

    if (src.type() == CV_16UC1) {
        if (!convert16To8bit(src, src8, minVal, maxVal)) {
            dst = src.clone();
            return;
        }
    }
    else if (src.type() == CV_8UC1) {
        src8 = src;
    }
    else {
        std::cout << "Unsupported image type\n";
        return;
    }

    cv::Ptr<cv::CLAHE> clahe =
        cv::createCLAHE(clipLimit, tileGridSize);
    clahe->apply(src8, dst8);

    if (src.type() == CV_16UC1)
        restore8To16bit(dst8, dst, minVal, maxVal);
    else
        dst = dst8;
}

void drawDoubleArrow(const cv::Mat& img, cv::Point p1, cv::Point p2, cv::Scalar color, int thickness, double tipLength)
{
    cv::arrowedLine(img, p1, p2, color,
        thickness, cv::LINE_AA, 0, tipLength);
    cv::arrowedLine(img, p2, p1, color,
        thickness, cv::LINE_AA, 0, tipLength);
}

std::vector<FindPeak::PeakInfo> FindPeak::run(const std::vector<double>& signal)
{
    if (signal.size() < 3) return {};

    auto extrema = findLocalExtrema(signal);
    applyProminenceFilter(extrema, signal);
    applyDistanceFilter(extrema);
    sortByPosition(extrema);

    return extrema;
}

std::vector<FindPeak::PeakInfo> FindPeak::findLocalExtrema(const std::vector<double>& s)
{
    std::vector<PeakInfo> out;
    for (int i = 1; i < (int)s.size() - 1; ++i) {
        if (m_mode == Mode::PEAK) {
            if (s[i] > s[i - 1] && s[i] > s[i + 1])
                out.push_back({ i, s[i] });
        }
        else {
            if (s[i] < s[i - 1] && s[i] < s[i + 1])
                out.push_back({ i, s[i] });
        }
    }
    return out;
}


void FindPeak::applyProminenceFilter(std::vector<PeakInfo>& peaks, const std::vector<double>& s)
{
    std::vector<PeakInfo> out;

    for (auto& p : peaks) {
        double leftBase = p.value;
        for (int i = p.position - 1; i >= 0; --i) {
            if ((m_mode == Mode::PEAK && s[i] > p.value) ||
                (m_mode == Mode::VALLEY && s[i] < p.value))
                break;
            leftBase = (m_mode == Mode::PEAK)
                ? std::min(leftBase, s[i])
                : std::max(leftBase, s[i]);
        }

        double rightBase = p.value;
        for (int i = p.position + 1; i < (int)s.size(); ++i) {
            if ((m_mode == Mode::PEAK && s[i] > p.value) ||
                (m_mode == Mode::VALLEY && s[i] < p.value))
                break;
            rightBase = (m_mode == Mode::PEAK)
                ? std::min(rightBase, s[i])
                : std::max(rightBase, s[i]);
        }

        double prominence = (m_mode == Mode::PEAK)
            ? p.value - std::max(leftBase, rightBase)
            : std::min(leftBase, rightBase) - p.value;

        if (prominence >= m_minProminence)
            out.push_back(p);
    }

    peaks.swap(out);
}


void FindPeak::applyDistanceFilter(std::vector<PeakInfo>& peaks)
{
    if (peaks.empty()) return;

    // sort by priority
    std::sort(peaks.begin(), peaks.end(),
        [this](const PeakInfo& a, const PeakInfo& b) {
            return (m_mode == Mode::PEAK)
                ? a.value > b.value
                : a.value < b.value;
        });

    std::vector<PeakInfo> selected;

    for (const auto& p : peaks) {
        bool conflict = false;
        for (const auto& s : selected) {
            if (std::abs(p.position - s.position) < m_minDistance) {
                conflict = true;
                break;
            }
        }
        if (!conflict)
            selected.push_back(p);
    }

    peaks.swap(selected);
}

void FindPeak::sortByPosition(std::vector<PeakInfo>& peaks)
{
    std::sort(peaks.begin(), peaks.end(),
        [](const PeakInfo& a, const PeakInfo& b) {
            return a.position < b.position;
        });
}

//--------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                                   End of file                                                                    //
//--------------------------------------------------------------------------------------------------------------------------------------------------//
