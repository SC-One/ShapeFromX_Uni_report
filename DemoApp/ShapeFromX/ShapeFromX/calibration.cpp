#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
//// Function to convert cv::Mat to QString with comma-separated values
// QString matToString(const cv::Mat &mat) {
//  std::ostringstream oss;
//  for (int i = 0; i < mat.rows; ++i) {
//    for (int j = 0; j < mat.cols; ++j) {
//      oss << mat.at<float>(
//          i, j); // Change 'float' to the type of your matrix if it's
//          different
//      if (j < mat.cols - 1) {
//        oss << ","; // Add comma if not at the end of a row
//      }
//    }
//    oss << "\n"; // Newline at the end of each row
//  }
//  return QString::fromStdString(oss.str());
//}

// https://learnopencv.com/camera-calibration-using-opencv/

// Defining the dimensions of checkerboard
int CHECKERBOARD[2]{7, 7}; // chess game it's 7x7 (max_Rindex * max_Cindex)

int main() {
  std::cout << "Main func started\n";
  // Creating vector to store vectors of 3D points for each checkerboard image
  std::vector<std::vector<cv::Point3f>> objpoints;

  // Creating vector to store vectors of 2D points for each checkerboard image
  std::vector<std::vector<cv::Point2f>> imgpoints;

  // Defining the world coordinates for 3D points
  std::vector<cv::Point3f> objp;
  for (int i{0}; i < CHECKERBOARD[1]; i++) {
    for (int j{0}; j < CHECKERBOARD[0]; j++)
      objp.push_back(cv::Point3f(j, i, 0));
  }

  // Extracting path of individual image stored in a given directory
  std::vector<cv::String> images;
  // Path of the folder containing checkerboard images
  std::string path = "C:/Users/SC/Documents/prj/ComputerVision_class/"
                     "monocular-visual-odometry/board_3/*.jpg";

  cv::glob(path, images);

  cv::Mat frame, gray;
  // vector to store the pixel coordinates of detected checker board corners
  std::vector<cv::Point2f> corner_pts;
  bool success;

  std::cout << std::endl;
  // Looping over all the images in the directory
  for (int i{0}; i < images.size(); i++) {
    std::cout << "Started: " << images[i] << std::endl;
    frame = cv::imread(images[i]);
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Finding checker board corners
    // If desired number of corners are found in the image then success = true
    success = cv::findChessboardCorners(
        gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts,
        cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK |
            cv::CALIB_CB_NORMALIZE_IMAGE);

    std::cout << "success: " << success;
    /*
     * If desired number of corner are detected,
     * we refine the pixel coordinates and display
     * them on the images of checker board
     */
    if (success) {
      cv::TermCriteria criteria(cv::TermCriteria::Type::EPS |
                                    cv::TermCriteria::Type::MAX_ITER,
                                30, 0.001);

      // refining pixel coordinates for given 2d points.
      cv::cornerSubPix(gray, corner_pts, cv::Size(11, 11), cv::Size(-1, -1),
                       criteria);

      // Displaying the detected corner points on the checker board
      cv::drawChessboardCorners(frame,
                                cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]),
                                corner_pts, success);

      objpoints.push_back(objp);
      imgpoints.push_back(corner_pts);
    }

    //    cv::imshow("Image", frame);
    //    cv::waitKey(0);
  }

  cv::destroyAllWindows();

  cv::Mat cameraMatrix, distCoeffs, R, T;

  /*
   * Performing camera calibration by
   * passing the value of known 3D points (objpoints)
   * and corresponding pixel coordinates of the
   * detected corners (imgpoints)
   */
  // camera_matrix=[fx 0 cx; 0 fy cy; 0 0 1]
  cv::calibrateCamera(objpoints, imgpoints, cv::Size(gray.rows, gray.cols),
                      cameraMatrix, distCoeffs, R, T);

  std::cout << "cameraMatrix : \n" << cameraMatrix << std::endl;
  std::cout << "distCoeffs : \n" << distCoeffs << std::endl;
  std::cout << "Rotation vector : \n" << R << std::endl;
  std::cout << "Translation vector : \n" << T << std::endl;

  return 0;
}
