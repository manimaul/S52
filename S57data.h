// S57data.h: interface to S57 geo data
//
// Project:  OpENCview

/*
    This file is part of the OpENCview project, a viewer of ENC.
    Copyright (C) 2000-2014 Sylvain Duclos sduclos@users.sourceforge.net

    OpENCview is free software: you can redistribute it and/or modify
    it under the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpENCview is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with OpENCview.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef _S57DATA_H_
#define _S57DATA_H_

#include <glib.h>       // guint, GArray, GData, GString, gconstpointer
#define GCPTR gconstpointer

// MAXINT-6 is how OGR tag an UNKNOWN value
// see gdal/ogr/ogrsf_frmts/s57/s57.h:126
// it is then turn into a string in gv_properties
#define EMPTY_NUMBER_MARKER "2147483641"  /* MAXINT-6 */

// internal geo enum used to link S52 to S57 geo
// S57 object type have a PLib enum: P,L,A
typedef enum S57_Obj_t {
    S57__META_T  =  0 ,       // meta geo stuff (ex: C_AGGR)
    S57_POINT_T  = 'P',       // 80 (point)
    S57_LINES_T  = 'L',       // 76 (line)
    S57_AREAS_T  = 'A',       // 65 (area)
    S57_N_OBJ_T  =  4         // number of object type
} S57_Obj_t;

// experimental (fail - because Edge ID are used to match geo)
// outer ring original Area Winding - info needed to revere S57_att
typedef enum S57_AW_t {
    S57_AW_NONE =  0,
    S57_AW_CW   =  1,
    S57_AW_CCW  =  2,
    S57_AW_MAX  =  3
} S57_AW_t;

typedef double geocoord;
typedef struct _S57_geo  S57_geo;
typedef struct _S57_prim S57_prim;

int       S57_doneData(S57_geo *geo, gpointer user_data);

S57_geo  *S57_setPOINT(geocoord *xyz);
S57_geo  *S57_setLINES(guint xyznbr, geocoord *xyz);
//S57_geo  *S57_setMLINE(guint linenbr, guint *linexyznbr, geocoord **linexyz);
S57_geo  *S57_setAREAS(guint ringnbr, guint *ringxyznbr, geocoord **ringxyz, S57_AW_t origAW);
S57_geo  *S57_set_META();

// used for PASTRK
S57_geo  *S57_setGeoLine(S57_geo *geo, guint xyznbr, geocoord *xyz);

S57_geo  *S57_getGeoLink(S57_geo *geo);
S57_geo  *S57_setGeoLink(S57_geo *geo, S57_geo *link);

#ifdef S52_USE_WORLD
S57_geo  *S57_setNextPoly(S57_geo *geo, S57_geo *nextPoly);
S57_geo  *S57_getNextPoly(S57_geo *geo);
S57_geo  *S57_delNextPoly(S57_geo *geo);
#endif

int       S57_setName(S57_geo *geo, const char *name);
GCPTR     S57_getName(S57_geo *geo);

// debug:
//int       S57_setOGRGeo(S57_geo *geo, void *hGeom);
//void     *S57_getOGRGeo(S57_geo *geo);

// get the number of rings
guint     S57_getRingNbr(S57_geo *geo);
// get data
int       S57_getGeoData(S57_geo *geo, guint ringNo, guint *npt, double **ppt);

// handling of S52/S57 object rendering primitive
S57_prim *S57_initPrim   (S57_prim *prim);
S57_prim *S57_donePrim   (S57_prim *prim);
S57_prim *S57_initPrimGeo(S57_geo  *geo);
S57_geo  *S57_donePrimGeo(S57_geo  *geo);
int       S57_begPrim    (S57_prim *prim, int mode);
int       S57_endPrim    (S57_prim *prim);

// GLES2 need float vertex
#if (defined(S52_USE_GL2) || defined(S52_USE_GLES2))
//#ifdef S52_USE_GLES2
typedef float  vertex_t;
#else
typedef double vertex_t;
#endif
int       S57_addPrimVertex(S57_prim *prim, vertex_t *ptr);

S57_prim *S57_getPrimGeo   (S57_geo  *geo);
guint     S57_getPrimData  (S57_prim *prim, guint *primNbr, vertex_t **vert, guint *vertNbr, guint *vboID);
GArray   *S57_getPrimVertex(S57_prim *prim);
int       S57_getPrimIdx   (S57_prim *prim, unsigned int i, int *mode, int *first, int *count);
//int       S57_getPrimIdx(S57_prim *prim, guint i, guint *mode, guint *first, guint *count);

