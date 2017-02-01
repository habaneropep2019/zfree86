/*
 * $XFree86: xc/lib/XThrStub/UIThrStubs.c,v 3.9 2008/10/15 20:59:11 tsi Exp $
 *
 * Copyright (c) 1995 David E. Wexelblat.  All rights reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL DAVID E. WEXELBLAT BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of David E. Wexelblat shall
 * not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization
 * from David E. Wexelblat.
 * 
 */
/*
 * Copyright (c) 1994-2004 by The XFree86 Project, Inc.
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

/*
 * Stubs for thread functions needed by the X library.  Supports 
 * UnixWare 2.x threads; may support Solaris 2 threads as well, but not
 * tested.  Defining things this way removes the dependency of the X
 * library on the threads library, but still supports threads if the user
 * specificies the thread library on the link line.
 */

/*
 * Modifications by Carlos A M dos Santos, XFree86 Project, November 1999.
 *
 * Explanation from <X11/Xos_r.h>:
 * The structure below is complicated, mostly because P1003.1c (the
 * IEEE POSIX Threads spec) went through lots of drafts, and some
 * vendors shipped systems based on draft API that were changed later.
 * Unfortunately POSIX did not provide a feature-test macro for
 * distinguishing each of the drafts.
 */

#include <errno.h>
#include <stdlib.h>

#ifdef CTHREADS
#include <cthreads.h>
typedef cthread_t xthread_t;
#define xthread_self cthread_self
#pragma weak cthread_self = _Xthr_self_stub_
#define xmutex_init mutex_init
#pragma weak mutex_init = _Xthr_zero_stub_
#pragma weak mutex_clear = _Xthr_zero_stub_
#pragma weak mutex_lock = _Xthr_zero_stub_
#pragma weak mutex_unlock = _Xthr_zero_stub_
#pragma weak condition_init = _Xthr_zero_stub_
#pragma weak condition_clear = _Xthr_zero_stub_
#pragma weak condition_wait = _Xthr_zero_stub_
#pragma weak condition_signal = _Xthr_zero_stub_
#pragma weak condition_broadcast = _Xthr_zero_stub_
#else /* !CTHREADS */
#if defined(SVR4) && !defined(__sgi)
#include <thread.h>
typedef thread_t xthread_t;
#pragma weak thr_self = _Xthr_self_stub_
#pragma weak mutex_init = _Xthr_zero_stub_
#pragma weak mutex_destroy = _Xthr_zero_stub_
#pragma weak mutex_lock = _Xthr_zero_stub_
#pragma weak mutex_unlock = _Xthr_zero_stub_
#pragma weak cond_init = _Xthr_zero_stub_
#pragma weak cond_destroy = _Xthr_zero_stub_
#pragma weak cond_wait = _Xthr_zero_stub_
#pragma weak cond_signal = _Xthr_zero_stub_
#pragma weak cond_broadcast = _Xthr_zero_stub_
#else /* !SVR4 */
#ifdef WIN32
    /*
     * Don't know what to do here. Is there something do be done at all?
     */
#else /* !WIN32 */
#ifdef USE_TIS_SUPPORT
#include <tis.h>
typedef pthread_t xthread_t;
#pragma weak tis_self = _Xthr_self_stub_
#pragma weak tis_mutex_init = _Xthr_zero_stub_
#pragma weak tis_mutex_destroy = _Xthr_zero_stub_
#pragma weak tis_mutex_lock = _Xthr_zero_stub_
#pragma weak tis_mutex_unlock = _Xthr_zero_stub_
#pragma weak tis_cond_init = _Xthr_zero_stub_
#pragma weak tis_cond_destroy = _Xthr_zero_stub_
#pragma weak tis_cond_wait = _Xthr_zero_stub_
#pragma weak tis_cond_signal = _Xthr_zero_stub_
#pragma weak tis_cond_broadcast = _Xthr_zero_stub_
#else
#ifdef sgi
#include <sys/pthread.h>
#else
#include <pthread.h>
#endif
typedef pthread_t xthread_t;
#if defined(__GNUC__) && (__GNUC__ >= 2) && \
    ((__GNUC__ > 2) || (__GNUC_MINOR__ >= 95))
xthread_t pthread_self()
          __attribute__ ((weak, alias ("_Xthr_self_stub_")));

int       pthread_mutex_init()
          __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int       pthread_mutex_destroy()
          __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int       pthread_mutex_lock()
          __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int       pthread_mutex_unlock()
          __attribute__ ((weak, alias ("_Xthr_zero_stub_")));

