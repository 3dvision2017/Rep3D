# visualisation of point cloud, repetitions in 3D and/or clusters of repetitions
# run after processing of data: vis.py
import csv
import sys
import numpy as np
from vispy import scene
from vispy.scene import visuals
from vispy.color import ColorArray
from vispy.color import Colormap
from random import randint



## main parameters
# point cloud
plotCloud = True
cloudFile="../data/processed/pointCloud.csv"
# clusters
plotClusters = False
clustersFile="../data/processed/repClusters.csv"



# repetition before hough transform
plotRawReps = False
rawReps="../data/processed/rawProjReps.csv"

# repetitions after hough transform
plotFilteredReps = False
repsonFacades="../data/processed/repsonfacades.csv"

# final repetitions after optimizing
plotFinalRep = True
finalReps="../data/processed/finalReps.csv"

# set which repetitions to use (file is plotted)
file = finalReps;
plotReps =True;


# vispy
repetition_line_width = 6;
camera_view ='arcball'
cm = Colormap(['r', 'g', 'b']) # colormap for later

################################################## functions ###########################################################


def get_PointCloud(cloudFileCsv):
    with open(cloudFile, 'r') as csvfile:

        cloudPos=[]

        reader = csv.reader(csvfile, delimiter=',')
        next(reader, None) #skip header
        for row in reader:
            cloudPos.append([float(row[1]), float(row[2]), float(row[3])])

    return np.array(cloudPos)

def get_repetitionClusters(clusterFileCsv, vispy_colormap):
    with open(clustersFile, 'r') as csvfile:
        clusterLines = np.zeros((1,4)) # 2D positions
        # Create an array of point connections :
        toconnect = [] #np.array([[0,1], [0,2], [1,4], [2,3], [2,4]])
        reader = csv.reader(csvfile, delimiter=',')
        next(reader, None) #skip header
        start = np.array
        end = np.array
        for counter,row in enumerate(reader):
            start = [float(row[1]),float(row[2]),float(row[3]),int(row[0])]
            end = [float(row[4]),float(row[5]),float(row[6]),int(row[0])]
            clusterLines = np.append(clusterLines,[start,end],axis=0)
            toconnect.append([counter*2,counter*2+1])

    clusterLines = np.delete(clusterLines,0,0)
    nrOfClusters = np.max(clusterLines[:,3])
    colors = []
    clusternrs = range(0,int(nrOfClusters+1),1)

    for counter,nr in enumerate(clusternrs):
        colInt = randint(0,nrOfClusters)
        for entry in clusterLines:
            if int(entry[3]) == nr:
                colors.append(cm[colInt*1/nrOfClusters].rgb)


    colors = np.array(colors)
    colors = colors.squeeze()
    toconnect = np.array(toconnect)
    clusterLines=clusterLines[:,0:3:1]
    dict = {'points' : clusterLines, 'connections' : toconnect, 'colors' : colors}

    return dict

def get_finalReps(finalRepetionsFileCsv, vispy_colormap):
    with open(file, 'r') as csvfile:
        replines = np.zeros((1,3)) # 2D positions
        # Create an array of point connections :
        repstoconnect = [] #np.array([[0,1], [0,2], [1,4], [2,3], [2,4]])
        reader = csv.reader(csvfile, delimiter=',')
        next(reader, None) #skip header

        start = np.array
        end = np.array
        ids = []
        for counter,row in enumerate(reader):
            start = [float(row[1]),float(row[2]),float(row[3])]
            end = [float(row[4]),float(row[5]),float(row[6])]
            replines = np.append(replines,[start,end],axis=0)
            repstoconnect.append([counter*2,counter*2+1])
            ids.append(row[0])
            ids.append(row[0])
    replines = np.delete(replines,0,0)
    uniqueids = list(set(ids))
    repcolors = np.zeros((replines.shape[0],3))
    for repNr,id in enumerate(uniqueids):
        for i,i_id in enumerate(ids):
            if i_id==id:
                repcolors[i] = cm[1/len(uniqueids)*repNr].rgb
                #repcolors[i] = cm[0.77777].rgb

    repstoconnect=np.array(repstoconnect)
   # repcolors = (1, 140/255, 0, 1.0);
    dict = {'points' : replines, 'connections' : repstoconnect, 'colors' : repcolors}

    return dict


################################################# Prepare the Data using read in functions ##############################

# get the cloud data from csv file
if plotCloud:
    cloudPos = get_PointCloud(cloudFile)


# get the clusters from csv files
if plotClusters:
    clusters = get_repetitionClusters(clustersFile,cm)
    clusters_line_width = 2;


# get the final reps from csv data
finalReps = get_finalReps(file,cm)


# test = get_rawReps()
######################################################### Draw #########################################################
# Make a canvas and add simple view (copied from https://github.com/vispy/vispy/issues/1257)


canvas = scene.SceneCanvas(keys='interactive', show=True, bgcolor='w')

view = canvas.central_widget.add_view()
view.camera = camera_view
view.camera.fov = 60
view.camera.center=[0,0,0]

#plot all reps
if plotReps:
    repsVis = visuals.Line(pos=finalReps['points'], connect=finalReps['connections'],parent=view.scene, width=repetition_line_width,color=finalReps['colors'])
#plot clusters
if plotClusters:
    clusterVis = visuals.Line(pos=clusters['points'] , connect=clusters['connections'],parent=view.scene, color=clusters['colors'],width=clusters_line_width)

# cloud scatter
if plotCloud:
    scatter = visuals.Markers(parent=view.scene)
    scatter.antialias = 0
    scatter.set_data(pos=cloudPos, edge_color=(0.0, 0.0, 0.0, 1.0), face_color=(0.6, 0.5, 0.4, 1.0), size=1)
    scatter.set_gl_state(depth_test=True, blend=True, blend_func=('src_alpha', 'one_minus_src_alpha'))



# Add axes
axis = visuals.XYZAxis(parent=view.scene)

# execute
if __name__ == '__main__' and sys.flags.interactive == 0:
    canvas.app.run()





