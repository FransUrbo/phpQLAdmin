<?php
// attribute plugin for phpQLAdmin configuration
// $Id: attrib.config.inc,v 1.56 2007-02-12 15:32:09 turbo Exp $

// {{{ attribute_output_java_window(void)
function attribute_output_java_window() {
  global $LANG, $_pql;
?>
  <script type="text/javascript" language="javascript"><!--
    function errorWindow() {
      myWindow = window.open("", "ProblemWindow", 'toolbar,width=850,height=400'), $LANG;
      myWindow.document.write("I can.t retreive the subschemaSubentry entry from base NULL. ");
	  myWindow.document.write("Make sure you have access to do a search that in the shell looks like this:<p>");

	  myWindow.document.write("<pre>");
	  myWindow.document.write("/usr/bin/ldapsearch -x -LLL -h localhost -s base -b '' 'objectClass=*' subschemaSubentry");
	  myWindow.document.write("</pre>");

	  myWindow.document.write("You should retreive something like this back:<p>");

	  myWindow.document.write("<pre>");
	  myWindow.document.write("dn:\nsubschemaSubentry: ".$_pql->ldap_subschema."\n");
	  myWindow.document.write("</pre>");

	  myWindow.document.write("If this succeeds, then try the following command:");
	  myWindow.document.write("<pre>");
	  myWindow.document.write("/usr/bin/ldapsearch -x -LLL -h localhost -s base -b '".$_pql->ldap_subschema."' 'objectClass=*' objectclasses");
	  myWindow.document.write("</pre>");
	  myWindow.document.write("This should return every objectclass and what attributes wanted/required for those. ");
	  myWindow.document.write("I that didn't work, debug your LDAP server possibly granting access to READ below the DN ".$_pql->ldap_subschema);

      myWindow.document.bgColor="white";
      myWindow.document.close();
    }
  //--></script>
<?php
}
// }}}

