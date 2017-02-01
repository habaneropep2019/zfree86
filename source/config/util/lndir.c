/* $XFree86: xc/config/util/lndir.c,v 3.27 2007/09/16 01:42:17 tsi Exp $ */
/* Create shadow link tree (after X11R4 script of the same name)
   Mark Reinhold (mbr@lcs.mit.edu)/3 January 1990 */

/* 
Copyright (c) 1990, 1998 The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/
/*
 * Copyright (c) 2005-2006 by The XFree86 Project, Inc.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 *   1.  Redistributions of source code must retain the above copyright
 *       notice, this list of conditions, and the following disclaimer.
 *
 *   2.  Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer
 *       in the documentation and/or other materials provided with the
 *       distribution, and in the same place and form as other copyright,
 *       license and disclaimer information.
 *
 *   3.  The end-user documentation included with the redistribution,
 *       if any, must include the following acknowledgment: "This product
 *       includes software developed by The XFree86 Project, Inc
 *       (http://www.xfree86.org/) and its contributors", in the same
 *       place and form as other third-party acknowledgments.  Alternately,
 *       this acknowledgment may appear in the software itself, in the
 *       same form and location as other such third-party acknowledgments.
 *
 *   4.  Except as contained in this notice, the name of The XFree86
 *       Project, Inc shall not be used in advertising or otherwise to
 *       promote the sale, use or other dealings in this Software without
 *       prior written authorization from The XFree86 Project, Inc.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE XFREE86 PROJECT, INC OR ITS CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* From the original /bin/sh script:

  Used to create a copy of the a directory tree that has links for all
  non-directories (except, by default, those named BitKeeper, RCS, SCCS,
  CVS.adm, or .svn).  If you are building the distribution on more than one
  machine, you should use this technique.

  If your master sources are located in /usr/local/src/X and you would like
  your link tree to be in /usr/local/src/new-X, do the following:

   	%  mkdir /usr/local/src/new-X
	%  cd /usr/local/src/new-X
   	%  lndir ../X
*/

#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#if !defined(MINIX) && !defined(Lynx)
#include <sys/param.h>
#endif
#include <errno.h>

#if !defined(X_NOT_POSIX) || defined(SYSV) || defined(USG)
#include <dirent.h>
#else
#include <sys/dir.h>
#ifndef dirent
#define dirent direct
#endif
#endif
#ifndef MAXPATHLEN
#define MAXPATHLEN 2048
#endif

#include <stdarg.h>

struct except {
    char *name;
    struct except *next;
};

static int silent = 0;			/* -silent */
static int ignore_links = 0;		/* -ignorelinks */
static int with_revinfo = 0;		/* -withrevinfo */
static int with_symdirs = 0;		/* -withsymdirs */
static int clean = 0;			/* -clean */
static int clean_only = 0;		/* -cleanonly */
static int no_exceptions = 0;
static struct except *exceptions = 0;

char *rcurdir;
char *curdir;

static void
quit (int code, char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf (stderr, fmt, args);
    va_end(args);
    putc ('\n', stderr);
    exit (code);
}

static void
quiterr (int code, char *s)
{
    perror (s);
    exit (code);
}

static void
msg (char * fmt, ...)
{
    va_list args;
    if (curdir) {
	fprintf (stderr, "%s:\n", curdir);
	curdir = 0;
    }
    va_start(args, fmt);
    vfprintf (stderr, fmt, args);
    va_end(args);
    putc ('\n', stderr);
}

static void
mperror (char *s)
{
    if (curdir) {
	fprintf (stderr, "%s:\n", curdir);
	curdir = 0;
    }
    perror (s);
}

static int 
equivalent(char *lname, char *rname, char **p)
{
    char *s;

    if (!strcmp(lname, rname))
	return 1;
    for (s = lname; *s && (s = strchr(s, '/')); s++) {
	while (s[1] == '/') {
	    strcpy(s+1, s+2);
	    if (*p) (*p)--;
	}
    }
    return !strcmp(lname, rname);
}

static void
add_exception(char *name)
{
    struct except *new = malloc(sizeof(struct except));

    if (!new)
	quiterr(1, name);

    new->name = strdup(name);
    if (!new->name)
	quiterr(1, name);

    new->next = exceptions;
    exceptions = new;
}

/* Recursively create symbolic links from the current directory to the "from"
   directory.  Assumes that files described by fs and ts are directories. */
