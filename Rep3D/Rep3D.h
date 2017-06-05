/**
    Rep3D.h
    Purpose: header file of Rep3D class

    @authors Pascal Enderli, Lukas Frischknecht, Timothy Marvel
    @version 1.0 6/01/17
*/

#pragma once

#include <iostream>
#include <string>			// std::string
#include <Eigen/Dense>
#include <Eigen/Geometry> 	// quaternion; crossproduct
#include <fstream>
#include <vector>			// std::vector
#include <deque>			// std::deque
#include <stdlib.h> 
#include <cstddef>         	// std::size_t
#include <algorithm>	   	// std::find()
#include <iomanip>
#include <dirent.h>
#include <omp.h>			// Multithreading
#include <ctime>			// take computing time
#include <sstream>
#include <cmath>
#include "DBSCAN.hpp"		// dbscan clustering
#include "camera.h"			// print vmrl file
#include "v3d_vrmlio.h"		// print vmrl file
#include "tools.hpp"		// small mathematical operations


using namespace Eigen;
using namespace std;


class Rep3D
{

	private:

		struct image_data;
		struct singleRepetition;

		int n_MainPlanes;							//tells us how many walls (main planes the hough transform gives us)
		int n_images;								//number of images in the textfile
		int n_3Dpoints;								//number of 3Dpoints in the colmap textfile
		Vector3f verticalDir;						//vertical direction in the building


		struct plane								//vectorial plane of each rep in 3D E=P+a*e1+b*e2 (a;b free parameter)
		{
			//bool is_plane;						//if this rep has a plane or not
			Vector3f P;								//from Eigen lib
			Vector3f e1;
			Vector3f e2;
			Vector3f ne3;							//normalized normal vector of plane
			vector<singleRepetition*> rep_ptr;
			unsigned int index;						//deque index
		};


        struct point3d								//contains the point cloud for fast acces (can be updated with new points using...)
        {
            float ID_3D;							//ID of 3D_point (just for colmap read out data)
            float X;								//X coordinate of 3D point
            float Y;								//Y coordinate of 3D point
            float Z;								//Z coordinate of 3D point
        };


    /*
     Concerning repetition structure:
     declare the repetition grid containing the start and end point pairs (x,y)
     the following would be a 3 by 1 repetition
     NOTE: only the x are stored (start and end of lines), the o are not of interest, or can be
     derived from the x if needed in subsequent steps
     NOTE: stored in a start point and an end point vector
     NOTE: corners are the first and last entries of the start and end point vectors
     x-----x-----x-----x //
     |     |     |     | //
     x-----o-----o-----x //
     |     |     |     | //
     x-----x-----x-----x //
    */

		struct singleRepetition{						//contains the amount of different reps found and stores the location of
			string repID;								//consists of picturename+number of repetition
			vector<pair<float,float>> startPoints_2D;  	//stores the startpoints of the rep grid lines in 2D image space
			vector<pair<float,float>> endPoints_2D;		//stores the endpoints of the rep grid lines in 2D image space
			vector<pair<float,float>> startAlphaBeta; 	//stores the start alpha and beta in the plane on which the rep is projected
			vector<pair<float,float>> endAlphaBeta; 	//stores the end alpha and beta in the plane on which the rep is projected
			float n_vertLines;							//number of vertical lines
			float n_horLines;							//number of horizontal lines
			plane* planeptr;							//definition of the projection plane of the rep in 3D
			image_data* image_ptr;
			vector<point3d> startPoints_3D;  			//stores the startpoints of the rep grid lines in 3D space
			vector<point3d> endPoints_3D;				//stores the endpoints of the rep grid lines 3D space
			vector<point3d> corners;					//still in use?
			vector<pair<float,float>> cornerAlphaBeta;	//contains the(clockwise) featureCoord of the corner points in alpha and beta coord
			bool valid;									//if a repetition is valid or rubbish
			int weight=1;								//used when joinning the reps



		};


        struct features								//contains information about the 2D feautures (colmap images.txt)
        {
            vector<float> u;						//u coordinate of feauture
            vector<float> v;						//v coordinate of feauture
            vector<float> ID_3D;                    //ID of coresponding 3D point in points3D.txt (-1 means there is no matching 3D point)
            vector<point3d*> point3D;               //vector of pointers to the 3D cloud points
        };

