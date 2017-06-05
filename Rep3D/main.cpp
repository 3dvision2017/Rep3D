/**
    main.cpp
    Purpose: example main function
    used this to obtain results in report

    @authors Pascal Enderli, Lukas Frischknecht, Timothy Marvel
    @version 1.0 6/01/17
*/

#include <iostream>
#include <Eigen/Dense>
#include "Rep3D.h"
using namespace std;

int main(int argc, char** argv)
{


  Eigen::initParallel(); //initialize Eigen in parallel mode
  cout << argv[0];
  string fullfilename_colmap_cameras  = string(argv[1]);			// path to cameras.txt file 
  string fullfilename_colmap_images   = string(argv[2]);			// path to images.txt file
  string fullfilename_colmap_points3D = string(argv[3]);			// path to points3D.txt file
  string fullFolderName_rep_folder    = string(argv[4]);			// path to folder where rep files are stored

  Rep3D data_rep3d;

  data_rep3d.read(fullfilename_colmap_images,fullfilename_colmap_points3D,fullfilename_colmap_cameras,fullFolderName_rep_folder);

  data_rep3d.calc_proj_planes();					//calculates the projectionplane for every repetition and saves as member
  data_rep3d.project_raw_reps(); 					//Project alls 2D rep dots to coresponding plane E in 3D space
  data_rep3d.writeRawProjRepsToCSV("rawProjReps.csv");
  data_rep3d.print_VRML("output_raw.wrl");
  data_rep3d.find_optimal_plane();
  data_rep3d.project_optimal_reps();
  data_rep3d.print_VRML("output_optPlanes.wrl");
  data_rep3d.writeRawProjRepsToCSV("repsOnFacades.csv");
  data_rep3d.find_optrep();
  data_rep3d.print_VRML("output_optimized.wrl");


  data_rep3d.writeRawProjRepsToCSV("finalReps.csv");
  data_rep3d.writeCloudToCSV();
  data_rep3d.writeClustersToCSV();

  data_rep3d.show_plot();

  return 0;
}