static int
dodir (char *fn,		/* name of "from" directory, either absolute or
				   relative to cwd */
       struct stat *fs, 
       struct stat *ts,		/* stats for the "from" directory and cwd */
       int rel)			/* if true, prepend "../" to fn before using */
{
    DIR *df;
    struct dirent *dp;
    char buf[MAXPATHLEN + 1], *p;
    char symbuf[MAXPATHLEN + 1];
    char basesym[MAXPATHLEN + 1];
    struct stat sb, sc;
    int n_dirs;
    int symlen;
    int basesymlen;
    char *ocurdir;
    struct except *cur;

    if ((fs->st_dev == ts->st_dev) && (fs->st_ino == ts->st_ino)) {
	msg ("%s: From and to directories are identical!", fn);
	return 1;
    }

    if (rel)
	strcpy (buf, "../");
    else
	buf[0] = '\0';
    strcat (buf, fn);
    
    if (!(df = opendir (buf))) {
	msg ("%s: Cannot opendir", buf);
	return 1;
    }

    p = buf + strlen (buf);
    if (*(p - 1) != '/')
	*p++ = '/';
    n_dirs = fs->st_nlink;
    while ((dp = readdir (df))) {
	if (!no_exceptions) {
	    if ((strlen(dp->d_name) <= 0) ||
		(dp->d_name[strlen(dp->d_name) - 1] == '~') ||
		((dp->d_name[0] == '.') && (dp->d_name[1] == '#')))
		continue;
	    /* Ignore these Mac OS X Finder data files */
	    if (!strcmp(dp->d_name, ".DS_Store") || 
		!strcmp(dp->d_name, "._.DS_Store")) 
		continue;
	}

	if (!with_revinfo) {
	    if (!strcmp (dp->d_name, ".cvsignore"))
		continue;
	    if (!strcmp (dp->d_name, ".gitignore"))
		continue;
	}

	for (cur = exceptions;  cur;  cur = cur->next)
	    if (!strcmp(dp->d_name, cur->name))
		goto next;

	strcpy (p, dp->d_name);

	if (n_dirs > 0) {
	    if (stat (buf, &sb) < 0) {
		mperror (buf);
		continue;
	    }

#ifdef S_ISDIR
	    if(S_ISDIR(sb.st_mode))
#else
	    if ((sb.st_mode & S_IFMT) == S_IFDIR) 
#endif
	    {
		/* directory */
		n_dirs--;

	do_symdir:
		if (dp->d_name[0] == '.' &&
		    (dp->d_name[1] == '\0' || (dp->d_name[1] == '.' &&
					       dp->d_name[2] == '\0')))
		    continue;
		if (!with_revinfo) {
		    if (!strcmp (dp->d_name, "BitKeeper"))
			continue;
		    if (!strcmp (dp->d_name, "RCS"))
			continue;
		    if (!strcmp (dp->d_name, "SCCS"))
			continue;
		    if (!strcmp (dp->d_name, "CVS"))
			continue;
		    if (!strcmp (dp->d_name, "CVS.adm"))
			continue;
		    if (!strcmp (dp->d_name, ".svn"))
			continue;
		}
		ocurdir = rcurdir;
		rcurdir = buf;
		curdir = silent ? buf : (char *)0;
		if (!silent)
		    printf ("%s:\n", buf);
		if ((stat (dp->d_name, &sc) < 0) && (errno == ENOENT)) {
		    if (mkdir (dp->d_name, 0777) < 0 ||
			stat (dp->d_name, &sc) < 0) {
			mperror (dp->d_name);
			curdir = rcurdir = ocurdir;
			continue;
		    }
		}
		if (readlink (dp->d_name, symbuf, sizeof(symbuf) - 1) >= 0) {
		    msg ("%s: is a link instead of a directory", dp->d_name);
		    curdir = rcurdir = ocurdir;
		    continue;
		}
		if (chdir (dp->d_name) < 0) {
		    mperror (dp->d_name);
		    curdir = rcurdir = ocurdir;
		    continue;
		}
		dodir (buf, &sb, &sc, (buf[0] != '/'));
		if (chdir ("..") < 0)
		    quiterr (1, "..");
		curdir = rcurdir = ocurdir;
		continue;
	    }
	}

	/* non-directory */
	symlen = readlink (dp->d_name, symbuf, sizeof(symbuf) - 1);
	if (symlen >= 0)
	    symbuf[symlen] = '\0';

	/* The option to ignore links exists mostly because
	   checking for them slows us down by 10-20%.
	   But it is off by default because this really is a useful check. */
	basesymlen = -1;
	if (!ignore_links) {
	    /* see if the file in the base tree was a symlink */
	    basesymlen = readlink(buf, basesym, sizeof(basesym) - 1);
	    if (basesymlen >= 0)
		basesym[basesymlen] = '\0';
	}

	if (symlen >= 0) {
	    /* Link exists in new tree.  Print message if it doesn't match. */
	    if (!equivalent (basesymlen>=0 ? basesym : buf, symbuf,
			     basesymlen>=0 ? (char **) 0 : &p))
		msg ("%s: %s", dp->d_name, symbuf);
	} else {
	    char *sympath;

	    if (with_symdirs) {
		if (stat (buf, &sb) < 0)
		    mperror (buf);
#ifdef S_ISDIR
		else if (S_ISDIR(sb.st_mode))
#else
		else if ((sb.st_mode & S_FMT) == S_IFDIR)
#endif
		    goto do_symdir;
	    }

	    if (basesymlen>=0) {
		if ((buf[0] == '.') && (buf[1] == '.') && (buf[2] == '/') &&
		    (basesym[0] == '.') && (basesym[1] == '.') &&
		    (basesym[2] == '/')) {
		    /* It becomes very tricky here. We have
		       ../../bar/foo symlinked to ../xxx/yyy. We
		       can't just use ../xxx/yyy. We have to use
		       ../../bar/foo/../xxx/yyy.  */

		    int i;
		    char *start, *end;

		    strcpy (symbuf, buf);
		    /* Find the first char after "../" in symbuf.  */
		    start = symbuf;
		    do {
			start += 3;
		    } while ((start[0] == '.') && (start[1] == '.') &&
			     (start[2] == '/'));

		    /* Then try to eliminate "../"s in basesym.  */
		    i = 0;
		    end = strrchr (symbuf, '/');
		    if (start < end) {
			do {
			    i += 3;
			    end--;
			    while ((*end != '/') && (end != start))
				end--;
			    if (end == start)
				break;
			} while ((basesym[i] == '.') &&
				 (basesym[i + 1] == '.') &&
				 (basesym[i + 2] == '/'));
		    }
		    if (*end == '/')
			end++;
		    strcpy (end, &basesym[i]);
		    sympath = symbuf;
		}
		else
		    sympath = basesym;
	    }
	    else
		sympath = buf;
	    if (symlink (sympath, dp->d_name) < 0)
		mperror (dp->d_name);
	}
next:;
    }

    closedir (df);
    return 0;
}