		struct image_data							//contains all essential colmap information for one picture
		{
			string			image_name;
			int				n_2Dpoints;				//length of coordinate vector per image
			int 			numberOfReps;			//how many repetitions are in this picture
			int				image_ID;
			int				camera_ID;
			float			k;						//radial disortion correction rule: u=u*k*((u-cx)^2+(v-cy)^2)
			Matrix3f		R;						//Rotation Q=w+xi+yj+zk <double>
			Vector3f		T; 						//Translation [x;y;z]
			Matrix3f		K;						//Kalibration Matrix
			features	    featureCoord;			//contains 2D coordinates of feautures
			vector<singleRepetition> repetitions;	//information about the repetitions in the picture
		};


		vector<image_data> 		colrep_data;		//contain all essential colmap info (one vector element for each picture)
		vector<point3d> 		pointCloud;			//container for the 3D point cloud only
		deque<singleRepetition> reps;				//list of all repetitions

		deque<plane>			planes;				//list of all planes
		deque<deque<singleRepetition*>> repClusters;	//reps that overlap with one an other
		deque<singleRepetition*> optimalReps;
	
		void read_images_txt(string path);			//reads the text file images into the ram
		void read_camera_txt(string path);			//read camera text file into the ram
		void read_points3D_txt(string path);		//read points 3D text file into the ram
		void readRepFiles(string path);             //reads .rep file into the ram
		void readRepData(string folderLocation);

		void merge(int Cam_ID, float f,float cx, float cy, float k); // 2D info is in member variable
        void assign3Dpoints(void);
        plane RANSAC_plane(MatrixXf points);						//fit a plane in 3D for a given set of points
        pair<Vector3f,pair<float,float>> LGS(float u, float v, plane E,Vector3f* T,Matrix3f* K, Matrix3f* R);		//solve a LGS to find projected 3D point of a 2D rep node point
		MatrixXf get_3D_data(void);
		void get_3D_rep_lines(vector<vector<float> > &startpkt, vector<vector<float> > &endpkt); //searches for all rep lines of the dataset
		void writeRepLinesToVRML(string filename);
		void writeColmapPointsToVRML(string filename);
		void cleanUpOutlierPlanes(void);
		MatrixXf Hough_TF(void);
		plane Hough_back_TF(vector<float>);
		bool plane_is_valid(plane E);				// checks if a projection plane is used by any rep
		void GetRep3dInfo(void);
		void clusterReps(void);
		void optimizeRepClusters(void);
		void straightenReps(singleRepetition* rA);
		bool isRepParallel(int index);
		float point3dNorm(point3d ptA,point3d ptB);
		bool SameDimensions(singleRepetition* rA, singleRepetition* rB);
		void uniteRep(singleRepetition *rA, singleRepetition *rB);
		bool EdgesAtSameLocation(singleRepetition* rA, singleRepetition* rB);

		bool doTheyOverLap(singleRepetition* rA,singleRepetition* rB );
		void BiggestRepWins(singleRepetition *rA, singleRepetition *rB);
		void clusteringUsingCorners(void);


	public:
		Rep3D();								//constructor
		virtual ~Rep3D();						//destructor
		void read(string fullfilename_colmap_images,string fullfilename_colmap_points3D,string fullfilename_colmap_cameras, string fullFolderName_rep_folder); //reads the essential colmap data into var:"colrep_data"
        void calc_proj_planes(void);			//calculates the projectionplane for every repetition and saves as member
        void project_raw_reps(void); 			//Project alls 2D rep dots to coresponding plane E in 3D space
		void print_VRML(string filename);  		//Prints the VRML File
		void find_optimal_plane(void);
		void project_optimal_reps(void);
		void show_plot(void);
        void find_optrep();



    /*  writing csv's
        point cloud
        Format: 	ID1,X1,Y1,Z1
                    ID2,X2,Y2,Z2
                    .	.	.	.
     */
		void writeCloudToCSV(void);
		void writeRawProjRepsToCSV(string filename);
		void writeClustersToCSV(void);


};

