<?php
// attribute plugin for phpQLAdmin configuration
// $Id: attrib.config.inc,v 1.41 2004-11-12 09:48:00 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
    global $error, $LANG;

	$attrib = $_REQUEST["attrib"];
	if(($attrib != pql_get_define("PQL_ATTR_SCRIPT_CREATE_USER")) and
	   ($attrib != pql_get_define("PQL_ATTR_SCRIPT_CREATE_DOMAIN"))) {
		// Unless it's a 'externalScript{User,Domain}' attribute - is it typed in?
		if(empty($_REQUEST[$attrib])) {
			$error[$attrib] = $LANG->_('Missing');
			return false;
		}
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
	$oldvalue = @$_REQUEST["oldvalue"];
	$rootdn = $_REQUEST["rootdn"];
	$toggle = $_REQUEST["toggle"];

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
	  $dn = $_pql->ldap_basedn[0];
	else
	  $dn = $rootdn;

	// Now, get the old values from the LDAP database,
	// overwriting the defines if existing in db.
	$value = pql_get_attribute($_pql->ldap_linkid, $dn, $attrib);

	// If it's an array, make it a comma separated list
	if(is_array($value)) {
		unset($oldval);
		for($i=0; $value[$i]; $i++) {
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
	  myWindow.document.write("dn:\nsubschemaSubentry: cn=Subschema\n");
	  myWindow.document.write("</pre>");

	  myWindow.document.write("If this succeeds, then try the following command:");
	  myWindow.document.write("<pre>");
	  myWindow.document.write("/usr/bin/ldapsearch -x -LLL -h localhost -s base -b 'cn=Subschema' 'objectClass=*' objectclasses");
	  myWindow.document.write("</pre>");
	  myWindow.document.write("This should return every objectclass and what attributes wanted/required for those. ");
	  myWindow.document.write("I that didn't work, debug your LDAP server possibly granting access to READ below the DN cn=Subschema");

      myWindow.document.bgColor="white";
      myWindow.document.close();
    }
  //--></script>

  <span class="title1"><?=$LANG->_('Change options')?></span>

  <br><br>

  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post" name="phpqladmin_modify">
<?php   if(($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_USER")) or
 		   ($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN"))) {
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

<?php	} ?>
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo pql_complete_constant($LANG->_('Add to attribute %attribute%'), array('attribute' => $attrib))."\n"; ?>
<?php   if(($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_USER")) or
 		   ($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN"))) {
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
				foreach($objectclasses as $key => $oc) {
                    $print = 1; $gotit = 0;

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
<?php					}
						$new_tr = 1;
?>
          <td class="<?=$class?>">
            <input type="checkbox" name="<?=$attrib?>_<?=$i?>" value="<?=$key?>" size="50"><?=$objectclasses[$key]["NAME"]."\n"?>
          </td>
        </tr>

<?php                   $i++;
					}
                }
?>
      <input type="hidden" name="<?=$attrib?>_<?=$i?>" value="on">
      <input type="hidden" name="<?=$attrib?>" value="<?=$i?>">
<?php		} else {
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
<?php		foreach($languages as $l => $lang) { ?>
              <option value="<?=$l?>"><?=$lang?></option>
<?php		} ?>
            </select>
<?php
        } else {
			// Not a userObjectClass or branchObjectClass
			if($attrib == pql_get_define("PQL_ATTR_PASSWORD_SCHEMES")) {
				$values = array('CRYPT', 'MD5', 'SHA', 'CLEAR', 'KERBEROS');
				if(is_array($values)) {
?>
          <td>
<?php				for($i=0; $values[$i]; $i++) {
						if(!eregi($values[$i], pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn))) {
							// We don't want to include schemes we've already
							// configured branch to use...
?>
            <input type="radio" name="<?=$attrib?>" value="<?=$values[$i]?>"><?=$values[$i]?><br>
<?php					}
					}
?>
          </td>
<?php			}
			} else {
?>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?><input type="text" name="<?=$attrib?>" value="<?php if($attrib != 'whoarewe') { echo $oldvalue; } ?>" size="40">
          </td>
<?php		}
	   }
?>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
<?php if($toggle) { ?>
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
<?php } ?>
    <input type="hidden" name="view" value="<?=$_REQUEST["view"]?>">
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

	$attrib = lc($_REQUEST["attrib"]);
	$attrib_val = @$_REQUEST[$attrib];
	$oldvalue = @$_REQUEST["oldvalue"];
	$toggle = @$_REQUEST["toggle"];
	$rootdn = @$_REQUEST["rootdn"];
	$delval = @$_REQUEST["delval"];

	// DLW: This should be chopped up into functions to make it more readable.
	
	$entry = array();

	if($attrib_val and !$delval) {
		if($attrib == pql_get_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME")) {
			$entry[$attrib][] = strtoupper($attrib_val);
		} elseif(is_array($_POST)) {
			for($i=0; $i < $_POST[$attrib]; $i++) {
				$var_name = $attrib . "_$i";
				if($_POST[$var_name]) 
				  $entry[$attrib][] = $_POST[$var_name];
			}
		} // DLW: Why isn't plain old $entry[$attrib][] = $attrib_val here?
		  // TURBO: Because if we've choosen a objectClass, then we have MULTIPLE
		  //		choices - amount if in the hidden input 'name=$attrib', and
		  //		all the choosen values are in 'name=$attrib\_$counter'.
		  //		See the section that starts with 'if(is_array($objectclasses)) {' above.
		  //		Also see the file 'attrib.administrator.inc' for more examples.
    }

	if($attrib_val and $delval and (($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_USER")) or
									($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN")) or
									($attrib == pql_get_define("PQL_ATTR_PASSWORD_SCHEMES")))) {
		// We're deleting one of the multivalued attributes. Retreive the old value(s) from the LDAP db.
		
		if($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_USER")) {
			$allowed = pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_USER", $rootdn));
		} elseif($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN")) {
			$allowed = pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_DOMAIN", $rootdn));
		} elseif($attrib == pql_get_define("PQL_ATTR_PASSWORD_SCHEMES")) {
			$allowed = pql_split_oldvalues(pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn));
		}

		if($allowed) {
			for($i=0; $allowed[$i]; $i++) {
				if(lc($allowed[$i]) != lc($attrib_val))
				  $entry[$attrib][] = $allowed[$i];
			}
		}

		// Just incase this is (was) the last entry...
		if(!is_array($entry[$attrib]))
		  $entry[$attrib] = array();
	} else {
		// Try to find out if we have an old value, or what we should do with the value we have.
		if($oldvalue and $toggle) {
			// A togglable value, reverse state
			
			if($oldvalue == 'FALSE')
			  $entry[$attrib][] = 'TRUE';
			elseif($oldvalue == 'TRUE')
			  $entry[$attrib][] = 'FALSE';
			else {
				$values = pql_split_oldvalues($oldvalue);
				for($i=0; $values[$i]; $i++)
				  $entry[$attrib][] = $values[$i];
			}
		} elseif($oldvalue and $delval) {
			// We should delete one attribute
			$values = pql_split_oldvalues($oldvalue);
			
			// Go through existing attributes, removing the chosen one
			for($i=0; $values[$i]; $i++)
			  if(! eregi($delval, $values[$i]))
				$entry[$attrib][] = $values[$i];
		} elseif($attrib_val and
				 ($attrib != pql_get_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME")) and
				 ($attrib != pql_get_define("PQL_ATTR_OBJECTCLASS_USER")) and
				 ($attrib != pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN")) and
				 ($attrib != pql_get_define("PQL_ATTR_PASSWORD_SCHEMES"))) {
			// We have a value for the attribute we're changing.
			// Any attribute BUT: defaultPasswordScheme, userObjectClass or branchObjectClass
			$entry[$attrib][] = $attrib_val;
		} else {
			if($rootdn and ($attrib != pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"))) {
				// Any attribute BUT controlsAdministrator - fetch old values from DB
				if(!$attrib_val and 
				   (($attrib == pql_get_define("PQL_ATTR_SCRIPT_CREATE_USER")) or
					($attrib == pql_get_define("PQL_ATTR_SCRIPT_CREATE_DOMAIN")))) {
					// If 'externalScript{User,Domain}' attribute(s) are empty, then
					// special circumstances!
					$delete[$attrib] = 1;
				} else {
					unset($oldvalues); $oldvalues = pql_get_attribute($_pql->ldap_linkid, $rootdn, $attrib);
					if(!$oldvalues) {
						// We don't have any old values in the LDAP database. Maybe we have some defaults?
						foreach($PQL_ATTRIBUTE as $level => $array) {
							foreach($array as $key => $value) {
								if(!is_array($value)) {
									if((lc($value) == lc($attrib)) and pql_get_define($key, $rootdn) and !eregi('_GLOB_', $key)) {
										// Split this value into an array
										$oldvalues = pql_split_oldvalues(pql_get_define($key, $rootdn));
										break;
									}
								}
							}
						}
					}

					if(is_array($oldvalues)) {
						foreach($oldvalues as $val) {
							if(!@eregi($delval, $val)) {
								// Remove the one we don't want
								$entry[$attrib][] = $val;
							}
						}
					} elseif($oldvalues) {
						if($toggle) {
							if($oldvalues == 'TRUE')
							  $entry[$attrib][] = 'FALSE';
							else
							  $entry[$attrib][] = 'TRUE';
						} else {
							// Only one value, add it
							$entry[$attrib][] = $oldvalues;
						}
					} elseif($toggle) {
						// No old values and it's a toggle -> check default and reverse it
						foreach($PQL_ATTRIBUTE as $level => $array) {
							foreach($array as $key => $value) {
								if(!is_array($value)) {
									if(lc($value) == $attrib) {
										if(pql_get_define($key, $rootdn) or (pql_get_define($key, $rootdn) == 'TRUE'))
										  $entry[$attrib][] = 'FALSE';
										else
										  $entry[$attrib][] = 'TRUE';
										
										break;
									}
								}
							}
						}
					} // ELSIF it's a toggle.
				} // IF attrib == externalScript{User,Domain}
// With the rewrite Oct 21, 2004 this should no longer be used/referenced... ?
//			} elseif(($attrib == pql_get_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME")) or
//					 ($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_USER")) or
//					 ($attrib == pql_get_define("PQL_ATTR_OBJECTCLASS_DOMAIN"))) {
//				// We don't have any values in the LDAP database, we're using
//				// the default values - get the old values from the define
//				
//				foreach($PQL_ATTRIBUTE as $level => $array) {
//					foreach($array as $key => $value) {
//						if(!is_array($value)) {
//							if((lc($value) == lc($attrib)) and pql_get_define($key, $rootdn) and !eregi('_GLOB_', $key)) {
//								// Split this value into an array
//								$values = pql_split_oldvalues(pql_get_define($key, $rootdn));
//								foreach($values as $val) {
//									// Add all values EXEPT the one to remove
//									if(lc($val) != lc($delval))
//									  $entry[$attrib][] = $val;
//								}
//								
//								break;
//							}
//						} else {
//							$entry[$attrib][] = $attrib_val;
//						}
//					}
//				}
			} elseif($toggle) {
				// We don't have a Root DN, it's not a PasswordScheme or {user,domain}ObjectClass
				// but it's a toggle. Get the old value and reverse it
				
				unset($oldvalues); $oldvalues = pql_get_attribute($_pql->ldap_linkid, $_pql->ldap_basedn[0], $attrib);
				if($oldvalues == 'TRUE')
				  $entry[$attrib][] = 'FALSE';
				else
				  $entry[$attrib][] = 'TRUE';
			}
		}
	}

	// -----------------------------
	// Do the actual modification(s)
	if(!$rootdn or ($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")) or $toggle) {
		// We don't have a root DN, we're changing controlsAdministrator OR
		// it's a toggle. This means it's a global variable, and should be
		// set in every namingContexts
		foreach($_pql->ldap_basedn as $dn) {
			$dn = urldecode($dn);

			if($attrib == pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")) {
				unset($entry);
				if($attrib_val)
				  $entry[$attrib][] = $attrib_val;

				// Fetch old values from DB
				unset($oldvalues); $oldvalues = pql_get_attribute($_pql->ldap_linkid, $dn, $attrib);

				if(is_array($oldvalues)) {
					foreach($oldvalues as $val) {
						if(!@eregi($delval, $val)) {
							// Remove the one we don't want
							$entry[$attrib][] = $val;
						}
					}
				} elseif($oldvalues) {
					// Only one value, add it
					$entry[$attrib][] = $oldvalues;
				}
			}

			if(!pql_modify_attribute($_pql->ldap_linkid, $dn, '', '', $entry))
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
			else {
				$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
											 array('what' => $attrib));
				$rlnb = 1;
			}
		} // FOREACH $_pql->ldap_basedn
	} else {
		$success = 0;

		if(@$delete[$attrib]) {
			// We're deleting the 'externalScript{User,Domain}' attribute(s).
			if(pql_modify_attribute($_pql->ldap_linkid, $rootdn, $attrib, '', ''))
              $success = 1;
		} else {
			if(pql_modify_attribute($_pql->ldap_linkid, $rootdn, '', '', $entry)) 
			  $success = 1;
		}

		if($success) {
			$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										 array('what' => $attrib));
			$rlnb = 1;
		} else {
			$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										 array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		}
	}

	attribute_forward($msg, $rlnb);
	break;
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
