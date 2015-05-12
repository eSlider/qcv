Instructions to use New Tsukuba data set (http://cvlab-home.blogspot.com/2012/05/h2fecha-2581457116665894170-displaynone.html) with QCV:

 1.- Download data sets

 2.- Untar NewYsukuba.tar.gz under data set directory (on the parent directory containing the groundtruth and illumination directories). This will place a camera.xml and poses.txt in the left directory of each data set.

 3.- Edit sequence_NewTsukuba*.txt and adapt the path to the downloaded data set. You can use sed here. In a bash terminal, run:

   mynewtsukubapath="path_to_newtsukuba_dataset"; sed -i "s@/home/hbadino/datasets/@$mynewtsukubapath@g" *.xml

  replace path_to_newtsukuba_dataset with the path to the newtsukuba data set in your system.

 4.- run from bin directory: 
    ./stereoSFM params_stereoSFM/newtsukuba/sequence_NewTsukubaFluorescent.xml params_stereoSFM/newtsukuba/params_stereoSFM.xml

Enjoy!