int       S57_setPrimDList (S57_prim *prim, guint DList);

// get/set extend
int       S57_setExt(S57_geo *geo, double  x1, double  y1, double  x2, double  y2);
int       S57_getExt(S57_geo *geo, double *x1, double *y1, double *x2, double *y2);

// get geo type (P,L,A) of this object
// Note: return the same thing as a call to S52_PL_getFTYP()
// Note: S52_Obj_t vs S57_Obj_t
//S52_Obj_t S57_getObjtype(S57_geo *geo);
S57_Obj_t S57_getObjtype(S57_geo *geo);

// return S57 attribute value of the attribute name
GString  *S57_getAttVal(S57_geo *geo, const char *name);
// set attribute name and value
GData    *S57_setAtt(S57_geo *geo, const char *name, const char *val);
// get str of the form ",KEY1:VAL1,KEY2:VAL2, ..." of S57 attribute only (not OGR)
GCPTR     S57_getAtt(S57_geo *geo);

int       S57_setTouchTOPMAR(S57_geo *geo, S57_geo *touch);
S57_geo  *S57_getTouchTOPMAR(S57_geo *geo);
int       S57_setTouchLIGHTS(S57_geo *geo, S57_geo *touch);
S57_geo  *S57_getTouchLIGHTS(S57_geo *geo);
int       S57_setTouchDEPARE(S57_geo *geo, S57_geo *touch);
S57_geo  *S57_getTouchDEPARE(S57_geo *geo);
int       S57_setTouchDEPVAL(S57_geo *geo, S57_geo *touch);
S57_geo  *S57_getTouchDEPVAL(S57_geo *geo);

double    S57_setScamin(S57_geo *geo, double scamin);
double    S57_getScamin(S57_geo *geo);
double    S57_resetScamin(S57_geo *geo);

int       S57_setRelationship(S57_geo *geo, S57_geo *geoRel);
S57_geo  *S57_getRelationship(S57_geo *geo);

#if 0
// count the number of 'real (6length)' attributes
int       S57_getNumAtt(S57_geo *geo);
// return the 'real' attributes of the geodata. name and val must be preallocated, and be sufficient large. (use S57_getNumAtt for counting)
int       S57_getAttributes(S57_geo *geo, char **name, char **val);
#endif

// debug
int       S57_dumpData(S57_geo *geo, int dumpCoords);
guint     S57_getGeoID(S57_geo *geo);

#ifdef S52_USE_PROJ
#include <proj_api.h>   // projXY, projUV, projPJ
int       S57_donePROJ();
int       S57_setMercPrj(double lat, double lon);
GCPTR     S57_getPrjStr(void);
projXY    S57_prj2geo(projUV uv);
int       S57_geo2prj3dv(guint npt, double *data);
int       S57_geo2prj(S57_geo *geo);
#endif

int       S57_isPtInside(int npt, double *xyz, gboolean close, double x, double y);
int       S57_touch(S57_geo *geoA, S57_geo *geoB);

guint     S57_getGeoSize(S57_geo *geo);
guint     S57_setGeoSize(S57_geo *geo, guint size);

int       S57_newCentroid(S57_geo *geo);
int       S57_addCentroid(S57_geo *geo, double  x, double  y);
int       S57_getNextCentroid(S57_geo *geo, double *x, double *y);
int       S57_hasCentroid(S57_geo *geo);

#ifdef S52_USE_SUPP_LINE_OVERLAP
int       S57_markOverlapGeo(S57_geo *geo, S57_geo *geoEdge);
GString  *S57_getRCIDstr(S57_geo *geo);
// experimental (fail)
//int       S57_sameChainNode(S57_geo *geoA, S57_geo *geoB);
// experimental (fail)
// debug - outer ring original Area Winding - info needed to revere S57_att
S57_AW_t  S57_getOrigAW (S57_geo *geo);
#endif

// FIXME: setHL() rather than hl ON/OFF !
//int     S57_setHighlight(S57_geo *geo, gboolean highlight);
int       S57_highlightON (S57_geo *geo);
int       S57_highlightOFF(S57_geo *geo);
gboolean  S57_isHighlighted(S57_geo *geo);

int       S57_setHazard(S57_geo *geo, gboolean hazard);
gboolean  S57_isHazard (S57_geo *geo);

//// returns the window boundary with the current projection. After  the geo2prj and initproj have been public, this function may be moved to application layer.
//void S57_getGeoWindowBoundary(double lat, double lng, double scale, int width, int height, double *latMin, double *latMax, double *lngMin, double *lngMax);
//#endif


#endif // _S57DATA_H_
