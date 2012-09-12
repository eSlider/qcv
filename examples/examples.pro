TEMPLATE = subdirs

unix:!embedded:!mac:CONFIG += x11

SUBDIRS	 =	toyClockExample \
                checkStereoPair \                
                anaglyphStereo  \
                helloWorld \
                imgPlayer \
                sobelExample
