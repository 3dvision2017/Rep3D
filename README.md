Rep3D
======

About
-----
Detection of repeated structures in SfM.
Find further details on https://3dvision2017.github.io/Rep3D/

Dependencies
---------------
Eigen3, OpenMP, c++11

I/O
---------------
Input:
From [COLMAP](https://colmap.github.io/) you need:
* cameras.txt
* images.txt
* points3D.txt

From [Single View Repetition Detection](http://ccwu.me/code.html) you need:
for each picture XYZ the following two files	
* PXYZ.rep
* PXYZ.rep2
	
	
Example, running the code from installation folder:
	main.cpp 
using the example main.cpp you can pass the path to the above files in the following order:
	path2cameras.txt path2images.txt path2points3d.txt path2folderContainingRepFiles

Output:
Checkout the example main.cpp, you can save the point cloud and the projected repetitions in VRML and CSV format wiht the following functions:
* print_VRML(path2saveFile)
* writeRawProjRepsToCSV(path2saveFile)
* writeCloudToCSV(path2saveFile)
* writeClustersToCSV(path2saveFile)

Set Up
---------------
Example:
1) clone to folder
2) unzip test data
3) make
4) run it with: 
	./main test_data/CAB_ETH/colmap/cameras.txt test_data/CAB_ETH/colmap/images.txt test_data/CAB_ETH/colmap/points3D.txt 			test_data/CAB_ETH/rep

Visualisation
---------------
You can use the vis.py file to visualise the processed data. You will need the [VisPy](http://vispy.org/) and [numpy](http://www.numpy.org/) package. The vis.py file is just an example of how to visualise the data.


License
-------

The software is licensed under the GNU General Public License v3 or later. If
you are interested in licensing the software for commercial purposes, without
disclosing your modifications, please contact the authors.

    Rep3D
    Copyright (C) 2017  Pascal Enderli, Lukas Frischcknecht, Timothy Marvel

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
