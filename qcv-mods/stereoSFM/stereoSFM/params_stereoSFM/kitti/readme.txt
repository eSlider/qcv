Instructions to use KITTI data sets (http://www.cvlibs.net/datasets/kitti/eval_odometry.php) with QCV:

 1.- Download data sets

 2.- Untar kitti.tar.gz under data set directory (on the parent directory containing the 00, 01, 02, ..., 21 directories). This will place a camera.xml and poses.txt in the image_0 directory of each data set.

 3.- Edit sequence_kitti??.txt and adapt the path to the downloaded data set. You can use sed here. In a bash terminal, run:

   mykittipath="path_to_kitti_dataset"; sed -i "s@/home/hbadino/datasets@$mykittipath@g" *.xml

  replace path_to_kitti_dataset with the path to the kitti data set in your system.

 4.- run from bin directory: 
    ./stereoSFM params_stereoSFM/kitti/sequence_kitti00.xml params_stereoSFM/kitti/params_stereoSFM.xml

Enjoy!

