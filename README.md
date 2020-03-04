# libproj4

WARNING: this is not an active project, but an attempt at preserving the content
of http://libproj4.maptools.org/

libproj4 was forked from https://github.com/OSGeo/PROJ (at that time under CVS),
known as PROJ.4, circa 2002 by Gerald I. Evenden.

Quote from http://libproj4.maptools.org/:
```
This site is the primary source for the cartographic projection library libproj4.  This library supports over 170 projections including many used in national grid systems.  Many more projections are available by selection of appropriate values for control parameters of current base projections.

Circa 2002 the libproj4 system is an outgrowth of the author's PROJ.4 system and corrected inconsistencies of the old system and  improved overall operation.  It also ensured that identifier namespace became restricted to PJ_*/pj_*.  The old PROJ.4 system is still available at some web sites such as Remote Sensing Organization site.  The web sites associated with PROJ.4 may have performed their own modification to the PROJ.4 library so there is no guarantee of the same collection of projections nor functional equivalence.

Notes on Installation of libproj4 system
Because of the extrodinarily extreme complexities of the manuals for making the typical "configure" procedure and the fact that libproj4 is an easy system to install it is left to the installer to do necessary "tweeks" to the Makefile.  In most circumstances, simply
make libproj4
make lproj
make install
is sufficient.  But be sure to check the Makefile to make sure the destination directories are suitable for your system.  Also check the README file for any additional caveats and suggestions.

The manual is slowly evolving.  Information provided describes usage of the library and many internal functions but complete documentation of the projections is incomplete.  Use the older manuals at the sites listed at the top of this page for material on
projections not described in the manual.pdf
```
