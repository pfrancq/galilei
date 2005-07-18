#--------------------- GALILEI LIB -----------------------------

#VARIABLES USE :
#	$GALILEI_LIB environnement variable used for developpment
#	$GALILEI_INC_PATH = the path specified in the --with-galilei-includes
#	$GALILEI_LIB_PATH = the path specified in the --with-galilei-libs
#	$GALILEI_INC_DIR = the directory where the includes were found
#	$GALILEI_LIB_DIR = the directory where the libraries were found
#       $LIB_GALILEI = the link to the library needed by the application

#---------------------------------------------------------

# GALILEI_INC_CHK(package_Name,file to check (*.h))
# ------------------------------------------------------------
#
# ---- def function to check for a specific GALILEI includes on the system
AC_DEFUN(GALILEI_INC_CHK,[
	GALILEI_INC_DIR=""
	for i in $GALILEI_INC_PATH ; do
		if test -f $i/$1/$2 ; then
			GALILEI_INC_DIR="-I$i/$1"
			add="$i/$1"
			break
		fi
		if test -f $i/galilei/$2 ; then
			GALILEI_INC_DIR="-I$i/galilei"
			GALILEI_INSTALL="yes"
			add="$i/galilei"
			break
		fi
		if test -f $i/$2 ; then
			GALILEI_INC_DIR="-I$i"
			GALILEI_INSTALL="yes"
			add="$i"
			break
		fi
	done
	if test -z "$GALILEI_INC_DIR"; then
		if test "x$GALILEI_INC_PATH" != "x"; then
			AC_MSG_ERROR($3)
		else
			AC_MSG_ERROR($3 Use --with-galilei-includes=DIR to specify non-default path.)
		fi
	fi
	if test "x$add" != "x$PAST_GALILEI_INC_DIR"; then
		CPPFLAGS="$GALILEI_INC_DIR $CPPFLAGS"
	fi
	PAST_GALILEI_INC_DIR="$add"
])


# GALILEI_LIB_CHK(package_Name , lib to check)
# ----------------------------------------------
#
# ---- def function to check for a specific GALILEI library on the system
AC_DEFUN(GALILEI_LIB_CHK,[
	str="$1/$2.*"
	GALILEI_LIB_DIR=""
	for j in `echo $str`; do
		if test -r $j; then
		GALILEI_LIB_DIR=$1
		break 2
	fi
	done
])
# --- end def function -----


# GALILEI_ALL_INC_CHK(list-of-frontend)
# ----------------------------------------------
#
# ---- def function to check for all GALILEI Include on the system
AC_DEFUN(GALILEI_ALL_INC_CHK,[

	# ---- add configure option : --with-galilei-includes=DIR ----
	if test "x$GALILEI_LIB" != "x" ; then
		GALILEI_INC_PATH=$GALILEI_LIB
	else
		GALILEI_INC_PATH="/usr/include /usr/local /usr/local/include ${prefix}/include"
	fi
	AC_ARG_WITH(galilei-includes,
		AC_HELP_STRING(
			[--with-galilei-includes=DIR],
			[where the GALILEI includes are, default: [/usr/include/galilei]. ]),
		GALILEI_INC_PATH="$withval")

	# ---- check the presence of the includes for all the GALILEI libraries
	GALILEI_INC_CHK(galilei/docs,gdocs.h,Cannot find GALILEI headers.)
	GALILEI_INC_CHK(galilei/engines,gengine.h,Cannot find GALILEI headers.)
	GALILEI_INC_CHK(galilei/groups,ggroups.h,Cannot find GALILEI headers.)
	GALILEI_INC_CHK(galilei/historic,ggrouphistory.h,Cannot find GALILEI headers.)
	GALILEI_INC_CHK(galilei/infos,gdict.h,Cannot find GALILEI headers.)
	GALILEI_INC_CHK(galilei/profiles, gprofile.h,Cannot find GALILEI headers.)
	GALILEI_INC_CHK(galilei/sessions,gsession.h,Cannot find GALILEI headers.)

	for l in $1 ; do
		if test "$l" = "kde"; then
			GALILEI_INC_CHK(galilei/frontend/kde,qlistviewitemtype.h,Cannot find GALILEI headers for KDE frontend.)
		fi
	done
	MSG="$MSG, headers $i"
	AC_MSG_RESULT($MSG)
])
# --- end def function -----

# GALILEI_ALL_LIB_CHK(list-of-frontend)
# ------------------------------------------------------------
#
# ---- def function to check for all GALILEI Libraries on the system
AC_DEFUN(GALILEI_ALL_LIB_CHK,[

	# ---- add configure option : --with-galilei-libs=DIR ----
	if test "x$GALILEI_LIB" != "x" ; then
		GALILEI_LIB_PATH=$GALILEI_LIB
	else
		GALILEI_LIB_PATH="/usr/lib /usr/local /usr/local/lib ${prefix}/libs"
	fi
	AC_ARG_WITH(galilei-libs,
		AC_HELP_STRING(
			[--with-galilei-libs=DIR],
			[where the GALILEI libraries are installed, default: [/usr/lib]. ]),
		GALILEI_LIB_PATH="$withval")

	# ---- end arg for configure ----

	# ---- check the presence of all GALILEI libraries
	GALILEI_LIB_DIR=""
	for i in $GALILEI_LIB_PATH ; do
		for t in $i/galilei $i; do
			str="$t/libgalilei.*"
			for j in `echo $str`; do
				if test -r $j; then
					GALILEI_LIB_DIR=$j
					break 2
				fi
			done
		done
		if test -z "$GALILEI_LIB_DIR"; then
			AC_MSG_ERROR(Cannot find GALILEI libraries)
		fi
		if test "x$GALILEI_LIB_DIR" != "x$PAST_GALILEI_LIB_DIR"; then
			LDFLAGS="-L$GALILEI_LIB_DIR $LDFLAGS"
		fi
		PAST_GALILEI_LIB_DIR="$GALILEI_LIB_DIR"
		LIB_GALILEI="$GALILEI_LIB_DIR $LIB_GALILEI"
	done
	MSG="libraries $i"
	GALILEIKDE_LIB_DIR=""
	for i in $GALILEI_LIB_PATH ; do
		for t in $i/galilei/frontend/kde $i/galilei $i; do
			str="$t/libgalileikde.*"
			for j in `echo $str`; do
				if test -r $j; then
					GALILEIKDE_LIB_DIR=$j
					break 2
				fi
			done
		done
		PAST_GALILEIKDE_LIB_DIR="$GALILEIKDE_LIB_DIR"
		LIBKDE_GALILEI="$GALILEIKDE_LIB_DIR $LIBKDE_GALILEI"
	done
])


# GALILEI_CHK(list-of-frontend)
# ----------------------------------------------
#
# ---- def function to check for all GALILEI includes and libraries on the system
AC_DEFUN(GALILEI_CHK,[
	AC_MSG_CHECKING(for GALILEI)
	GALILEI_ALL_LIB_CHK($1)
	GALILEI_ALL_INC_CHK($1)
	AC_SUBST(LIB_GALILEI)
	AC_SUBST(LIBKDE_GALILEI)
])
# --- end def function -----
