<?php
// ----------------------------
// pql_templates.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_templates.inc,v 2.16 2007-09-29 21:15:14 turbo Exp $
//

// {{{ Generate the group template (hardcoded for the moment)
function pql_templates_group() {
  if($_SESSION["NEW_STYLE_QUOTA"]) {
	$TEMPLATE["dn"]    = 'internal';
	$TEMPLATE["name"]  = 'internal_group';
	$TEMPLATE["short"] = 'Internal group account';
	$TEMPLATE["long"]  = 'Group delivery with verification (~ mailinglists)';

	// We need the 'person' object class because 'qmailGroup' (and 'qmailUser')
	// is AUXILIARY, and OpenLDAP >2.2 _require one_ STRUCTURAL.
	if(@pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == sprintf("%s", pql_get_define("PQL_ATTR_UID")))
	  // Need 'uidObject' because of the 'uid' attribute we're referencing users with...
	  $TEMPLATE["userobjectclass"] = array('person', 'uidObject', 'qmailGroup');
	else
	  // Not using 'uid' as reference - do it the QmailLDAP way :(
	  // Let's hope that any one of the MAY in either one of these three object
	  // classes allow the attribute we're referencing users with...
	  $TEMPLATE["userobjectclass"] = array('person', 'qmailUser', 'qmailGroup');
  }

  return($TEMPLATE);
}
// }}}

// {{{ pql_get_templates()
// Get ALL templates availible
function pql_get_templates() {
  global $_pql;

  $filter    = '(&(objectClass=phpQLAdminUserTemplate)('.pql_get_define("PQL_ATTR_USER_TEMPLATE_NAME").'=*))';
  $templates = $_pql->get_dn(pql_get_define("PQL_CONF_SUBTREE_TEMPLATES").','.$_SESSION["BASE_DN"][0], $filter, 'ONELEVEL');

  if(is_array($templates)) {
	// {{{ Setup attributes to retreive.
	$attribs = array("name"            		=> pql_get_define("PQL_ATTR_USER_TEMPLATE_NAME"),
					 "short"           		=> pql_get_define("PQL_ATTR_USER_TEMPLATE_DESC_SHORT"),
					 "long"            		=> pql_get_define("PQL_ATTR_USER_TEMPLATE_DESC_LONG"),
					 "userobjectclass" 		=> pql_get_define("PQL_ATTR_OBJECTCLASS_USER"),
					 "passwordscheme"  		=> pql_get_define("PQL_ATTR_PASSWORD_SCHEMES"),
					 "autocreateugidnumber"	=> pql_get_define("PQL_ATTR_AUTOCREATE_UGIDNUMBER"));
	// }}}
	
	for($i=0; $i < count($templates); $i++) {
	  // Remember the DN
	  $TEMPLATE[$i]["dn"] = $templates[$i];
	  
	  // {{{ Get all information of interest about this template
	  foreach($attribs as $key => $attrib) {
		$value = $_pql->get_attribute($templates[$i], $attrib);
		if($value) {
		  if(!is_array($value) and ($key != "name") and ($key != "short") and ($key != "long") and
			 ($key != "reference") and ($key != "autocreateugidnumber"))
			{
			  // Must be an array for pql_format_table_single() to work...
			  // At the time of this writing, the only stuff that should be an array is 'userObjectClass'
			  // and 'passWordScheme' but I'm predicting a change :)
			  $TEMPLATE[$i][$key] = array($value);
			} else
			  // name, short or long description. Isn't used with pql_format_table_single().
			  $TEMPLATE[$i][$key] = $value;
		}
		
		// Deal with the boolean(s)
		if($attrib == sprintf("%s", pql_get_define("PQL_ATTR_AUTOCREATE_UGIDNUMBER"))) {
		  if(empty($TEMPLATE[$i][pql_get_define("PQL_ATTR_AUTOCREATE_UGIDNUMBER")]))
			$TEMPLATE[$i][pql_get_define("PQL_ATTR_AUTOCREATE_UGIDNUMBER")] = 'FALSE';
		  pql_format_bool($TEMPLATE[$i][pql_get_define("PQL_ATTR_AUTOCREATE_UGIDNUMBER")]);
		}
	  }
	  // }}}
	  
	  // {{{ The old way (pre 2.1.4) of doing it was to have the values in the root DN(s).
	  // Look for the password scheme
	  if(!@$TEMPLATE[$i]["passwordscheme"]) {
		$passwordscheme = $_pql->get_attribute($_SESSION["BASE_DN"][0], pql_get_define("PQL_ATTR_PASSWORD_SCHEMES"));
		if($passwordscheme) {
		  $TEMPLATE[$i]["passwordscheme"] = $passwordscheme;
		  $TEMPLATE[$i]["oldformat"][pql_get_define("PQL_ATTR_PASSWORD_SCHEMES")] = $passwordscheme;
		}
	  }
	  
	  // Look for the user object class
	  if(!$TEMPLATE[$i]["userobjectclass"]) {
		$userobjectclass = $_pql->get_attribute($_SESSION["BASE_DN"][0], pql_get_define("PQL_ATTR_OBJECTCLASS_USER"));
		if($userobjectclass) {
		  $TEMPLATE[$i]["userobjectclass"] = $userobjectclass;
		  $TEMPLATE[$i]["oldformat"][pql_get_define("PQL_ATTR_OBJECTCLASS_USER")] = $userobjectclass;
		}
	  }
	  // }}}
	}
  } else
	$i = 0;

  // Add the group template to the list of templates
  $TEMPLATE[$i] = pql_templates_group();

  return($TEMPLATE);
}
// }}}

