/**[txh]********************************************************************

  Copyright (c) 2004 by Salvador E. Tropea.
  Covered by the GPL license.

  Module: Variable objects.
  Comments:
  GDB/MI commands for the "Variable Objects" section.@p

@<pre>
gdb command:              Imp? Description:
-var-create               Yes  create a variable object
-var-delete               Yes  delete the variable object and its children
-var-set-format           Yes  set the display format of this variable
-var-show-format          Yes  show the display format of this variable
-var-info-num-children    Yes  tells how many children this object has
-var-list-children        Yes* return a list of the object's children
-var-info-type            Yes  show the type of this variable object
-var-info-expression      Yes  print what this variable object represents
-var-show-attributes      Yes  is this variable editable?
-var-evaluate-expression  Yes  get the value of this variable
-var-assign               Yes  set the value of this variable
-var-update               Yes* update the variable and its children
@</pre>

Notes:@p
1) I suggest letting gdb to choose the names for the variables.@*
2) -var-list-children supports an optional "show values" argument in MI v2.
It isn't implemented.@*
@p

* MI v1 and v2 result formats supported.@p

***************************************************************************/

#include "mi_gdb.h"

/* Low level versions. */

void mi_var_create(mi_h *h, const char *name, int frame, const char *exp)
{
 const char *n=name ? name : "-";

 if (frame<0)
    mi_send(h,"-var-create %s * %s\n",n,exp);
 else
    mi_send(h,"-var-create %s %d %s\n",n,frame,exp);
}

void mi_var_delete(mi_h *h, const char *name)
{
 mi_send(h,"-var-delete %s\n",name);
}

void mi_var_set_format(mi_h *h, const char *name, const char *format)
{
 mi_send(h,"-var-set-format %s %s\n",name,format);
}

void mi_var_show_format(mi_h *h, const char *name)
{
 mi_send(h,"-var-show-format %s\n",name);
}

void mi_var_info_num_children(mi_h *h, const char *name)
{
 mi_send(h,"-var-info-num-children %s\n",name);
}

void mi_var_info_type(mi_h *h, const char *name)
{
 mi_send(h,"-var-info-type %s\n",name);
}

void mi_var_info_expression(mi_h *h, const char *name)
{
 mi_send(h,"-var-info-expression %s\n",name);
}

void mi_var_show_attributes(mi_h *h, const char *name)
{
 mi_send(h,"-var-show-attributes %s\n",name);
}

void mi_var_update(mi_h *h, const char *name)
{
 if (name)
    mi_send(h,"-var-update %s\n",name);
 else
    mi_send(h,"-var-update *\n");
}

void mi_var_assign(mi_h *h, const char *name, const char *expression)
{
 mi_send(h,"-var-assign %s %s\n",name,expression);
}

void mi_var_evaluate_expression(mi_h *h, const char *name)
{
 mi_send(h,"-var-evaluate-expression %s\n",name);
}

void mi_var_list_children(mi_h *h, const char *name)
{
 mi_send(h,"-var-list-children %s\n",name);
}

/* High level versions. */

/**[txh]********************************************************************

  Description:
  Create a variable object. I recommend using @x{gmi_var_create} and letting
gdb choose the names.

  Command: -var-create
  Return: A new mi_gvar strcture or NULL on error.
  
***************************************************************************/

mi_gvar *gmi_var_create_nm(mi_h *h, const char *name, int frame, const char *exp)
{
 mi_var_create(h,name,frame,exp);
 return mi_res_gvar(h,NULL,exp);
}

/**[txh]********************************************************************

  Description:
  Create a variable object. The name is selected by gdb. Alternative:
@x{gmi_full_var_create}.

  Command: -var-create [auto name]
  Return: A new mi_gvar strcture or NULL on error.
  
***************************************************************************/

mi_gvar *gmi_var_create(mi_h *h, int frame, const char *exp)
{
 return gmi_var_create_nm(h,NULL,frame,exp);
}

/**[txh]********************************************************************

  Description:
  Delete a variable object. Doesn't free the mi_gvar data.

  Command: -var-delete
  Return: !=0 OK
  
***************************************************************************/

int gmi_var_delete(mi_h *h, mi_gvar *var)
{
 mi_var_delete(h,var->name);
 return mi_res_simple_done(h);
}

/**[txh]********************************************************************

  Description:
  Set the format used to represent the result.

  Command: -var-set-format
  Return: !=0 OK
  
***************************************************************************/

