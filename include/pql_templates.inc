<?php
// ----------------------------
// pql_search.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_templates.inc,v 2.4 2005-03-19 10:50:54 turbo Exp $
//

// {{{ Generate the group template (hardcoded for the moment)
function pql_templates_group() {
  if($_SESSION["NEW_STYLE_QUOTA"]) {
	$TEMPLATE["dn"]    = 'internal';
	$TEMPLATE["name"]  = 'group';
	$TEMPLATE["short"] = 'Group account';
	$TEMPLATE["long"]  = 'Group delivery with verification (~ mailinglists)';

	// We need the 'person' object class because 'qmailGroup' (and 'qmailUser')
	// is AUXILIARY, and OpenLDAP >2.2 _require one_ STRUCTURAL.
	if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == pql_get_define("PQL_ATTR_UID"))
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

// {{{ pql_get_templates(linkid)
function pql_get_templates($linkid) {
  $filter    = '(&(objectClass=phpQLAdminUserTemplate)('.pql_get_define("PQL_ATTR_USER_TEMPLATE_NAME").'=*))';
  $templates = pql_get_dn($linkid, 'ou=Templates,'.$_SESSION["BASE_DN"][0], $filter, 'ONELEVEL');

  // {{{ Setup attributes to retreive.
  $attribs = array("name"            => pql_get_define("PQL_ATTR_USER_TEMPLATE_NAME"),
				   "short"           => pql_get_define("PQL_ATTR_USER_TEMPLATE_DESC_SHORT"),
				   "long"            => pql_get_define("PQL_ATTR_USER_TEMPLATE_DESC_LONG"),
				   "userobjectclass" => pql_get_define("PQL_ATTR_OBJECTCLASS_USER"),
				   "passwordscheme"  => pql_get_define("PQL_ATTR_PASSWORD_SCHEMES"));
  // }}}

  for($i=0; $templates[$i]; $i++) {
	// Remember the DN
	$TEMPLATE[$i]["dn"] = $templates[$i];
    
    // {{{ Get all information of interest about this template
    foreach($attribs as $key => $attrib) {
      $value = pql_get_attribute($linkid, $templates[$i], $attrib);
	  if($value) {
		if(!is_array($value) and ($key != "name") and ($key != "short") and ($key != "long") and ($key != "reference")) {
		  // Must be an array for pql_format_table_single() to work...
		  // At the time of this writing, the only stuff that should be an array is 'userObjectClass'
		  // and 'passWordScheme' but I'm predicting a change :)
		  $TEMPLATE[$i][$key] = array($value);
		} else
		  // name, short or long description. Isn't used with pql_format_table_single().
		  $TEMPLATE[$i][$key] = $value;
	  }
	}
	// }}}

	// {{{ The old way (pre 2.1.4) of doing it was to have the values in the root DN(s).
	
	// Look for the password scheme
	if(!$TEMPLATE[$i]["passwordscheme"]) {
	  $passwordscheme = pql_get_attribute($linkid, $_SESSION["BASE_DN"][0], pql_get_define("PQL_ATTR_PASSWORD_SCHEMES"));
	  if($passwordscheme) {
		$TEMPLATE[$i]["passwordscheme"] = $passwordscheme;
		$TEMPLATE[$i]["oldformat"][pql_get_define("PQL_ATTR_PASSWORD_SCHEMES")] = $passwordscheme;
	  }
	}
	
	// Look for the user object class
	if(!$TEMPLATE[$i]["userobjectclass"]) {
	  $userobjectclass = pql_get_attribute($linkid, $_SESSION["BASE_DN"][0], pql_get_define("PQL_ATTR_OBJECTCLASS_USER"));
	  if($userobjectclass) {
		$TEMPLATE[$i]["userobjectclass"] = $userobjectclass;
		$TEMPLATE[$i]["oldformat"][pql_get_define("PQL_ATTR_OBJECTCLASS_USER")] = $userobjectclass;
	  }
	}
	// }}}
  }
  
  // {{{ Add the group template to the list of templates
  $TEMPLATE[$i] = pql_templates_group();
  // }}}

  return($TEMPLATE);
}
// }}}

// {{{ pql_get_template(linkid, name)
function pql_get_template($linkid, $name) {
  if($name == 'group') {
	return pql_templates_group();
  } else {
  // Get ALL templates
  $templates = pql_get_templates($linkid);

  // Get the one we where asked for
  for($i=0; $templates[$i]; $i++) {
  	if($templates[$i]["name"] == lc($name)) {
	  return $templates[$i];
	}
  }

  return false;
}
}
// }}}

// {{{ pql_templates_attributes(linkid, objectclasses)
function pql_templates_attributes($linkid, $objectclasses) {
  // Get all objectclasses the LDAP server understand
  $schema = pql_get_subschema($linkid, 'objectclasses');
  
  // Get all the MUST attributes for all the included object classes
  for($j=0; $objectclasses[$j]; $j++) {
	$oc = lc($objectclasses[$j]); // short form
	if($schema[$oc]["MUST"]) {
	  for($k=0; $schema[$oc]["MUST"][$k]; $k++) {
		$must[] = $schema[$oc]["MUST"][$k];
	  }
	}

	if($schema[$oc]["MAY"]) {
	  for($k=0; $schema[$oc]["MAY"][$k]; $k++) {
		$may[] = $schema[$oc]["MAY"][$k];
	  }
	}
  }
  
  // Sort and 'uniquify' the array of MUST attributes
  $must = pql_uniq($must);

  // Sort and 'uniquify' the array of MAY attributes
  $may = pql_uniq($may);

  return(array('must'=>$must, 'may'=>$may));
}
// }}}

// {{{ pql_templates_check_attribute(linkid, template, attribute, type)
// Template can be either the template array OR a template name
// Returns '1' if it's a MUST and '2' if it's a MAY.
function pql_templates_check_attribute($linkid, $template, $attribute, $type = NULL) {
  // This is quite ugly and maybe a little 'fuzzy'. But we're always (?) called from within
  // (or between) forms where the root DN is specified in the request so it should be safe
  // to do it this way...
  if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]) == $attribute) {
	// We're referencing users with this attribute - require this!
	return 1;
  }

  if(is_array($template)) {
	$template_def = $template;
  } else {
	// Retreive the user template we've choosen to use when creating this user
	$template_def = pql_get_template($linkid, $template);
  }

  // For each of the userObjectClass'es in the template, get required attributes 
  $attribs = pql_templates_attributes($linkid, $template_def["userobjectclass"]);

  if(!$type or (lc($type) == "must")) {
	for($i=0; $attribs['must'][$i]; $i++) {
	  if(lc($attribs['must'][$i]) == lc($attribute))
		return 1;
	}
  }

  if(!$type or (lc($type) == "may")) {
	for($i=0; $attribs['may'][$i]; $i++) {
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
