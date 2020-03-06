# libproject / libproj4

WARNING: this is not an active project, but an attempt at preserving the content
of http://libproj4.maptools.org/ and
https://web.archive.org/web/20130802001811/home.comcast.net/~gevenden56/proj/index.html

This repository contains 3 versions:
* [tag REL_050319](https://github.com/rouault/libproj4/tree/REL_050319) for a release dating from 2005/03/19, coming from http://libproj4.maptools.org/dl/lbp4_050319S.tar.gz
* [tag v4.3](https://github.com/rouault/libproj4/tree/v4.3) for a release dating from 2008/11/20 (latest known, current "master"), coming from https://karczmarczuk.users.greyc.fr/TEACH/InfoGeo/Work/libproj4.3_20081120.tar.bz2 or https://web.archive.org/web/20130802001811/home.comcast.net/~gevenden56/proj/libproj4.3_20081120.tar.bz2)
* [tag libproject-1.01](https://github.com/rouault/libproj4/tree/libproject-1.01), with latest changes dating from 2009/05/22, coming from https://web.archive.org/web/20130802001811/home.comcast.net/~gevenden56/proj/libproject-1.01.tar.bz2

libproj4 was forked from https://github.com/OSGeo/PROJ (at that time under CVS),
known as PROJ.4, circa 2002 by [Gerald I. Evenden](https://www.wikidata.org/wiki/Q30467686).

libproject is a successor to libproj4, with additional comments and autoconf support.

## libproject: Introduction given on https://web.archive.org/web/20130802001811/home.comcast.net/~gevenden56/proj/index.html

Cartographic Projections / libproject C library, successor to proj, proj4 and libproj4

Purpose and scope of library

The general purpose of this library is to provide C language procedures related to cartographic processes. Procedures for each of the processes will be strictly categorized and although they may share common subfunctions they will not intersect in scope.
Current procedure categories consist of:

* Cartographic projections---the largest library section consisting of well over 180 entries.
* Geodesic procedures---procedures for determining the properties of the shortest path on a sphere or ellipsoid.
* [future] Bivariate approximation---resurrection of original proj's approximation procedure. Seems to be still useful.
* General support---general support procedures such as [de-]formating geographic coordinates, controlling option lists, selecting ellipsoids, ... .
* [vague future] Datum conversions---in part will include resurrected material from original proj distribution as well as alternative methods,

In addition to the library will be programs for usage of one or more library elements:

* lproj---semi-production procedure for performing cartographic projections on tabular data. lproj is currently part of the libproj4 distribution.
* project---(proposed) is an expanded version of lproj with more user friendly interface. Eventually will replace lproj.
* geodesic---a user friendly, command driven replacement for program geod of the original proj distribution.

All libproject software will be distributed with configure and associated tools for installation on Linux/Unix class platforms. Shared library will be installed when applicable. Sorry, no Microsoft support available. 

Current distribution:
* libproj4.3_20081120.tar.bz2   "Mature," thus final, distribution of libproj4.
* libproject-1.01.tar.bz2       libproject that includes support, geodesic procedures and cartographic projection procedures.
* geodesic-1.0.tar.bz2          Program geodesic. A user friendly access to the geodesic procedures. It requires libproject.
* [OF90-284.pdf](https://web.archive.org/web/20130802001811/home.comcast.net/~gevenden56/proj/OF90-284.pdf)     1990 USGS Open-File Report manual of projection system. Useful reading for description of some attributes of program proj utilization which later became program lproj in the libproj4 distribution.
* [manual.pdf](https://web.archive.org/web/20130802001811/home.comcast.net/~gevenden56/proj/manual.pdf)     Current manual for libproj4 which is more complete mathematical description of projections.
* sproj-1.0.tar.bz2             Basic cartographic projection program. Requires libproject library.

### Note1 from Gerald I. Evenden / Disclaimer and disassociation

The "proj" system was made available in the early 1990's and has undoubtedly been used in both open and proprietary applications as is allowed by its public domain origins and subsequent MIT/X11 license. In nearly all cases, use of proj has been without with my participation or consultation, however, there has been one important exception and I want to make it clear of my disassociation with that operaton.

A distribution of 'proj' ([see this site](https://trac.osgeo.org/proj)) is available that represent coding practices that compromised the quality of 'proj,' its management and use. For several years I was associated with this group's ([mailing list archive](https://lists.osgeo.org/pipermail/proj)) and was a persistant nuisance complaining from the year 2000 of the coding abuse but to no avail. I finally decided that for my mental health it was time to get away from it.

What is the problem? The problem relates to the issue of interweaving into one software collection two separate operations and disciplines: datum conversion and cartographic projections. Indeed, in many cases both operations are important in GIS applications and chart making but the operations are distinct and independent of each other and it is very poor programming practice to tightly combine the processes. That is, we do not alter the math library sine function to somehow facilitate the use of 'sin' in an application.

Why make such an issue about it? The principle reason for making such a big deal over this problem is that it makes the maintenance of the software more difficult and thus more prone to errors and more difficult to include enhancements and updates. People are also more likely to be expert in one or the other aspect of the problem and they should not be burdened with trying to wade though sections of code that have nothing to do with with their background. All the two disciplines should be concerned with is the interfacing between their sections.

After 9 years of bashing my head on this problem I finally gave up.

## libproj4: Introduction given on http://libproj4.maptools.org/

This site is the primary source for the cartographic projection library libproj4.  This library supports over 170 projections including many used in national grid systems.  Many more projections are available by selection of appropriate values for control parameters of current base projections.

Circa 2002 the libproj4 system is an outgrowth of the author's PROJ.4 system and corrected inconsistencies of the old system and  improved overall operation.  It also ensured that identifier namespace became restricted to PJ_*/pj_*.  The old PROJ.4 system is still available at some web sites such as Remote Sensing Organization site.  The web sites associated with PROJ.4 may have performed their own modification to the PROJ.4 library so there is no guarantee of the same collection of projections nor functional equivalence.

Notes on Installation of libproj4 system
Because of the extrodinarily extreme complexities of the manuals for making the typical "configure" procedure and the fact that libproj4 is an easy system to install it is left to the installer to do necessary "tweeks" to the Makefile.  In most circumstances, simply
```
make libproj4
make lproj
make install
```
is sufficient.  But be sure to check the Makefile to make sure the destination directories are suitable for your system.  Also check the README file for any additional caveats and suggestions.

The manual is slowly evolving.  Information provided describes usage of the library and many internal functions but complete documentation of the projections is incomplete.  Use the older manuals at the sites listed at the top of this page for material on
projections not described in the manual.pdf