int gmi_var_set_format(mi_h *h, mi_gvar *var, enum mi_gvar_fmt format)
{
 int ret;

 mi_var_set_format(h,var->name,mi_format_enum_to_str(format));
 ret=mi_res_simple_done(h);
 if (ret)
    var->format=format;
 return ret;
}

/**[txh]********************************************************************

  Description:
  Fill the format field with info from gdb.

  Command: -var-show-format
  Return: !=0 OK.
  
***************************************************************************/

int gmi_var_show_format(mi_h *h, mi_gvar *var)
{
 mi_var_show_format(h,var->name);
 return mi_res_gvar(h,var,NULL)!=NULL;
}

/**[txh]********************************************************************

  Description:
  Fill the numchild field with info from gdb.

  Command: -var-info-num-children
  Return: !=0 OK
  
***************************************************************************/

int gmi_var_info_num_children(mi_h *h, mi_gvar *var)
{
 mi_var_info_num_children(h,var->name);
 return mi_res_gvar(h,var,NULL)!=NULL;
}

/**[txh]********************************************************************

  Description:
  Fill the type field with info from gdb.

  Command: -var-info-type
  Return: !=0 OK
  
***************************************************************************/

int gmi_var_info_type(mi_h *h, mi_gvar *var)
{
 mi_var_info_type(h,var->name);
 return mi_res_gvar(h,var,NULL)!=NULL;
}

/**[txh]********************************************************************

  Description:
  Fill the expression and lang fields with info from gdb. Note that lang
isn't filled during creation.

  Command: -var-info-expression
  Return: !=0 OK
  
***************************************************************************/

int gmi_var_info_expression(mi_h *h, mi_gvar *var)
{
 mi_var_info_expression(h,var->name);
 return mi_res_gvar(h,var,NULL)!=NULL;
}


/**[txh]********************************************************************

  Description:
  Fill the attr field with info from gdb. Note that attr isn't filled
during creation.

  Command: -var-show-attributes
  Return: !=0 OK
  
***************************************************************************/

int gmi_var_show_attributes(mi_h *h, mi_gvar *var)
{
 mi_var_show_attributes(h,var->name);
 return mi_res_gvar(h,var,NULL)!=NULL;
}

/**[txh]********************************************************************

  Description:
  Create the variable and also fill the lang and attr fields. The name is
selected by gdb.

  Command: -var-create + -var-info-expression + -var-show-attributes
  Return: A new mi_gvar strcture or NULL on error.
  
***************************************************************************/

mi_gvar *gmi_full_var_create(mi_h *h, int frame, const char *exp)
{
 mi_gvar *var=gmi_var_create_nm(h,NULL,frame,exp);
 if (var)
   {/* What if it fails? */
    gmi_var_info_expression(h,var);
    gmi_var_show_attributes(h,var);
   }
 return var;
}

/**[txh]********************************************************************

  Description:
  Update variable. Use NULL for all. Note that *changed can be NULL if none
updated.

  Command: -var-update
  Return: !=0 OK. The @var{changed} list contains the list of changed vars.
  
***************************************************************************/

int gmi_var_update(mi_h *h, mi_gvar *var, mi_gvar_chg **changed)
{
 mi_var_update(h,var ? var->name : NULL);
 return mi_res_changelist(h,changed);
}

/**[txh]********************************************************************

  Description:
  Change variable. Returns the new value.

  Command: -var-assign
  Return: A new mi_results with the assigned value.
  
***************************************************************************/

mi_results *gmi_var_assign(mi_h *h, mi_gvar *var, const char *expression)
{
 mi_var_assign(h,var->name,expression);
 return mi_res_done_var(h,"value");
}

/**[txh]********************************************************************

  Description:
  Get current value for a variable.

  Command: -var-evaluate-expression
  Return: The current value (plain text) or NULL on error.
  
***************************************************************************/

char *gmi_var_evaluate_expression(mi_h *h, mi_gvar *var)
{
 mi_var_evaluate_expression(h,var->name);
 return mi_res_value(h);
}

/**[txh]********************************************************************

  Description:
  List children. It ONLY returns the first level information. :-(

  Command: -var-list-children
  Return: A new list of mi_gvar_children or NULL on error.
  
***************************************************************************/

mi_gvar_children *gmi_var_list_children(mi_h *h, mi_gvar *var)
{
 mi_var_list_children(h,var->name);
 return mi_res_children(h);
}

