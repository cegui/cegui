/*
** Lua binding: CEGUI
** Generated automatically by tolua++-1.0.6pre2 on 05/08/05 23:28:29.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int tolua_CEGUI_open (lua_State* tolua_S);

#include "required.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CEGUI__Point (lua_State* tolua_S)
{
 CEGUI::Point* self = (CEGUI::Point*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CEGUI__HeaderSequenceEventArgs (lua_State* tolua_S)
{
 CEGUI::HeaderSequenceEventArgs* self = (CEGUI::HeaderSequenceEventArgs*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CEGUI__WindowEventArgs (lua_State* tolua_S)
{
 CEGUI::WindowEventArgs* self = (CEGUI::WindowEventArgs*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CEGUI__colour (lua_State* tolua_S)
{
 CEGUI::colour* self = (CEGUI::colour*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CEGUI__Size (lua_State* tolua_S)
{
 CEGUI::Size* self = (CEGUI::Size*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CEGUI__ActivationEventArgs (lua_State* tolua_S)
{
 CEGUI::ActivationEventArgs* self = (CEGUI::ActivationEventArgs*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CEGUI__MouseCursorEventArgs (lua_State* tolua_S)
{
 CEGUI::MouseCursorEventArgs* self = (CEGUI::MouseCursorEventArgs*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CEGUI__KeyEventArgs (lua_State* tolua_S)
{
 CEGUI::KeyEventArgs* self = (CEGUI::KeyEventArgs*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CEGUI__MouseEventArgs (lua_State* tolua_S)
{
 CEGUI::MouseEventArgs* self = (CEGUI::MouseEventArgs*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CEGUI__EventArgs (lua_State* tolua_S)
{
 CEGUI::EventArgs* self = (CEGUI::EventArgs*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CEGUI__Rect (lua_State* tolua_S)
{
 CEGUI::Rect* self = (CEGUI::Rect*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_CEGUI__Vector2 (lua_State* tolua_S)
{
 CEGUI::Vector2* self = (CEGUI::Vector2*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CEGUI::Logger");
 tolua_usertype(tolua_S,"CEGUI::Image");
 tolua_usertype(tolua_S,"CEGUI::WindowEventArgs");
 tolua_usertype(tolua_S,"CEGUI::colour");
 tolua_usertype(tolua_S,"CEGUI::Font");
 tolua_usertype(tolua_S,"CEGUI::Texture");
 tolua_usertype(tolua_S,"CEGUI::MouseCursorEventArgs");
 tolua_usertype(tolua_S,"CEGUI::SchemeManager");
 tolua_usertype(tolua_S,"CEGUI::EventArgs");
 tolua_usertype(tolua_S,"CEGUI::Scheme");
 tolua_usertype(tolua_S,"CEGUI::Window");
 tolua_usertype(tolua_S,"CEGUI::MouseCursor");
 tolua_usertype(tolua_S,"CEGUI::WindowManager");
 tolua_usertype(tolua_S,"CEGUI::System");
 tolua_usertype(tolua_S,"CEGUI::HeaderSequenceEventArgs");
 tolua_usertype(tolua_S,"CEGUI::Renderer");
 tolua_usertype(tolua_S,"CEGUI::GlobalEventSet");
 tolua_usertype(tolua_S,"CEGUI::Rect");
 tolua_usertype(tolua_S,"CEGUI::MouseEventArgs");
 tolua_usertype(tolua_S,"CEGUI::ActivationEventArgs");
 tolua_usertype(tolua_S,"CEGUI::Point");
 tolua_usertype(tolua_S,"CEGUI::KeyEventArgs");
 tolua_usertype(tolua_S,"CEGUI::FontManager");
 tolua_usertype(tolua_S,"CEGUI::Size");
 tolua_usertype(tolua_S,"CEGUI::ImagesetManager");
 tolua_usertype(tolua_S,"CEGUI::Imageset");
 tolua_usertype(tolua_S,"CEGUI::Vector2");
}

/* get function: d_x of class  CEGUI::Vector2 */
static int tolua_get_CEGUI__Vector2_x(lua_State* tolua_S)
{
  CEGUI::Vector2* self = (CEGUI::Vector2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_x'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->d_x);
 return 1;
}

/* set function: d_x of class  CEGUI::Vector2 */
static int tolua_set_CEGUI__Vector2_x(lua_State* tolua_S)
{
  CEGUI::Vector2* self = (CEGUI::Vector2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_x'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->d_x = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: d_y of class  CEGUI::Vector2 */
static int tolua_get_CEGUI__Vector2_y(lua_State* tolua_S)
{
  CEGUI::Vector2* self = (CEGUI::Vector2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_y'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->d_y);
 return 1;
}

/* set function: d_y of class  CEGUI::Vector2 */
static int tolua_set_CEGUI__Vector2_y(lua_State* tolua_S)
{
  CEGUI::Vector2* self = (CEGUI::Vector2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_y'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->d_y = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* method: operator+ of class  CEGUI::Vector2 */
static int tolua_CEGUI_CEGUI_Vector2__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Vector2",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Vector2",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Vector2* self = (const CEGUI::Vector2*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Vector2* vec = ((const CEGUI::Vector2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
 {
  CEGUI::Vector2 tolua_ret = (CEGUI::Vector2)  self->operator+(*vec);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Vector2(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Vector2");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Vector2));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Vector2");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}

/* method: operator- of class  CEGUI::Vector2 */
static int tolua_CEGUI_CEGUI_Vector2__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Vector2",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Vector2",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Vector2* self = (const CEGUI::Vector2*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Vector2* vec = ((const CEGUI::Vector2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
 {
  CEGUI::Vector2 tolua_ret = (CEGUI::Vector2)  self->operator-(*vec);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Vector2(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Vector2");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Vector2));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Vector2");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}

/* method: operator* of class  CEGUI::Vector2 */
static int tolua_CEGUI_CEGUI_Vector2__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Vector2",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Vector2",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Vector2* self = (const CEGUI::Vector2*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Vector2* vec = ((const CEGUI::Vector2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
 {
  CEGUI::Vector2 tolua_ret = (CEGUI::Vector2)  self->operator*(*vec);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Vector2(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Vector2");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Vector2));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Vector2");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}

/* method: operator== of class  CEGUI::Vector2 */
static int tolua_CEGUI_CEGUI_Vector2__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Vector2",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Vector2",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Vector2* self = (const CEGUI::Vector2*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Vector2* vec = ((const CEGUI::Vector2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->operator==(*vec);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  CEGUI::Vector2 */
static int tolua_CEGUI_CEGUI_Vector2_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Vector2",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::Vector2* tolua_ret = (CEGUI::Vector2*)  new CEGUI::Vector2();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Vector2");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  CEGUI::Vector2 */
static int tolua_CEGUI_CEGUI_Vector2_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Vector2",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::Vector2* tolua_ret = (CEGUI::Vector2*)  new CEGUI::Vector2();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::Vector2");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  CEGUI::Vector2 */
static int tolua_CEGUI_CEGUI_Vector2_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Vector2",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
 {
  CEGUI::Vector2* tolua_ret = (CEGUI::Vector2*)  new CEGUI::Vector2(x,y);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Vector2");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Vector2_new00(tolua_S);
}

/* method: new_local of class  CEGUI::Vector2 */
static int tolua_CEGUI_CEGUI_Vector2_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Vector2",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
 {
  CEGUI::Vector2* tolua_ret = (CEGUI::Vector2*)  new CEGUI::Vector2(x,y);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::Vector2");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Vector2_new00_local(tolua_S);
}

/* get function: d_x of class  CEGUI::Point */
static int tolua_get_CEGUI__Point_x(lua_State* tolua_S)
{
  CEGUI::Point* self = (CEGUI::Point*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_x'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->d_x);
 return 1;
}

/* set function: d_x of class  CEGUI::Point */
static int tolua_set_CEGUI__Point_x(lua_State* tolua_S)
{
  CEGUI::Point* self = (CEGUI::Point*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_x'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->d_x = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: d_y of class  CEGUI::Point */
static int tolua_get_CEGUI__Point_y(lua_State* tolua_S)
{
  CEGUI::Point* self = (CEGUI::Point*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_y'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->d_y);
 return 1;
}

/* set function: d_y of class  CEGUI::Point */
static int tolua_set_CEGUI__Point_y(lua_State* tolua_S)
{
  CEGUI::Point* self = (CEGUI::Point*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_y'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->d_y = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* method: operator+ of class  CEGUI::Point */
static int tolua_CEGUI_CEGUI_Point__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Point* self = (const CEGUI::Point*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* vec = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->operator+(*vec);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}

/* method: operator- of class  CEGUI::Point */
static int tolua_CEGUI_CEGUI_Point__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Point* self = (const CEGUI::Point*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* vec = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->operator-(*vec);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}

/* method: operator* of class  CEGUI::Point */
static int tolua_CEGUI_CEGUI_Point__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Point* self = (const CEGUI::Point*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* vec = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->operator*(*vec);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}

/* method: operator== of class  CEGUI::Point */
static int tolua_CEGUI_CEGUI_Point__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Point* self = (const CEGUI::Point*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* vec = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->operator==(*vec);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  CEGUI::Point */
static int tolua_CEGUI_CEGUI_Point_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::Point* tolua_ret = (CEGUI::Point*)  new CEGUI::Point();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Point");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  CEGUI::Point */
static int tolua_CEGUI_CEGUI_Point_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::Point* tolua_ret = (CEGUI::Point*)  new CEGUI::Point();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::Point");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  CEGUI::Point */
static int tolua_CEGUI_CEGUI_Point_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Point",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
 {
  CEGUI::Point* tolua_ret = (CEGUI::Point*)  new CEGUI::Point(x,y);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Point");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Point_new00(tolua_S);
}

/* method: new_local of class  CEGUI::Point */
static int tolua_CEGUI_CEGUI_Point_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Point",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
 {
  CEGUI::Point* tolua_ret = (CEGUI::Point*)  new CEGUI::Point(x,y);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::Point");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Point_new00_local(tolua_S);
}

/* get function: d_width of class  CEGUI::Size */
static int tolua_get_CEGUI__Size_width(lua_State* tolua_S)
{
  CEGUI::Size* self = (CEGUI::Size*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_width'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->d_width);
 return 1;
}

/* set function: d_width of class  CEGUI::Size */
static int tolua_set_CEGUI__Size_width(lua_State* tolua_S)
{
  CEGUI::Size* self = (CEGUI::Size*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_width'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->d_width = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: d_height of class  CEGUI::Size */
static int tolua_get_CEGUI__Size_height(lua_State* tolua_S)
{
  CEGUI::Size* self = (CEGUI::Size*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_height'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->d_height);
 return 1;
}

/* set function: d_height of class  CEGUI::Size */
static int tolua_set_CEGUI__Size_height(lua_State* tolua_S)
{
  CEGUI::Size* self = (CEGUI::Size*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_height'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->d_height = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* method: operator== of class  CEGUI::Size */
static int tolua_CEGUI_CEGUI_Size__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Size* self = (const CEGUI::Size*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->operator==(*sz);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  CEGUI::Size */
static int tolua_CEGUI_CEGUI_Size_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::Size* tolua_ret = (CEGUI::Size*)  new CEGUI::Size();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Size");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  CEGUI::Size */
static int tolua_CEGUI_CEGUI_Size_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::Size* tolua_ret = (CEGUI::Size*)  new CEGUI::Size();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::Size");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  CEGUI::Size */
static int tolua_CEGUI_CEGUI_Size_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Size",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
  float h = ((float)  tolua_tonumber(tolua_S,3,0));
 {
  CEGUI::Size* tolua_ret = (CEGUI::Size*)  new CEGUI::Size(w,h);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Size");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Size_new00(tolua_S);
}

/* method: new_local of class  CEGUI::Size */
static int tolua_CEGUI_CEGUI_Size_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Size",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
  float h = ((float)  tolua_tonumber(tolua_S,3,0));
 {
  CEGUI::Size* tolua_ret = (CEGUI::Size*)  new CEGUI::Size(w,h);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::Size");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Size_new00_local(tolua_S);
}

/* get function: d_top of class  CEGUI::Rect */
static int tolua_get_CEGUI__Rect_top(lua_State* tolua_S)
{
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_top'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->d_top);
 return 1;
}

/* set function: d_top of class  CEGUI::Rect */
static int tolua_set_CEGUI__Rect_top(lua_State* tolua_S)
{
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_top'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->d_top = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: d_bottom of class  CEGUI::Rect */
static int tolua_get_CEGUI__Rect_bottom(lua_State* tolua_S)
{
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_bottom'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->d_bottom);
 return 1;
}

/* set function: d_bottom of class  CEGUI::Rect */
static int tolua_set_CEGUI__Rect_bottom(lua_State* tolua_S)
{
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_bottom'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->d_bottom = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: d_left of class  CEGUI::Rect */
static int tolua_get_CEGUI__Rect_left(lua_State* tolua_S)
{
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_left'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->d_left);
 return 1;
}

/* set function: d_left of class  CEGUI::Rect */
static int tolua_set_CEGUI__Rect_left(lua_State* tolua_S)
{
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_left'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->d_left = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: d_right of class  CEGUI::Rect */
static int tolua_get_CEGUI__Rect_right(lua_State* tolua_S)
{
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_right'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->d_right);
 return 1;
}

/* set function: d_right of class  CEGUI::Rect */
static int tolua_set_CEGUI__Rect_right(lua_State* tolua_S)
{
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_right'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->d_right = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* method: getPosition of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Rect* self = (const CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->getPosition();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: getWidth of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_getWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Rect* self = (const CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWidth'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getWidth();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWidth'.",&tolua_err);
 return 0;
#endif
}

/* method: getHeight of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_getHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Rect* self = (const CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHeight'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getHeight();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHeight'.",&tolua_err);
 return 0;
#endif
}

/* method: getSize of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_getSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Rect* self = (const CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->getSize();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSize'.",&tolua_err);
 return 0;
#endif
}

/* method: getIntersection of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_getIntersection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Rect* self = (const CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Rect* rect = ((const CEGUI::Rect*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIntersection'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->getIntersection(*rect);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIntersection'.",&tolua_err);
 return 0;
#endif
}

/* method: isPointInRect of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_isPointInRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Rect* self = (const CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* p = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPointInRect'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isPointInRect(*p);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPointInRect'.",&tolua_err);
 return 0;
#endif
}

/* method: setPosition of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Rect",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* p = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
 {
  self->setPosition(*p);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: setWidth of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_setWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWidth'",NULL);
#endif
 {
  self->setWidth(w);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setWidth'.",&tolua_err);
 return 0;
#endif
}

/* method: setHeight of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_setHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
  float h = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setHeight'",NULL);
#endif
 {
  self->setHeight(h);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setHeight'.",&tolua_err);
 return 0;
#endif
}

/* method: setSize of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_setSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Rect",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSize'",NULL);
#endif
 {
  self->setSize(*sz);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSize'.",&tolua_err);
 return 0;
#endif
}

/* method: offset of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_offset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Rect",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* p = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'offset'",NULL);
#endif
 {
  CEGUI::Rect& tolua_ret = (CEGUI::Rect&)  self->offset(*p);
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CEGUI::Rect");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'offset'.",&tolua_err);
 return 0;
#endif
}

/* method: constrainSizeMax of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_constrainSizeMax00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Rect",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'constrainSizeMax'",NULL);
#endif
 {
  CEGUI::Rect& tolua_ret = (CEGUI::Rect&)  self->constrainSizeMax(*sz);
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CEGUI::Rect");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'constrainSizeMax'.",&tolua_err);
 return 0;
#endif
}

/* method: constrainSizeMin of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_constrainSizeMin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Rect",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'constrainSizeMin'",NULL);
#endif
 {
  CEGUI::Rect& tolua_ret = (CEGUI::Rect&)  self->constrainSizeMin(*sz);
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CEGUI::Rect");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'constrainSizeMin'.",&tolua_err);
 return 0;
#endif
}

/* method: constrainSize of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_constrainSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Rect",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Rect* self = (CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* min = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
  const CEGUI::Size* max = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'constrainSize'",NULL);
#endif
 {
  CEGUI::Rect& tolua_ret = (CEGUI::Rect&)  self->constrainSize(*min,*max);
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CEGUI::Rect");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'constrainSize'.",&tolua_err);
 return 0;
#endif
}

/* method: operator== of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Rect* self = (const CEGUI::Rect*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Rect* r = ((const CEGUI::Rect*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->operator==(*r);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::Rect* tolua_ret = (CEGUI::Rect*)  new CEGUI::Rect();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Rect");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::Rect* tolua_ret = (CEGUI::Rect*)  new CEGUI::Rect();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::Rect");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float l = ((float)  tolua_tonumber(tolua_S,2,0));
  float t = ((float)  tolua_tonumber(tolua_S,3,0));
  float r = ((float)  tolua_tonumber(tolua_S,4,0));
  float b = ((float)  tolua_tonumber(tolua_S,5,0));
 {
  CEGUI::Rect* tolua_ret = (CEGUI::Rect*)  new CEGUI::Rect(l,t,r,b);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Rect");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Rect_new00(tolua_S);
}

/* method: new_local of class  CEGUI::Rect */
static int tolua_CEGUI_CEGUI_Rect_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float l = ((float)  tolua_tonumber(tolua_S,2,0));
  float t = ((float)  tolua_tonumber(tolua_S,3,0));
  float r = ((float)  tolua_tonumber(tolua_S,4,0));
  float b = ((float)  tolua_tonumber(tolua_S,5,0));
 {
  CEGUI::Rect* tolua_ret = (CEGUI::Rect*)  new CEGUI::Rect(l,t,r,b);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::Rect");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Rect_new00_local(tolua_S);
}

/* method: new of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::colour",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::colour* tolua_ret = (CEGUI::colour*)  new CEGUI::colour();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::colour");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::colour",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::colour* tolua_ret = (CEGUI::colour*)  new CEGUI::colour();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::colour");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::colour",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  float g = ((float)  tolua_tonumber(tolua_S,3,0));
  float b = ((float)  tolua_tonumber(tolua_S,4,0));
  float a = ((float)  tolua_tonumber(tolua_S,5,0));
 {
  CEGUI::colour* tolua_ret = (CEGUI::colour*)  new CEGUI::colour(r,g,b,a);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::colour");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_colour_new00(tolua_S);
}

/* method: new_local of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::colour",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  float g = ((float)  tolua_tonumber(tolua_S,3,0));
  float b = ((float)  tolua_tonumber(tolua_S,4,0));
  float a = ((float)  tolua_tonumber(tolua_S,5,0));
 {
  CEGUI::colour* tolua_ret = (CEGUI::colour*)  new CEGUI::colour(r,g,b,a);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::colour");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_colour_new00_local(tolua_S);
}

/* method: getAlpha of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_getAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::colour* self = (const CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAlpha'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getAlpha();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAlpha'.",&tolua_err);
 return 0;
#endif
}

/* method: getRed of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_getRed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::colour* self = (const CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRed'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getRed();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRed'.",&tolua_err);
 return 0;
#endif
}

/* method: getGreen of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_getGreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::colour* self = (const CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGreen'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getGreen();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGreen'.",&tolua_err);
 return 0;
#endif
}

/* method: getBlue of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_getBlue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::colour* self = (const CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBlue'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getBlue();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBlue'.",&tolua_err);
 return 0;
#endif
}

/* method: getHue of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_getHue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::colour* self = (const CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHue'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getHue();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHue'.",&tolua_err);
 return 0;
#endif
}

/* method: getSaturation of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_getSaturation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::colour* self = (const CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSaturation'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getSaturation();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSaturation'.",&tolua_err);
 return 0;
#endif
}

/* method: getLumination of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_getLumination00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::colour* self = (const CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLumination'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getLumination();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLumination'.",&tolua_err);
 return 0;
#endif
}

/* method: set of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::colour",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::colour* self = (CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  float g = ((float)  tolua_tonumber(tolua_S,3,0));
  float b = ((float)  tolua_tonumber(tolua_S,4,0));
  float a = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
 {
  self->set(r,g,b,a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set'.",&tolua_err);
 return 0;
#endif
}

/* method: setAlpha of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_setAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::colour",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::colour* self = (CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAlpha'",NULL);
#endif
 {
  self->setAlpha(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAlpha'.",&tolua_err);
 return 0;
#endif
}

/* method: setRed of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_setRed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::colour",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::colour* self = (CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRed'",NULL);
#endif
 {
  self->setRed(r);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRed'.",&tolua_err);
 return 0;
#endif
}

/* method: setGreen of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_setGreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::colour",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::colour* self = (CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
  float g = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGreen'",NULL);
#endif
 {
  self->setGreen(g);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setGreen'.",&tolua_err);
 return 0;
#endif
}

/* method: setBlue of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_setBlue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::colour",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::colour* self = (CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
  float b = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBlue'",NULL);
#endif
 {
  self->setBlue(b);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBlue'.",&tolua_err);
 return 0;
#endif
}

/* method: setRGB of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_setRGB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::colour",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::colour* self = (CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  float g = ((float)  tolua_tonumber(tolua_S,3,0));
  float b = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRGB'",NULL);
#endif
 {
  self->setRGB(r,g,b);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRGB'.",&tolua_err);
 return 0;
#endif
}

/* method: setHSL of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour_setHSL00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::colour",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::colour* self = (CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
  float hue = ((float)  tolua_tonumber(tolua_S,2,0));
  float saturation = ((float)  tolua_tonumber(tolua_S,3,0));
  float luminance = ((float)  tolua_tonumber(tolua_S,4,0));
  float alpha = ((float)  tolua_tonumber(tolua_S,5,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setHSL'",NULL);
#endif
 {
  self->setHSL(hue,saturation,luminance,alpha);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setHSL'.",&tolua_err);
 return 0;
#endif
}

/* method: operator+ of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::colour* self = (const CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::colour* c = ((const CEGUI::colour*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
 {
  CEGUI::colour tolua_ret = (CEGUI::colour)  self->operator+(*c);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::colour(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::colour");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::colour));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::colour");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}

/* method: operator- of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::colour* self = (const CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::colour* c = ((const CEGUI::colour*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
 {
  CEGUI::colour tolua_ret = (CEGUI::colour)  self->operator-(*c);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::colour(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::colour");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::colour));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::colour");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}

/* method: operator== of class  CEGUI::colour */
static int tolua_CEGUI_CEGUI_colour__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::colour",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::colour* self = (const CEGUI::colour*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::colour* c = ((const CEGUI::colour*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->operator==(*c);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
 return 0;
#endif
}

/* method: getSingleton of class  CEGUI::Logger */
static int tolua_CEGUI_CEGUI_Logger_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Logger",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::Logger& tolua_ret = (CEGUI::Logger&)  CEGUI::Logger::getSingleton();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CEGUI::Logger");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}

/* method: setLoggingLevel of class  CEGUI::Logger */
static int tolua_CEGUI_CEGUI_Logger_setLoggingLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Logger",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Logger* self = (CEGUI::Logger*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::LoggingLevel level = ((CEGUI::LoggingLevel) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLoggingLevel'",NULL);
#endif
 {
  self->setLoggingLevel(level);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLoggingLevel'.",&tolua_err);
 return 0;
#endif
}

/* method: getLoggingLevel of class  CEGUI::Logger */
static int tolua_CEGUI_CEGUI_Logger_getLoggingLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Logger",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Logger* self = (const CEGUI::Logger*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLoggingLevel'",NULL);
#endif
 {
  CEGUI::LoggingLevel tolua_ret = (CEGUI::LoggingLevel)  self->getLoggingLevel();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLoggingLevel'.",&tolua_err);
 return 0;
#endif
}

/* method: logEvent of class  CEGUI::Logger */
static int tolua_CEGUI_CEGUI_Logger_logEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Logger",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Logger* self = (CEGUI::Logger*)  tolua_tousertype(tolua_S,1,0);
  string message = ((string)  tolua_tocppstring(tolua_S,2,0));
  CEGUI::LoggingLevel level = ((CEGUI::LoggingLevel) (int)  tolua_tonumber(tolua_S,3,CEGUI::Standard));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'logEvent'",NULL);
#endif
 {
  self->logEvent(message,level);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'logEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: getName of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
 {
  string tolua_ret = (string)  self->getName();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}

/* method: defineFontGlyphs of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_defineFontGlyphs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Font",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Font* self = (CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  unsigned long first = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
  unsigned long last = ((unsigned long)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'defineFontGlyphs'",NULL);
#endif
 {
  self->defineFontGlyphs(first,last);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'defineFontGlyphs'.",&tolua_err);
 return 0;
#endif
}

/* method: defineFontGlyphs of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_defineFontGlyphs01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Font",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Font* self = (CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  string glyph_set = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'defineFontGlyphs'",NULL);
#endif
 {
  self->defineFontGlyphs(glyph_set);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Font_defineFontGlyphs00(tolua_S);
}

/* method: isCodepointAvailable of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_isCodepointAvailable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  unsigned long cp = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCodepointAvailable'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isCodepointAvailable(cp);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCodepointAvailable'.",&tolua_err);
 return 0;
#endif
}

/* method: getAvailableGlyphs of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_getAvailableGlyphs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAvailableGlyphs'",NULL);
#endif
 {
  string tolua_ret = (string)  self->getAvailableGlyphs();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAvailableGlyphs'.",&tolua_err);
 return 0;
#endif
}

/* method: getPointSize of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_getPointSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPointSize'",NULL);
#endif
 {
  unsigned int tolua_ret = (unsigned int)  self->getPointSize();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPointSize'.",&tolua_err);
 return 0;
#endif
}

/* method: setNativeResolution of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_setNativeResolution00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Font",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Font* self = (CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setNativeResolution'",NULL);
#endif
 {
  self->setNativeResolution(*sz);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setNativeResolution'.",&tolua_err);
 return 0;
#endif
}

/* method: getNativeResolution of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_getNativeResolution00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNativeResolution'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->getNativeResolution();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNativeResolution'.",&tolua_err);
 return 0;
#endif
}

/* method: notifyScreenResolution of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_notifyScreenResolution00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Font",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Font* self = (CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'notifyScreenResolution'",NULL);
#endif
 {
  self->notifyScreenResolution(*sz);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'notifyScreenResolution'.",&tolua_err);
 return 0;
#endif
}

/* method: setAutoScalingEnabled of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_setAutoScalingEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Font",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Font* self = (CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAutoScalingEnabled'",NULL);
#endif
 {
  self->setAutoScalingEnabled(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAutoScalingEnabled'.",&tolua_err);
 return 0;
#endif
}

/* method: isAutoScaled of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_isAutoScaled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isAutoScaled'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isAutoScaled();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isAutoScaled'.",&tolua_err);
 return 0;
#endif
}

/* method: setAntiAliased of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_setAntiAliased00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Font",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Font* self = (CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAntiAliased'",NULL);
#endif
 {
  self->setAntiAliased(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAntiAliased'.",&tolua_err);
 return 0;
#endif
}

/* method: isAntiAliased of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_isAntiAliased00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isAntiAliased'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isAntiAliased();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isAntiAliased'.",&tolua_err);
 return 0;
#endif
}

/* method: getTextExtent of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_getTextExtent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  string text = ((string)  tolua_tocppstring(tolua_S,2,0));
  float xscale = ((float)  tolua_tonumber(tolua_S,3,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextExtent'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getTextExtent(text,xscale);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextExtent'.",&tolua_err);
 return 0;
#endif
}

/* method: getLineSpacing of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_getLineSpacing00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  float yscale = ((float)  tolua_tonumber(tolua_S,2,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLineSpacing'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getLineSpacing(yscale);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLineSpacing'.",&tolua_err);
 return 0;
#endif
}

/* method: getFontHeight of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_getFontHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  float yscale = ((float)  tolua_tonumber(tolua_S,2,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFontHeight'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getFontHeight(yscale);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFontHeight'.",&tolua_err);
 return 0;
#endif
}

/* method: getCharAtPixel of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_getCharAtPixel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  string text = ((string)  tolua_tocppstring(tolua_S,2,0));
  unsigned long start_char = ((unsigned long)  tolua_tonumber(tolua_S,3,0));
  float pixel = ((float)  tolua_tonumber(tolua_S,4,0));
  float xscale = ((float)  tolua_tonumber(tolua_S,5,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCharAtPixel'",NULL);
#endif
 {
  unsigned long tolua_ret = (unsigned long)  self->getCharAtPixel(text,start_char,pixel,xscale);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCharAtPixel'.",&tolua_err);
 return 0;
#endif
}

/* method: getCharAtPixel of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_getCharAtPixel01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  string text = ((string)  tolua_tocppstring(tolua_S,2,0));
  float pixel = ((float)  tolua_tonumber(tolua_S,3,0));
  float xscale = ((float)  tolua_tonumber(tolua_S,4,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCharAtPixel'",NULL);
#endif
 {
  unsigned long tolua_ret = (unsigned long)  self->getCharAtPixel(text,pixel,xscale);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Font_getCharAtPixel00(tolua_S);
}

/* method: getFormattedLineCount of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_getFormattedLineCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  string text = ((string)  tolua_tocppstring(tolua_S,2,0));
  const CEGUI::Rect* format_area = ((const CEGUI::Rect*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::TextFormatting fmt = ((CEGUI::TextFormatting) (int)  tolua_tonumber(tolua_S,4,0));
  float xscale = ((float)  tolua_tonumber(tolua_S,5,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFormattedLineCount'",NULL);
#endif
 {
  unsigned long tolua_ret = (unsigned long)  self->getFormattedLineCount(text,*format_area,fmt,xscale);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFormattedLineCount'.",&tolua_err);
 return 0;
#endif
}

/* method: getFormattedTextExtent of class  CEGUI::Font */
static int tolua_CEGUI_CEGUI_Font_getFormattedTextExtent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Font* self = (const CEGUI::Font*)  tolua_tousertype(tolua_S,1,0);
  string text = ((string)  tolua_tocppstring(tolua_S,2,0));
  const CEGUI::Rect* format_area = ((const CEGUI::Rect*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::TextFormatting fmt = ((CEGUI::TextFormatting) (int)  tolua_tonumber(tolua_S,4,0));
  float xscale = ((float)  tolua_tonumber(tolua_S,5,1));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFormattedTextExtent'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getFormattedTextExtent(text,*format_area,fmt,xscale);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFormattedTextExtent'.",&tolua_err);
 return 0;
#endif
}

/* method: getSingleton of class  CEGUI::FontManager */
static int tolua_CEGUI_CEGUI_FontManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::FontManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::FontManager& tolua_ret = (CEGUI::FontManager&)  CEGUI::FontManager::getSingleton();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CEGUI::FontManager");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}

/* method: createFont of class  CEGUI::FontManager */
static int tolua_CEGUI_CEGUI_FontManager_createFont00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::FontManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,6,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::FontManager* self = (CEGUI::FontManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  string fontname = ((string)  tolua_tocppstring(tolua_S,3,0));
  unsigned int size = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
  unsigned int flags = ((unsigned int)  tolua_tonumber(tolua_S,5,0));
  string resourcegroup = ((string)  tolua_tocppstring(tolua_S,6,""));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createFont'",NULL);
#endif
 {
  CEGUI::Font* tolua_ret = (CEGUI::Font*)  self->createFont(name,fontname,size,flags,resourcegroup);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Font");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createFont'.",&tolua_err);
 return 0;
#endif
}

/* method: createFont of class  CEGUI::FontManager */
static int tolua_CEGUI_CEGUI_FontManager_createFont01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::FontManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::FontManager* self = (CEGUI::FontManager*)  tolua_tousertype(tolua_S,1,0);
  string filename = ((string)  tolua_tocppstring(tolua_S,2,0));
  string resourcegroup = ((string)  tolua_tocppstring(tolua_S,3,""));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createFont'",NULL);
#endif
 {
  CEGUI::Font* tolua_ret = (CEGUI::Font*)  self->createFont(filename,resourcegroup);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Font");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_FontManager_createFont00(tolua_S);
}

/* method: destroyFont of class  CEGUI::FontManager */
static int tolua_CEGUI_CEGUI_FontManager_destroyFont00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::FontManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::FontManager* self = (CEGUI::FontManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyFont'",NULL);
#endif
 {
  self->destroyFont(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroyFont'.",&tolua_err);
 return 0;
#endif
}

/* method: destroyFont of class  CEGUI::FontManager */
static int tolua_CEGUI_CEGUI_FontManager_destroyFont01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::FontManager",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Font",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::FontManager* self = (CEGUI::FontManager*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Font* font = ((CEGUI::Font*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyFont'",NULL);
#endif
 {
  self->destroyFont(font);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_FontManager_destroyFont00(tolua_S);
}

/* method: destroyAllFonts of class  CEGUI::FontManager */
static int tolua_CEGUI_CEGUI_FontManager_destroyAllFonts00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::FontManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::FontManager* self = (CEGUI::FontManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyAllFonts'",NULL);
#endif
 {
  self->destroyAllFonts();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroyAllFonts'.",&tolua_err);
 return 0;
#endif
}

/* method: isFontPresent of class  CEGUI::FontManager */
static int tolua_CEGUI_CEGUI_FontManager_isFontPresent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::FontManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::FontManager* self = (const CEGUI::FontManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isFontPresent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isFontPresent(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isFontPresent'.",&tolua_err);
 return 0;
#endif
}

/* method: getFont of class  CEGUI::FontManager */
static int tolua_CEGUI_CEGUI_FontManager_getFont00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::FontManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::FontManager* self = (const CEGUI::FontManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFont'",NULL);
#endif
 {
  CEGUI::Font* tolua_ret = (CEGUI::Font*)  self->getFont(name);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Font");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFont'.",&tolua_err);
 return 0;
#endif
}

/* method: notifyScreenResolution of class  CEGUI::FontManager */
static int tolua_CEGUI_CEGUI_FontManager_notifyScreenResolution00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::FontManager",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::FontManager* self = (CEGUI::FontManager*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'notifyScreenResolution'",NULL);
#endif
 {
  self->notifyScreenResolution(*sz);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'notifyScreenResolution'.",&tolua_err);
 return 0;
#endif
}

/* method: getSize of class  CEGUI::Image */
static int tolua_CEGUI_CEGUI_Image_getSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Image* self = (const CEGUI::Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->getSize();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSize'.",&tolua_err);
 return 0;
#endif
}

/* method: getWidth of class  CEGUI::Image */
static int tolua_CEGUI_CEGUI_Image_getWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Image* self = (const CEGUI::Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWidth'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getWidth();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWidth'.",&tolua_err);
 return 0;
#endif
}

/* method: getHeight of class  CEGUI::Image */
static int tolua_CEGUI_CEGUI_Image_getHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Image* self = (const CEGUI::Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHeight'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getHeight();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHeight'.",&tolua_err);
 return 0;
#endif
}

/* method: getOffsets of class  CEGUI::Image */
static int tolua_CEGUI_CEGUI_Image_getOffsets00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Image* self = (const CEGUI::Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOffsets'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->getOffsets();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOffsets'.",&tolua_err);
 return 0;
#endif
}

/* method: getOffsetX of class  CEGUI::Image */
static int tolua_CEGUI_CEGUI_Image_getOffsetX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Image* self = (const CEGUI::Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOffsetX'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getOffsetX();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOffsetX'.",&tolua_err);
 return 0;
#endif
}

/* method: getOffsetY of class  CEGUI::Image */
static int tolua_CEGUI_CEGUI_Image_getOffsetY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Image* self = (const CEGUI::Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOffsetY'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getOffsetY();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOffsetY'.",&tolua_err);
 return 0;
#endif
}

/* method: getName of class  CEGUI::Image */
static int tolua_CEGUI_CEGUI_Image_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Image* self = (const CEGUI::Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
 {
  string tolua_ret = (string)  self->getName();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}

/* method: getImagesetName of class  CEGUI::Image */
static int tolua_CEGUI_CEGUI_Image_getImagesetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Image* self = (const CEGUI::Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImagesetName'",NULL);
#endif
 {
  string tolua_ret = (string)  self->getImagesetName();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImagesetName'.",&tolua_err);
 return 0;
#endif
}

/* method: getSourceTextureArea of class  CEGUI::Image */
static int tolua_CEGUI_CEGUI_Image_getSourceTextureArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Image* self = (const CEGUI::Image*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSourceTextureArea'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->getSourceTextureArea();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSourceTextureArea'.",&tolua_err);
 return 0;
#endif
}

/* method: getWidth of class  CEGUI::Texture */
static int tolua_CEGUI_CEGUI_Texture_getWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Texture",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Texture* self = (const CEGUI::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWidth'",NULL);
#endif
 {
  unsigned short tolua_ret = (unsigned short)  self->getWidth();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWidth'.",&tolua_err);
 return 0;
#endif
}

/* method: getHeight of class  CEGUI::Texture */
static int tolua_CEGUI_CEGUI_Texture_getHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Texture",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Texture* self = (const CEGUI::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHeight'",NULL);
#endif
 {
  unsigned short tolua_ret = (unsigned short)  self->getHeight();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHeight'.",&tolua_err);
 return 0;
#endif
}

/* method: loadFromFile of class  CEGUI::Texture */
static int tolua_CEGUI_CEGUI_Texture_loadFromFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Texture",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Texture* self = (CEGUI::Texture*)  tolua_tousertype(tolua_S,1,0);
  string filename = ((string)  tolua_tocppstring(tolua_S,2,0));
  string resourcegroup = ((string)  tolua_tocppstring(tolua_S,3,""));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadFromFile'",NULL);
#endif
 {
  self->loadFromFile(filename,resourcegroup);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadFromFile'.",&tolua_err);
 return 0;
#endif
}

/* method: getRenderer of class  CEGUI::Texture */
static int tolua_CEGUI_CEGUI_Texture_getRenderer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Texture",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Texture* self = (const CEGUI::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRenderer'",NULL);
#endif
 {
  CEGUI::Renderer* tolua_ret = (CEGUI::Renderer*)  self->getRenderer();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Renderer");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRenderer'.",&tolua_err);
 return 0;
#endif
}

/* method: getTexture of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_getTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTexture'",NULL);
#endif
 {
  CEGUI::Texture* tolua_ret = (CEGUI::Texture*)  self->getTexture();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Texture");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTexture'.",&tolua_err);
 return 0;
#endif
}

/* method: getName of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
 {
  string tolua_ret = (string)  self->getName();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}

/* method: getImageCount of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_getImageCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImageCount'",NULL);
#endif
 {
  unsigned int tolua_ret = (unsigned int)  self->getImageCount();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImageCount'.",&tolua_err);
 return 0;
#endif
}

/* method: isImageDefined of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_isImageDefined00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isImageDefined'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isImageDefined(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isImageDefined'.",&tolua_err);
 return 0;
#endif
}

/* method: getImage of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_getImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImage'",NULL);
#endif
 {
  const CEGUI::Image& tolua_ret = (const CEGUI::Image&)  self->getImage(name);
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const CEGUI::Image");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImage'.",&tolua_err);
 return 0;
#endif
}

/* method: undefineImage of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_undefineImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Imageset",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Imageset* self = (CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'undefineImage'",NULL);
#endif
 {
  self->undefineImage(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'undefineImage'.",&tolua_err);
 return 0;
#endif
}

/* method: undefineAllImages of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_undefineAllImages00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Imageset",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Imageset* self = (CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'undefineAllImages'",NULL);
#endif
 {
  self->undefineAllImages();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'undefineAllImages'.",&tolua_err);
 return 0;
#endif
}

/* method: getImageSize of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_getImageSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImageSize'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->getImageSize(name);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImageSize'.",&tolua_err);
 return 0;
#endif
}

/* method: getImageWidth of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_getImageWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImageWidth'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getImageWidth(name);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImageWidth'.",&tolua_err);
 return 0;
#endif
}

/* method: getImageHeight of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_getImageHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImageHeight'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getImageHeight(name);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImageHeight'.",&tolua_err);
 return 0;
#endif
}

/* method: getImageOffset of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_getImageOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImageOffset'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->getImageOffset(name);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImageOffset'.",&tolua_err);
 return 0;
#endif
}

/* method: getImageOffsetX of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_getImageOffsetX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImageOffsetX'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getImageOffsetX(name);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImageOffsetX'.",&tolua_err);
 return 0;
#endif
}

/* method: getImageOffsetY of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_getImageOffsetY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImageOffsetY'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getImageOffsetY(name);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImageOffsetY'.",&tolua_err);
 return 0;
#endif
}

/* method: defineImage of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_defineImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Imageset",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,4,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,5,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Imageset* self = (CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  const CEGUI::Point* pos = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,3,0));
  const CEGUI::Size* size = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,4,0));
  const CEGUI::Point* render_offset = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'defineImage'",NULL);
#endif
 {
  self->defineImage(name,*pos,*size,*render_offset);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'defineImage'.",&tolua_err);
 return 0;
#endif
}

/* method: defineImage of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_defineImage01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Imageset",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,4,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Imageset* self = (CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  const CEGUI::Rect* rect = ((const CEGUI::Rect*)  tolua_tousertype(tolua_S,3,0));
  const CEGUI::Point* render_offset = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'defineImage'",NULL);
#endif
 {
  self->defineImage(name,*rect,*render_offset);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Imageset_defineImage00(tolua_S);
}

/* method: isAutoScaled of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_isAutoScaled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isAutoScaled'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isAutoScaled();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isAutoScaled'.",&tolua_err);
 return 0;
#endif
}

/* method: setAutoScalingEnabled of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_setAutoScalingEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Imageset",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Imageset* self = (CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAutoScalingEnabled'",NULL);
#endif
 {
  self->setAutoScalingEnabled(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAutoScalingEnabled'.",&tolua_err);
 return 0;
#endif
}

/* method: getNativeResolution of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_getNativeResolution00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Imageset",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Imageset* self = (const CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNativeResolution'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->getNativeResolution();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNativeResolution'.",&tolua_err);
 return 0;
#endif
}

/* method: setNativeResolution of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_setNativeResolution00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Imageset",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Imageset* self = (CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setNativeResolution'",NULL);
#endif
 {
  self->setNativeResolution(*sz);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setNativeResolution'.",&tolua_err);
 return 0;
#endif
}

/* method: notifyScreenResolution of class  CEGUI::Imageset */
static int tolua_CEGUI_CEGUI_Imageset_notifyScreenResolution00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Imageset",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Imageset* self = (CEGUI::Imageset*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'notifyScreenResolution'",NULL);
#endif
 {
  self->notifyScreenResolution(*sz);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'notifyScreenResolution'.",&tolua_err);
 return 0;
#endif
}

/* method: getSingleton of class  CEGUI::ImagesetManager */
static int tolua_CEGUI_CEGUI_ImagesetManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::ImagesetManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::ImagesetManager& tolua_ret = (CEGUI::ImagesetManager&)  CEGUI::ImagesetManager::getSingleton();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CEGUI::ImagesetManager");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}

/* method: createImageset of class  CEGUI::ImagesetManager */
static int tolua_CEGUI_CEGUI_ImagesetManager_createImageset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::ImagesetManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"CEGUI::Texture",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::ImagesetManager* self = (CEGUI::ImagesetManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  CEGUI::Texture* texture = ((CEGUI::Texture*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createImageset'",NULL);
#endif
 {
  CEGUI::Imageset* tolua_ret = (CEGUI::Imageset*)  self->createImageset(name,texture);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Imageset");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createImageset'.",&tolua_err);
 return 0;
#endif
}

/* method: createImageset of class  CEGUI::ImagesetManager */
static int tolua_CEGUI_CEGUI_ImagesetManager_createImageset01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::ImagesetManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::ImagesetManager* self = (CEGUI::ImagesetManager*)  tolua_tousertype(tolua_S,1,0);
  string filename = ((string)  tolua_tocppstring(tolua_S,2,0));
  string resourcegroup = ((string)  tolua_tocppstring(tolua_S,3,""));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createImageset'",NULL);
#endif
 {
  CEGUI::Imageset* tolua_ret = (CEGUI::Imageset*)  self->createImageset(filename,resourcegroup);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Imageset");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_ImagesetManager_createImageset00(tolua_S);
}

/* method: destroyImageset of class  CEGUI::ImagesetManager */
static int tolua_CEGUI_CEGUI_ImagesetManager_destroyImageset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::ImagesetManager",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Imageset",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::ImagesetManager* self = (CEGUI::ImagesetManager*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Imageset* imageset = ((CEGUI::Imageset*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyImageset'",NULL);
#endif
 {
  self->destroyImageset(imageset);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroyImageset'.",&tolua_err);
 return 0;
#endif
}

/* method: destroyImageset of class  CEGUI::ImagesetManager */
static int tolua_CEGUI_CEGUI_ImagesetManager_destroyImageset01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::ImagesetManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::ImagesetManager* self = (CEGUI::ImagesetManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyImageset'",NULL);
#endif
 {
  self->destroyImageset(name);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_ImagesetManager_destroyImageset00(tolua_S);
}

/* method: destroyAllImagesets of class  CEGUI::ImagesetManager */
static int tolua_CEGUI_CEGUI_ImagesetManager_destroyAllImagesets00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::ImagesetManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::ImagesetManager* self = (CEGUI::ImagesetManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyAllImagesets'",NULL);
#endif
 {
  self->destroyAllImagesets();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroyAllImagesets'.",&tolua_err);
 return 0;
#endif
}

/* method: getImageset of class  CEGUI::ImagesetManager */
static int tolua_CEGUI_CEGUI_ImagesetManager_getImageset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::ImagesetManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::ImagesetManager* self = (const CEGUI::ImagesetManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImageset'",NULL);
#endif
 {
  CEGUI::Imageset* tolua_ret = (CEGUI::Imageset*)  self->getImageset(name);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Imageset");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImageset'.",&tolua_err);
 return 0;
#endif
}

/* method: isImagesetPresent of class  CEGUI::ImagesetManager */
static int tolua_CEGUI_CEGUI_ImagesetManager_isImagesetPresent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::ImagesetManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::ImagesetManager* self = (const CEGUI::ImagesetManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isImagesetPresent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isImagesetPresent(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isImagesetPresent'.",&tolua_err);
 return 0;
#endif
}

/* method: notifyScreenResolution of class  CEGUI::ImagesetManager */
static int tolua_CEGUI_CEGUI_ImagesetManager_notifyScreenResolution00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::ImagesetManager",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::ImagesetManager* self = (CEGUI::ImagesetManager*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* size = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'notifyScreenResolution'",NULL);
#endif
 {
  self->notifyScreenResolution(*size);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'notifyScreenResolution'.",&tolua_err);
 return 0;
#endif
}

/* get function: handled of class  CEGUI::EventArgs */
static int tolua_get_CEGUI__EventArgs_handled(lua_State* tolua_S)
{
  CEGUI::EventArgs* self = (CEGUI::EventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'handled'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->handled);
 return 1;
}

/* set function: handled of class  CEGUI::EventArgs */
static int tolua_set_CEGUI__EventArgs_handled(lua_State* tolua_S)
{
  CEGUI::EventArgs* self = (CEGUI::EventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'handled'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->handled = ((bool)  tolua_toboolean(tolua_S,2,0));
 return 0;
}

/* method: new of class  CEGUI::EventArgs */
static int tolua_CEGUI_CEGUI_EventArgs_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::EventArgs",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::EventArgs* tolua_ret = (CEGUI::EventArgs*)  new CEGUI::EventArgs();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::EventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  CEGUI::EventArgs */
static int tolua_CEGUI_CEGUI_EventArgs_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::EventArgs",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::EventArgs* tolua_ret = (CEGUI::EventArgs*)  new CEGUI::EventArgs();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::EventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* get function: mouseCursor of class  CEGUI::MouseCursorEventArgs */
static int tolua_get_CEGUI__MouseCursorEventArgs_mouseCursor_ptr(lua_State* tolua_S)
{
  CEGUI::MouseCursorEventArgs* self = (CEGUI::MouseCursorEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseCursor'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->mouseCursor,"CEGUI::MouseCursor");
 return 1;
}

/* set function: mouseCursor of class  CEGUI::MouseCursorEventArgs */
static int tolua_set_CEGUI__MouseCursorEventArgs_mouseCursor_ptr(lua_State* tolua_S)
{
  CEGUI::MouseCursorEventArgs* self = (CEGUI::MouseCursorEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseCursor'",NULL);
 if (!tolua_isusertype(tolua_S,2,"CEGUI::MouseCursor",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mouseCursor = ((CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: image of class  CEGUI::MouseCursorEventArgs */
static int tolua_get_CEGUI__MouseCursorEventArgs_image_ptr(lua_State* tolua_S)
{
  CEGUI::MouseCursorEventArgs* self = (CEGUI::MouseCursorEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'image'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->image,"const CEGUI::Image");
 return 1;
}

/* method: new of class  CEGUI::MouseCursorEventArgs */
static int tolua_CEGUI_CEGUI_MouseCursorEventArgs_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::MouseCursorEventArgs",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* cursor = ((CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,2,0));
 {
  CEGUI::MouseCursorEventArgs* tolua_ret = (CEGUI::MouseCursorEventArgs*)  new CEGUI::MouseCursorEventArgs(cursor);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::MouseCursorEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  CEGUI::MouseCursorEventArgs */
static int tolua_CEGUI_CEGUI_MouseCursorEventArgs_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::MouseCursorEventArgs",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* cursor = ((CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,2,0));
 {
  CEGUI::MouseCursorEventArgs* tolua_ret = (CEGUI::MouseCursorEventArgs*)  new CEGUI::MouseCursorEventArgs(cursor);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::MouseCursorEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* get function: window of class  CEGUI::WindowEventArgs */
static int tolua_get_CEGUI__WindowEventArgs_window_ptr(lua_State* tolua_S)
{
  CEGUI::WindowEventArgs* self = (CEGUI::WindowEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'window'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->window,"CEGUI::Window");
 return 1;
}

/* set function: window of class  CEGUI::WindowEventArgs */
static int tolua_set_CEGUI__WindowEventArgs_window_ptr(lua_State* tolua_S)
{
  CEGUI::WindowEventArgs* self = (CEGUI::WindowEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'window'",NULL);
 if (!tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->window = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: new of class  CEGUI::WindowEventArgs */
static int tolua_CEGUI_CEGUI_WindowEventArgs_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::WindowEventArgs",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* wnd = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
 {
  CEGUI::WindowEventArgs* tolua_ret = (CEGUI::WindowEventArgs*)  new CEGUI::WindowEventArgs(wnd);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::WindowEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  CEGUI::WindowEventArgs */
static int tolua_CEGUI_CEGUI_WindowEventArgs_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::WindowEventArgs",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* wnd = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
 {
  CEGUI::WindowEventArgs* tolua_ret = (CEGUI::WindowEventArgs*)  new CEGUI::WindowEventArgs(wnd);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::WindowEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* get function: otherWindow of class  CEGUI::ActivationEventArgs */
static int tolua_get_CEGUI__ActivationEventArgs_otherWindow_ptr(lua_State* tolua_S)
{
  CEGUI::ActivationEventArgs* self = (CEGUI::ActivationEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'otherWindow'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->otherWindow,"CEGUI::Window");
 return 1;
}

/* set function: otherWindow of class  CEGUI::ActivationEventArgs */
static int tolua_set_CEGUI__ActivationEventArgs_otherWindow_ptr(lua_State* tolua_S)
{
  CEGUI::ActivationEventArgs* self = (CEGUI::ActivationEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'otherWindow'",NULL);
 if (!tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->otherWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* method: new of class  CEGUI::ActivationEventArgs */
static int tolua_CEGUI_CEGUI_ActivationEventArgs_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::ActivationEventArgs",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* wnd = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
 {
  CEGUI::ActivationEventArgs* tolua_ret = (CEGUI::ActivationEventArgs*)  new CEGUI::ActivationEventArgs(wnd);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::ActivationEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  CEGUI::ActivationEventArgs */
static int tolua_CEGUI_CEGUI_ActivationEventArgs_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::ActivationEventArgs",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* wnd = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
 {
  CEGUI::ActivationEventArgs* tolua_ret = (CEGUI::ActivationEventArgs*)  new CEGUI::ActivationEventArgs(wnd);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::ActivationEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* get function: d_oldIdx of class  CEGUI::HeaderSequenceEventArgs */
static int tolua_get_CEGUI__HeaderSequenceEventArgs_unsigned_oldIdx(lua_State* tolua_S)
{
  CEGUI::HeaderSequenceEventArgs* self = (CEGUI::HeaderSequenceEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_oldIdx'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->d_oldIdx);
 return 1;
}

/* set function: d_oldIdx of class  CEGUI::HeaderSequenceEventArgs */
static int tolua_set_CEGUI__HeaderSequenceEventArgs_unsigned_oldIdx(lua_State* tolua_S)
{
  CEGUI::HeaderSequenceEventArgs* self = (CEGUI::HeaderSequenceEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_oldIdx'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->d_oldIdx = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: d_newIdx of class  CEGUI::HeaderSequenceEventArgs */
static int tolua_get_CEGUI__HeaderSequenceEventArgs_unsigned_newIdx(lua_State* tolua_S)
{
  CEGUI::HeaderSequenceEventArgs* self = (CEGUI::HeaderSequenceEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_newIdx'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->d_newIdx);
 return 1;
}

/* set function: d_newIdx of class  CEGUI::HeaderSequenceEventArgs */
static int tolua_set_CEGUI__HeaderSequenceEventArgs_unsigned_newIdx(lua_State* tolua_S)
{
  CEGUI::HeaderSequenceEventArgs* self = (CEGUI::HeaderSequenceEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'd_newIdx'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->d_newIdx = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* method: new of class  CEGUI::HeaderSequenceEventArgs */
static int tolua_CEGUI_CEGUI_HeaderSequenceEventArgs_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::HeaderSequenceEventArgs",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* wnd = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  unsigned int old_index = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int new_index = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
 {
  CEGUI::HeaderSequenceEventArgs* tolua_ret = (CEGUI::HeaderSequenceEventArgs*)  new CEGUI::HeaderSequenceEventArgs(wnd,old_index,new_index);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::HeaderSequenceEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  CEGUI::HeaderSequenceEventArgs */
static int tolua_CEGUI_CEGUI_HeaderSequenceEventArgs_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::HeaderSequenceEventArgs",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* wnd = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  unsigned int old_index = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int new_index = ((unsigned int)  tolua_tonumber(tolua_S,4,0));
 {
  CEGUI::HeaderSequenceEventArgs* tolua_ret = (CEGUI::HeaderSequenceEventArgs*)  new CEGUI::HeaderSequenceEventArgs(wnd,old_index,new_index);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::HeaderSequenceEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* get function: position of class  CEGUI::MouseEventArgs */
static int tolua_get_CEGUI__MouseEventArgs_position(lua_State* tolua_S)
{
  CEGUI::MouseEventArgs* self = (CEGUI::MouseEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->position,"CEGUI::Point");
 return 1;
}

/* set function: position of class  CEGUI::MouseEventArgs */
static int tolua_set_CEGUI__MouseEventArgs_position(lua_State* tolua_S)
{
  CEGUI::MouseEventArgs* self = (CEGUI::MouseEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
 if (!tolua_isusertype(tolua_S,2,"CEGUI::Point",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->position = *((CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: moveDelta of class  CEGUI::MouseEventArgs */
static int tolua_get_CEGUI__MouseEventArgs_moveDelta(lua_State* tolua_S)
{
  CEGUI::MouseEventArgs* self = (CEGUI::MouseEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'moveDelta'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->moveDelta,"CEGUI::Vector2");
 return 1;
}

/* set function: moveDelta of class  CEGUI::MouseEventArgs */
static int tolua_set_CEGUI__MouseEventArgs_moveDelta(lua_State* tolua_S)
{
  CEGUI::MouseEventArgs* self = (CEGUI::MouseEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'moveDelta'",NULL);
 if (!tolua_isusertype(tolua_S,2,"CEGUI::Vector2",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->moveDelta = *((CEGUI::Vector2*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: button of class  CEGUI::MouseEventArgs */
static int tolua_get_CEGUI__MouseEventArgs_button(lua_State* tolua_S)
{
  CEGUI::MouseEventArgs* self = (CEGUI::MouseEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'button'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->button);
 return 1;
}

/* set function: button of class  CEGUI::MouseEventArgs */
static int tolua_set_CEGUI__MouseEventArgs_button(lua_State* tolua_S)
{
  CEGUI::MouseEventArgs* self = (CEGUI::MouseEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'button'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->button = ((CEGUI::MouseButton) (int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sysKeys of class  CEGUI::MouseEventArgs */
static int tolua_get_CEGUI__MouseEventArgs_unsigned_sysKeys(lua_State* tolua_S)
{
  CEGUI::MouseEventArgs* self = (CEGUI::MouseEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sysKeys'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sysKeys);
 return 1;
}

/* set function: sysKeys of class  CEGUI::MouseEventArgs */
static int tolua_set_CEGUI__MouseEventArgs_unsigned_sysKeys(lua_State* tolua_S)
{
  CEGUI::MouseEventArgs* self = (CEGUI::MouseEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sysKeys'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sysKeys = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: wheelChange of class  CEGUI::MouseEventArgs */
static int tolua_get_CEGUI__MouseEventArgs_wheelChange(lua_State* tolua_S)
{
  CEGUI::MouseEventArgs* self = (CEGUI::MouseEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelChange'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->wheelChange);
 return 1;
}

/* set function: wheelChange of class  CEGUI::MouseEventArgs */
static int tolua_set_CEGUI__MouseEventArgs_wheelChange(lua_State* tolua_S)
{
  CEGUI::MouseEventArgs* self = (CEGUI::MouseEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelChange'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wheelChange = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* method: new of class  CEGUI::MouseEventArgs */
static int tolua_CEGUI_CEGUI_MouseEventArgs_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::MouseEventArgs",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* wnd = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
 {
  CEGUI::MouseEventArgs* tolua_ret = (CEGUI::MouseEventArgs*)  new CEGUI::MouseEventArgs(wnd);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::MouseEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  CEGUI::MouseEventArgs */
static int tolua_CEGUI_CEGUI_MouseEventArgs_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::MouseEventArgs",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* wnd = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
 {
  CEGUI::MouseEventArgs* tolua_ret = (CEGUI::MouseEventArgs*)  new CEGUI::MouseEventArgs(wnd);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::MouseEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* get function: codepoint of class  CEGUI::KeyEventArgs */
static int tolua_get_CEGUI__KeyEventArgs_unsigned_codepoint(lua_State* tolua_S)
{
  CEGUI::KeyEventArgs* self = (CEGUI::KeyEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'codepoint'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->codepoint);
 return 1;
}

/* set function: codepoint of class  CEGUI::KeyEventArgs */
static int tolua_set_CEGUI__KeyEventArgs_unsigned_codepoint(lua_State* tolua_S)
{
  CEGUI::KeyEventArgs* self = (CEGUI::KeyEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'codepoint'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->codepoint = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: scancode of class  CEGUI::KeyEventArgs */
static int tolua_get_CEGUI__KeyEventArgs_scancode(lua_State* tolua_S)
{
  CEGUI::KeyEventArgs* self = (CEGUI::KeyEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'scancode'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->scancode);
 return 1;
}

/* set function: scancode of class  CEGUI::KeyEventArgs */
static int tolua_set_CEGUI__KeyEventArgs_scancode(lua_State* tolua_S)
{
  CEGUI::KeyEventArgs* self = (CEGUI::KeyEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'scancode'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->scancode = ((CEGUI::Key::Scan) (int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sysKeys of class  CEGUI::KeyEventArgs */
static int tolua_get_CEGUI__KeyEventArgs_unsigned_sysKeys(lua_State* tolua_S)
{
  CEGUI::KeyEventArgs* self = (CEGUI::KeyEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sysKeys'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sysKeys);
 return 1;
}

/* set function: sysKeys of class  CEGUI::KeyEventArgs */
static int tolua_set_CEGUI__KeyEventArgs_unsigned_sysKeys(lua_State* tolua_S)
{
  CEGUI::KeyEventArgs* self = (CEGUI::KeyEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sysKeys'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sysKeys = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* method: new of class  CEGUI::KeyEventArgs */
static int tolua_CEGUI_CEGUI_KeyEventArgs_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::KeyEventArgs",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* wnd = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
 {
  CEGUI::KeyEventArgs* tolua_ret = (CEGUI::KeyEventArgs*)  new CEGUI::KeyEventArgs(wnd);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::KeyEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  CEGUI::KeyEventArgs */
static int tolua_CEGUI_CEGUI_KeyEventArgs_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::KeyEventArgs",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* wnd = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
 {
  CEGUI::KeyEventArgs* tolua_ret = (CEGUI::KeyEventArgs*)  new CEGUI::KeyEventArgs(wnd);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"CEGUI::KeyEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* function: CEGUI::toMouseCursorEventArgs */
static int tolua_CEGUI_CEGUI_toMouseCursorEventArgs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::EventArgs",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::EventArgs* e = ((const CEGUI::EventArgs*)  tolua_tousertype(tolua_S,1,0));
 {
  const CEGUI::MouseCursorEventArgs& tolua_ret = (const CEGUI::MouseCursorEventArgs&)  CEGUI::toMouseCursorEventArgs(*e);
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const CEGUI::MouseCursorEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toMouseCursorEventArgs'.",&tolua_err);
 return 0;
#endif
}

/* function: CEGUI::toWindowEventArgs */
static int tolua_CEGUI_CEGUI_toWindowEventArgs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::EventArgs",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::EventArgs* e = ((const CEGUI::EventArgs*)  tolua_tousertype(tolua_S,1,0));
 {
  const CEGUI::WindowEventArgs& tolua_ret = (const CEGUI::WindowEventArgs&)  CEGUI::toWindowEventArgs(*e);
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const CEGUI::WindowEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toWindowEventArgs'.",&tolua_err);
 return 0;
#endif
}

/* function: CEGUI::toActivationEventArgs */
static int tolua_CEGUI_CEGUI_toActivationEventArgs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::EventArgs",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::EventArgs* e = ((const CEGUI::EventArgs*)  tolua_tousertype(tolua_S,1,0));
 {
  const CEGUI::ActivationEventArgs& tolua_ret = (const CEGUI::ActivationEventArgs&)  CEGUI::toActivationEventArgs(*e);
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const CEGUI::ActivationEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toActivationEventArgs'.",&tolua_err);
 return 0;
#endif
}

/* function: CEGUI::toHeaderSequenceEventArgs */
static int tolua_CEGUI_CEGUI_toHeaderSequenceEventArgs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::EventArgs",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::EventArgs* e = ((const CEGUI::EventArgs*)  tolua_tousertype(tolua_S,1,0));
 {
  const CEGUI::HeaderSequenceEventArgs& tolua_ret = (const CEGUI::HeaderSequenceEventArgs&)  CEGUI::toHeaderSequenceEventArgs(*e);
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const CEGUI::HeaderSequenceEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toHeaderSequenceEventArgs'.",&tolua_err);
 return 0;
#endif
}

/* function: CEGUI::toMouseEventArgs */
static int tolua_CEGUI_CEGUI_toMouseEventArgs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::EventArgs",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::EventArgs* e = ((const CEGUI::EventArgs*)  tolua_tousertype(tolua_S,1,0));
 {
  const CEGUI::MouseEventArgs& tolua_ret = (const CEGUI::MouseEventArgs&)  CEGUI::toMouseEventArgs(*e);
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const CEGUI::MouseEventArgs");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toMouseEventArgs'.",&tolua_err);
 return 0;
#endif
}

/* method: getSingleton of class  CEGUI::GlobalEventSet */
static int tolua_CEGUI_CEGUI_GlobalEventSet_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::GlobalEventSet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::GlobalEventSet& tolua_ret = (CEGUI::GlobalEventSet&)  CEGUI::GlobalEventSet::getSingleton();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CEGUI::GlobalEventSet");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}

/* method: addEvent of class  CEGUI::GlobalEventSet */
static int tolua_CEGUI_CEGUI_GlobalEventSet_addEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::GlobalEventSet",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::GlobalEventSet* self = (CEGUI::GlobalEventSet*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEvent'",NULL);
#endif
 {
  self->addEvent(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: removeEvent of class  CEGUI::GlobalEventSet */
static int tolua_CEGUI_CEGUI_GlobalEventSet_removeEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::GlobalEventSet",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::GlobalEventSet* self = (CEGUI::GlobalEventSet*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeEvent'",NULL);
#endif
 {
  self->removeEvent(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: removeAllEvents of class  CEGUI::GlobalEventSet */
static int tolua_CEGUI_CEGUI_GlobalEventSet_removeAllEvents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::GlobalEventSet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::GlobalEventSet* self = (CEGUI::GlobalEventSet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAllEvents'",NULL);
#endif
 {
  self->removeAllEvents();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAllEvents'.",&tolua_err);
 return 0;
#endif
}

/* method: isEventPresent of class  CEGUI::GlobalEventSet */
static int tolua_CEGUI_CEGUI_GlobalEventSet_isEventPresent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::GlobalEventSet",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::GlobalEventSet* self = (CEGUI::GlobalEventSet*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isEventPresent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isEventPresent(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isEventPresent'.",&tolua_err);
 return 0;
#endif
}

/* method: subscribeScriptedEvent of class  CEGUI::GlobalEventSet */
static int tolua_CEGUI_CEGUI_GlobalEventSet_subscribeEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::GlobalEventSet",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::GlobalEventSet* self = (CEGUI::GlobalEventSet*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  string callback_name = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'subscribeScriptedEvent'",NULL);
#endif
 {
  self->subscribeScriptedEvent(name,callback_name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'subscribeEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: fireEvent of class  CEGUI::GlobalEventSet */
static int tolua_CEGUI_CEGUI_GlobalEventSet_fireEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::GlobalEventSet",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"CEGUI::EventArgs",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,4,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::GlobalEventSet* self = (CEGUI::GlobalEventSet*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  CEGUI::EventArgs* args = ((CEGUI::EventArgs*)  tolua_tousertype(tolua_S,3,0));
  string eventnamespace = ((string)  tolua_tocppstring(tolua_S,4,""));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fireEvent'",NULL);
#endif
 {
  self->fireEvent(name,*args,eventnamespace);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fireEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: isMuted of class  CEGUI::GlobalEventSet */
static int tolua_CEGUI_CEGUI_GlobalEventSet_isMuted00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::GlobalEventSet",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::GlobalEventSet* self = (const CEGUI::GlobalEventSet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isMuted'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isMuted();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isMuted'.",&tolua_err);
 return 0;
#endif
}

/* method: setMutedState of class  CEGUI::GlobalEventSet */
static int tolua_CEGUI_CEGUI_GlobalEventSet_setMutedState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::GlobalEventSet",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::GlobalEventSet* self = (CEGUI::GlobalEventSet*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMutedState'",NULL);
#endif
 {
  self->setMutedState(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMutedState'.",&tolua_err);
 return 0;
#endif
}

/* method: getSingleton of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::MouseCursor& tolua_ret = (CEGUI::MouseCursor&)  CEGUI::MouseCursor::getSingleton();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CEGUI::MouseCursor");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}

/* method: setImage of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_setImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Image* image = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setImage'",NULL);
#endif
 {
  self->setImage(image);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setImage'.",&tolua_err);
 return 0;
#endif
}

/* method: setImage of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_setImage01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
  string imageset = ((string)  tolua_tocppstring(tolua_S,2,0));
  string image = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setImage'",NULL);
#endif
 {
  self->setImage(imageset,image);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_MouseCursor_setImage00(tolua_S);
}

/* method: getImage of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_getImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::MouseCursor* self = (const CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImage'",NULL);
#endif
 {
  const CEGUI::Image* tolua_ret = (const CEGUI::Image*)  self->getImage();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"const CEGUI::Image");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImage'.",&tolua_err);
 return 0;
#endif
}

/* method: setPosition of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* pos = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
 {
  self->setPosition(*pos);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: offsetPosition of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_offsetPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* offset = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'offsetPosition'",NULL);
#endif
 {
  self->offsetPosition(*offset);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'offsetPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: setConstraintArea of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_setConstraintArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Rect* area = ((const CEGUI::Rect*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setConstraintArea'",NULL);
#endif
 {
  self->setConstraintArea(area);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setConstraintArea'.",&tolua_err);
 return 0;
#endif
}

/* method: getPosition of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::MouseCursor* self = (const CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->getPosition();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: getConstraintArea of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_getConstraintArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::MouseCursor* self = (const CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getConstraintArea'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->getConstraintArea();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getConstraintArea'.",&tolua_err);
 return 0;
#endif
}

/* method: getDisplayIndependantPosition of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_getDisplayIndependantPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::MouseCursor* self = (const CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDisplayIndependantPosition'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->getDisplayIndependantPosition();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDisplayIndependantPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: hide of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_hide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hide'",NULL);
#endif
 {
  self->hide();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hide'.",&tolua_err);
 return 0;
#endif
}

/* method: show of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'show'",NULL);
#endif
 {
  self->show();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'show'.",&tolua_err);
 return 0;
#endif
}

/* method: isVisible of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_isVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::MouseCursor* self = (const CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isVisible'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isVisible();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isVisible'.",&tolua_err);
 return 0;
#endif
}

/* method: addEvent of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_addEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEvent'",NULL);
#endif
 {
  self->addEvent(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: removeEvent of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_removeEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeEvent'",NULL);
#endif
 {
  self->removeEvent(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: removeAllEvents of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_removeAllEvents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAllEvents'",NULL);
#endif
 {
  self->removeAllEvents();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAllEvents'.",&tolua_err);
 return 0;
#endif
}

/* method: isEventPresent of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_isEventPresent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isEventPresent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isEventPresent(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isEventPresent'.",&tolua_err);
 return 0;
#endif
}

/* method: subscribeScriptedEvent of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_subscribeEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  string callback_name = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'subscribeScriptedEvent'",NULL);
#endif
 {
  self->subscribeScriptedEvent(name,callback_name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'subscribeEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: fireEvent of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_fireEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"CEGUI::EventArgs",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,4,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  CEGUI::EventArgs* args = ((CEGUI::EventArgs*)  tolua_tousertype(tolua_S,3,0));
  string eventnamespace = ((string)  tolua_tocppstring(tolua_S,4,""));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fireEvent'",NULL);
#endif
 {
  self->fireEvent(name,*args,eventnamespace);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fireEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: isMuted of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_isMuted00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::MouseCursor* self = (const CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isMuted'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isMuted();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isMuted'.",&tolua_err);
 return 0;
#endif
}

/* method: setMutedState of class  CEGUI::MouseCursor */
static int tolua_CEGUI_CEGUI_MouseCursor_setMutedState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::MouseCursor",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::MouseCursor* self = (CEGUI::MouseCursor*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMutedState'",NULL);
#endif
 {
  self->setMutedState(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMutedState'.",&tolua_err);
 return 0;
#endif
}

/* method: getType of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
 {
  string tolua_ret = (string)  self->getType();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}

/* method: getName of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
 {
  string tolua_ret = (string)  self->getName();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}

/* method: isDestroyedByParent of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isDestroyedByParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isDestroyedByParent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isDestroyedByParent();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isDestroyedByParent'.",&tolua_err);
 return 0;
#endif
}

/* method: isAlwaysOnTop of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isAlwaysOnTop00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isAlwaysOnTop'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isAlwaysOnTop();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isAlwaysOnTop'.",&tolua_err);
 return 0;
#endif
}

/* method: isDisabled of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isDisabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isDisabled'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isDisabled();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isDisabled'.",&tolua_err);
 return 0;
#endif
}

/* method: isVisible of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isVisible'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isVisible();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isVisible'.",&tolua_err);
 return 0;
#endif
}

/* method: isActive of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isActive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isActive'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isActive();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isActive'.",&tolua_err);
 return 0;
#endif
}

/* method: isClippedByParent of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isClippedByParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isClippedByParent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isClippedByParent();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isClippedByParent'.",&tolua_err);
 return 0;
#endif
}

/* method: setDestroyedByParent of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setDestroyedByParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDestroyedByParent'",NULL);
#endif
 {
  self->setDestroyedByParent(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDestroyedByParent'.",&tolua_err);
 return 0;
#endif
}

/* method: setAlwaysOnTop of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setAlwaysOnTop00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAlwaysOnTop'",NULL);
#endif
 {
  self->setAlwaysOnTop(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAlwaysOnTop'.",&tolua_err);
 return 0;
#endif
}

/* method: setEnabled of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setEnabled'",NULL);
#endif
 {
  self->setEnabled(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setEnabled'.",&tolua_err);
 return 0;
#endif
}

/* method: enable of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_enable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enable'",NULL);
#endif
 {
  self->enable();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enable'.",&tolua_err);
 return 0;
#endif
}

/* method: disable of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_disable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'disable'",NULL);
#endif
 {
  self->disable();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'disable'.",&tolua_err);
 return 0;
#endif
}

/* method: setVisible of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVisible'",NULL);
#endif
 {
  self->setVisible(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVisible'.",&tolua_err);
 return 0;
#endif
}

/* method: show of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'show'",NULL);
#endif
 {
  self->show();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'show'.",&tolua_err);
 return 0;
#endif
}

/* method: hide of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_hide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hide'",NULL);
#endif
 {
  self->hide();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hide'.",&tolua_err);
 return 0;
#endif
}

/* method: activate of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_activate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'activate'",NULL);
#endif
 {
  self->activate();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'activate'.",&tolua_err);
 return 0;
#endif
}

/* method: deactivate of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_deactivate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deactivate'",NULL);
#endif
 {
  self->deactivate();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deactivate'.",&tolua_err);
 return 0;
#endif
}

/* method: setClippedByParent of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setClippedByParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setClippedByParent'",NULL);
#endif
 {
  self->setClippedByParent(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setClippedByParent'.",&tolua_err);
 return 0;
#endif
}

/* method: getID of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getID'",NULL);
#endif
 {
  unsigned int tolua_ret = (unsigned int)  self->getID();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getID'.",&tolua_err);
 return 0;
#endif
}

/* method: setID of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  unsigned int ID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setID'",NULL);
#endif
 {
  self->setID(ID);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setID'.",&tolua_err);
 return 0;
#endif
}

/* method: getChildCount of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getChildCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChildCount'",NULL);
#endif
 {
  unsigned int tolua_ret = (unsigned int)  self->getChildCount();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChildCount'.",&tolua_err);
 return 0;
#endif
}

/* method: isChild of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isChild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  unsigned int ID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isChild'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isChild(ID);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isChild'.",&tolua_err);
 return 0;
#endif
}

/* method: isChild of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isChild01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isChild'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isChild(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_isChild00(tolua_S);
}

/* method: isChild of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isChild02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Window* window = ((const CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isChild'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isChild(window);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_isChild01(tolua_S);
}

/* method: addChildWindow of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_addChildWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addChildWindow'",NULL);
#endif
 {
  self->addChildWindow(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addChildWindow'.",&tolua_err);
 return 0;
#endif
}

/* method: addChildWindow of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_addChildWindow01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* window = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addChildWindow'",NULL);
#endif
 {
  self->addChildWindow(window);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_addChildWindow00(tolua_S);
}

/* method: removeChildWindow of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_removeChildWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  unsigned int ID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeChildWindow'",NULL);
#endif
 {
  self->removeChildWindow(ID);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeChildWindow'.",&tolua_err);
 return 0;
#endif
}

/* method: removeChildWindow of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_removeChildWindow01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeChildWindow'",NULL);
#endif
 {
  self->removeChildWindow(name);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_removeChildWindow00(tolua_S);
}

/* method: removeChildWindow of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_removeChildWindow02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* window = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeChildWindow'",NULL);
#endif
 {
  self->removeChildWindow(window);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_removeChildWindow01(tolua_S);
}

/* method: getChild of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getChild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  unsigned int ID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChild'",NULL);
#endif
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getChild(ID);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChild'.",&tolua_err);
 return 0;
#endif
}

/* method: getChild of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getChild01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChild'",NULL);
#endif
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getChild(name);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_getChild00(tolua_S);
}

/* method: getChildAtIdx of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getChildAtIdx00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  unsigned int ID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChildAtIdx'",NULL);
#endif
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getChildAtIdx(ID);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChildAtIdx'.",&tolua_err);
 return 0;
#endif
}

/* method: getActiveChild of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getActiveChild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getActiveChild'",NULL);
#endif
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getActiveChild();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getActiveChild'.",&tolua_err);
 return 0;
#endif
}

/* method: getChildAtPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getChildAtPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* pos = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChildAtPosition'",NULL);
#endif
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getChildAtPosition(*pos);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChildAtPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: getParent of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParent'",NULL);
#endif
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getParent();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getParent'.",&tolua_err);
 return 0;
#endif
}

/* method: isAncestor of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isAncestor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  unsigned int ID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isAncestor'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isAncestor(ID);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isAncestor'.",&tolua_err);
 return 0;
#endif
}

/* method: isAncestor of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isAncestor01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isAncestor'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isAncestor(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_isAncestor00(tolua_S);
}

/* method: isAncestor of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isAncestor02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Window* window = ((const CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isAncestor'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isAncestor(window);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_isAncestor01(tolua_S);
}

/* method: setFont of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setFont00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFont'",NULL);
#endif
 {
  self->setFont(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFont'.",&tolua_err);
 return 0;
#endif
}

/* method: setFont of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setFont01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Font",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Font* font = ((const CEGUI::Font*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFont'",NULL);
#endif
 {
  self->setFont(font);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_setFont00(tolua_S);
}

/* method: getFont of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getFont00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFont'",NULL);
#endif
 {
  CEGUI::Font* tolua_ret = (CEGUI::Font*)  self->getFont();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Font");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFont'.",&tolua_err);
 return 0;
#endif
}

/* method: setText of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string text = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setText'",NULL);
#endif
 {
  self->setText(text);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setText'.",&tolua_err);
 return 0;
#endif
}

/* method: getText of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getText'",NULL);
#endif
 {
  string tolua_ret = (string)  self->getText();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getText'.",&tolua_err);
 return 0;
#endif
}

/* method: inheritsAlpha of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_inheritsAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'inheritsAlpha'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->inheritsAlpha();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'inheritsAlpha'.",&tolua_err);
 return 0;
#endif
}

/* method: getAlpha of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAlpha'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getAlpha();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAlpha'.",&tolua_err);
 return 0;
#endif
}

/* method: getEffectiveAlpha of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getEffectiveAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEffectiveAlpha'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getEffectiveAlpha();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEffectiveAlpha'.",&tolua_err);
 return 0;
#endif
}

/* method: setAlpha of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAlpha'",NULL);
#endif
 {
  self->setAlpha(a);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAlpha'.",&tolua_err);
 return 0;
#endif
}

/* method: setInheritsAlpha of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setInheritsAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInheritsAlpha'",NULL);
#endif
 {
  self->setInheritsAlpha(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInheritsAlpha'.",&tolua_err);
 return 0;
#endif
}

/* method: requestRedraw of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_requestRedraw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestRedraw'",NULL);
#endif
 {
  self->requestRedraw();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestRedraw'.",&tolua_err);
 return 0;
#endif
}

/* method: isZOrderingEnabled of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isZOrderingEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isZOrderingEnabled'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isZOrderingEnabled();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isZOrderingEnabled'.",&tolua_err);
 return 0;
#endif
}

/* method: setZOrderingEnabled of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setZOrderingEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setZOrderingEnabled'",NULL);
#endif
 {
  self->setZOrderingEnabled(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setZOrderingEnabled'.",&tolua_err);
 return 0;
#endif
}

/* method: getRect of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRect'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->getRect();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRect'.",&tolua_err);
 return 0;
#endif
}

/* method: getPixelRect of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getPixelRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPixelRect'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->getPixelRect();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPixelRect'.",&tolua_err);
 return 0;
#endif
}

/* method: getInnerRect of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getInnerRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInnerRect'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->getInnerRect();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInnerRect'.",&tolua_err);
 return 0;
#endif
}

/* method: getUnclippedPixelRect of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getUnclippedPixelRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUnclippedPixelRect'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->getUnclippedPixelRect();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUnclippedPixelRect'.",&tolua_err);
 return 0;
#endif
}

/* method: getUnclippedInnerRect of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getUnclippedInnerRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUnclippedInnerRect'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->getUnclippedInnerRect();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUnclippedInnerRect'.",&tolua_err);
 return 0;
#endif
}

/* method: getCaptureWindow of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getCaptureWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  CEGUI::Window::getCaptureWindow();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCaptureWindow'.",&tolua_err);
 return 0;
#endif
}

/* method: setRestoreCapture of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setRestoreCapture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRestoreCapture'",NULL);
#endif
 {
  self->setRestoreCapture(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRestoreCapture'.",&tolua_err);
 return 0;
#endif
}

/* method: restoresOldCapture of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_restoresOldCapture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'restoresOldCapture'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->restoresOldCapture();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'restoresOldCapture'.",&tolua_err);
 return 0;
#endif
}

/* method: distributesCapturedInputs of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_distributesCapturedInputs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'distributesCapturedInputs'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->distributesCapturedInputs();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'distributesCapturedInputs'.",&tolua_err);
 return 0;
#endif
}

/* method: setDistributesCapturedInputs of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setDistributesCapturedInputs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDistributesCapturedInputs'",NULL);
#endif
 {
  self->setDistributesCapturedInputs(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDistributesCapturedInputs'.",&tolua_err);
 return 0;
#endif
}

/* method: captureInput of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_captureInput00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'captureInput'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->captureInput();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'captureInput'.",&tolua_err);
 return 0;
#endif
}

/* method: releaseInput of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_releaseInput00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'releaseInput'",NULL);
#endif
 {
  self->releaseInput();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'releaseInput'.",&tolua_err);
 return 0;
#endif
}

/* method: isCapturedByThis of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isCapturedByThis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCapturedByThis'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isCapturedByThis();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCapturedByThis'.",&tolua_err);
 return 0;
#endif
}

/* method: isCapturedByAncestor of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isCapturedByAncestor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCapturedByAncestor'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isCapturedByAncestor();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCapturedByAncestor'.",&tolua_err);
 return 0;
#endif
}

/* method: isCapturedByChild of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isCapturedByChild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isCapturedByChild'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isCapturedByChild();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCapturedByChild'.",&tolua_err);
 return 0;
#endif
}

/* method: isHit of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isHit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* pos = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isHit'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isHit(*pos);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isHit'.",&tolua_err);
 return 0;
#endif
}

/* method: getMetricsMode of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getMetricsMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMetricsMode'",NULL);
#endif
 {
  CEGUI::MetricsMode tolua_ret = (CEGUI::MetricsMode)  self->getMetricsMode();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMetricsMode'.",&tolua_err);
 return 0;
#endif
}

/* method: setMetricsMode of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setMetricsMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMetricsMode'",NULL);
#endif
 {
  self->setMetricsMode(mode);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMetricsMode'.",&tolua_err);
 return 0;
#endif
}

/* method: getMouseCursor of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getMouseCursor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMouseCursor'",NULL);
#endif
 {
  const CEGUI::Image* tolua_ret = (const CEGUI::Image*)  self->getMouseCursor();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"const CEGUI::Image");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMouseCursor'.",&tolua_err);
 return 0;
#endif
}

/* method: setMouseCursor of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setMouseCursor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MouseCursorImage cursor = ((CEGUI::MouseCursorImage) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMouseCursor'",NULL);
#endif
 {
  self->setMouseCursor(cursor);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMouseCursor'.",&tolua_err);
 return 0;
#endif
}

/* method: setMouseCursor of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setMouseCursor01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Image* image = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMouseCursor'",NULL);
#endif
 {
  self->setMouseCursor(image);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_setMouseCursor00(tolua_S);
}

/* method: setMouseCursor of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setMouseCursor02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string imageset = ((string)  tolua_tocppstring(tolua_S,2,0));
  string image = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMouseCursor'",NULL);
#endif
 {
  self->setMouseCursor(imageset,image);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_setMouseCursor01(tolua_S);
}

/* method: getRelativeRect of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getRelativeRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRelativeRect'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->getRelativeRect();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRelativeRect'.",&tolua_err);
 return 0;
#endif
}

/* method: getRelativePosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getRelativePosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRelativePosition'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->getRelativePosition();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRelativePosition'.",&tolua_err);
 return 0;
#endif
}

/* method: getRelativeXPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getRelativeXPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRelativeXPosition'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getRelativeXPosition();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRelativeXPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: getRelativeYPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getRelativeYPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRelativeYPosition'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getRelativeYPosition();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRelativeYPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: getRelativeSize of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getRelativeSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRelativeSize'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->getRelativeSize();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRelativeSize'.",&tolua_err);
 return 0;
#endif
}

/* method: getRelativeWidth of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getRelativeWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRelativeWidth'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getRelativeWidth();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRelativeWidth'.",&tolua_err);
 return 0;
#endif
}

/* method: getRelativeHeight of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getRelativeHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRelativeHeight'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getRelativeHeight();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRelativeHeight'.",&tolua_err);
 return 0;
#endif
}

/* method: getAbsoluteRect of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getAbsoluteRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteRect'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->getAbsoluteRect();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteRect'.",&tolua_err);
 return 0;
#endif
}

/* method: getAbsolutePosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getAbsolutePosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsolutePosition'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->getAbsolutePosition();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsolutePosition'.",&tolua_err);
 return 0;
#endif
}

/* method: getAbsoluteXPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getAbsoluteXPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteXPosition'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getAbsoluteXPosition();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteXPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: getAbsoluteYPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getAbsoluteYPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteYPosition'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getAbsoluteYPosition();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteYPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: getAbsoluteSize of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getAbsoluteSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteSize'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->getAbsoluteSize();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteSize'.",&tolua_err);
 return 0;
#endif
}

/* method: getAbsoluteWidth of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getAbsoluteWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteWidth'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getAbsoluteWidth();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteWidth'.",&tolua_err);
 return 0;
#endif
}

/* method: getAbsoluteHeight of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getAbsoluteHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteHeight'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getAbsoluteHeight();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteHeight'.",&tolua_err);
 return 0;
#endif
}

/* method: setPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
  const CEGUI::Point* pos = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
 {
  self->setPosition(mode,*pos);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: setXPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setXPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
  float x = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setXPosition'",NULL);
#endif
 {
  self->setXPosition(mode,x);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setXPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: setYPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setYPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setYPosition'",NULL);
#endif
 {
  self->setYPosition(mode,y);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setYPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: setWidth of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
  float w = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWidth'",NULL);
#endif
 {
  self->setWidth(mode,w);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setWidth'.",&tolua_err);
 return 0;
#endif
}

/* method: setHeight of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
  float h = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setHeight'",NULL);
#endif
 {
  self->setHeight(mode,h);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setHeight'.",&tolua_err);
 return 0;
#endif
}

/* method: setSize of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSize'",NULL);
#endif
 {
  self->setSize(mode,*sz);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSize'.",&tolua_err);
 return 0;
#endif
}

/* method: setRect of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
  const CEGUI::Rect* r = ((const CEGUI::Rect*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRect'",NULL);
#endif
 {
  self->setRect(mode,*r);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRect'.",&tolua_err);
 return 0;
#endif
}

/* method: getPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->getPosition(mode);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: getXPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getXPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getXPosition'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getXPosition(mode);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getXPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: getYPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getYPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getYPosition'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getYPosition(mode);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getYPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: getWidth of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWidth'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getWidth(mode);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWidth'.",&tolua_err);
 return 0;
#endif
}

/* method: getHeight of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHeight'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getHeight(mode);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHeight'.",&tolua_err);
 return 0;
#endif
}

/* method: getSize of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->getSize(mode);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSize'.",&tolua_err);
 return 0;
#endif
}

/* method: getRect of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getRect01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MetricsMode mode = ((CEGUI::MetricsMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRect'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->getRect(mode);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_getRect00(tolua_S);
}

/* method: setMinimumSize of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setMinimumSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMinimumSize'",NULL);
#endif
 {
  self->setMinimumSize(*sz);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMinimumSize'.",&tolua_err);
 return 0;
#endif
}

/* method: setMaximumSize of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setMaximumSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaximumSize'",NULL);
#endif
 {
  self->setMaximumSize(*sz);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaximumSize'.",&tolua_err);
 return 0;
#endif
}

/* method: setSize of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setSize01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSize'",NULL);
#endif
 {
  self->setSize(*sz);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_setSize00(tolua_S);
}

/* method: setWidth of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setWidth01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWidth'",NULL);
#endif
 {
  self->setWidth(w);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_setWidth00(tolua_S);
}

/* method: setHeight of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setHeight01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float h = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setHeight'",NULL);
#endif
 {
  self->setHeight(h);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_setHeight00(tolua_S);
}

/* method: getWidth of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getWidth01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWidth'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getWidth();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_getWidth00(tolua_S);
}

/* method: getHeight of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getHeight01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHeight'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getHeight();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_getHeight00(tolua_S);
}

/* method: getSize of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getSize01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->getSize();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_getSize00(tolua_S);
}

/* method: getMinimumSize of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getMinimumSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMinimumSize'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->getMinimumSize();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMinimumSize'.",&tolua_err);
 return 0;
#endif
}

/* method: getMaximumSize of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getMaximumSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMaximumSize'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->getMaximumSize();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMaximumSize'.",&tolua_err);
 return 0;
#endif
}

/* method: setPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setPosition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* pos = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
 {
  self->setPosition(*pos);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_setPosition00(tolua_S);
}

/* method: setXPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setXPosition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setXPosition'",NULL);
#endif
 {
  self->setXPosition(x);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_setXPosition00(tolua_S);
}

/* method: setYPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setYPosition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setYPosition'",NULL);
#endif
 {
  self->setYPosition(y);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_setYPosition00(tolua_S);
}

/* method: getXPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getXPosition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getXPosition'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getXPosition();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_getXPosition00(tolua_S);
}

/* method: getYPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getYPosition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getYPosition'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getYPosition();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_getYPosition00(tolua_S);
}

/* method: getPosition of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getPosition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->getPosition();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_getPosition00(tolua_S);
}

/* method: setAreaRect of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setAreaRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Rect* rect = ((const CEGUI::Rect*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAreaRect'",NULL);
#endif
 {
  self->setAreaRect(*rect);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAreaRect'.",&tolua_err);
 return 0;
#endif
}

/* method: absoluteToRelativeX of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_absoluteToRelativeX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'absoluteToRelativeX'",NULL);
#endif
 {
  float tolua_ret = (float)  self->absoluteToRelativeX(x);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'absoluteToRelativeX'.",&tolua_err);
 return 0;
#endif
}

/* method: absoluteToRelativeY of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_absoluteToRelativeY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'absoluteToRelativeY'",NULL);
#endif
 {
  float tolua_ret = (float)  self->absoluteToRelativeY(y);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'absoluteToRelativeY'.",&tolua_err);
 return 0;
#endif
}

/* method: absoluteToRelative of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_absoluteToRelative00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* p = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'absoluteToRelative'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->absoluteToRelative(*p);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'absoluteToRelative'.",&tolua_err);
 return 0;
#endif
}

/* method: absoluteToRelative of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_absoluteToRelative01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'absoluteToRelative'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->absoluteToRelative(*sz);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_absoluteToRelative00(tolua_S);
}

/* method: absoluteToRelative of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_absoluteToRelative02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Rect* r = ((const CEGUI::Rect*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'absoluteToRelative'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->absoluteToRelative(*r);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_absoluteToRelative01(tolua_S);
}

/* method: relativeToAbsoluteX of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_relativeToAbsoluteX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'relativeToAbsoluteX'",NULL);
#endif
 {
  float tolua_ret = (float)  self->relativeToAbsoluteX(x);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'relativeToAbsoluteX'.",&tolua_err);
 return 0;
#endif
}

/* method: relativeToAbsoluteY of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_relativeToAbsoluteY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'relativeToAbsoluteY'",NULL);
#endif
 {
  float tolua_ret = (float)  self->relativeToAbsoluteY(y);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'relativeToAbsoluteY'.",&tolua_err);
 return 0;
#endif
}

/* method: relativeToAbsolute of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_relativeToAbsolute00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* p = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'relativeToAbsolute'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->relativeToAbsolute(*p);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'relativeToAbsolute'.",&tolua_err);
 return 0;
#endif
}

/* method: relativeToAbsolute of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_relativeToAbsolute01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'relativeToAbsolute'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->relativeToAbsolute(*sz);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_relativeToAbsolute00(tolua_S);
}

/* method: relativeToAbsolute of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_relativeToAbsolute02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Rect* r = ((const CEGUI::Rect*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'relativeToAbsolute'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->relativeToAbsolute(*r);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_relativeToAbsolute01(tolua_S);
}

/* method: windowToScreenX of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_windowToScreenX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'windowToScreenX'",NULL);
#endif
 {
  float tolua_ret = (float)  self->windowToScreenX(x);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'windowToScreenX'.",&tolua_err);
 return 0;
#endif
}

/* method: windowToScreenY of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_windowToScreenY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'windowToScreenY'",NULL);
#endif
 {
  float tolua_ret = (float)  self->windowToScreenY(y);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'windowToScreenY'.",&tolua_err);
 return 0;
#endif
}

/* method: windowToScreen of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_windowToScreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* p = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'windowToScreen'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->windowToScreen(*p);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'windowToScreen'.",&tolua_err);
 return 0;
#endif
}

/* method: windowToScreen of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_windowToScreen01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'windowToScreen'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->windowToScreen(*sz);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_windowToScreen00(tolua_S);
}

/* method: windowToScreen of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_windowToScreen02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Rect* r = ((const CEGUI::Rect*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'windowToScreen'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->windowToScreen(*r);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_windowToScreen01(tolua_S);
}

/* method: screenToWindowX of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_screenToWindowX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'screenToWindowX'",NULL);
#endif
 {
  float tolua_ret = (float)  self->screenToWindowX(x);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'screenToWindowX'.",&tolua_err);
 return 0;
#endif
}

/* method: screenToWindowY of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_screenToWindowY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'screenToWindowY'",NULL);
#endif
 {
  float tolua_ret = (float)  self->screenToWindowY(y);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'screenToWindowY'.",&tolua_err);
 return 0;
#endif
}

/* method: screenToWindow of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_screenToWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Point",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Point* p = ((const CEGUI::Point*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'screenToWindow'",NULL);
#endif
 {
  CEGUI::Point tolua_ret = (CEGUI::Point)  self->screenToWindow(*p);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Point(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Point));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Point");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'screenToWindow'.",&tolua_err);
 return 0;
#endif
}

/* method: screenToWindow of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_screenToWindow01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'screenToWindow'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->screenToWindow(*sz);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_screenToWindow00(tolua_S);
}

/* method: screenToWindow of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_screenToWindow02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Rect",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Rect* r = ((const CEGUI::Rect*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'screenToWindow'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->screenToWindow(*r);
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Window_screenToWindow01(tolua_S);
}

/* method: moveToFront of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_moveToFront00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'moveToFront'",NULL);
#endif
 {
  self->moveToFront();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'moveToFront'.",&tolua_err);
 return 0;
#endif
}

/* method: moveToBack of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_moveToBack00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'moveToBack'",NULL);
#endif
 {
  self->moveToBack();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'moveToBack'.",&tolua_err);
 return 0;
#endif
}

/* method: wantsMultiClickEvents of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_wantsMultiClickEvents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'wantsMultiClickEvents'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->wantsMultiClickEvents();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wantsMultiClickEvents'.",&tolua_err);
 return 0;
#endif
}

/* method: isMouseAutoRepeatEnabled of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isMouseAutoRepeatEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isMouseAutoRepeatEnabled'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isMouseAutoRepeatEnabled();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isMouseAutoRepeatEnabled'.",&tolua_err);
 return 0;
#endif
}

/* method: getAutoRepeatDelay of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getAutoRepeatDelay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAutoRepeatDelay'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getAutoRepeatDelay();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAutoRepeatDelay'.",&tolua_err);
 return 0;
#endif
}

/* method: getAutoRepeatRate of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getAutoRepeatRate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAutoRepeatRate'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getAutoRepeatRate();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAutoRepeatRate'.",&tolua_err);
 return 0;
#endif
}

/* method: setWantsMultiClickEvents of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setWantsMultiClickEvents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWantsMultiClickEvents'",NULL);
#endif
 {
  self->setWantsMultiClickEvents(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setWantsMultiClickEvents'.",&tolua_err);
 return 0;
#endif
}

/* method: setMouseAutoRepeatEnabled of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setMouseAutoRepeatEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMouseAutoRepeatEnabled'",NULL);
#endif
 {
  self->setMouseAutoRepeatEnabled(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMouseAutoRepeatEnabled'.",&tolua_err);
 return 0;
#endif
}

/* method: setAutoRepeatDelay of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setAutoRepeatDelay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float delay = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAutoRepeatDelay'",NULL);
#endif
 {
  self->setAutoRepeatDelay(delay);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAutoRepeatDelay'.",&tolua_err);
 return 0;
#endif
}

/* method: setAutoRepeatRate of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setAutoRepeatRate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  float rate = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAutoRepeatRate'",NULL);
#endif
 {
  self->setAutoRepeatRate(rate);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAutoRepeatRate'.",&tolua_err);
 return 0;
#endif
}

/* method: destroy of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
 {
  self->destroy();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}

/* method: setProperty of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setProperty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  string value = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setProperty'",NULL);
#endif
 {
  self->setProperty(name,value);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setProperty'.",&tolua_err);
 return 0;
#endif
}

/* method: getProperty of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getProperty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getProperty'",NULL);
#endif
 {
  string tolua_ret = (string)  self->getProperty(name);
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getProperty'.",&tolua_err);
 return 0;
#endif
}

/* method: getPropertyDefault of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getPropertyDefault00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPropertyDefault'",NULL);
#endif
 {
  string tolua_ret = (string)  self->getPropertyDefault(name);
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPropertyDefault'.",&tolua_err);
 return 0;
#endif
}

/* method: getPropertyHelp of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_getPropertyHelp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPropertyHelp'",NULL);
#endif
 {
  string tolua_ret = (string)  self->getPropertyHelp(name);
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPropertyHelp'.",&tolua_err);
 return 0;
#endif
}

/* method: isPropertyPresent of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isPropertyPresent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPropertyPresent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isPropertyPresent(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPropertyPresent'.",&tolua_err);
 return 0;
#endif
}

/* method: isPropertyDefault of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isPropertyDefault00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPropertyDefault'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isPropertyDefault(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPropertyDefault'.",&tolua_err);
 return 0;
#endif
}

/* method: addEvent of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_addEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEvent'",NULL);
#endif
 {
  self->addEvent(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: removeEvent of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_removeEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeEvent'",NULL);
#endif
 {
  self->removeEvent(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: removeAllEvents of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_removeAllEvents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAllEvents'",NULL);
#endif
 {
  self->removeAllEvents();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAllEvents'.",&tolua_err);
 return 0;
#endif
}

/* method: isEventPresent of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isEventPresent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isEventPresent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isEventPresent(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isEventPresent'.",&tolua_err);
 return 0;
#endif
}

/* method: subscribeScriptedEvent of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_subscribeEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  string callback_name = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'subscribeScriptedEvent'",NULL);
#endif
 {
  self->subscribeScriptedEvent(name,callback_name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'subscribeEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: fireEvent of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_fireEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"CEGUI::EventArgs",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,4,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  CEGUI::EventArgs* args = ((CEGUI::EventArgs*)  tolua_tousertype(tolua_S,3,0));
  string eventnamespace = ((string)  tolua_tocppstring(tolua_S,4,""));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fireEvent'",NULL);
#endif
 {
  self->fireEvent(name,*args,eventnamespace);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fireEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: isMuted of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_isMuted00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Window* self = (const CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isMuted'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isMuted();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isMuted'.",&tolua_err);
 return 0;
#endif
}

/* method: setMutedState of class  CEGUI::Window */
static int tolua_CEGUI_CEGUI_Window_setMutedState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* self = (CEGUI::Window*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMutedState'",NULL);
#endif
 {
  self->setMutedState(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMutedState'.",&tolua_err);
 return 0;
#endif
}

/* method: getCurrentZ of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_getCurrentZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Renderer* self = (const CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurrentZ'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getCurrentZ();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurrentZ'.",&tolua_err);
 return 0;
#endif
}

/* method: getZLayer of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_getZLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Renderer* self = (const CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
  unsigned int layer = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getZLayer'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getZLayer(layer);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getZLayer'.",&tolua_err);
 return 0;
#endif
}

/* method: isQueueingEnabled of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_isQueueingEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Renderer* self = (const CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isQueueingEnabled'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isQueueingEnabled();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isQueueingEnabled'.",&tolua_err);
 return 0;
#endif
}

/* method: createTexture of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_createTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Renderer* self = (CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createTexture'",NULL);
#endif
 {
  CEGUI::Texture* tolua_ret = (CEGUI::Texture*)  self->createTexture();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Texture");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createTexture'.",&tolua_err);
 return 0;
#endif
}

/* method: createTexture of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_createTexture01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Renderer* self = (CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
  float size = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createTexture'",NULL);
#endif
 {
  CEGUI::Texture* tolua_ret = (CEGUI::Texture*)  self->createTexture(size);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Texture");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Renderer_createTexture00(tolua_S);
}

/* method: createTexture of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_createTexture02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Renderer",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::Renderer* self = (CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
  string filename = ((string)  tolua_tocppstring(tolua_S,2,0));
  string resourcegroup = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createTexture'",NULL);
#endif
 {
  CEGUI::Texture* tolua_ret = (CEGUI::Texture*)  self->createTexture(filename,resourcegroup);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Texture");
 }
 }
 return 1;
tolua_lerror:
 return tolua_CEGUI_CEGUI_Renderer_createTexture01(tolua_S);
}

/* method: destroyTexture of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_destroyTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Texture",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Renderer* self = (CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Texture* tex = ((CEGUI::Texture*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyTexture'",NULL);
#endif
 {
  self->destroyTexture(tex);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroyTexture'.",&tolua_err);
 return 0;
#endif
}

/* method: destroyAllTextures of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_destroyAllTextures00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Renderer* self = (CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyAllTextures'",NULL);
#endif
 {
  self->destroyAllTextures();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroyAllTextures'.",&tolua_err);
 return 0;
#endif
}

/* method: getWidth of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_getWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Renderer* self = (const CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWidth'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getWidth();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWidth'.",&tolua_err);
 return 0;
#endif
}

/* method: getHeight of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_getHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Renderer* self = (const CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHeight'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getHeight();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHeight'.",&tolua_err);
 return 0;
#endif
}

/* method: getSize of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_getSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Renderer* self = (const CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'",NULL);
#endif
 {
  CEGUI::Size tolua_ret = (CEGUI::Size)  self->getSize();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Size(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Size));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Size");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSize'.",&tolua_err);
 return 0;
#endif
}

/* method: getRect of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_getRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Renderer* self = (const CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRect'",NULL);
#endif
 {
  CEGUI::Rect tolua_ret = (CEGUI::Rect)  self->getRect();
 {
#ifdef __cplusplus
 void* tolua_obj = new CEGUI::Rect(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CEGUI::Rect));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"CEGUI::Rect");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRect'.",&tolua_err);
 return 0;
#endif
}

/* method: getMaxTextureSize of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_getMaxTextureSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Renderer* self = (const CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMaxTextureSize'",NULL);
#endif
 {
  unsigned int tolua_ret = (unsigned int)  self->getMaxTextureSize();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMaxTextureSize'.",&tolua_err);
 return 0;
#endif
}

/* method: getHorzScreenDPI of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_getHorzScreenDPI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Renderer* self = (const CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHorzScreenDPI'",NULL);
#endif
 {
  unsigned int tolua_ret = (unsigned int)  self->getHorzScreenDPI();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHorzScreenDPI'.",&tolua_err);
 return 0;
#endif
}

/* method: getVertScreenDPI of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_getVertScreenDPI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Renderer* self = (const CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVertScreenDPI'",NULL);
#endif
 {
  unsigned int tolua_ret = (unsigned int)  self->getVertScreenDPI();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVertScreenDPI'.",&tolua_err);
 return 0;
#endif
}

/* method: addEvent of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_addEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Renderer",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Renderer* self = (CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEvent'",NULL);
#endif
 {
  self->addEvent(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: removeEvent of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_removeEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Renderer",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Renderer* self = (CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeEvent'",NULL);
#endif
 {
  self->removeEvent(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: removeAllEvents of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_removeAllEvents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Renderer* self = (CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAllEvents'",NULL);
#endif
 {
  self->removeAllEvents();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAllEvents'.",&tolua_err);
 return 0;
#endif
}

/* method: isEventPresent of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_isEventPresent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Renderer",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Renderer* self = (CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isEventPresent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isEventPresent(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isEventPresent'.",&tolua_err);
 return 0;
#endif
}

/* method: subscribeScriptedEvent of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_subscribeEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Renderer",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Renderer* self = (CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  string callback_name = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'subscribeScriptedEvent'",NULL);
#endif
 {
  self->subscribeScriptedEvent(name,callback_name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'subscribeEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: fireEvent of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_fireEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Renderer",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"CEGUI::EventArgs",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,4,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Renderer* self = (CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  CEGUI::EventArgs* args = ((CEGUI::EventArgs*)  tolua_tousertype(tolua_S,3,0));
  string eventnamespace = ((string)  tolua_tocppstring(tolua_S,4,""));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fireEvent'",NULL);
#endif
 {
  self->fireEvent(name,*args,eventnamespace);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fireEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: isMuted of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_isMuted00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Renderer* self = (const CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isMuted'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isMuted();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isMuted'.",&tolua_err);
 return 0;
#endif
}

/* method: setMutedState of class  CEGUI::Renderer */
static int tolua_CEGUI_CEGUI_Renderer_setMutedState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Renderer",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Renderer* self = (CEGUI::Renderer*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMutedState'",NULL);
#endif
 {
  self->setMutedState(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMutedState'.",&tolua_err);
 return 0;
#endif
}

/* method: getSingleton of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::System& tolua_ret = (CEGUI::System&)  CEGUI::System::getSingleton();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CEGUI::System");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}

/* method: getRenderer of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_getRenderer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRenderer'",NULL);
#endif
 {
  CEGUI::Renderer* tolua_ret = (CEGUI::Renderer*)  self->getRenderer();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Renderer");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRenderer'.",&tolua_err);
 return 0;
#endif
}

/* method: setDefaultFont of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_setDefaultFont00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDefaultFont'",NULL);
#endif
 {
  self->setDefaultFont(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDefaultFont'.",&tolua_err);
 return 0;
#endif
}

/* method: setDefaultFont of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_setDefaultFont01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Font",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Font* font = ((CEGUI::Font*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDefaultFont'",NULL);
#endif
 {
  self->setDefaultFont(font);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_System_setDefaultFont00(tolua_S);
}

/* method: signalRedraw of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_signalRedraw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'signalRedraw'",NULL);
#endif
 {
  self->signalRedraw();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'signalRedraw'.",&tolua_err);
 return 0;
#endif
}

/* method: isRedrawRequested of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_isRedrawRequested00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isRedrawRequested'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isRedrawRequested();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isRedrawRequested'.",&tolua_err);
 return 0;
#endif
}

/* method: setGUISheet of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_setGUISheet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* sheet = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGUISheet'",NULL);
#endif
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->setGUISheet(sheet);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setGUISheet'.",&tolua_err);
 return 0;
#endif
}

/* method: getGUISheet of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_getGUISheet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGUISheet'",NULL);
#endif
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getGUISheet();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGUISheet'.",&tolua_err);
 return 0;
#endif
}

/* method: setSingleClickTimeout of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_setSingleClickTimeout00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  double timeout = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSingleClickTimeout'",NULL);
#endif
 {
  self->setSingleClickTimeout(timeout);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSingleClickTimeout'.",&tolua_err);
 return 0;
#endif
}

/* method: setMultiClickTimeout of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_setMultiClickTimeout00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  double timeout = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMultiClickTimeout'",NULL);
#endif
 {
  self->setMultiClickTimeout(timeout);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMultiClickTimeout'.",&tolua_err);
 return 0;
#endif
}

/* method: setMultiClickToleranceAreaSize of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_setMultiClickToleranceAreaSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Size",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Size* sz = ((const CEGUI::Size*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMultiClickToleranceAreaSize'",NULL);
#endif
 {
  self->setMultiClickToleranceAreaSize(*sz);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMultiClickToleranceAreaSize'.",&tolua_err);
 return 0;
#endif
}

/* method: getSingleClickTimeout of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_getSingleClickTimeout00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSingleClickTimeout'",NULL);
#endif
 {
  double tolua_ret = (double)  self->getSingleClickTimeout();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleClickTimeout'.",&tolua_err);
 return 0;
#endif
}

/* method: getMultiClickTimeout of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_getMultiClickTimeout00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMultiClickTimeout'",NULL);
#endif
 {
  double tolua_ret = (double)  self->getMultiClickTimeout();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMultiClickTimeout'.",&tolua_err);
 return 0;
#endif
}

/* method: getMultiClickToleranceAreaSize of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_getMultiClickToleranceAreaSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMultiClickToleranceAreaSize'",NULL);
#endif
 {
  const CEGUI::Size& tolua_ret = (const CEGUI::Size&)  self->getMultiClickToleranceAreaSize();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const CEGUI::Size");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMultiClickToleranceAreaSize'.",&tolua_err);
 return 0;
#endif
}

/* method: setDefaultMouseCursor of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_setDefaultMouseCursor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MouseCursorImage image = ((CEGUI::MouseCursorImage) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDefaultMouseCursor'",NULL);
#endif
 {
  self->setDefaultMouseCursor(image);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDefaultMouseCursor'.",&tolua_err);
 return 0;
#endif
}

/* method: setDefaultMouseCursor of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_setDefaultMouseCursor01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Image",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Image* image = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDefaultMouseCursor'",NULL);
#endif
 {
  self->setDefaultMouseCursor(image);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_System_setDefaultMouseCursor00(tolua_S);
}

/* method: setDefaultMouseCursor of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_setDefaultMouseCursor02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  string imageset_name = ((string)  tolua_tocppstring(tolua_S,2,0));
  string image_name = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDefaultMouseCursor'",NULL);
#endif
 {
  self->setDefaultMouseCursor(imageset_name,image_name);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_System_setDefaultMouseCursor01(tolua_S);
}

/* method: getDefaultMouseCursor of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_getDefaultMouseCursor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDefaultMouseCursor'",NULL);
#endif
 {
  const CEGUI::Image* tolua_ret = (const CEGUI::Image*)  self->getDefaultMouseCursor();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"const CEGUI::Image");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDefaultMouseCursor'.",&tolua_err);
 return 0;
#endif
}

/* method: getWindowContainingMouse of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_getWindowContainingMouse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWindowContainingMouse'",NULL);
#endif
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getWindowContainingMouse();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWindowContainingMouse'.",&tolua_err);
 return 0;
#endif
}

/* method: executeScriptFile of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_executeScriptFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  string filename = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'executeScriptFile'",NULL);
#endif
 {
  self->executeScriptFile(filename);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'executeScriptFile'.",&tolua_err);
 return 0;
#endif
}

/* method: executeScriptGlobal of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_executeScriptGlobal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  string global_name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'executeScriptGlobal'",NULL);
#endif
 {
  int tolua_ret = (int)  self->executeScriptGlobal(global_name);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'executeScriptGlobal'.",&tolua_err);
 return 0;
#endif
}

/* method: executeScriptString of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_executeScriptString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  string lua_string = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'executeScriptString'",NULL);
#endif
 {
  self->executeScriptString(lua_string);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'executeScriptString'.",&tolua_err);
 return 0;
#endif
}

/* method: getMouseMoveScaling of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_getMouseMoveScaling00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMouseMoveScaling'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getMouseMoveScaling();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMouseMoveScaling'.",&tolua_err);
 return 0;
#endif
}

/* method: setMouseMoveScaling of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_setMouseMoveScaling00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  float scaling = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMouseMoveScaling'",NULL);
#endif
 {
  self->setMouseMoveScaling(scaling);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMouseMoveScaling'.",&tolua_err);
 return 0;
#endif
}

/* method: notifyWindowDestroyed of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_notifyWindowDestroyed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Window* window = ((const CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'notifyWindowDestroyed'",NULL);
#endif
 {
  self->notifyWindowDestroyed(window);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'notifyWindowDestroyed'.",&tolua_err);
 return 0;
#endif
}

/* method: injectMouseMove of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_injectMouseMove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  float dx = ((float)  tolua_tonumber(tolua_S,2,0));
  float dy = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectMouseMove'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->injectMouseMove(dx,dy);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectMouseMove'.",&tolua_err);
 return 0;
#endif
}

/* method: injectMouseLeaves of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_injectMouseLeaves00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectMouseLeaves'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->injectMouseLeaves();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectMouseLeaves'.",&tolua_err);
 return 0;
#endif
}

/* method: injectMouseButtonDown of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_injectMouseButtonDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MouseButton btn = ((CEGUI::MouseButton) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectMouseButtonDown'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->injectMouseButtonDown(btn);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectMouseButtonDown'.",&tolua_err);
 return 0;
#endif
}

/* method: injectMouseButtonUp of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_injectMouseButtonUp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::MouseButton btn = ((CEGUI::MouseButton) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectMouseButtonUp'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->injectMouseButtonUp(btn);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectMouseButtonUp'.",&tolua_err);
 return 0;
#endif
}

/* method: injectKeyDown of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_injectKeyDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  unsigned int keycode = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectKeyDown'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->injectKeyDown(keycode);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectKeyDown'.",&tolua_err);
 return 0;
#endif
}

/* method: injectKeyUp of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_injectKeyUp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  unsigned int keycode = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectKeyUp'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->injectKeyUp(keycode);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectKeyUp'.",&tolua_err);
 return 0;
#endif
}

/* method: injectChar of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_injectChar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  unsigned long code_point = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectChar'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->injectChar(code_point);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectChar'.",&tolua_err);
 return 0;
#endif
}

/* method: injectMouseWheelChange of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_injectMouseWheelChange00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  float delta = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectMouseWheelChange'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->injectMouseWheelChange(delta);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectMouseWheelChange'.",&tolua_err);
 return 0;
#endif
}

/* method: injectMousePosition of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_injectMousePosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectMousePosition'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->injectMousePosition(x,y);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectMousePosition'.",&tolua_err);
 return 0;
#endif
}

/* method: injectTimePulse of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_injectTimePulse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  float timeElapsed = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'injectTimePulse'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->injectTimePulse(timeElapsed);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'injectTimePulse'.",&tolua_err);
 return 0;
#endif
}

/* method: addEvent of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_addEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEvent'",NULL);
#endif
 {
  self->addEvent(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: removeEvent of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_removeEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeEvent'",NULL);
#endif
 {
  self->removeEvent(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: removeAllEvents of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_removeAllEvents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAllEvents'",NULL);
#endif
 {
  self->removeAllEvents();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAllEvents'.",&tolua_err);
 return 0;
#endif
}

/* method: isEventPresent of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_isEventPresent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isEventPresent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isEventPresent(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isEventPresent'.",&tolua_err);
 return 0;
#endif
}

/* method: subscribeScriptedEvent of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_subscribeEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  string callback_name = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'subscribeScriptedEvent'",NULL);
#endif
 {
  self->subscribeScriptedEvent(name,callback_name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'subscribeEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: fireEvent of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_fireEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"CEGUI::EventArgs",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,4,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  CEGUI::EventArgs* args = ((CEGUI::EventArgs*)  tolua_tousertype(tolua_S,3,0));
  string eventnamespace = ((string)  tolua_tocppstring(tolua_S,4,""));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fireEvent'",NULL);
#endif
 {
  self->fireEvent(name,*args,eventnamespace);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fireEvent'.",&tolua_err);
 return 0;
#endif
}

/* method: isMuted of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_isMuted00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::System",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::System* self = (const CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isMuted'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isMuted();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isMuted'.",&tolua_err);
 return 0;
#endif
}

/* method: setMutedState of class  CEGUI::System */
static int tolua_CEGUI_CEGUI_System_setMutedState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::System",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::System* self = (CEGUI::System*)  tolua_tousertype(tolua_S,1,0);
  bool setting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMutedState'",NULL);
#endif
 {
  self->setMutedState(setting);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMutedState'.",&tolua_err);
 return 0;
#endif
}

/* method: getSingleton of class  CEGUI::WindowManager */
static int tolua_CEGUI_CEGUI_WindowManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::WindowManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::WindowManager& tolua_ret = (CEGUI::WindowManager&)  CEGUI::WindowManager::getSingleton();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CEGUI::WindowManager");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}

/* method: createWindow of class  CEGUI::WindowManager */
static int tolua_CEGUI_CEGUI_WindowManager_createWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::WindowManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::WindowManager* self = (CEGUI::WindowManager*)  tolua_tousertype(tolua_S,1,0);
  string type = ((string)  tolua_tocppstring(tolua_S,2,0));
  string name = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createWindow'",NULL);
#endif
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->createWindow(type,name);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createWindow'.",&tolua_err);
 return 0;
#endif
}

/* method: loadWindowLayout of class  CEGUI::WindowManager */
static int tolua_CEGUI_CEGUI_WindowManager_loadWindowLayout00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::WindowManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,1,&tolua_err) ||
 !tolua_iscppstring(tolua_S,4,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::WindowManager* self = (CEGUI::WindowManager*)  tolua_tousertype(tolua_S,1,0);
  string filename = ((string)  tolua_tocppstring(tolua_S,2,0));
  string name_prefix = ((string)  tolua_tocppstring(tolua_S,3,""));
  string resourcegroup = ((string)  tolua_tocppstring(tolua_S,4,""));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadWindowLayout'",NULL);
#endif
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->loadWindowLayout(filename,name_prefix,resourcegroup);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadWindowLayout'.",&tolua_err);
 return 0;
#endif
}

/* method: destroyWindow of class  CEGUI::WindowManager */
static int tolua_CEGUI_CEGUI_WindowManager_destroyWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::WindowManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::WindowManager* self = (CEGUI::WindowManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyWindow'",NULL);
#endif
 {
  self->destroyWindow(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroyWindow'.",&tolua_err);
 return 0;
#endif
}

/* method: destroyWindow of class  CEGUI::WindowManager */
static int tolua_CEGUI_CEGUI_WindowManager_destroyWindow01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::WindowManager",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  CEGUI::WindowManager* self = (CEGUI::WindowManager*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* window = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyWindow'",NULL);
#endif
 {
  self->destroyWindow(window);
 }
 }
 return 0;
tolua_lerror:
 return tolua_CEGUI_CEGUI_WindowManager_destroyWindow00(tolua_S);
}

/* method: destroyAllWindows of class  CEGUI::WindowManager */
static int tolua_CEGUI_CEGUI_WindowManager_destroyAllWindows00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::WindowManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::WindowManager* self = (CEGUI::WindowManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyAllWindows'",NULL);
#endif
 {
  self->destroyAllWindows();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroyAllWindows'.",&tolua_err);
 return 0;
#endif
}

/* method: getWindow of class  CEGUI::WindowManager */
static int tolua_CEGUI_CEGUI_WindowManager_getWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::WindowManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::WindowManager* self = (const CEGUI::WindowManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWindow'",NULL);
#endif
 {
  CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getWindow(name);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWindow'.",&tolua_err);
 return 0;
#endif
}

/* method: isWindowPresent of class  CEGUI::WindowManager */
static int tolua_CEGUI_CEGUI_WindowManager_isWindowPresent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::WindowManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::WindowManager* self = (const CEGUI::WindowManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isWindowPresent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isWindowPresent(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isWindowPresent'.",&tolua_err);
 return 0;
#endif
}

/* method: isDeadPoolEmpty of class  CEGUI::WindowManager */
static int tolua_CEGUI_CEGUI_WindowManager_isDeadPoolEmpty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::WindowManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::WindowManager* self = (const CEGUI::WindowManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isDeadPoolEmpty'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isDeadPoolEmpty();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isDeadPoolEmpty'.",&tolua_err);
 return 0;
#endif
}

/* method: cleanDeadPool of class  CEGUI::WindowManager */
static int tolua_CEGUI_CEGUI_WindowManager_cleanDeadPool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::WindowManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::WindowManager* self = (CEGUI::WindowManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cleanDeadPool'",NULL);
#endif
 {
  self->cleanDeadPool();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cleanDeadPool'.",&tolua_err);
 return 0;
#endif
}

/* method: loadResources of class  CEGUI::Scheme */
static int tolua_CEGUI_CEGUI_Scheme_loadResources00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Scheme",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Scheme* self = (CEGUI::Scheme*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadResources'",NULL);
#endif
 {
  self->loadResources();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadResources'.",&tolua_err);
 return 0;
#endif
}

/* method: unloadResources of class  CEGUI::Scheme */
static int tolua_CEGUI_CEGUI_Scheme_unloadResources00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::Scheme",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::Scheme* self = (CEGUI::Scheme*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unloadResources'",NULL);
#endif
 {
  self->unloadResources();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unloadResources'.",&tolua_err);
 return 0;
#endif
}

/* method: resourcesLoaded of class  CEGUI::Scheme */
static int tolua_CEGUI_CEGUI_Scheme_resourcesLoaded00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Scheme",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Scheme* self = (const CEGUI::Scheme*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resourcesLoaded'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->resourcesLoaded();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resourcesLoaded'.",&tolua_err);
 return 0;
#endif
}

/* method: getName of class  CEGUI::Scheme */
static int tolua_CEGUI_CEGUI_Scheme_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::Scheme",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::Scheme* self = (const CEGUI::Scheme*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
 {
  string tolua_ret = (string)  self->getName();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}

/* method: getSingleton of class  CEGUI::SchemeManager */
static int tolua_CEGUI_CEGUI_SchemeManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CEGUI::SchemeManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  CEGUI::SchemeManager& tolua_ret = (CEGUI::SchemeManager&)  CEGUI::SchemeManager::getSingleton();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CEGUI::SchemeManager");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}

/* method: loadScheme of class  CEGUI::SchemeManager */
static int tolua_CEGUI_CEGUI_SchemeManager_loadScheme00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::SchemeManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::SchemeManager* self = (CEGUI::SchemeManager*)  tolua_tousertype(tolua_S,1,0);
  string filename = ((string)  tolua_tocppstring(tolua_S,2,0));
  string resourcegroup = ((string)  tolua_tocppstring(tolua_S,3,""));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadScheme'",NULL);
#endif
 {
  CEGUI::Scheme* tolua_ret = (CEGUI::Scheme*)  self->loadScheme(filename,resourcegroup);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Scheme");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadScheme'.",&tolua_err);
 return 0;
#endif
}

/* method: unloadScheme of class  CEGUI::SchemeManager */
static int tolua_CEGUI_CEGUI_SchemeManager_unloadScheme00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::SchemeManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::SchemeManager* self = (CEGUI::SchemeManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unloadScheme'",NULL);
#endif
 {
  self->unloadScheme(name);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unloadScheme'.",&tolua_err);
 return 0;
#endif
}

/* method: isSchemePresent of class  CEGUI::SchemeManager */
static int tolua_CEGUI_CEGUI_SchemeManager_isSchemePresent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::SchemeManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::SchemeManager* self = (const CEGUI::SchemeManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isSchemePresent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isSchemePresent(name);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isSchemePresent'.",&tolua_err);
 return 0;
#endif
}

/* method: getScheme of class  CEGUI::SchemeManager */
static int tolua_CEGUI_CEGUI_SchemeManager_getScheme00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const CEGUI::SchemeManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const CEGUI::SchemeManager* self = (const CEGUI::SchemeManager*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScheme'",NULL);
#endif
 {
  CEGUI::Scheme* tolua_ret = (CEGUI::Scheme*)  self->getScheme(name);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Scheme");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getScheme'.",&tolua_err);
 return 0;
#endif
}

/* method: unloadAllSchemes of class  CEGUI::SchemeManager */
static int tolua_CEGUI_CEGUI_SchemeManager_unloadAllSchemes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CEGUI::SchemeManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::SchemeManager* self = (CEGUI::SchemeManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unloadAllSchemes'",NULL);
#endif
 {
  self->unloadAllSchemes();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unloadAllSchemes'.",&tolua_err);
 return 0;
#endif
}

/* Open function */
TOLUA_API int tolua_CEGUI_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_module(tolua_S,"CEGUI",0);
 tolua_beginmodule(tolua_S,"CEGUI");
#ifdef __cplusplus
 tolua_cclass(tolua_S,"Vector2","CEGUI::Vector2","",tolua_collect_CEGUI__Vector2);
#else
 tolua_cclass(tolua_S,"Vector2","CEGUI::Vector2","",NULL);
#endif
 tolua_beginmodule(tolua_S,"Vector2");
 tolua_variable(tolua_S,"x",tolua_get_CEGUI__Vector2_x,tolua_set_CEGUI__Vector2_x);
 tolua_variable(tolua_S,"y",tolua_get_CEGUI__Vector2_y,tolua_set_CEGUI__Vector2_y);
 tolua_function(tolua_S,".add",tolua_CEGUI_CEGUI_Vector2__add00);
 tolua_function(tolua_S,".sub",tolua_CEGUI_CEGUI_Vector2__sub00);
 tolua_function(tolua_S,".mul",tolua_CEGUI_CEGUI_Vector2__mul00);
 tolua_function(tolua_S,".eq",tolua_CEGUI_CEGUI_Vector2__eq00);
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_Vector2_new00);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_Vector2_new00_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_Vector2_new00_local, "CEGUI::Vector2");
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_Vector2_new01);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_Vector2_new01_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_Vector2_new01_local, "CEGUI::Vector2");
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"Point","CEGUI::Point","",tolua_collect_CEGUI__Point);
#else
 tolua_cclass(tolua_S,"Point","CEGUI::Point","",NULL);
#endif
 tolua_beginmodule(tolua_S,"Point");
 tolua_variable(tolua_S,"x",tolua_get_CEGUI__Point_x,tolua_set_CEGUI__Point_x);
 tolua_variable(tolua_S,"y",tolua_get_CEGUI__Point_y,tolua_set_CEGUI__Point_y);
 tolua_function(tolua_S,".add",tolua_CEGUI_CEGUI_Point__add00);
 tolua_function(tolua_S,".sub",tolua_CEGUI_CEGUI_Point__sub00);
 tolua_function(tolua_S,".mul",tolua_CEGUI_CEGUI_Point__mul00);
 tolua_function(tolua_S,".eq",tolua_CEGUI_CEGUI_Point__eq00);
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_Point_new00);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_Point_new00_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_Point_new00_local, "CEGUI::Point");
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_Point_new01);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_Point_new01_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_Point_new01_local, "CEGUI::Point");
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"Size","CEGUI::Size","",tolua_collect_CEGUI__Size);
#else
 tolua_cclass(tolua_S,"Size","CEGUI::Size","",NULL);
#endif
 tolua_beginmodule(tolua_S,"Size");
 tolua_variable(tolua_S,"width",tolua_get_CEGUI__Size_width,tolua_set_CEGUI__Size_width);
 tolua_variable(tolua_S,"height",tolua_get_CEGUI__Size_height,tolua_set_CEGUI__Size_height);
 tolua_function(tolua_S,".eq",tolua_CEGUI_CEGUI_Size__eq00);
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_Size_new00);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_Size_new00_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_Size_new00_local, "CEGUI::Size");
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_Size_new01);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_Size_new01_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_Size_new01_local, "CEGUI::Size");
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"Rect","CEGUI::Rect","",tolua_collect_CEGUI__Rect);
#else
 tolua_cclass(tolua_S,"Rect","CEGUI::Rect","",NULL);
#endif
 tolua_beginmodule(tolua_S,"Rect");
 tolua_variable(tolua_S,"top",tolua_get_CEGUI__Rect_top,tolua_set_CEGUI__Rect_top);
 tolua_variable(tolua_S,"bottom",tolua_get_CEGUI__Rect_bottom,tolua_set_CEGUI__Rect_bottom);
 tolua_variable(tolua_S,"left",tolua_get_CEGUI__Rect_left,tolua_set_CEGUI__Rect_left);
 tolua_variable(tolua_S,"right",tolua_get_CEGUI__Rect_right,tolua_set_CEGUI__Rect_right);
 tolua_function(tolua_S,"getPosition",tolua_CEGUI_CEGUI_Rect_getPosition00);
 tolua_function(tolua_S,"getWidth",tolua_CEGUI_CEGUI_Rect_getWidth00);
 tolua_function(tolua_S,"getHeight",tolua_CEGUI_CEGUI_Rect_getHeight00);
 tolua_function(tolua_S,"getSize",tolua_CEGUI_CEGUI_Rect_getSize00);
 tolua_function(tolua_S,"getIntersection",tolua_CEGUI_CEGUI_Rect_getIntersection00);
 tolua_function(tolua_S,"isPointInRect",tolua_CEGUI_CEGUI_Rect_isPointInRect00);
 tolua_function(tolua_S,"setPosition",tolua_CEGUI_CEGUI_Rect_setPosition00);
 tolua_function(tolua_S,"setWidth",tolua_CEGUI_CEGUI_Rect_setWidth00);
 tolua_function(tolua_S,"setHeight",tolua_CEGUI_CEGUI_Rect_setHeight00);
 tolua_function(tolua_S,"setSize",tolua_CEGUI_CEGUI_Rect_setSize00);
 tolua_function(tolua_S,"offset",tolua_CEGUI_CEGUI_Rect_offset00);
 tolua_function(tolua_S,"constrainSizeMax",tolua_CEGUI_CEGUI_Rect_constrainSizeMax00);
 tolua_function(tolua_S,"constrainSizeMin",tolua_CEGUI_CEGUI_Rect_constrainSizeMin00);
 tolua_function(tolua_S,"constrainSize",tolua_CEGUI_CEGUI_Rect_constrainSize00);
 tolua_function(tolua_S,".eq",tolua_CEGUI_CEGUI_Rect__eq00);
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_Rect_new00);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_Rect_new00_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_Rect_new00_local, "CEGUI::Rect");
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_Rect_new01);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_Rect_new01_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_Rect_new01_local, "CEGUI::Rect");
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"colour","CEGUI::colour","",tolua_collect_CEGUI__colour);
#else
 tolua_cclass(tolua_S,"colour","CEGUI::colour","",NULL);
#endif
 tolua_beginmodule(tolua_S,"colour");
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_colour_new00);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_colour_new00_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_colour_new00_local, "CEGUI::colour");
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_colour_new01);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_colour_new01_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_colour_new01_local, "CEGUI::colour");
 tolua_function(tolua_S,"getAlpha",tolua_CEGUI_CEGUI_colour_getAlpha00);
 tolua_function(tolua_S,"getRed",tolua_CEGUI_CEGUI_colour_getRed00);
 tolua_function(tolua_S,"getGreen",tolua_CEGUI_CEGUI_colour_getGreen00);
 tolua_function(tolua_S,"getBlue",tolua_CEGUI_CEGUI_colour_getBlue00);
 tolua_function(tolua_S,"getHue",tolua_CEGUI_CEGUI_colour_getHue00);
 tolua_function(tolua_S,"getSaturation",tolua_CEGUI_CEGUI_colour_getSaturation00);
 tolua_function(tolua_S,"getLumination",tolua_CEGUI_CEGUI_colour_getLumination00);
 tolua_function(tolua_S,"set",tolua_CEGUI_CEGUI_colour_set00);
 tolua_function(tolua_S,"setAlpha",tolua_CEGUI_CEGUI_colour_setAlpha00);
 tolua_function(tolua_S,"setRed",tolua_CEGUI_CEGUI_colour_setRed00);
 tolua_function(tolua_S,"setGreen",tolua_CEGUI_CEGUI_colour_setGreen00);
 tolua_function(tolua_S,"setBlue",tolua_CEGUI_CEGUI_colour_setBlue00);
 tolua_function(tolua_S,"setRGB",tolua_CEGUI_CEGUI_colour_setRGB00);
 tolua_function(tolua_S,"setHSL",tolua_CEGUI_CEGUI_colour_setHSL00);
 tolua_function(tolua_S,".add",tolua_CEGUI_CEGUI_colour__add00);
 tolua_function(tolua_S,".sub",tolua_CEGUI_CEGUI_colour__sub00);
 tolua_function(tolua_S,".eq",tolua_CEGUI_CEGUI_colour__eq00);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"Errors",CEGUI::Errors);
 tolua_constant(tolua_S,"Standard",CEGUI::Standard);
 tolua_constant(tolua_S,"Informative",CEGUI::Informative);
 tolua_constant(tolua_S,"Insane",CEGUI::Insane);
 tolua_cclass(tolua_S,"Logger","CEGUI::Logger","",NULL);
 tolua_beginmodule(tolua_S,"Logger");
 tolua_function(tolua_S,"getSingleton",tolua_CEGUI_CEGUI_Logger_getSingleton00);
 tolua_function(tolua_S,"setLoggingLevel",tolua_CEGUI_CEGUI_Logger_setLoggingLevel00);
 tolua_function(tolua_S,"getLoggingLevel",tolua_CEGUI_CEGUI_Logger_getLoggingLevel00);
 tolua_function(tolua_S,"logEvent",tolua_CEGUI_CEGUI_Logger_logEvent00);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"LeftAligned",CEGUI::LeftAligned);
 tolua_constant(tolua_S,"RightAligned",CEGUI::RightAligned);
 tolua_constant(tolua_S,"Centred",CEGUI::Centred);
 tolua_constant(tolua_S,"Justified",CEGUI::Justified);
 tolua_constant(tolua_S,"WordWrapLeftAligned",CEGUI::WordWrapLeftAligned);
 tolua_constant(tolua_S,"WordWrapRightAligned",CEGUI::WordWrapRightAligned);
 tolua_constant(tolua_S,"WordWrapCentred",CEGUI::WordWrapCentred);
 tolua_constant(tolua_S,"WordWrapJustified",CEGUI::WordWrapJustified);
 tolua_cclass(tolua_S,"Font","CEGUI::Font","",NULL);
 tolua_beginmodule(tolua_S,"Font");
 tolua_function(tolua_S,"getName",tolua_CEGUI_CEGUI_Font_getName00);
 tolua_function(tolua_S,"defineFontGlyphs",tolua_CEGUI_CEGUI_Font_defineFontGlyphs00);
 tolua_function(tolua_S,"defineFontGlyphs",tolua_CEGUI_CEGUI_Font_defineFontGlyphs01);
 tolua_function(tolua_S,"isCodepointAvailable",tolua_CEGUI_CEGUI_Font_isCodepointAvailable00);
 tolua_function(tolua_S,"getAvailableGlyphs",tolua_CEGUI_CEGUI_Font_getAvailableGlyphs00);
 tolua_function(tolua_S,"getPointSize",tolua_CEGUI_CEGUI_Font_getPointSize00);
 tolua_function(tolua_S,"setNativeResolution",tolua_CEGUI_CEGUI_Font_setNativeResolution00);
 tolua_function(tolua_S,"getNativeResolution",tolua_CEGUI_CEGUI_Font_getNativeResolution00);
 tolua_function(tolua_S,"notifyScreenResolution",tolua_CEGUI_CEGUI_Font_notifyScreenResolution00);
 tolua_function(tolua_S,"setAutoScalingEnabled",tolua_CEGUI_CEGUI_Font_setAutoScalingEnabled00);
 tolua_function(tolua_S,"isAutoScaled",tolua_CEGUI_CEGUI_Font_isAutoScaled00);
 tolua_function(tolua_S,"setAntiAliased",tolua_CEGUI_CEGUI_Font_setAntiAliased00);
 tolua_function(tolua_S,"isAntiAliased",tolua_CEGUI_CEGUI_Font_isAntiAliased00);
 tolua_function(tolua_S,"getTextExtent",tolua_CEGUI_CEGUI_Font_getTextExtent00);
 tolua_function(tolua_S,"getLineSpacing",tolua_CEGUI_CEGUI_Font_getLineSpacing00);
 tolua_function(tolua_S,"getFontHeight",tolua_CEGUI_CEGUI_Font_getFontHeight00);
 tolua_function(tolua_S,"getCharAtPixel",tolua_CEGUI_CEGUI_Font_getCharAtPixel00);
 tolua_function(tolua_S,"getCharAtPixel",tolua_CEGUI_CEGUI_Font_getCharAtPixel01);
 tolua_function(tolua_S,"getFormattedLineCount",tolua_CEGUI_CEGUI_Font_getFormattedLineCount00);
 tolua_function(tolua_S,"getFormattedTextExtent",tolua_CEGUI_CEGUI_Font_getFormattedTextExtent00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"FontManager","CEGUI::FontManager","",NULL);
 tolua_beginmodule(tolua_S,"FontManager");
 tolua_function(tolua_S,"getSingleton",tolua_CEGUI_CEGUI_FontManager_getSingleton00);
 tolua_function(tolua_S,"createFont",tolua_CEGUI_CEGUI_FontManager_createFont00);
 tolua_function(tolua_S,"createFont",tolua_CEGUI_CEGUI_FontManager_createFont01);
 tolua_function(tolua_S,"destroyFont",tolua_CEGUI_CEGUI_FontManager_destroyFont00);
 tolua_function(tolua_S,"destroyFont",tolua_CEGUI_CEGUI_FontManager_destroyFont01);
 tolua_function(tolua_S,"destroyAllFonts",tolua_CEGUI_CEGUI_FontManager_destroyAllFonts00);
 tolua_function(tolua_S,"isFontPresent",tolua_CEGUI_CEGUI_FontManager_isFontPresent00);
 tolua_function(tolua_S,"getFont",tolua_CEGUI_CEGUI_FontManager_getFont00);
 tolua_function(tolua_S,"notifyScreenResolution",tolua_CEGUI_CEGUI_FontManager_notifyScreenResolution00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"Image","CEGUI::Image","",NULL);
 tolua_beginmodule(tolua_S,"Image");
 tolua_function(tolua_S,"getSize",tolua_CEGUI_CEGUI_Image_getSize00);
 tolua_function(tolua_S,"getWidth",tolua_CEGUI_CEGUI_Image_getWidth00);
 tolua_function(tolua_S,"getHeight",tolua_CEGUI_CEGUI_Image_getHeight00);
 tolua_function(tolua_S,"getOffsets",tolua_CEGUI_CEGUI_Image_getOffsets00);
 tolua_function(tolua_S,"getOffsetX",tolua_CEGUI_CEGUI_Image_getOffsetX00);
 tolua_function(tolua_S,"getOffsetY",tolua_CEGUI_CEGUI_Image_getOffsetY00);
 tolua_function(tolua_S,"getName",tolua_CEGUI_CEGUI_Image_getName00);
 tolua_function(tolua_S,"getImagesetName",tolua_CEGUI_CEGUI_Image_getImagesetName00);
 tolua_function(tolua_S,"getSourceTextureArea",tolua_CEGUI_CEGUI_Image_getSourceTextureArea00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"Texture","CEGUI::Texture","",NULL);
 tolua_beginmodule(tolua_S,"Texture");
 tolua_function(tolua_S,"getWidth",tolua_CEGUI_CEGUI_Texture_getWidth00);
 tolua_function(tolua_S,"getHeight",tolua_CEGUI_CEGUI_Texture_getHeight00);
 tolua_function(tolua_S,"loadFromFile",tolua_CEGUI_CEGUI_Texture_loadFromFile00);
 tolua_function(tolua_S,"getRenderer",tolua_CEGUI_CEGUI_Texture_getRenderer00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"Imageset","CEGUI::Imageset","",NULL);
 tolua_beginmodule(tolua_S,"Imageset");
 tolua_function(tolua_S,"getTexture",tolua_CEGUI_CEGUI_Imageset_getTexture00);
 tolua_function(tolua_S,"getName",tolua_CEGUI_CEGUI_Imageset_getName00);
 tolua_function(tolua_S,"getImageCount",tolua_CEGUI_CEGUI_Imageset_getImageCount00);
 tolua_function(tolua_S,"isImageDefined",tolua_CEGUI_CEGUI_Imageset_isImageDefined00);
 tolua_function(tolua_S,"getImage",tolua_CEGUI_CEGUI_Imageset_getImage00);
 tolua_function(tolua_S,"undefineImage",tolua_CEGUI_CEGUI_Imageset_undefineImage00);
 tolua_function(tolua_S,"undefineAllImages",tolua_CEGUI_CEGUI_Imageset_undefineAllImages00);
 tolua_function(tolua_S,"getImageSize",tolua_CEGUI_CEGUI_Imageset_getImageSize00);
 tolua_function(tolua_S,"getImageWidth",tolua_CEGUI_CEGUI_Imageset_getImageWidth00);
 tolua_function(tolua_S,"getImageHeight",tolua_CEGUI_CEGUI_Imageset_getImageHeight00);
 tolua_function(tolua_S,"getImageOffset",tolua_CEGUI_CEGUI_Imageset_getImageOffset00);
 tolua_function(tolua_S,"getImageOffsetX",tolua_CEGUI_CEGUI_Imageset_getImageOffsetX00);
 tolua_function(tolua_S,"getImageOffsetY",tolua_CEGUI_CEGUI_Imageset_getImageOffsetY00);
 tolua_function(tolua_S,"defineImage",tolua_CEGUI_CEGUI_Imageset_defineImage00);
 tolua_function(tolua_S,"defineImage",tolua_CEGUI_CEGUI_Imageset_defineImage01);
 tolua_function(tolua_S,"isAutoScaled",tolua_CEGUI_CEGUI_Imageset_isAutoScaled00);
 tolua_function(tolua_S,"setAutoScalingEnabled",tolua_CEGUI_CEGUI_Imageset_setAutoScalingEnabled00);
 tolua_function(tolua_S,"getNativeResolution",tolua_CEGUI_CEGUI_Imageset_getNativeResolution00);
 tolua_function(tolua_S,"setNativeResolution",tolua_CEGUI_CEGUI_Imageset_setNativeResolution00);
 tolua_function(tolua_S,"notifyScreenResolution",tolua_CEGUI_CEGUI_Imageset_notifyScreenResolution00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"ImagesetManager","CEGUI::ImagesetManager","",NULL);
 tolua_beginmodule(tolua_S,"ImagesetManager");
 tolua_function(tolua_S,"getSingleton",tolua_CEGUI_CEGUI_ImagesetManager_getSingleton00);
 tolua_function(tolua_S,"createImageset",tolua_CEGUI_CEGUI_ImagesetManager_createImageset00);
 tolua_function(tolua_S,"createImageset",tolua_CEGUI_CEGUI_ImagesetManager_createImageset01);
 tolua_function(tolua_S,"destroyImageset",tolua_CEGUI_CEGUI_ImagesetManager_destroyImageset00);
 tolua_function(tolua_S,"destroyImageset",tolua_CEGUI_CEGUI_ImagesetManager_destroyImageset01);
 tolua_function(tolua_S,"destroyAllImagesets",tolua_CEGUI_CEGUI_ImagesetManager_destroyAllImagesets00);
 tolua_function(tolua_S,"getImageset",tolua_CEGUI_CEGUI_ImagesetManager_getImageset00);
 tolua_function(tolua_S,"isImagesetPresent",tolua_CEGUI_CEGUI_ImagesetManager_isImagesetPresent00);
 tolua_function(tolua_S,"notifyScreenResolution",tolua_CEGUI_CEGUI_ImagesetManager_notifyScreenResolution00);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"Key",0);
 tolua_beginmodule(tolua_S,"Key");
 tolua_constant(tolua_S,"Escape",CEGUI::Key::Escape);
 tolua_constant(tolua_S,"One",CEGUI::Key::One);
 tolua_constant(tolua_S,"Two",CEGUI::Key::Two);
 tolua_constant(tolua_S,"Three",CEGUI::Key::Three);
 tolua_constant(tolua_S,"Four",CEGUI::Key::Four);
 tolua_constant(tolua_S,"Five",CEGUI::Key::Five);
 tolua_constant(tolua_S,"Six",CEGUI::Key::Six);
 tolua_constant(tolua_S,"Seven",CEGUI::Key::Seven);
 tolua_constant(tolua_S,"Eight",CEGUI::Key::Eight);
 tolua_constant(tolua_S,"Nine",CEGUI::Key::Nine);
 tolua_constant(tolua_S,"Zero",CEGUI::Key::Zero);
 tolua_constant(tolua_S,"Minus",CEGUI::Key::Minus);
 tolua_constant(tolua_S,"Equals",CEGUI::Key::Equals);
 tolua_constant(tolua_S,"Backspace",CEGUI::Key::Backspace);
 tolua_constant(tolua_S,"Tab",CEGUI::Key::Tab);
 tolua_constant(tolua_S,"Q",CEGUI::Key::Q);
 tolua_constant(tolua_S,"W",CEGUI::Key::W);
 tolua_constant(tolua_S,"E",CEGUI::Key::E);
 tolua_constant(tolua_S,"R",CEGUI::Key::R);
 tolua_constant(tolua_S,"T",CEGUI::Key::T);
 tolua_constant(tolua_S,"Y",CEGUI::Key::Y);
 tolua_constant(tolua_S,"U",CEGUI::Key::U);
 tolua_constant(tolua_S,"I",CEGUI::Key::I);
 tolua_constant(tolua_S,"O",CEGUI::Key::O);
 tolua_constant(tolua_S,"P",CEGUI::Key::P);
 tolua_constant(tolua_S,"LeftBracket",CEGUI::Key::LeftBracket);
 tolua_constant(tolua_S,"RightBracket",CEGUI::Key::RightBracket);
 tolua_constant(tolua_S,"Return",CEGUI::Key::Return);
 tolua_constant(tolua_S,"LeftControl",CEGUI::Key::LeftControl);
 tolua_constant(tolua_S,"A",CEGUI::Key::A);
 tolua_constant(tolua_S,"S",CEGUI::Key::S);
 tolua_constant(tolua_S,"D",CEGUI::Key::D);
 tolua_constant(tolua_S,"F",CEGUI::Key::F);
 tolua_constant(tolua_S,"G",CEGUI::Key::G);
 tolua_constant(tolua_S,"H",CEGUI::Key::H);
 tolua_constant(tolua_S,"J",CEGUI::Key::J);
 tolua_constant(tolua_S,"K",CEGUI::Key::K);
 tolua_constant(tolua_S,"L",CEGUI::Key::L);
 tolua_constant(tolua_S,"Semicolon",CEGUI::Key::Semicolon);
 tolua_constant(tolua_S,"Apostrophe",CEGUI::Key::Apostrophe);
 tolua_constant(tolua_S,"Grave",CEGUI::Key::Grave);
 tolua_constant(tolua_S,"LeftShift",CEGUI::Key::LeftShift);
 tolua_constant(tolua_S,"Backslash",CEGUI::Key::Backslash);
 tolua_constant(tolua_S,"Z",CEGUI::Key::Z);
 tolua_constant(tolua_S,"X",CEGUI::Key::X);
 tolua_constant(tolua_S,"C",CEGUI::Key::C);
 tolua_constant(tolua_S,"V",CEGUI::Key::V);
 tolua_constant(tolua_S,"B",CEGUI::Key::B);
 tolua_constant(tolua_S,"N",CEGUI::Key::N);
 tolua_constant(tolua_S,"M",CEGUI::Key::M);
 tolua_constant(tolua_S,"Comma",CEGUI::Key::Comma);
 tolua_constant(tolua_S,"Period",CEGUI::Key::Period);
 tolua_constant(tolua_S,"Slash",CEGUI::Key::Slash);
 tolua_constant(tolua_S,"RightShift",CEGUI::Key::RightShift);
 tolua_constant(tolua_S,"Multiply",CEGUI::Key::Multiply);
 tolua_constant(tolua_S,"LeftAlt",CEGUI::Key::LeftAlt);
 tolua_constant(tolua_S,"Space",CEGUI::Key::Space);
 tolua_constant(tolua_S,"Capital",CEGUI::Key::Capital);
 tolua_constant(tolua_S,"F1",CEGUI::Key::F1);
 tolua_constant(tolua_S,"F2",CEGUI::Key::F2);
 tolua_constant(tolua_S,"F3",CEGUI::Key::F3);
 tolua_constant(tolua_S,"F4",CEGUI::Key::F4);
 tolua_constant(tolua_S,"F5",CEGUI::Key::F5);
 tolua_constant(tolua_S,"F6",CEGUI::Key::F6);
 tolua_constant(tolua_S,"F7",CEGUI::Key::F7);
 tolua_constant(tolua_S,"F8",CEGUI::Key::F8);
 tolua_constant(tolua_S,"F9",CEGUI::Key::F9);
 tolua_constant(tolua_S,"F10",CEGUI::Key::F10);
 tolua_constant(tolua_S,"NumLock",CEGUI::Key::NumLock);
 tolua_constant(tolua_S,"ScrollLock",CEGUI::Key::ScrollLock);
 tolua_constant(tolua_S,"Numpad7",CEGUI::Key::Numpad7);
 tolua_constant(tolua_S,"Numpad8",CEGUI::Key::Numpad8);
 tolua_constant(tolua_S,"Numpad9",CEGUI::Key::Numpad9);
 tolua_constant(tolua_S,"Subtract",CEGUI::Key::Subtract);
 tolua_constant(tolua_S,"Numpad4",CEGUI::Key::Numpad4);
 tolua_constant(tolua_S,"Numpad5",CEGUI::Key::Numpad5);
 tolua_constant(tolua_S,"Numpad6",CEGUI::Key::Numpad6);
 tolua_constant(tolua_S,"Add",CEGUI::Key::Add);
 tolua_constant(tolua_S,"Numpad1",CEGUI::Key::Numpad1);
 tolua_constant(tolua_S,"Numpad2",CEGUI::Key::Numpad2);
 tolua_constant(tolua_S,"Numpad3",CEGUI::Key::Numpad3);
 tolua_constant(tolua_S,"Numpad0",CEGUI::Key::Numpad0);
 tolua_constant(tolua_S,"Decimal",CEGUI::Key::Decimal);
 tolua_constant(tolua_S,"OEM_102",CEGUI::Key::OEM_102);
 tolua_constant(tolua_S,"F11",CEGUI::Key::F11);
 tolua_constant(tolua_S,"F12",CEGUI::Key::F12);
 tolua_constant(tolua_S,"F13",CEGUI::Key::F13);
 tolua_constant(tolua_S,"F14",CEGUI::Key::F14);
 tolua_constant(tolua_S,"F15",CEGUI::Key::F15);
 tolua_constant(tolua_S,"Kana",CEGUI::Key::Kana);
 tolua_constant(tolua_S,"ABNT_C1",CEGUI::Key::ABNT_C1);
 tolua_constant(tolua_S,"Convert",CEGUI::Key::Convert);
 tolua_constant(tolua_S,"NoConvert",CEGUI::Key::NoConvert);
 tolua_constant(tolua_S,"Yen",CEGUI::Key::Yen);
 tolua_constant(tolua_S,"ABNT_C2",CEGUI::Key::ABNT_C2);
 tolua_constant(tolua_S,"NumpadEquals",CEGUI::Key::NumpadEquals);
 tolua_constant(tolua_S,"PrevTrack",CEGUI::Key::PrevTrack);
 tolua_constant(tolua_S,"At",CEGUI::Key::At);
 tolua_constant(tolua_S,"Colon",CEGUI::Key::Colon);
 tolua_constant(tolua_S,"Underline",CEGUI::Key::Underline);
 tolua_constant(tolua_S,"Kanji",CEGUI::Key::Kanji);
 tolua_constant(tolua_S,"Stop",CEGUI::Key::Stop);
 tolua_constant(tolua_S,"AX",CEGUI::Key::AX);
 tolua_constant(tolua_S,"Unlabeled",CEGUI::Key::Unlabeled);
 tolua_constant(tolua_S,"NextTrack",CEGUI::Key::NextTrack);
 tolua_constant(tolua_S,"NumpadEnter",CEGUI::Key::NumpadEnter);
 tolua_constant(tolua_S,"RightControl",CEGUI::Key::RightControl);
 tolua_constant(tolua_S,"Mute",CEGUI::Key::Mute);
 tolua_constant(tolua_S,"Calculator",CEGUI::Key::Calculator);
 tolua_constant(tolua_S,"PlayPause",CEGUI::Key::PlayPause);
 tolua_constant(tolua_S,"MediaStop",CEGUI::Key::MediaStop);
 tolua_constant(tolua_S,"VolumeDown",CEGUI::Key::VolumeDown);
 tolua_constant(tolua_S,"VolumeUp",CEGUI::Key::VolumeUp);
 tolua_constant(tolua_S,"WebHome",CEGUI::Key::WebHome);
 tolua_constant(tolua_S,"NumpadComma",CEGUI::Key::NumpadComma);
 tolua_constant(tolua_S,"Divide",CEGUI::Key::Divide);
 tolua_constant(tolua_S,"SysRq",CEGUI::Key::SysRq);
 tolua_constant(tolua_S,"RightAlt",CEGUI::Key::RightAlt);
 tolua_constant(tolua_S,"Pause",CEGUI::Key::Pause);
 tolua_constant(tolua_S,"Home",CEGUI::Key::Home);
 tolua_constant(tolua_S,"ArrowUp",CEGUI::Key::ArrowUp);
 tolua_constant(tolua_S,"PageUp",CEGUI::Key::PageUp);
 tolua_constant(tolua_S,"ArrowLeft",CEGUI::Key::ArrowLeft);
 tolua_constant(tolua_S,"ArrowRight",CEGUI::Key::ArrowRight);
 tolua_constant(tolua_S,"End",CEGUI::Key::End);
 tolua_constant(tolua_S,"ArrowDown",CEGUI::Key::ArrowDown);
 tolua_constant(tolua_S,"PageDown",CEGUI::Key::PageDown);
 tolua_constant(tolua_S,"Insert",CEGUI::Key::Insert);
 tolua_constant(tolua_S,"Delete",CEGUI::Key::Delete);
 tolua_constant(tolua_S,"LeftWindows",CEGUI::Key::LeftWindows);
 tolua_constant(tolua_S,"RightWindow",CEGUI::Key::RightWindow);
 tolua_constant(tolua_S,"AppMenu",CEGUI::Key::AppMenu);
 tolua_constant(tolua_S,"Power",CEGUI::Key::Power);
 tolua_constant(tolua_S,"Sleep",CEGUI::Key::Sleep);
 tolua_constant(tolua_S,"Wake",CEGUI::Key::Wake);
 tolua_constant(tolua_S,"WebSearch",CEGUI::Key::WebSearch);
 tolua_constant(tolua_S,"WebFavorites",CEGUI::Key::WebFavorites);
 tolua_constant(tolua_S,"WebRefresh",CEGUI::Key::WebRefresh);
 tolua_constant(tolua_S,"WebStop",CEGUI::Key::WebStop);
 tolua_constant(tolua_S,"WebForward",CEGUI::Key::WebForward);
 tolua_constant(tolua_S,"WebBack",CEGUI::Key::WebBack);
 tolua_constant(tolua_S,"MyComputer",CEGUI::Key::MyComputer);
 tolua_constant(tolua_S,"Mail",CEGUI::Key::Mail);
 tolua_constant(tolua_S,"MediaSelect",CEGUI::Key::MediaSelect);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"LeftMouse",CEGUI::LeftMouse);
 tolua_constant(tolua_S,"RightMouse",CEGUI::RightMouse);
 tolua_constant(tolua_S,"Shift",CEGUI::Shift);
 tolua_constant(tolua_S,"Control",CEGUI::Control);
 tolua_constant(tolua_S,"MiddleMouse",CEGUI::MiddleMouse);
 tolua_constant(tolua_S,"X1Mouse",CEGUI::X1Mouse);
 tolua_constant(tolua_S,"X2Mouse",CEGUI::X2Mouse);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"EventArgs","CEGUI::EventArgs","",tolua_collect_CEGUI__EventArgs);
#else
 tolua_cclass(tolua_S,"EventArgs","CEGUI::EventArgs","",NULL);
#endif
 tolua_beginmodule(tolua_S,"EventArgs");
 tolua_variable(tolua_S,"handled",tolua_get_CEGUI__EventArgs_handled,tolua_set_CEGUI__EventArgs_handled);
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_EventArgs_new00);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_EventArgs_new00_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_EventArgs_new00_local, "CEGUI::EventArgs");
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"MouseCursorEventArgs","CEGUI::MouseCursorEventArgs","CEGUI::EventArgs",tolua_collect_CEGUI__MouseCursorEventArgs);
#else
 tolua_cclass(tolua_S,"MouseCursorEventArgs","CEGUI::MouseCursorEventArgs","CEGUI::EventArgs",NULL);
#endif
 tolua_beginmodule(tolua_S,"MouseCursorEventArgs");
 tolua_variable(tolua_S,"mouseCursor",tolua_get_CEGUI__MouseCursorEventArgs_mouseCursor_ptr,tolua_set_CEGUI__MouseCursorEventArgs_mouseCursor_ptr);
 tolua_variable(tolua_S,"image",tolua_get_CEGUI__MouseCursorEventArgs_image_ptr,NULL);
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_MouseCursorEventArgs_new00);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_MouseCursorEventArgs_new00_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_MouseCursorEventArgs_new00_local, "CEGUI::MouseCursorEventArgs");
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"WindowEventArgs","CEGUI::WindowEventArgs","CEGUI::EventArgs",tolua_collect_CEGUI__WindowEventArgs);
#else
 tolua_cclass(tolua_S,"WindowEventArgs","CEGUI::WindowEventArgs","CEGUI::EventArgs",NULL);
#endif
 tolua_beginmodule(tolua_S,"WindowEventArgs");
 tolua_variable(tolua_S,"window",tolua_get_CEGUI__WindowEventArgs_window_ptr,tolua_set_CEGUI__WindowEventArgs_window_ptr);
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_WindowEventArgs_new00);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_WindowEventArgs_new00_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_WindowEventArgs_new00_local, "CEGUI::WindowEventArgs");
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"ActivationEventArgs","CEGUI::ActivationEventArgs","CEGUI::WindowEventArgs",tolua_collect_CEGUI__ActivationEventArgs);
#else
 tolua_cclass(tolua_S,"ActivationEventArgs","CEGUI::ActivationEventArgs","CEGUI::WindowEventArgs",NULL);
#endif
 tolua_beginmodule(tolua_S,"ActivationEventArgs");
 tolua_variable(tolua_S,"otherWindow",tolua_get_CEGUI__ActivationEventArgs_otherWindow_ptr,tolua_set_CEGUI__ActivationEventArgs_otherWindow_ptr);
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_ActivationEventArgs_new00);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_ActivationEventArgs_new00_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_ActivationEventArgs_new00_local, "CEGUI::ActivationEventArgs");
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"HeaderSequenceEventArgs","CEGUI::HeaderSequenceEventArgs","CEGUI::WindowEventArgs",tolua_collect_CEGUI__HeaderSequenceEventArgs);
#else
 tolua_cclass(tolua_S,"HeaderSequenceEventArgs","CEGUI::HeaderSequenceEventArgs","CEGUI::WindowEventArgs",NULL);
#endif
 tolua_beginmodule(tolua_S,"HeaderSequenceEventArgs");
 tolua_variable(tolua_S,"oldIdx",tolua_get_CEGUI__HeaderSequenceEventArgs_unsigned_oldIdx,tolua_set_CEGUI__HeaderSequenceEventArgs_unsigned_oldIdx);
 tolua_variable(tolua_S,"newIdx",tolua_get_CEGUI__HeaderSequenceEventArgs_unsigned_newIdx,tolua_set_CEGUI__HeaderSequenceEventArgs_unsigned_newIdx);
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_HeaderSequenceEventArgs_new00);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_HeaderSequenceEventArgs_new00_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_HeaderSequenceEventArgs_new00_local, "CEGUI::HeaderSequenceEventArgs");
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"LeftButton",CEGUI::LeftButton);
 tolua_constant(tolua_S,"RightButton",CEGUI::RightButton);
 tolua_constant(tolua_S,"MiddleButton",CEGUI::MiddleButton);
 tolua_constant(tolua_S,"X1Button",CEGUI::X1Button);
 tolua_constant(tolua_S,"X2Button",CEGUI::X2Button);
 tolua_constant(tolua_S,"MouseButtonCount",CEGUI::MouseButtonCount);
 tolua_constant(tolua_S,"NoButton",CEGUI::NoButton);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"MouseEventArgs","CEGUI::MouseEventArgs","CEGUI::WindowEventArgs",tolua_collect_CEGUI__MouseEventArgs);
#else
 tolua_cclass(tolua_S,"MouseEventArgs","CEGUI::MouseEventArgs","CEGUI::WindowEventArgs",NULL);
#endif
 tolua_beginmodule(tolua_S,"MouseEventArgs");
 tolua_variable(tolua_S,"position",tolua_get_CEGUI__MouseEventArgs_position,tolua_set_CEGUI__MouseEventArgs_position);
 tolua_variable(tolua_S,"moveDelta",tolua_get_CEGUI__MouseEventArgs_moveDelta,tolua_set_CEGUI__MouseEventArgs_moveDelta);
 tolua_variable(tolua_S,"button",tolua_get_CEGUI__MouseEventArgs_button,tolua_set_CEGUI__MouseEventArgs_button);
 tolua_variable(tolua_S,"sysKeys",tolua_get_CEGUI__MouseEventArgs_unsigned_sysKeys,tolua_set_CEGUI__MouseEventArgs_unsigned_sysKeys);
 tolua_variable(tolua_S,"wheelChange",tolua_get_CEGUI__MouseEventArgs_wheelChange,tolua_set_CEGUI__MouseEventArgs_wheelChange);
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_MouseEventArgs_new00);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_MouseEventArgs_new00_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_MouseEventArgs_new00_local, "CEGUI::MouseEventArgs");
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"KeyEventArgs","CEGUI::KeyEventArgs","CEGUI::WindowEventArgs",tolua_collect_CEGUI__KeyEventArgs);
#else
 tolua_cclass(tolua_S,"KeyEventArgs","CEGUI::KeyEventArgs","CEGUI::WindowEventArgs",NULL);
#endif
 tolua_beginmodule(tolua_S,"KeyEventArgs");
 tolua_variable(tolua_S,"codepoint",tolua_get_CEGUI__KeyEventArgs_unsigned_codepoint,tolua_set_CEGUI__KeyEventArgs_unsigned_codepoint);
 tolua_variable(tolua_S,"scancode",tolua_get_CEGUI__KeyEventArgs_scancode,tolua_set_CEGUI__KeyEventArgs_scancode);
 tolua_variable(tolua_S,"sysKeys",tolua_get_CEGUI__KeyEventArgs_unsigned_sysKeys,tolua_set_CEGUI__KeyEventArgs_unsigned_sysKeys);
 tolua_function(tolua_S,"new",tolua_CEGUI_CEGUI_KeyEventArgs_new00);
 tolua_function(tolua_S,"new_local",tolua_CEGUI_CEGUI_KeyEventArgs_new00_local);
 tolua_set_call_event(tolua_S,tolua_CEGUI_CEGUI_KeyEventArgs_new00_local, "CEGUI::KeyEventArgs");
 tolua_endmodule(tolua_S);
 tolua_function(tolua_S,"toMouseCursorEventArgs",tolua_CEGUI_CEGUI_toMouseCursorEventArgs00);
 tolua_function(tolua_S,"toWindowEventArgs",tolua_CEGUI_CEGUI_toWindowEventArgs00);
 tolua_function(tolua_S,"toActivationEventArgs",tolua_CEGUI_CEGUI_toActivationEventArgs00);
 tolua_function(tolua_S,"toHeaderSequenceEventArgs",tolua_CEGUI_CEGUI_toHeaderSequenceEventArgs00);
 tolua_function(tolua_S,"toMouseEventArgs",tolua_CEGUI_CEGUI_toMouseEventArgs00);
 tolua_cclass(tolua_S,"GlobalEventSet","CEGUI::GlobalEventSet","",NULL);
 tolua_beginmodule(tolua_S,"GlobalEventSet");
 tolua_function(tolua_S,"getSingleton",tolua_CEGUI_CEGUI_GlobalEventSet_getSingleton00);
 tolua_function(tolua_S,"addEvent",tolua_CEGUI_CEGUI_GlobalEventSet_addEvent00);
 tolua_function(tolua_S,"removeEvent",tolua_CEGUI_CEGUI_GlobalEventSet_removeEvent00);
 tolua_function(tolua_S,"removeAllEvents",tolua_CEGUI_CEGUI_GlobalEventSet_removeAllEvents00);
 tolua_function(tolua_S,"isEventPresent",tolua_CEGUI_CEGUI_GlobalEventSet_isEventPresent00);
 tolua_function(tolua_S,"subscribeEvent",tolua_CEGUI_CEGUI_GlobalEventSet_subscribeEvent00);
 tolua_function(tolua_S,"fireEvent",tolua_CEGUI_CEGUI_GlobalEventSet_fireEvent00);
 tolua_function(tolua_S,"isMuted",tolua_CEGUI_CEGUI_GlobalEventSet_isMuted00);
 tolua_function(tolua_S,"setMutedState",tolua_CEGUI_CEGUI_GlobalEventSet_setMutedState00);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"BlankMouseCursor",CEGUI::BlankMouseCursor);
 tolua_constant(tolua_S,"DefaultMouseCursor",CEGUI::DefaultMouseCursor);
 tolua_cclass(tolua_S,"MouseCursor","CEGUI::MouseCursor","",NULL);
 tolua_beginmodule(tolua_S,"MouseCursor");
 tolua_function(tolua_S,"getSingleton",tolua_CEGUI_CEGUI_MouseCursor_getSingleton00);
 tolua_function(tolua_S,"setImage",tolua_CEGUI_CEGUI_MouseCursor_setImage00);
 tolua_function(tolua_S,"setImage",tolua_CEGUI_CEGUI_MouseCursor_setImage01);
 tolua_function(tolua_S,"getImage",tolua_CEGUI_CEGUI_MouseCursor_getImage00);
 tolua_function(tolua_S,"setPosition",tolua_CEGUI_CEGUI_MouseCursor_setPosition00);
 tolua_function(tolua_S,"offsetPosition",tolua_CEGUI_CEGUI_MouseCursor_offsetPosition00);
 tolua_function(tolua_S,"setConstraintArea",tolua_CEGUI_CEGUI_MouseCursor_setConstraintArea00);
 tolua_function(tolua_S,"getPosition",tolua_CEGUI_CEGUI_MouseCursor_getPosition00);
 tolua_function(tolua_S,"getConstraintArea",tolua_CEGUI_CEGUI_MouseCursor_getConstraintArea00);
 tolua_function(tolua_S,"getDisplayIndependantPosition",tolua_CEGUI_CEGUI_MouseCursor_getDisplayIndependantPosition00);
 tolua_function(tolua_S,"hide",tolua_CEGUI_CEGUI_MouseCursor_hide00);
 tolua_function(tolua_S,"show",tolua_CEGUI_CEGUI_MouseCursor_show00);
 tolua_function(tolua_S,"isVisible",tolua_CEGUI_CEGUI_MouseCursor_isVisible00);
 tolua_function(tolua_S,"addEvent",tolua_CEGUI_CEGUI_MouseCursor_addEvent00);
 tolua_function(tolua_S,"removeEvent",tolua_CEGUI_CEGUI_MouseCursor_removeEvent00);
 tolua_function(tolua_S,"removeAllEvents",tolua_CEGUI_CEGUI_MouseCursor_removeAllEvents00);
 tolua_function(tolua_S,"isEventPresent",tolua_CEGUI_CEGUI_MouseCursor_isEventPresent00);
 tolua_function(tolua_S,"subscribeEvent",tolua_CEGUI_CEGUI_MouseCursor_subscribeEvent00);
 tolua_function(tolua_S,"fireEvent",tolua_CEGUI_CEGUI_MouseCursor_fireEvent00);
 tolua_function(tolua_S,"isMuted",tolua_CEGUI_CEGUI_MouseCursor_isMuted00);
 tolua_function(tolua_S,"setMutedState",tolua_CEGUI_CEGUI_MouseCursor_setMutedState00);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"Relative",CEGUI::Relative);
 tolua_constant(tolua_S,"Absolute",CEGUI::Absolute);
 tolua_constant(tolua_S,"Inherited",CEGUI::Inherited);
 tolua_cclass(tolua_S,"Window","CEGUI::Window","",NULL);
 tolua_beginmodule(tolua_S,"Window");
 tolua_function(tolua_S,"getType",tolua_CEGUI_CEGUI_Window_getType00);
 tolua_function(tolua_S,"getName",tolua_CEGUI_CEGUI_Window_getName00);
 tolua_function(tolua_S,"isDestroyedByParent",tolua_CEGUI_CEGUI_Window_isDestroyedByParent00);
 tolua_function(tolua_S,"isAlwaysOnTop",tolua_CEGUI_CEGUI_Window_isAlwaysOnTop00);
 tolua_function(tolua_S,"isDisabled",tolua_CEGUI_CEGUI_Window_isDisabled00);
 tolua_function(tolua_S,"isVisible",tolua_CEGUI_CEGUI_Window_isVisible00);
 tolua_function(tolua_S,"isActive",tolua_CEGUI_CEGUI_Window_isActive00);
 tolua_function(tolua_S,"isClippedByParent",tolua_CEGUI_CEGUI_Window_isClippedByParent00);
 tolua_function(tolua_S,"setDestroyedByParent",tolua_CEGUI_CEGUI_Window_setDestroyedByParent00);
 tolua_function(tolua_S,"setAlwaysOnTop",tolua_CEGUI_CEGUI_Window_setAlwaysOnTop00);
 tolua_function(tolua_S,"setEnabled",tolua_CEGUI_CEGUI_Window_setEnabled00);
 tolua_function(tolua_S,"enable",tolua_CEGUI_CEGUI_Window_enable00);
 tolua_function(tolua_S,"disable",tolua_CEGUI_CEGUI_Window_disable00);
 tolua_function(tolua_S,"setVisible",tolua_CEGUI_CEGUI_Window_setVisible00);
 tolua_function(tolua_S,"show",tolua_CEGUI_CEGUI_Window_show00);
 tolua_function(tolua_S,"hide",tolua_CEGUI_CEGUI_Window_hide00);
 tolua_function(tolua_S,"activate",tolua_CEGUI_CEGUI_Window_activate00);
 tolua_function(tolua_S,"deactivate",tolua_CEGUI_CEGUI_Window_deactivate00);
 tolua_function(tolua_S,"setClippedByParent",tolua_CEGUI_CEGUI_Window_setClippedByParent00);
 tolua_function(tolua_S,"getID",tolua_CEGUI_CEGUI_Window_getID00);
 tolua_function(tolua_S,"setID",tolua_CEGUI_CEGUI_Window_setID00);
 tolua_function(tolua_S,"getChildCount",tolua_CEGUI_CEGUI_Window_getChildCount00);
 tolua_function(tolua_S,"isChild",tolua_CEGUI_CEGUI_Window_isChild00);
 tolua_function(tolua_S,"isChild",tolua_CEGUI_CEGUI_Window_isChild01);
 tolua_function(tolua_S,"isChild",tolua_CEGUI_CEGUI_Window_isChild02);
 tolua_function(tolua_S,"addChildWindow",tolua_CEGUI_CEGUI_Window_addChildWindow00);
 tolua_function(tolua_S,"addChildWindow",tolua_CEGUI_CEGUI_Window_addChildWindow01);
 tolua_function(tolua_S,"removeChildWindow",tolua_CEGUI_CEGUI_Window_removeChildWindow00);
 tolua_function(tolua_S,"removeChildWindow",tolua_CEGUI_CEGUI_Window_removeChildWindow01);
 tolua_function(tolua_S,"removeChildWindow",tolua_CEGUI_CEGUI_Window_removeChildWindow02);
 tolua_function(tolua_S,"getChild",tolua_CEGUI_CEGUI_Window_getChild00);
 tolua_function(tolua_S,"getChild",tolua_CEGUI_CEGUI_Window_getChild01);
 tolua_function(tolua_S,"getChildAtIdx",tolua_CEGUI_CEGUI_Window_getChildAtIdx00);
 tolua_function(tolua_S,"getActiveChild",tolua_CEGUI_CEGUI_Window_getActiveChild00);
 tolua_function(tolua_S,"getChildAtPosition",tolua_CEGUI_CEGUI_Window_getChildAtPosition00);
 tolua_function(tolua_S,"getParent",tolua_CEGUI_CEGUI_Window_getParent00);
 tolua_function(tolua_S,"isAncestor",tolua_CEGUI_CEGUI_Window_isAncestor00);
 tolua_function(tolua_S,"isAncestor",tolua_CEGUI_CEGUI_Window_isAncestor01);
 tolua_function(tolua_S,"isAncestor",tolua_CEGUI_CEGUI_Window_isAncestor02);
 tolua_function(tolua_S,"setFont",tolua_CEGUI_CEGUI_Window_setFont00);
 tolua_function(tolua_S,"setFont",tolua_CEGUI_CEGUI_Window_setFont01);
 tolua_function(tolua_S,"getFont",tolua_CEGUI_CEGUI_Window_getFont00);
 tolua_function(tolua_S,"setText",tolua_CEGUI_CEGUI_Window_setText00);
 tolua_function(tolua_S,"getText",tolua_CEGUI_CEGUI_Window_getText00);
 tolua_function(tolua_S,"inheritsAlpha",tolua_CEGUI_CEGUI_Window_inheritsAlpha00);
 tolua_function(tolua_S,"getAlpha",tolua_CEGUI_CEGUI_Window_getAlpha00);
 tolua_function(tolua_S,"getEffectiveAlpha",tolua_CEGUI_CEGUI_Window_getEffectiveAlpha00);
 tolua_function(tolua_S,"setAlpha",tolua_CEGUI_CEGUI_Window_setAlpha00);
 tolua_function(tolua_S,"setInheritsAlpha",tolua_CEGUI_CEGUI_Window_setInheritsAlpha00);
 tolua_function(tolua_S,"requestRedraw",tolua_CEGUI_CEGUI_Window_requestRedraw00);
 tolua_function(tolua_S,"isZOrderingEnabled",tolua_CEGUI_CEGUI_Window_isZOrderingEnabled00);
 tolua_function(tolua_S,"setZOrderingEnabled",tolua_CEGUI_CEGUI_Window_setZOrderingEnabled00);
 tolua_function(tolua_S,"getRect",tolua_CEGUI_CEGUI_Window_getRect00);
 tolua_function(tolua_S,"getPixelRect",tolua_CEGUI_CEGUI_Window_getPixelRect00);
 tolua_function(tolua_S,"getInnerRect",tolua_CEGUI_CEGUI_Window_getInnerRect00);
 tolua_function(tolua_S,"getUnclippedPixelRect",tolua_CEGUI_CEGUI_Window_getUnclippedPixelRect00);
 tolua_function(tolua_S,"getUnclippedInnerRect",tolua_CEGUI_CEGUI_Window_getUnclippedInnerRect00);
 tolua_function(tolua_S,"getCaptureWindow",tolua_CEGUI_CEGUI_Window_getCaptureWindow00);
 tolua_function(tolua_S,"setRestoreCapture",tolua_CEGUI_CEGUI_Window_setRestoreCapture00);
 tolua_function(tolua_S,"restoresOldCapture",tolua_CEGUI_CEGUI_Window_restoresOldCapture00);
 tolua_function(tolua_S,"distributesCapturedInputs",tolua_CEGUI_CEGUI_Window_distributesCapturedInputs00);
 tolua_function(tolua_S,"setDistributesCapturedInputs",tolua_CEGUI_CEGUI_Window_setDistributesCapturedInputs00);
 tolua_function(tolua_S,"captureInput",tolua_CEGUI_CEGUI_Window_captureInput00);
 tolua_function(tolua_S,"releaseInput",tolua_CEGUI_CEGUI_Window_releaseInput00);
 tolua_function(tolua_S,"isCapturedByThis",tolua_CEGUI_CEGUI_Window_isCapturedByThis00);
 tolua_function(tolua_S,"isCapturedByAncestor",tolua_CEGUI_CEGUI_Window_isCapturedByAncestor00);
 tolua_function(tolua_S,"isCapturedByChild",tolua_CEGUI_CEGUI_Window_isCapturedByChild00);
 tolua_function(tolua_S,"isHit",tolua_CEGUI_CEGUI_Window_isHit00);
 tolua_function(tolua_S,"getMetricsMode",tolua_CEGUI_CEGUI_Window_getMetricsMode00);
 tolua_function(tolua_S,"setMetricsMode",tolua_CEGUI_CEGUI_Window_setMetricsMode00);
 tolua_function(tolua_S,"getMouseCursor",tolua_CEGUI_CEGUI_Window_getMouseCursor00);
 tolua_function(tolua_S,"setMouseCursor",tolua_CEGUI_CEGUI_Window_setMouseCursor00);
 tolua_function(tolua_S,"setMouseCursor",tolua_CEGUI_CEGUI_Window_setMouseCursor01);
 tolua_function(tolua_S,"setMouseCursor",tolua_CEGUI_CEGUI_Window_setMouseCursor02);
 tolua_function(tolua_S,"getRelativeRect",tolua_CEGUI_CEGUI_Window_getRelativeRect00);
 tolua_function(tolua_S,"getRelativePosition",tolua_CEGUI_CEGUI_Window_getRelativePosition00);
 tolua_function(tolua_S,"getRelativeXPosition",tolua_CEGUI_CEGUI_Window_getRelativeXPosition00);
 tolua_function(tolua_S,"getRelativeYPosition",tolua_CEGUI_CEGUI_Window_getRelativeYPosition00);
 tolua_function(tolua_S,"getRelativeSize",tolua_CEGUI_CEGUI_Window_getRelativeSize00);
 tolua_function(tolua_S,"getRelativeWidth",tolua_CEGUI_CEGUI_Window_getRelativeWidth00);
 tolua_function(tolua_S,"getRelativeHeight",tolua_CEGUI_CEGUI_Window_getRelativeHeight00);
 tolua_function(tolua_S,"getAbsoluteRect",tolua_CEGUI_CEGUI_Window_getAbsoluteRect00);
 tolua_function(tolua_S,"getAbsolutePosition",tolua_CEGUI_CEGUI_Window_getAbsolutePosition00);
 tolua_function(tolua_S,"getAbsoluteXPosition",tolua_CEGUI_CEGUI_Window_getAbsoluteXPosition00);
 tolua_function(tolua_S,"getAbsoluteYPosition",tolua_CEGUI_CEGUI_Window_getAbsoluteYPosition00);
 tolua_function(tolua_S,"getAbsoluteSize",tolua_CEGUI_CEGUI_Window_getAbsoluteSize00);
 tolua_function(tolua_S,"getAbsoluteWidth",tolua_CEGUI_CEGUI_Window_getAbsoluteWidth00);
 tolua_function(tolua_S,"getAbsoluteHeight",tolua_CEGUI_CEGUI_Window_getAbsoluteHeight00);
 tolua_function(tolua_S,"setPosition",tolua_CEGUI_CEGUI_Window_setPosition00);
 tolua_function(tolua_S,"setXPosition",tolua_CEGUI_CEGUI_Window_setXPosition00);
 tolua_function(tolua_S,"setYPosition",tolua_CEGUI_CEGUI_Window_setYPosition00);
 tolua_function(tolua_S,"setWidth",tolua_CEGUI_CEGUI_Window_setWidth00);
 tolua_function(tolua_S,"setHeight",tolua_CEGUI_CEGUI_Window_setHeight00);
 tolua_function(tolua_S,"setSize",tolua_CEGUI_CEGUI_Window_setSize00);
 tolua_function(tolua_S,"setRect",tolua_CEGUI_CEGUI_Window_setRect00);
 tolua_function(tolua_S,"getPosition",tolua_CEGUI_CEGUI_Window_getPosition00);
 tolua_function(tolua_S,"getXPosition",tolua_CEGUI_CEGUI_Window_getXPosition00);
 tolua_function(tolua_S,"getYPosition",tolua_CEGUI_CEGUI_Window_getYPosition00);
 tolua_function(tolua_S,"getWidth",tolua_CEGUI_CEGUI_Window_getWidth00);
 tolua_function(tolua_S,"getHeight",tolua_CEGUI_CEGUI_Window_getHeight00);
 tolua_function(tolua_S,"getSize",tolua_CEGUI_CEGUI_Window_getSize00);
 tolua_function(tolua_S,"getRect",tolua_CEGUI_CEGUI_Window_getRect01);
 tolua_function(tolua_S,"setMinimumSize",tolua_CEGUI_CEGUI_Window_setMinimumSize00);
 tolua_function(tolua_S,"setMaximumSize",tolua_CEGUI_CEGUI_Window_setMaximumSize00);
 tolua_function(tolua_S,"setSize",tolua_CEGUI_CEGUI_Window_setSize01);
 tolua_function(tolua_S,"setWidth",tolua_CEGUI_CEGUI_Window_setWidth01);
 tolua_function(tolua_S,"setHeight",tolua_CEGUI_CEGUI_Window_setHeight01);
 tolua_function(tolua_S,"getWidth",tolua_CEGUI_CEGUI_Window_getWidth01);
 tolua_function(tolua_S,"getHeight",tolua_CEGUI_CEGUI_Window_getHeight01);
 tolua_function(tolua_S,"getSize",tolua_CEGUI_CEGUI_Window_getSize01);
 tolua_function(tolua_S,"getMinimumSize",tolua_CEGUI_CEGUI_Window_getMinimumSize00);
 tolua_function(tolua_S,"getMaximumSize",tolua_CEGUI_CEGUI_Window_getMaximumSize00);
 tolua_function(tolua_S,"setPosition",tolua_CEGUI_CEGUI_Window_setPosition01);
 tolua_function(tolua_S,"setXPosition",tolua_CEGUI_CEGUI_Window_setXPosition01);
 tolua_function(tolua_S,"setYPosition",tolua_CEGUI_CEGUI_Window_setYPosition01);
 tolua_function(tolua_S,"getXPosition",tolua_CEGUI_CEGUI_Window_getXPosition01);
 tolua_function(tolua_S,"getYPosition",tolua_CEGUI_CEGUI_Window_getYPosition01);
 tolua_function(tolua_S,"getPosition",tolua_CEGUI_CEGUI_Window_getPosition01);
 tolua_function(tolua_S,"setAreaRect",tolua_CEGUI_CEGUI_Window_setAreaRect00);
 tolua_function(tolua_S,"absoluteToRelativeX",tolua_CEGUI_CEGUI_Window_absoluteToRelativeX00);
 tolua_function(tolua_S,"absoluteToRelativeY",tolua_CEGUI_CEGUI_Window_absoluteToRelativeY00);
 tolua_function(tolua_S,"absoluteToRelative",tolua_CEGUI_CEGUI_Window_absoluteToRelative00);
 tolua_function(tolua_S,"absoluteToRelative",tolua_CEGUI_CEGUI_Window_absoluteToRelative01);
 tolua_function(tolua_S,"absoluteToRelative",tolua_CEGUI_CEGUI_Window_absoluteToRelative02);
 tolua_function(tolua_S,"relativeToAbsoluteX",tolua_CEGUI_CEGUI_Window_relativeToAbsoluteX00);
 tolua_function(tolua_S,"relativeToAbsoluteY",tolua_CEGUI_CEGUI_Window_relativeToAbsoluteY00);
 tolua_function(tolua_S,"relativeToAbsolute",tolua_CEGUI_CEGUI_Window_relativeToAbsolute00);
 tolua_function(tolua_S,"relativeToAbsolute",tolua_CEGUI_CEGUI_Window_relativeToAbsolute01);
 tolua_function(tolua_S,"relativeToAbsolute",tolua_CEGUI_CEGUI_Window_relativeToAbsolute02);
 tolua_function(tolua_S,"windowToScreenX",tolua_CEGUI_CEGUI_Window_windowToScreenX00);
 tolua_function(tolua_S,"windowToScreenY",tolua_CEGUI_CEGUI_Window_windowToScreenY00);
 tolua_function(tolua_S,"windowToScreen",tolua_CEGUI_CEGUI_Window_windowToScreen00);
 tolua_function(tolua_S,"windowToScreen",tolua_CEGUI_CEGUI_Window_windowToScreen01);
 tolua_function(tolua_S,"windowToScreen",tolua_CEGUI_CEGUI_Window_windowToScreen02);
 tolua_function(tolua_S,"screenToWindowX",tolua_CEGUI_CEGUI_Window_screenToWindowX00);
 tolua_function(tolua_S,"screenToWindowY",tolua_CEGUI_CEGUI_Window_screenToWindowY00);
 tolua_function(tolua_S,"screenToWindow",tolua_CEGUI_CEGUI_Window_screenToWindow00);
 tolua_function(tolua_S,"screenToWindow",tolua_CEGUI_CEGUI_Window_screenToWindow01);
 tolua_function(tolua_S,"screenToWindow",tolua_CEGUI_CEGUI_Window_screenToWindow02);
 tolua_function(tolua_S,"moveToFront",tolua_CEGUI_CEGUI_Window_moveToFront00);
 tolua_function(tolua_S,"moveToBack",tolua_CEGUI_CEGUI_Window_moveToBack00);
 tolua_function(tolua_S,"wantsMultiClickEvents",tolua_CEGUI_CEGUI_Window_wantsMultiClickEvents00);
 tolua_function(tolua_S,"isMouseAutoRepeatEnabled",tolua_CEGUI_CEGUI_Window_isMouseAutoRepeatEnabled00);
 tolua_function(tolua_S,"getAutoRepeatDelay",tolua_CEGUI_CEGUI_Window_getAutoRepeatDelay00);
 tolua_function(tolua_S,"getAutoRepeatRate",tolua_CEGUI_CEGUI_Window_getAutoRepeatRate00);
 tolua_function(tolua_S,"setWantsMultiClickEvents",tolua_CEGUI_CEGUI_Window_setWantsMultiClickEvents00);
 tolua_function(tolua_S,"setMouseAutoRepeatEnabled",tolua_CEGUI_CEGUI_Window_setMouseAutoRepeatEnabled00);
 tolua_function(tolua_S,"setAutoRepeatDelay",tolua_CEGUI_CEGUI_Window_setAutoRepeatDelay00);
 tolua_function(tolua_S,"setAutoRepeatRate",tolua_CEGUI_CEGUI_Window_setAutoRepeatRate00);
 tolua_function(tolua_S,"destroy",tolua_CEGUI_CEGUI_Window_destroy00);
 tolua_function(tolua_S,"setProperty",tolua_CEGUI_CEGUI_Window_setProperty00);
 tolua_function(tolua_S,"getProperty",tolua_CEGUI_CEGUI_Window_getProperty00);
 tolua_function(tolua_S,"getPropertyDefault",tolua_CEGUI_CEGUI_Window_getPropertyDefault00);
 tolua_function(tolua_S,"getPropertyHelp",tolua_CEGUI_CEGUI_Window_getPropertyHelp00);
 tolua_function(tolua_S,"isPropertyPresent",tolua_CEGUI_CEGUI_Window_isPropertyPresent00);
 tolua_function(tolua_S,"isPropertyDefault",tolua_CEGUI_CEGUI_Window_isPropertyDefault00);
 tolua_function(tolua_S,"addEvent",tolua_CEGUI_CEGUI_Window_addEvent00);
 tolua_function(tolua_S,"removeEvent",tolua_CEGUI_CEGUI_Window_removeEvent00);
 tolua_function(tolua_S,"removeAllEvents",tolua_CEGUI_CEGUI_Window_removeAllEvents00);
 tolua_function(tolua_S,"isEventPresent",tolua_CEGUI_CEGUI_Window_isEventPresent00);
 tolua_function(tolua_S,"subscribeEvent",tolua_CEGUI_CEGUI_Window_subscribeEvent00);
 tolua_function(tolua_S,"fireEvent",tolua_CEGUI_CEGUI_Window_fireEvent00);
 tolua_function(tolua_S,"isMuted",tolua_CEGUI_CEGUI_Window_isMuted00);
 tolua_function(tolua_S,"setMutedState",tolua_CEGUI_CEGUI_Window_setMutedState00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"Renderer","CEGUI::Renderer","",NULL);
 tolua_beginmodule(tolua_S,"Renderer");
 tolua_function(tolua_S,"getCurrentZ",tolua_CEGUI_CEGUI_Renderer_getCurrentZ00);
 tolua_function(tolua_S,"getZLayer",tolua_CEGUI_CEGUI_Renderer_getZLayer00);
 tolua_function(tolua_S,"isQueueingEnabled",tolua_CEGUI_CEGUI_Renderer_isQueueingEnabled00);
 tolua_function(tolua_S,"createTexture",tolua_CEGUI_CEGUI_Renderer_createTexture00);
 tolua_function(tolua_S,"createTexture",tolua_CEGUI_CEGUI_Renderer_createTexture01);
 tolua_function(tolua_S,"createTexture",tolua_CEGUI_CEGUI_Renderer_createTexture02);
 tolua_function(tolua_S,"destroyTexture",tolua_CEGUI_CEGUI_Renderer_destroyTexture00);
 tolua_function(tolua_S,"destroyAllTextures",tolua_CEGUI_CEGUI_Renderer_destroyAllTextures00);
 tolua_function(tolua_S,"getWidth",tolua_CEGUI_CEGUI_Renderer_getWidth00);
 tolua_function(tolua_S,"getHeight",tolua_CEGUI_CEGUI_Renderer_getHeight00);
 tolua_function(tolua_S,"getSize",tolua_CEGUI_CEGUI_Renderer_getSize00);
 tolua_function(tolua_S,"getRect",tolua_CEGUI_CEGUI_Renderer_getRect00);
 tolua_function(tolua_S,"getMaxTextureSize",tolua_CEGUI_CEGUI_Renderer_getMaxTextureSize00);
 tolua_function(tolua_S,"getHorzScreenDPI",tolua_CEGUI_CEGUI_Renderer_getHorzScreenDPI00);
 tolua_function(tolua_S,"getVertScreenDPI",tolua_CEGUI_CEGUI_Renderer_getVertScreenDPI00);
 tolua_function(tolua_S,"addEvent",tolua_CEGUI_CEGUI_Renderer_addEvent00);
 tolua_function(tolua_S,"removeEvent",tolua_CEGUI_CEGUI_Renderer_removeEvent00);
 tolua_function(tolua_S,"removeAllEvents",tolua_CEGUI_CEGUI_Renderer_removeAllEvents00);
 tolua_function(tolua_S,"isEventPresent",tolua_CEGUI_CEGUI_Renderer_isEventPresent00);
 tolua_function(tolua_S,"subscribeEvent",tolua_CEGUI_CEGUI_Renderer_subscribeEvent00);
 tolua_function(tolua_S,"fireEvent",tolua_CEGUI_CEGUI_Renderer_fireEvent00);
 tolua_function(tolua_S,"isMuted",tolua_CEGUI_CEGUI_Renderer_isMuted00);
 tolua_function(tolua_S,"setMutedState",tolua_CEGUI_CEGUI_Renderer_setMutedState00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"System","CEGUI::System","",NULL);
 tolua_beginmodule(tolua_S,"System");
 tolua_function(tolua_S,"getSingleton",tolua_CEGUI_CEGUI_System_getSingleton00);
 tolua_function(tolua_S,"getRenderer",tolua_CEGUI_CEGUI_System_getRenderer00);
 tolua_function(tolua_S,"setDefaultFont",tolua_CEGUI_CEGUI_System_setDefaultFont00);
 tolua_function(tolua_S,"setDefaultFont",tolua_CEGUI_CEGUI_System_setDefaultFont01);
 tolua_function(tolua_S,"signalRedraw",tolua_CEGUI_CEGUI_System_signalRedraw00);
 tolua_function(tolua_S,"isRedrawRequested",tolua_CEGUI_CEGUI_System_isRedrawRequested00);
 tolua_function(tolua_S,"setGUISheet",tolua_CEGUI_CEGUI_System_setGUISheet00);
 tolua_function(tolua_S,"getGUISheet",tolua_CEGUI_CEGUI_System_getGUISheet00);
 tolua_function(tolua_S,"setSingleClickTimeout",tolua_CEGUI_CEGUI_System_setSingleClickTimeout00);
 tolua_function(tolua_S,"setMultiClickTimeout",tolua_CEGUI_CEGUI_System_setMultiClickTimeout00);
 tolua_function(tolua_S,"setMultiClickToleranceAreaSize",tolua_CEGUI_CEGUI_System_setMultiClickToleranceAreaSize00);
 tolua_function(tolua_S,"getSingleClickTimeout",tolua_CEGUI_CEGUI_System_getSingleClickTimeout00);
 tolua_function(tolua_S,"getMultiClickTimeout",tolua_CEGUI_CEGUI_System_getMultiClickTimeout00);
 tolua_function(tolua_S,"getMultiClickToleranceAreaSize",tolua_CEGUI_CEGUI_System_getMultiClickToleranceAreaSize00);
 tolua_function(tolua_S,"setDefaultMouseCursor",tolua_CEGUI_CEGUI_System_setDefaultMouseCursor00);
 tolua_function(tolua_S,"setDefaultMouseCursor",tolua_CEGUI_CEGUI_System_setDefaultMouseCursor01);
 tolua_function(tolua_S,"setDefaultMouseCursor",tolua_CEGUI_CEGUI_System_setDefaultMouseCursor02);
 tolua_function(tolua_S,"getDefaultMouseCursor",tolua_CEGUI_CEGUI_System_getDefaultMouseCursor00);
 tolua_function(tolua_S,"getWindowContainingMouse",tolua_CEGUI_CEGUI_System_getWindowContainingMouse00);
 tolua_function(tolua_S,"executeScriptFile",tolua_CEGUI_CEGUI_System_executeScriptFile00);
 tolua_function(tolua_S,"executeScriptGlobal",tolua_CEGUI_CEGUI_System_executeScriptGlobal00);
 tolua_function(tolua_S,"executeScriptString",tolua_CEGUI_CEGUI_System_executeScriptString00);
 tolua_function(tolua_S,"getMouseMoveScaling",tolua_CEGUI_CEGUI_System_getMouseMoveScaling00);
 tolua_function(tolua_S,"setMouseMoveScaling",tolua_CEGUI_CEGUI_System_setMouseMoveScaling00);
 tolua_function(tolua_S,"notifyWindowDestroyed",tolua_CEGUI_CEGUI_System_notifyWindowDestroyed00);
 tolua_function(tolua_S,"injectMouseMove",tolua_CEGUI_CEGUI_System_injectMouseMove00);
 tolua_function(tolua_S,"injectMouseLeaves",tolua_CEGUI_CEGUI_System_injectMouseLeaves00);
 tolua_function(tolua_S,"injectMouseButtonDown",tolua_CEGUI_CEGUI_System_injectMouseButtonDown00);
 tolua_function(tolua_S,"injectMouseButtonUp",tolua_CEGUI_CEGUI_System_injectMouseButtonUp00);
 tolua_function(tolua_S,"injectKeyDown",tolua_CEGUI_CEGUI_System_injectKeyDown00);
 tolua_function(tolua_S,"injectKeyUp",tolua_CEGUI_CEGUI_System_injectKeyUp00);
 tolua_function(tolua_S,"injectChar",tolua_CEGUI_CEGUI_System_injectChar00);
 tolua_function(tolua_S,"injectMouseWheelChange",tolua_CEGUI_CEGUI_System_injectMouseWheelChange00);
 tolua_function(tolua_S,"injectMousePosition",tolua_CEGUI_CEGUI_System_injectMousePosition00);
 tolua_function(tolua_S,"injectTimePulse",tolua_CEGUI_CEGUI_System_injectTimePulse00);
 tolua_function(tolua_S,"addEvent",tolua_CEGUI_CEGUI_System_addEvent00);
 tolua_function(tolua_S,"removeEvent",tolua_CEGUI_CEGUI_System_removeEvent00);
 tolua_function(tolua_S,"removeAllEvents",tolua_CEGUI_CEGUI_System_removeAllEvents00);
 tolua_function(tolua_S,"isEventPresent",tolua_CEGUI_CEGUI_System_isEventPresent00);
 tolua_function(tolua_S,"subscribeEvent",tolua_CEGUI_CEGUI_System_subscribeEvent00);
 tolua_function(tolua_S,"fireEvent",tolua_CEGUI_CEGUI_System_fireEvent00);
 tolua_function(tolua_S,"isMuted",tolua_CEGUI_CEGUI_System_isMuted00);
 tolua_function(tolua_S,"setMutedState",tolua_CEGUI_CEGUI_System_setMutedState00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"WindowManager","CEGUI::WindowManager","",NULL);
 tolua_beginmodule(tolua_S,"WindowManager");
 tolua_function(tolua_S,"getSingleton",tolua_CEGUI_CEGUI_WindowManager_getSingleton00);
 tolua_function(tolua_S,"createWindow",tolua_CEGUI_CEGUI_WindowManager_createWindow00);
 tolua_function(tolua_S,"loadWindowLayout",tolua_CEGUI_CEGUI_WindowManager_loadWindowLayout00);
 tolua_function(tolua_S,"destroyWindow",tolua_CEGUI_CEGUI_WindowManager_destroyWindow00);
 tolua_function(tolua_S,"destroyWindow",tolua_CEGUI_CEGUI_WindowManager_destroyWindow01);
 tolua_function(tolua_S,"destroyAllWindows",tolua_CEGUI_CEGUI_WindowManager_destroyAllWindows00);
 tolua_function(tolua_S,"getWindow",tolua_CEGUI_CEGUI_WindowManager_getWindow00);
 tolua_function(tolua_S,"isWindowPresent",tolua_CEGUI_CEGUI_WindowManager_isWindowPresent00);
 tolua_function(tolua_S,"isDeadPoolEmpty",tolua_CEGUI_CEGUI_WindowManager_isDeadPoolEmpty00);
 tolua_function(tolua_S,"cleanDeadPool",tolua_CEGUI_CEGUI_WindowManager_cleanDeadPool00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"Scheme","CEGUI::Scheme","",NULL);
 tolua_beginmodule(tolua_S,"Scheme");
 tolua_function(tolua_S,"loadResources",tolua_CEGUI_CEGUI_Scheme_loadResources00);
 tolua_function(tolua_S,"unloadResources",tolua_CEGUI_CEGUI_Scheme_unloadResources00);
 tolua_function(tolua_S,"resourcesLoaded",tolua_CEGUI_CEGUI_Scheme_resourcesLoaded00);
 tolua_function(tolua_S,"getName",tolua_CEGUI_CEGUI_Scheme_getName00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"SchemeManager","CEGUI::SchemeManager","",NULL);
 tolua_beginmodule(tolua_S,"SchemeManager");
 tolua_function(tolua_S,"getSingleton",tolua_CEGUI_CEGUI_SchemeManager_getSingleton00);
 tolua_function(tolua_S,"loadScheme",tolua_CEGUI_CEGUI_SchemeManager_loadScheme00);
 tolua_function(tolua_S,"unloadScheme",tolua_CEGUI_CEGUI_SchemeManager_unloadScheme00);
 tolua_function(tolua_S,"isSchemePresent",tolua_CEGUI_CEGUI_SchemeManager_isSchemePresent00);
 tolua_function(tolua_S,"getScheme",tolua_CEGUI_CEGUI_SchemeManager_getScheme00);
 tolua_function(tolua_S,"unloadAllSchemes",tolua_CEGUI_CEGUI_SchemeManager_unloadAllSchemes00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}
