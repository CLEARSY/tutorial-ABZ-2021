# tutorial-ABZ-2021

![CLEARSY Safety Platform Tutorial](https://github.com/CLEARSY/tutorial-ABZ-2021/blob/main/pictures/SS-tuto.jpg)

__Using B to program the CLEARSY Safety Platform__

This tutorial presents the programming model of the CLEARSY Safety Platform to specify, develop and prove command and control applications with B. A number of examples are exploited to demonstrate how the platform could be used for education, bridging the gap between formal methods and embedded systems / automation.

The tutorial is given by:
- Thierry LECOMTE. Thierry is R&D Director at CLEARSY and has been involved in safety and security formal modelling research for decades. He led the LCHIP (Low Cost High Integrity Platform) project during 4 years to build up the CLEARSY Safety Platform.
- Bruno LAVAUD. Bruno is lead developer at CLEARSY and was heavily involved in the development and certification of the CLEARSY Safety Platform.

This tutorial completes the talk "Programming the CLEARSY Safety Platform" by providing more practical details on how to use it for education and research. This tutorial was first given during the RSSR 2017 conference (https://conferences.ncl.ac.uk/rssrail/tutorials/#tut2). The tutorial proposed for ABZ 2021 will contain more material based on feedback collected from teachers. It will also present the evolution from the SK0 board, its simulation-based software only version released in 2021 and its industry version, coming with a more open programming model.

__Attending__  
The tutorial will be run on June 8th 2021 at 3:30pm-5:30pm CET, within the framework of the ABZ 2021 international conference (https://abz2021.uni-ulm.de/).  
It is freely accessible to anyone through the following registration link: https://my.demio.com/ref/XCAMy6daO1HAhmUl  

__Outline__ (Duration : 2 hours)

With this tutorial, you are going to learn on a concrete example how to use a formal method for "serious thing" - that is far more than model, predicate and proof. We will have a look at:
- the system - a railway level crossing - chosen as anyone knows the subject and because it is complex enough to demonstrate the approach
- the software - mainly developed formally. The formal notation is introduced when required (it is not a course on B)
- the physical implementation including some bits of electronics

A level crossing controller is going to be exercised with several boards and different environments to demonstrate how the CLEARSY Safety Platform could be used for safety critical applications.
![Level Crossing](https://github.com/CLEARSY/tutorial-ABZ-2021/blob/main/pictures/SS-CS.jpg)

__Agenda__
1. Introduction
   1. Safety, standards & embedded systems
   2. The railway crossing example
   3. The CLEARSY Safety Platform in a nutshell
2. The CLEARSY Safety Platform for education
   1. The starter kit SK0
   2. The software simulator (VM)
   3. Implementing the level crossing controller
3. The CLEARSY Safety Platform for industry
   1. The safety core computer CS0
   2. Implementing the level crossing controller
3. Conclusion 

__Resources__  
Slides, models, and source code are hosted at https://github.com/CLEARSY/tutorial-ABZ-2021  
Most resources will be released after the tutorial, to keep the suspense intact.

__Requirements__
- intermediate software development, 
- comfortable with formal logic. 

__Skills learned__
- introduction to safety (software, electronics, system)
- programming a logic controller with B

__Contributers__
- Thierry Lecomte: slides, edition, live presentation
- Bruno Lavaud: CS0, C code, specific electronics, live presentation
- Patrick Péronne: SK0, software simulator, level crossing example
- Lilian Burdy: B models

Shield: [![CC BY 4.0][cc-by-shield]][cc-by]

This work is licensed under a
[Creative Commons Attribution 4.0 International License][cc-by].

[![CC BY 4.0][cc-by-image]][cc-by]

[cc-by]: http://creativecommons.org/licenses/by/4.0/
[cc-by-image]: https://i.creativecommons.org/l/by/4.0/88x31.png
[cc-by-shield]: https://img.shields.io/badge/License-CC%20BY%204.0-lightgrey.svg