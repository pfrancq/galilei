CONTENT
-------
This repository contains the GALILEI projects. It contains 4 directories:
1. The "galilei" directory contains the main library provinding the platform. It implements the basic classes and the logic.
2. The "plugins" directory contains different plugins implementing algorithms for the different tasks (extraction information, build profile, compute similarities, etc.).
3. The "upgalilei" directory provides a simple program that launches the update process of the platform (analyze new documents, update profiles and communities, etc.). This can launched as a "cron" job.
4. The "qgalilei" directory provides a Qt-based desktop program to launch tasks and view results.


BUILD
-----
Each directory contains a README file describing the way to build the code. It should be notice that the GALILEI platform supposes that a database exist.

By default, the GALILEI platform relies on MySQL. The directory "plugins/gmysql/gmysql" contains an empty database that can be used for creation.


MORE INFORMATION
----------------
More details on the GALILEI platform are available on the following Web pages:

http://www.otlet-institute.org/GALILEI_Platform_en.html

http://www.otlet-institute.org/wikics/GALILEI_Framework.html
