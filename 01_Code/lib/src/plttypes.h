/* defines types useful for the plotting machine */
#ifndef _PMTYPES_H
#define _PMTYPES_H

typedef enum {
	OFF,
	ON
} PMstate;

typedef float PMfloat;

typedef
struct PMcontext {
	PMfloat penv_x, penv_y;
};

typedef enum  {
	PM_SUCCESS,
	PM_FAILURE
} PMresult;

typedef
unsigned long
PMduration_ms;

#define NULL ((void *) 0)

#endif
