Implementation:

-Testing monster and player requires only their respective .h and .cpp files
-using the visualizer requires the following:
	
	loadBMP.cpp added to the makefile and directory
	use my version of util.h and util.cpp
	pics folder added to the working directory

Notes: Player and monster used pathfinding based on my interpretation of the A* algorithm. Further expanation in the comment. The visualizer tends to slow down and even stall on some more comlpicated maps. I expect its because of memory leaks in the openGL portion. Most of these functions aren't mine, but given more time I would find the leaks and fix them to get a more stable GUI.

Flavor text:
Welcome to the Adventures of Aamnaam and Artix! 

(they both go by A* for short, conveniently enough) 

Artix will do his very best to run from the fuzzy and quite fatal grasp of Aamnaam. Aamnaam is the misunderstood monster who just wants a hug, and is willing to hunt Artix down to get one. 

Let the games begin!
