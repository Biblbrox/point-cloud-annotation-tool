#ifndef CVTOOLS_HPP
#define CVTOOLS_HPP

#include <opencv2/imgproc.hpp>
#include <algorithm>


namespace cvtools
{
    /**
     * Add border to image
     * @param mat
     * @param borderSize
     * @return
     */
    cv::Mat addBorder(const cv::Mat& mat, int borderSize);

    /**
     * Calculate truncation of rect in image coordinates.
     * @param mat
     * @param rect
     * @param mat
     * @param rect
     * @param imgPos
     * @return float from 0 to 1x
     */
    float calcTruncated(const cv::Mat& mat, const cv::Rect& rect, const cv::Point& imgPos);
};

#endif //CVTOOLS_HPP