/*
 * Recursively clean out symbolic links that point nowhere and remove
 * empty directories.
 *
 * Return values:
 *   1: directory is empty and should be removed.
 *   0: OK, nothing else to do for this directory.
 *  -1: error.
 */
static int
cleandir(const char *dir)
{
    DIR *df;
    struct dirent *dp;
    char reldir[MAXPATHLEN + 1];
    char symbuf[MAXPATHLEN + 1];
    struct stat sb;
    int empty = 1;

    if (!(df = opendir("."))) {
	msg ("%s: Cannot opendir", dir);
	return -1;
    }

    while ((dp = readdir(df))) {

	if (strcmp(dp->d_name, ".") == 0 ||
	    strcmp(dp->d_name, "..") == 0) {
	    continue;
	}

	/* Check for dangling links, and remove them. */
	if (stat(dp->d_name, &sb) < 0 && errno == ENOENT) {
	    if (strcmp(dir, ".") == 0)
		msg("Removing dangling link: %s", dp->d_name);
	    else
		msg("Removing dangling link: %s/%s", dir, dp->d_name);
	    if (unlink(dp->d_name) < 0)
		mperror("Link removal failed");
	    continue;
	}

#ifdef S_ISDIR
	if (S_ISDIR(sb.st_mode))
#else
	if ((sb.st_mode & S_IFMT) == S_IFDIR) 
#endif
	{
	    /* directory */
	    if (readlink (dp->d_name, symbuf, sizeof(symbuf) - 1) < 0) {
		/* Not a symlink to a directory. */
		if (chdir (dp->d_name) < 0) {
		    mperror (dp->d_name);
		    continue;
		}
		if (strcmp(dir, ".") == 0)
		    strcpy(reldir, dp->d_name);
		else {
		    if (strlen(dir) + strlen(dp->d_name) + 1 > MAXPATHLEN)
			quit(1, "path name too long: %s/%s", dir, dp->d_name);
		    sprintf(reldir, "%s/%s", dir, dp->d_name);
		}
		switch (cleandir(reldir)) {
		case -1:
		    return -1;
		    break;
		case 0:
		    /* Directory not empty, which means curdir is not empty. */
		    if (chdir ("..") < 0)
			quiterr (1, "..");
		    empty = 0;
		    break;
		case 1:
		    /* Directory is empty, so remove it. */
		    if (chdir ("..") < 0)
			quiterr (1, "..");
		    msg("Removing empty directory: %s", reldir);
		    if (rmdir(dp->d_name) < 0)
			mperror("Directory removal failed");
		    break;
		default:
		    /* Never get here. */
		    break;
		}

	    }
	    continue;
	}

	/* A remaining entry, so this directory is not empty. */
	empty = 0;
    }
    closedir (df);
    return empty;
}