// {{{ attribute_check(void)
function attribute_check() {
    global $error, $LANG;

	if(@$_REQUEST["toggle"])
	  // No point in checking a toggle...
	  return true;

	$attrib = $_REQUEST["attrib"];
	if(($attrib != pql_get_define("PQL_ATTR_SCRIPT_CREATE_USER")) and
	   ($attrib != pql_get_define("PQL_ATTR_SCRIPT_CREATE_DOMAIN")) and
	   ($attrib != pql_get_define("PQL_ATTR_RADIUS_CONFIG_IDENTITY")))
	{
		// Unless it's a 'externalScript{User,Domain}' attribute - is it typed in?
		if(empty($_REQUEST[$attrib])) {
			$error[$attrib] = $LANG->_('Missing');
			return false;
		}
	} elseif($attrib != pql_get_define("PQL_ATTR_RADIUS_CONFIG_IDENTITY")) {
	  // Check to see that the radius_identity value is filled in/choosen
	  if(!@$_REQUEST['radius_identity_name'] or !@$_REQUEST['radius_identity_dn'])
		$error[$attrib] = $LANG->_('Missing');
	}
    
    return true;
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
	global $_pql, $error, $LANG, $PQL_ATTRIBUTE;

	// DLW: This form doesn't print the "$error" entries.

	// Save some typing.
	$attrib = strtolower($_REQUEST["attrib"]);
	$rootdn = $_REQUEST["rootdn"];
	$oldvalue = (empty($_REQUEST["oldvalue"]) ? '' : $_REQUEST["oldvalue"]);
	$toggle   = (empty($_REQUEST["toggle"]) ? 0 : 1);

	// Get the old values from the define
	foreach($PQL_ATTRIBUTE as $level => $array) {
		foreach($array as $key => $value) {
			$value = strtolower($value);
			if(($value == $attrib) and pql_get_define($key, $rootdn)) {
				$oldvalue = pql_get_define($key, $rootdn);
				break;
			}
		}
	}

	if(! $rootdn)
	  $rootdn = $_SESSION["BASE_DN"][0];
	$dn = $rootdn;

	// Now, get the old values from the LDAP database,
	// overwriting the defines if existing in db.
	$value = pql_get_attribute($_pql->ldap_linkid, $dn, $attrib);

	// If it's an array, make it a comma separated list
	if(is_array($value)) {
		$oldval = '';
		for($i=0; $i < count($value); $i++) {
			if($oldval)
			  $oldval .= ",".$value[$i];
			else
			  $oldval = $value[$i];
		}
		
		$oldvalue = $oldval;
	} else {
		if($value != '')
		  $oldvalue = $value;
	}

	attribute_output_java_window();
?>

  <span class="title1"><?=$LANG->_('Change options')?></span>

  <br><br>

  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post" name="phpqladmin_modify">
<?php   if(($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_USER")) or
 		   ($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN"))) {
			 // {{{ Special header/table if it's a '{branch,user}ObjectClass' value
?>
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Set/Modify attribute %attribute%'), array('attribute' => $attrib))."\n"; ?>
        <tr class="subtitle">
          <table>
            <td colspan="2"><img src="images/info.png" width="16" height="16" border="0" align="left"></td>
            <td>
              <?=$LANG->_('These are the objectclasses that exists in the LDAP server, exept those that\'s already included for use')?>.
            </td>
          </table>
        </tr>
      </th>
    </table>

<?php
// }}}
		}

		if($attrib == pql_get_define("PQL_ATTR_RADIUS_CONFIG_IDENTITY")) {
		  // {{{ Changing the RADIUS identity
		  // Find all 'simpleSecurityObject' or 'account' objects
		  $filter = '(|('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=simpleSecurityObject)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=account))';
		  $dns = pql_get_dn($_pql->ldap_linkid, $dn, $filter);
		  if(is_array($dns)) {
			$checked = 0;
?>
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Use existing DN as RADIUS identity')?>:</td>
          <td class="<?=pql_format_table(1)?>">
<?php		for($i=0; $dns[$i]; $i++) {
			  echo '            <input type="radio" name="radius_identity_dn" value="'.$dns[$i].'"';
			  if(!$checked and (($dns[$i] == $oldvalue) or eregi('radius', $dns[$i]))) {
				$checked = 1;
				echo " CHECKED";
			  }
			  echo '>'.$dns[$i]."<br>\n";
			}

			// Find all 'organizationalUnit's for direction on WHERE to put the new user
			$filter = '(&('.pql_get_define("PQL_ATTR_OU").'=*)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=organizationalUnit))';
			$ous = pql_get_dn($_pql->ldap_linkid, $dn, $filter);
			asort($ous);
?>
          </td>
        </tr>
      </th>

      <th colspan="3" align="left"><?=$LANG->_('or')?>:
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Create new RADIUS identity')?>:</td>
          <td class="<?=pql_format_table(1)?>">
            uid=<input type="text" name="radius_identity_name" value="<?=$_REQUEST["radius_identity_name"]?>" size="10">,
            <select name="ou_dn">
<?php		for($i=0; $ous[$i]; $i++) { ?>
              <option value="<?=$ous[$i]?>"><?=$ous[$i]?></option>
<?php		} ?>
            </select>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Password')?>:</td>
          <td class="<?=pql_format_table(1)?>">
            <input type="text" name="radius_identity_pass" value="<?=$_REQUEST["radius_identity_pass"]?>" size="20">
            &nbsp;&nbsp;&nbsp;
            <img src="images/info.png" width="16" height="16" alt="" border="0">
            <?=$LANG->_('Password will be added using a MD5 encryption.')?>
          </td>
        </tr>
        </tr>
      </th>
    </table>
<?php	  }
// }}}
		} else {
		  // {{{ Other value than RADIUS identity
?>
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">
            <?=pql_complete_constant($LANG->_('Add to attribute %attribute%'), array('attribute' => $attrib))."\n"; ?>
          </td>
<?php		$class=pql_format_table(0); ?>
          <td class="<?=$class?>">
<?php   	if(($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_USER")) or
			   ($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN")))
			{
				 if($attrib == 'userobjectclass')
				 $existing = pql_get_define("PQL_CONF_OBJECTCLASS_USER", $dn);
				 else
				 $existing = pql_get_define("PQL_CONF_OBJECTCLASS_DOMAIN", $dn);
				 if(is_array($existing))
				 $existing = pql_split_oldvalues($existing); // Convert to array
				 else
				 $existing = array();
				 
				 $objectclasses = pql_get_subschema($_pql->ldap_linkid, "objectclasses");
				 if(is_array($objectclasses)) {
				   $i = 0; $new_tr = 0;
				   $div_counter = 10; // Initialize the global counter
				   
				   foreach($objectclasses as $key => $oc) {
					 $print = 1; $gotit = 0;
					 
					 if($key == 'ALIAS')
					   break;
					 
					 foreach($existing as $e) {
					   if((($e == $key) or eregi('phpQLAdmin', $key)) and !$gotit) {
						 $print = 0; $gotit = 1;
					   }
					 }
					 
					 if($print) {
					   $class=pql_format_table(0);
					   if($new_tr) {
?>

        <tr>
          <td class="title"></td>
          <td class="<?=$class?>">
<?php					}
						$new_tr = 1;

						$txt  = '<input type="checkbox" name="'.$attrib.'_'.$i.'" value="'.$key.'" size="50">';
						$txt .= $objectclasses[$key]["NAME"];
						pql_format_tree($txt, '');

						if($objectclasses[$key]["MUST"]["count"]) {
?>
            <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>Must:</b>
<?php						echo "            ";
							for($counter=0; $counter < $objectclasses[$key]["MUST"]["count"]; $counter++) {
								echo $objectclasses[$key]["MUST"][$counter];
								if(!empty($objectclasses[$key]["MUST"][$counter+1]))
								  echo ", ";
							}
						}

						if($objectclasses[$key]["MAY"]["count"]) {
?>

            <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>May:</b> &nbsp;
<?php						echo "            ";
							for($counter=0; $counter < $objectclasses[$key]["MAY"]["count"]; $counter++) {
								echo $objectclasses[$key]["MAY"][$counter];
								if(!empty($objectclasses[$key]["MAY"][$counter+1]))
								  echo ", ";
							}
						}

						// This an ending for the domain tree
						echo "\n"; pql_format_tree_end();
?>
          </td>
        </tr>
<?php					$i++;
					 } // if(print) 
				   } // foreach(objectclasses)
?>
      <input type="hidden" name="<?=$attrib?>_<?=$i?>" value="on">
      <input type="hidden" name="<?=$attrib?>" value="<?=$i?>">
<?php			 } else {
				// No objectclasses in LDAP!
?>
          <td class="<?=$class?>"><i><a href="javascript:errorWindow()"><?=$LANG->_('No objectclasses')?>!</i></a></td
<?php
				 }
			} elseif($attrib == pql_get_define("PQL_ATTR_LANG")) {
			  // We're to change the language -> show a select box with existing languages
			  $languages = $LANG->get_supported();
?>
            <select name="<?=$attrib?>">
<?php			foreach($languages as $l => $lang) { ?>
              <option value="<?=$l?>"><?=$lang?></option>
<?php			} ?>
            </select>
<?php
			} else {
			  // Not a userObjectClass or branchObjectClass
			  if($attrib == pql_get_define("PQL_ATTR_PASSWORD_SCHEMES")) {
				$values = array('CRYPT', 'MD5', 'SHA', 'CLEAR', 'KERBEROS');
				if(is_array($values)) {
?>
          <td>
<?php				for($i=0, $j=0; $i < count($values); $i++) {
						if(!eregi($values[$i], pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn))) {
							// We don't want to include schemes we've already
							// configured branch to use...
?>
            <input type="checkbox" name="<?=$attrib?>_<?=$j?>" value="<?=$values[$i]?>"><?=$values[$i]?><br>
<?php						$j++;
						}
					}
?>
            <input type="hidden"   name="<?=$attrib?>"   value="<?=$j?>">
          </td>
<?php			}
			  } else {
?>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?><input type="text" name="<?=$attrib?>" value="<?php if($attrib != 'whoarewe') { echo $oldvalue; } ?>" size="40">
          </td>
<?php		  }
			}
?>
        </tr>
      </th>
    </table>
<?php
// }}}
		}
?>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
<?php if($toggle) { ?>
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
<?php } ?>
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

  <script language="JavaScript">
  <!--
    document.phpqladmin_modify.<?=$attrib?>.focus();
	// -->
  </script>
<?php
}
// }}}

// {{{ attribute_save(void)
function attribute_save() {
  global $_pql, $LANG, $PQL_ATTRIBUTE;
  $entry = array();

  // ---------------------------------

  $rootdn		= (empty($_REQUEST["rootdn"]) ? '' : $_REQUEST["rootdn"]);
  $domain		= (empty($_REQUEST["domain"]) ? '' : $_REQUEST["domain"]);

  $attrib		= lc($_REQUEST["attrib"]);
  $attrib_val	= (empty($_REQUEST[$attrib]) ? '' : $_REQUEST[$attrib]);
  $oldvalue		= (empty($_REQUEST["oldvalue"]) ? '' : $_REQUEST["oldvalue"]);
  $toggle		= (empty($_REQUEST["toggle"]) ? 0 : 1);
  $action		= (empty($_REQUEST["action"]) ? '' : $_REQUEST["action"]);
  
  if($domain)
	$dn = $domain;
  elseif($rootdn)
	$dn = $rootdn;
  else
	// It's a config value and should be applied on ALL base DN's.
	$dn = '';
  
  // ---------------------------------

  if($toggle) {
	// {{{ A togglable value.
	if(!$oldvalue) {
	  settype($oldvalue, "boolean");

	  // We don't know the previous value, fetch it.
	  if($dn)
		$oldvalue = pql_get_attribute($_pql->ldap_linkid, $dn, $attrib);
	  else
		$oldvalue = pql_get_attribute($_pql->ldap_linkid, $_SESSION["BASE_DN"][0], $attrib);
	}

	// Check again (this time falling back to defaults set in include/{defaults,config}.inc etc)
	if(empty($oldvalue)) {
	  foreach($PQL_ATTRIBUTE as $level => $array) {
		foreach($array as $key => $value) {
		  if(!is_array($value)) {
			if(lc($value) == $attrib) {
			  $oldvalue = pql_get_define($key, $rootdn);
			  break;
			}
		  }
		}
	  }
	}

	// Reverse state
	if($oldvalue == 1)
	  $entry[$attrib][] = 'FALSE';
	elseif($oldvalue == 'TRUE')
	  $entry[$attrib][] = 'FALSE';
	elseif(($oldvalue == 0) or empty($oldvalue))
	  $entry[$attrib][] = 'TRUE';
	elseif($oldvalue == 'FALSE')
	  $entry[$attrib][] = 'TRUE';
	// }}}
  } elseif(($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_USER")) or
		   ($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN"))) {
	// {{{ One of the many multivalued values (add or del)
	// ($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"))
	// ($attrib == pql_get_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME"))
	// ($attrib == pql_get_define("PQL_ATTR_PASSWORD_SCHEMES"))

	// {{{ Retreive the old value(s).
	if($dn)
	  $oldvalue = pql_get_attribute($_pql->ldap_linkid, $dn, $attrib);
	else
	  $oldvalue = pql_get_attribute($_pql->ldap_linkid, $_SESSION["BASE_DN"][0], $attrib);

	if($oldvalue) {
	  if(!is_array($oldvalue))
		$oldvalue = array($oldvalue);

	  for($i=0; $i < count($oldvalue); $i++) {
		if($action == 'del') {
		  if(!eregi($attrib_val, $oldvalue[$i]))
			$entry[$attrib][] = $oldvalue[$i];
		} else
		  $entry[$attrib][] = $oldvalue[$i];
	  }
	}
	// }}}

	// {{{ Add ALL of the specified values to the array we're sending to the LDAP server.
	if($action != 'del') {
	  $got_attrib_val = 0;
	  for($i=0; $i < $_POST[$attrib]; $i++) {
		$var_name = $attrib . "_$i";
		if(!empty($_POST[$var_name])) {
		  $got_attrib_val = 1;
		  $entry[$attrib][] = $_POST[$var_name];
		}
	  }
	  
	  if(!$got_attrib_val)
		// This is just case we haven't specified multiple values to add
		$entry[$attrib][] = $attrib_val;
	}
	// }}}
	// }}}
  } elseif($attrib == pql_get_define("PQL_ATTR_RADIUS_CONFIG_IDENTITY")) {
	// {{{ Changing the RADIUS identity
	if($_REQUEST['radius_identity_name']) 
	  $entry[$attrib][] = pql_get_define("PQL_ATTR_UID").'='.$_REQUEST["radius_identity_name"].','.$_REQUEST["ou_dn"];
	else
	  $entry[$attrib][] = $_REQUEST['radius_identity_dn'];
// }}}
  } else
	$entry[$attrib][] = $attrib_val;

  // ---------------------------------

  // {{{ Do the actual modification(s)
  if(!empty($rootdn) and $toggle) {
	// {{{ One place where this could happen: 'tables/config_details-template.inc->Generate uid and gid numbers [y/n]'
	if(!pql_modify_attribute($_pql->ldap_linkid, $dn, '', 1, $entry))
	  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
								   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
	else
	  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
								   array('what' => $attrib));
	// }}}
  } elseif(empty($rootdn) or ($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")) or $toggle) {
	// {{{ We don't have a DN, we're changing controlsAdministrator OR it's a toggle.
	// This means it's a global variable, and should be set in every namingContexts
	foreach($_SESSION["BASE_DN"] as $dn) {
	  if(!pql_modify_attribute($_pql->ldap_linkid, $dn, '', 1, $entry))
		$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									 array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
	  else
		$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									 array('what' => $attrib));
	} // FOREACH $_SESSION["BASE_DN"]
	// }}}
  } else {
	// {{{ Have a DN
	if(pql_modify_attribute($_pql->ldap_linkid, $rootdn, '', 1, $entry)) {
	  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
								   array('what' => $attrib));

	  if($attrib == pql_get_define("PQL_ATTR_RADIUS_CONFIG_IDENTITY")) {
		// Check to see if the choosen DN exists
		$dn = pql_get_dn($_pql->ldap_linkid, $entry[$attrib][0], pql_get_define("PQL_ATTR_OBJECTCLASS").'=*');
		if(!@$dn) {
		  // Does not exists - create a simpleSecurityObject/account object

		  // Generate a hashed password
		  $password = pql_password_hash($_REQUEST['radius_identity_pass'], '{MD5}');

		  // Setup the object array
		  $dn = $entry[$attrib][0]; unset($entry);
		  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][]	= 'simpleSecurityObject';
		  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][]	= 'account';
		  $entry[pql_get_define("PQL_ATTR_UID")]			= $_REQUEST['radius_identity_name'];
		  $entry[pql_get_define("PQL_ATTR_PASSWD")]			= $password;

		  // Add object to LDAP database
		  if(pql_write_add($_pql->ldap_linkid, $dn, $entry, 'radius', 'attrib.config.inc:radius_add'))
			$msg .= '<br>'.pql_complete_constant($LANG->_('Successfully added %what%'), array('what' => $dn));
		  else
			$msg .= '<br>'.pql_complete_constant($LANG->_('Failed to add %what%'), array('what' => $dn));
		}
	  }
	} else
	  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
								   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
  // }}}
  }
	
  attribute_forward($msg, $rlnb);
  break;
  // }}}
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