// {{{ pql_get_template(name)
// Get all data about a specific template
function pql_get_template($name) {
  if($name == 'group') {
	return pql_templates_group();
  } else {
	// Get ALL templates
	$templates = pql_get_templates();
	
	// Get the one we where asked for
	for($i=0; $i < count($templates); $i++) {
	  if($templates[$i]["name"] == lc($name)) {
		return $templates[$i];
	  }
	}
	
	return false;
  }
}
// }}}

// {{{ pql_templates_attributes(objectclasses)
function pql_templates_attributes($objectclasses) {
  // Get all objectclasses the LDAP server understand
  $schema = pql_get_subschema('objectclasses');
  
  // Get all the MUST attributes for all the included object classes
  for($j=0; $j < count($objectclasses); $j++) {
	$oc = lc($objectclasses[$j]); // short form
	if(!empty($schema[$oc]["MUST"])) {
	  for($k=0; $k < $schema[$oc]["MUST"]["count"]; $k++)
		$must[] = $schema[$oc]["MUST"][$k];
	}

	if(!empty($schema[$oc]["MAY"])) {
	  for($k=0; $k < $schema[$oc]["MAY"]["count"]; $k++)
		$may[] = $schema[$oc]["MAY"][$k];
	}
  }
  
  if(!empty($must))
	// Sort and 'uniquify' the array of MUST attributes
	$must = pql_uniq($must);

  if(!empty($may))
	// Sort and 'uniquify' the array of MAY attributes
	$may = pql_uniq($may);

  return(array('must' => (empty($must) ? NULL : $must),
			   'may'  => (empty($may) ? NULL : $may)));
}
// }}}

// {{{ pql_templates_check_attribute(attribs, attribute, type)
// Template can be either the template array OR a template name
// Returns '1' if it's a MUST and '2' if it's a MAY.
//
// TODO (if/when '$type' is set):
//	* If looking for a MUST, then return '1' if it's a MUST and '0' if it's a MAY
//	* If looking for a MAY,  then return '1' if it's a MAY  and '0' if it's a MUST
function pql_templates_check_attribute($attribs, $attribute, $type = NULL) {
  // This is quite ugly and maybe a little 'fuzzy'. But we're always (?) called from within
  // (or between) forms where the root DN is specified in the request so it should be safe
  // to do it this way...
  if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == $attribute)
	// We're referencing users with this attribute - require this!
	return 1;

  if(!$type or (lc($type) == "must")) {
	for($i=0; $i < count($attribs['must']); $i++) {
	  if(ereg(":", $attribs['must'][$i])){
		list($real, $alias) = split(":", $attribs['must'][$i]);
		if( (lc($real) == lc($attribute)) or (lc($alias) == lc($attribute)))
		  return 1;
	  }

	  if(lc($attribs['must'][$i]) == lc($attribute))
		return 1;
	}
  }

  if(!$type or (lc($type) == "may")) {
	for($i=0; $i < count($attribs['may']); $i++) {
	  if(ereg(":", $attribs['may'][$i])){
		list($real, $alias) = split(":", $attribs['may'][$i]);
		if( (lc($real) == lc($attribute)) or (lc($alias) == lc($attribute)))
		  return 2;
	  }

	  if(lc($attribs['may'][$i]) == lc($attribute))
		return 2;
	}
  }

  return false;
}
// }}}

// {{{ pql_templates_mark_importance(importance)
// Bah. I'm lazy. This is just here because I wanted some
// simplicity in my life :)
// Look at 'tables/user_add-details.inc' for it's usage
// in real life...
// Called with '1' (MUST) or '2' (MAY).
function pql_templates_mark_importance($importance) {
  if($importance == 1)
	return "<font color=red>*</font>\n";
  else
	return "&nbsp;&nbsp;\n";
}
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