int       pthread_cond_init()
          __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int       pthread_cond_destroy()
          __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int       pthread_cond_wait()
          __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int       pthread_cond_signal()
          __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int       pthread_cond_broadcast()
          __attribute__ ((weak, alias ("_Xthr_zero_stub_")));

/* These are added for libGL */
int       pthread_key_create()
          __attribute__ ((weak, alias ("_Xthr_key_create_stub_")));
int       pthread_key_delete()
          __attribute__ ((weak, alias ("_Xthr_key_delete_stub_")));
void     *pthread_getspecific()
          __attribute__ ((weak, alias ("_Xthr_getspecific_stub_")));
int       pthread_setspecific()
          __attribute__ ((weak, alias ("_Xthr_setspecific_stub_")));

int       pthread_once()
          __attribute__ ((weak, alias ("_Xthr_once_stub_")));

#if defined(_DECTHREADS_) || defined(linux)
/* See Xthreads.h! */
int       pthread_equal()
          __attribute__ ((weak, alias ("_Xthr_equal_stub_")));
#endif /* _DECTHREADS_ || linux */
#else	/* __GNUC__ */
#pragma weak pthread_self           = _Xthr_self_stub_

#pragma weak pthread_mutex_init     = _Xthr_zero_stub_
#pragma weak pthread_mutex_destroy  = _Xthr_zero_stub_
#pragma weak pthread_mutex_lock     = _Xthr_zero_stub_
#pragma weak pthread_mutex_unlock   = _Xthr_zero_stub_

#pragma weak pthread_cond_init      = _Xthr_zero_stub_
#pragma weak pthread_cond_destroy   = _Xthr_zero_stub_
#pragma weak pthread_cond_wait      = _Xthr_zero_stub_
#pragma weak pthread_cond_signal    = _Xthr_zero_stub_
#pragma weak pthread_cond_broadcast = _Xthr_zero_stub_

/* These are added for libGL */
#pragma weak pthread_key_create     = _Xthr_key_create_stub_
#pragma weak pthread_key_delete     = _Xthr_key_delete_stub_
#pragma weak pthread_getspecific    = _Xthr_getspecific_stub_
#pragma weak pthread_setspecific    = _Xthr_setspecific_stub_

#pragma weak pthread_once           = _Xthr_once_stub_

#if defined(_DECTHREADS_) || defined(linux)
/* See Xthreads.h! */
#pragma weak pthread_equal          = _Xthr_equal_stub_
#endif /* _DECTHREADS_ || linux */
#endif	/* __GNUC__ */

#if defined(_DECTHREADS_) || defined(linux)
int
_Xthr_equal_stub_(xthread_t a, xthread_t b)
{
    return(1);
}
#endif /* _DECTHREADS_ || linux */
#endif /* USE_TIS_SUPPORT */
#endif /* WIN32 */
#endif /* SVR4 */
#endif /* CTHREADS */

#ifndef NULL
#define NULL ((void *)0)
#endif


xthread_t _Xthr_self_stub_(void);
xthread_t 
_Xthr_self_stub_(void)
{
    static xthread_t _X_no_thread_id;

    return(_X_no_thread_id);	/* defined by <X11/Xthreads.h> */
}

int _Xthr_zero_stub_(void);
int 
_Xthr_zero_stub_(void)
{
    return(0);
}

int _Xthr_once_stub_(void *, void (*)(void));
int
_Xthr_once_stub_(void *id, void (*routine)(void))
{
    static char done = 0;

    if (done)
	return 0;

    (*routine)();
    done = 1;

    return 0;
}

static void **keys = NULL;
static unsigned int last_key = 0;

int _Xthr_key_create_stub_(unsigned int *, void (*)(void *));
int
_Xthr_key_create_stub_(unsigned int *key, void (*destructor)(void *))
{
    void **newkeys = realloc(keys, (last_key + 1) * sizeof(*keys));

    if (!newkeys)
	return ENOMEM;

    keys = newkeys;
    keys[last_key] = NULL;
    *key = last_key++;
    return 0;
}

int _Xthr_key_delete_stub_(unsigned int);
int
_Xthr_key_delete_stub_(unsigned int key)
{
    if (key >= last_key)
	return EINVAL;

    keys[key] = NULL;
    return 0;
}

void * _Xthr_getspecific_stub_(unsigned int);
void *
_Xthr_getspecific_stub_(unsigned int key)
{
    if (key >= last_key)
	return NULL;

    return keys[key];
}

int _Xthr_setspecific_stub_(unsigned int, void *);
int
_Xthr_setspecific_stub_(unsigned int key, void *value)
{
    if (key >= last_key)
	return EINVAL;

    keys[key] = value;
    return 0;
}
