<?php
// attribute plugin for phpQLAdmin configuration
// $Id: attrib.config.inc,v 1.26 2003-08-15 08:06:17 turbo Exp $

function attribute_check() {
    global $administrator, $domain, $oldvalue, $_pql, $error, $LANG;
	global $attrib, $$attrib, $rootdn, $value;

    // is typed in ?
    if($$attrib == "") {
		$error[$attrib] = $LANG->_('Missing');
		return false;
    }
    
    return true;
}

function attribute_print_form() {
    global $administrator, $domain, $PHP_SELF, $attrib, $$attrib, $oldvalue, $error, $_pql, $toggle;
	global $PQL_ATTRIBUTE, $rootdn, $delval, $view, $LANG;
	$attrib = strtolower($attrib);

	// Get the old values from the define
	foreach($PQL_ATTRIBUTE as $key => $value) {
		$value = strtolower($value);
		if(($value == $attrib) and pql_get_define($key, $rootdn)) {
			$oldvalue = pql_get_define($key, $rootdn);
			break;
		}
	}

	if(! $rootdn)
	  $dn = $_pql->ldap_basedn[0];
	else
	  $dn = $rootdn;

	// Now, get the old values from the LDAP database,
	// overwriting the defines if existing in db.
	$value = pql_get_domain_value($_pql, $dn, $attrib);

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

  <form action="<?php echo $PHP_SELF ?>" method="post" name="phpqladmin_modify">
<?php   if(($attrib == lc($PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"])) or
 		   ($attrib == lc($PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"]))) {
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
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo pql_complete_constant($LANG->_('Add to attribute %attribute%'), array('attribute' => $attrib))."\n"; ?>
<?php   if(($attrib == lc($PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_USER"])) or
 		   ($attrib == lc($PQL_ATTRIBUTE["PQL_CONF_OBJECTCLASS_DOMAIN"]))) {
            if($attrib == 'userobjectclass')
                $existing = pql_get_define("PQL_CONF_OBJECTCLASS_USER", $dn);
            else
                $existing = pql_get_define("PQL_CONF_OBJECTCLASS_DOMAIN", $dn);
            $existing = pql_split_oldvalues($existing); // Convert to array

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
					    $class=table_bgcolor(0);
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
        } elseif($attrib == lc($PQL_ATTRIBUTE["PQL_GLOB_LANG"])) {
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
			if($attrib == lc($PQL_ATTRIBUTE["PQL_CONF_PASSWORD_SCHEMES"])) {
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
            <?php echo format_error($error[$attrib]); ?><input type="text" name="<?=$attrib?>" value="<?php if($attrib != 'whoarewe') { echo $oldvalue; } ?>" size="40">
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
    <input type="hidden" name="view" value="<?=$view?>">
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

function attribute_save() {
    global $domain, $attrib, $$attrib, $oldvalue, $administrator, $_pql, $LANG;
	global $delval, $toggle, $PQL_ATTRIBUTE, $view, $rootdn;
	$attrib = lc($attrib);

	if($$attrib) {
		if($attrib == 'passwordscheme') {
			$entry[$attrib][] = strtoupper($$attrib);
		} elseif(is_array($_POST)) {
			for($i=0; $i < $_POST[$attrib]; $i++) {
				$var_name = $attrib . "_$i";
				if($_POST[$var_name]) 
				  $entry[$attrib][] = $_POST[$var_name];
			}
		}
    }

	// Try to find out if we have an old value, or what we should do with the value we have
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
	} elseif($$attrib and ($attrib != 'passwordscheme') and
			($attrib != 'userobjectclass') and ($attrib != 'branchobjectclass')) {
		$entry[$attrib][] = $$attrib;
	} else {
		if($rootdn) {
			if($attrib != pql_get_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR")) {
				// Fetch old values from DB
				unset($oldvalues); $oldvalues = pql_get_domain_value($_pql, $rootdn, $attrib);
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
					foreach($PQL_ATTRIBUTE as $key => $value) {
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
			}
		} elseif(($attrib == 'passwordscheme') or ($attrib == 'userobjectclass') or ($attrib == 'branchobjectclass')) {
			// We don't have any values in the LDAP database, we're using
			// the default values - get the old values from the define

			foreach($PQL_ATTRIBUTE as $key => $value) {
				if(!is_array($value)) {
					if((lc($value) == lc($attrib)) and pql_get_define($key, $rootdn) and !eregi('_GLOB_', $key)) {
						// Split this value into an array
						$values = pql_split_oldvalues(pql_get_define($key, $rootdn));
						foreach($values as $val) {
							// Add all values EXEPT the one to remove
							if(lc($val) != lc($delval))
							  $entry[$attrib][] = $val;
						}

						break;
					}
	            } else {
					$entry[$attrib][] = $$attrib;
				}
			}
        } elseif($toggle) {
			// We don't have a Root DN, it's not a PasswordScheme or {user,domain}ObjectClass
			// but it's a toggle. Get the old value and reverse it

			unset($oldvalues); $oldvalues = pql_get_domain_value($_pql, $_pql->ldap_basedn[0], $attrib);
			if($oldvalues == 'TRUE')
			  $entry[$attrib][] = 'FALSE';
			else
			  $entry[$attrib][] = 'TRUE';
		}
	}

	// -----------------------------
	// Do the actual modification(s)
	if(!$rootdn or ($attrib == pql_get_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR")) or $toggle) {
		// We don't have a root DN, we're changing controlsAdministrator OR
		// it's a toggle. This means it's a global variable, and should be
		// set in every namingContexts

		foreach($_pql->ldap_basedn as $dn) {
			if($attrib == pql_get_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR")) {
				unset($entry);
				if($$attrib)
				  $entry[$attrib][] = $$attrib;

				// Fetch old values from DB
				unset($oldvalues); $oldvalues = pql_get_domain_value($_pql, $dn, $attrib);
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

			if(! pql_set_domain_value($_pql->ldap_linkid, $dn, '', $entry))
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
			else {
				$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
											 array('what' => $attrib));
				$rlnb = 1;
			}
		}
	} else {
		if(! pql_set_domain_value($_pql->ldap_linkid, $rootdn, '', $entry)) 
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		else {
			$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										 array('what' => $attrib));
			$rlnb = 1;
		}
	}

	attribute_forward($msg, $rlnb);
	break;
}

function attribute_verify_objectclass($ldap) {
	global $rootdn;

	// Get all object classes of the DN, making sure it
	// contains the phpQLAdminBranch
	$sr  = @ldap_read($ldap->ldap_linkid, $rootdn, $rootdn, array('objectclass'));
	$ocs = @ldap_get_entries($ldap->ldap_linkid, $sr) or pql_errormsg($ldap->ldap_linkid);
	for($j=0; $j < $ocs[0][pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")]["count"]; $j++) {
		if(eregi('phpQLAdminConfig', $ocs[0][pql_get_define("PQL_GLOB_ATTR_OBJECTCLASS")][$j]))
		  return 1;
	}

	return 0;
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
