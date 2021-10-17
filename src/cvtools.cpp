#include "cvtools.hpp"

cv::Mat cvtools::addBorder(const cv::Mat &mat, int borderSize)
{
    cv::Mat img;
    cv::copyMakeBorder(mat, img, borderSize, borderSize,
                       borderSize, borderSize, cv::BORDER_CONSTANT);

    return img;
}

float cvtools::calcTruncated(const cv::Mat &mat, const cv::Rect &rect, const cv::Point& imgPos)
{
    // Coordinates of intersection area
    float xstart = std::max(imgPos.x, rect.x);
    float ystart = std::max(imgPos.y, rect.y);
    float xend = std::min(imgPos.x + mat.cols, rect.x + rect.width);
    float yend = std::min(imgPos.y + mat.rows, rect.y + rect.height);

    // Doesn't intersect at all
    if (xend < xstart || yend < ystart)
        return 1.f;

    float interArea = std::abs((xend - xstart) * (yend - ystart));
    float boxArea = rect.width * rect.height;

    float trunc = interArea / boxArea;

    return 1.f - trunc;
}