int
main (int ac, char *av[])
{
    char *prog_name = av[0];
    char *fn = NULL, *tn;
    struct stat fs, ts;
    int ret = 0, len;

    while (++av, --ac) {
	if ((strcmp(*av, "-silent") == 0) ||
	    (strcmp(*av, "-s") == 0))
	    silent = 1;
	else if ((strcmp(*av, "-clean") == 0) ||
		 (strcmp(*av, "-c") == 0))
	    clean = 1;
	else if ((strcmp(*av, "-cleanonly") == 0))
	    clean_only = 1;
	else if ((strcmp(*av, "-ignorelinks") == 0) ||
		 (strcmp(*av, "-i") == 0))
	    ignore_links = 1;
	else if ((strcmp(*av, "-withrevinfo") == 0) ||
		 (strcmp(*av, "-r") == 0))
	    with_revinfo = 1;
	else if ((strcmp(*av, "-withsymdirs") == 0) ||
		 (strcmp(*av, "-d") == 0))
	    with_symdirs = 1;
	else if ((strcmp(*av, "-noexceptions") == 0) ||
		 (strcmp(*av, "-E") == 0))
	    no_exceptions = 1;
	else if ((ac > 2) &&
		 ((strcmp(*av , "-except") == 0) ||
		  (strcmp(*av, "-e") == 0))) {
	    av++;
	    ac--;

	    add_exception(*av);
	}
	else if (strcmp(*av, "--") == 0) {
	    ++av, --ac;
	    break;
	}
	else
	    break;
    }

    if ((ac < 1 && !clean_only) || ac > 2 || (clean_only && ac > 1))
	quit (1, "usage: %s [-s] [-i] [-E] [-r] [-d] [-c] [-e <filename>] ... fromdir [todir]",
	      prog_name);

    if (!clean_only) {
	fn = av[0];
	av++, --ac;
    }

    if (ac == 1)
	tn = av[0];
    else
	tn = ".";

    /* to directory */
    if (stat (tn, &ts) < 0)
	quiterr (1, tn);
#ifdef S_ISDIR
    if (!(S_ISDIR(ts.st_mode)))
#else
    if ((ts.st_mode & S_IFMT) != S_IFDIR)
#endif
	quit (2, "%s: Not a directory", tn);
    if (chdir (tn) < 0)
	quiterr (1, tn);

    if (clean_only) {
	if (cleandir(tn) == -1)
	    return 1;
	return 0;
    }

    /* from directory */
    if (stat (fn, &fs) < 0)
	quiterr (1, fn);
#ifdef S_ISDIR
    if (!(S_ISDIR(fs.st_mode)))
#else
    if ((fs.st_mode & S_IFMT) != S_IFDIR)
#endif
	quit (2, "%s: Not a directory", fn);

    /* Strip off from directory's trailing self references */
    len = strlen(fn);
    while ((--len > 1) &&
	   ((fn[len] == '/') ||
	    ((fn[len] == '.') && (fn[len - 1] == '/'))))
	fn[len] = '\0';

    ret = dodir (fn, &fs, &ts, 0);

    if (ret == 0 && clean)
	if (cleandir(tn) < 0)
	    ret = 1;

    return ret;
}
