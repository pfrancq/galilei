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
	str="$1/$2"
	for i in $GALILEI_INC_PATH ; do
		test -f $i/$str                    && GALILEI_INC_DIR=$i/$1 && break
		#test -f $i/galilei/$str              && GALILEI_INC_DIR=$i/galilei/$1 && break
	done
	AC_MSG_CHECKING(checking for $1/$2)
	if test -z "$GALILEI_INC_DIR"; then
		if test "x$GALILEI_INC_PATH" != "x"; then
			AC_MSG_RESULT(no)
			AC_MSG_ERROR(Cannot find $1 header files under $GALILEI_INC_PATH)
		else
			AC_MSG_RESULT(no)
			AC_MSG_ERROR(Cannot find $2 headers.  Use --with-galilei-includes=DIR to specify non-default path.)
		fi
	fi
	AC_MSG_RESULT(yes in $GALILEI_INC_DIR)

	if test "x$i" != "x$PAST_GALILEI_INC_DIR"; then
		CPPFLAGS="-I$i $CPPFLAGS"
	fi
	PAST_GALILEI_INC_DIR="$i"
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
		GALILEI_INC_PATH="/usr/include /usr/include/galilei /usr/local /usr/local/include /usr/local/include/galilei ${prefix}/include/galilei"
	fi
	AC_ARG_WITH(galilei-includes,
		AC_HELP_STRING(
			[--with-galilei-includes=DIR],
			[where the GALILEI includes are, default: [/usr/include/galilei]. ]),
		GALILEI_INC_PATH="$withval")

	# ---- end arg for configure ----

	
	# ---- check the presence of the includes for all the GALILEI libraries
	GALILEI_INC_CHK(docs,gdocs.h)
	GALILEI_INC_CHK(engines,gengine.h)
	GALILEI_INC_CHK(groups,ggroups.h)
	GALILEI_INC_CHK(historic,ggrouphistory.h)
	GALILEI_INC_CHK(infos,gdict.h)
	GALILEI_INC_CHK(profiles, gprofile.h)
	GALILEI_INC_CHK(sessions,gsession.h)
	
	for l in $1 ; do
		if test "$l" = "kde"; then
			# ---- check the presence of the includes for all the GALILEI frontend libraries ----
			GALILEI_INC_CHK(frontend/kde,qlistviewitemtype.h)
		fi
	done
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
		GALILEI_LIB_PATH="/usr/lib /usr/lib/galilei /usr/local /usr/local/lib /usr/local/lib/galilei ${prefix}/lib/galilei"
	fi
	AC_ARG_WITH(galilei-libs,
		AC_HELP_STRING(
			[--with-galilei-libs=DIR],
			[where the GALILEI libraries are installed, default: [/usr/lib/galilei]. ]),
		GALILEI_LIB_PATH="$withval")

	# ---- end arg for configure ----

# ---- check the presence of all GALILEI libraries
	for l in docs engines groups historic infos profiles sessions; do

		#AC_MSG_CHECKING(checking for lib$l)
		for i in $GALILEI_LIB_PATH ; do
			GALILEI_LIB_CHK($i/$l,libgalilei_$l)
			GALILEI_LIB_CHK($i,libgalilei_$l)
			GALILEI_LIB_CHK($i/galilei,libgalilei_$l)
		done
		AC_MSG_CHECKING(checking for libgalilei_$l)
		if test -z "$GALILEI_LIB_DIR"; then
			AC_MSG_RESULT(no)
			AC_MSG_ERROR(Cannot find libgalilei_$l)
		else
			AC_MSG_RESULT(yes in $GALILEI_LIB_DIR)
		fi
		if test "x$GALILEI_LIB_DIR" != "x$PAST_GALILEI_LIB_DIR"; then
			LDFLAGS="-L$GALILEI_LIB_DIR $LDFLAGS"
		fi
		PAST_GALILEI_LIB_DIR="$GALILEI_LIB_DIR"
		LIB_GALILEI="$GALILEI_LIB_DIR/libgalilei_$l.la $LIB_GALILEI"

	done
	
	# ---- check the presence of all R frontend libraries ----
	# --------------------------------------------------------------
	for l in $1 ; do
		
		if test "$l" = "kde"; then
			for i in $GALILEI_LIB_PATH ; do
				GALILEI_LIB_CHK($i/frontend/kde,libkde_galilei)
				GALILEI_LIB_CHK($i,libkde_galilei)
				GALILEI_LIB_CHK($i/galilei,libkde_galilei)
			done

			AC_MSG_CHECKING(for libkde_galilei)
			if test -z "$GALILEI_LIB_DIR"; then
				AC_MSG_RESULT(no)
				AC_MSG_ERROR(Cannot find libkde_galilei.
No GUI will be available for GALILEI libraries.
Verify that GALILEI was installed
using the --enable-kde=yes option !!!
-------------------------------------)

			else
				AC_MSG_RESULT(yes in $GALILEI_LIB_DIR)
	
				if test "x$GALILEI_LIB_DIR" != "x$PAST_GALILEI_LIB_DIR"; then
        		
					LDFLAGS="-L$GALILEI_LIB_DIR $LDFLAGS"
				fi
				LIB_GALILEI="$GALILEI_LIB_DIR/libkde_galilei.la $LIB_GALILEI"
			fi

			PAST_GALILEI_LIB_DIR="$GALILEI_LIB_DIR"
		fi
	done
])


# GALILEI_CHK(list-of-frontend)
# ----------------------------------------------
#
# ---- def function to check for all GALILEI includes and libraries on the system
AC_DEFUN(GALILEI_CHK,[
	GALILEI_ALL_INC_CHK($1)
	GALILEI_ALL_LIB_CHK($1)
	AC_SUBST(LIB_GALILEI)
])
# --- end def function -----
