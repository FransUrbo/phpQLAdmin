<?php
// attribute plugin for phpQLAdmin configuration
// $Id: attrib.config.inc,v 1.14 2003-04-24 07:50:59 turbo Exp $

function attribute_check() {
    global $administrator, $domain, $oldvalue, $_pql, $error;
	global $attrib, $$attrib, $rootdn, $value;

    // is typed in ?
    if($$attrib == ""){
		$error[$attrib] = PQL_LANG_MISSING;
		return false;
    }
    
    return true;
}

function attribute_print_form() {
    global $administrator, $domain, $PHP_SELF, $attrib;
	global $$attrib, $oldvalue, $error, $_pql, $toggle;
	global $PQL_ATTRIBUTE, $rootdn, $delval, $config;

	$test = strtolower($attrib);

	// Get the old values from the define
	foreach($PQL_ATTRIBUTE as $key => $value) {
		$value = strtolower($value);
		if(($value == $test) and $config[$key][$rootdn]) {
			$oldvalue = $config[$key][$rootdn];
			break;
		}
	}

	if(! $rootdn)
	  $dn = $_pql->ldap_basedn[0];
	else
	  $dn = $rootdn;

	// Now, get the old values from the LDAP database,
	// overwriting the defines if existing in db.
	$value = pql_get_domain_value($_pql, $dn, $test);

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

	if($toggle) {
		// A togglable value, reverse state
		if(!$oldvalue or ($oldvalue == 'FALSE'))
		  $entry[$attrib] = 'TRUE';
		else
		  $entry[$attrib] = 'FALSE';
	} elseif($delval) {
		// We should delete one attribute

		$values = pql_split_oldvalues($oldvalue);

		// Go through existing attributes, removing the chosen one
		for($i=0; $values[$i]; $i++)
		  if(! eregi($delval, $values[$i]))
			$entry[$test][] = $values[$i];
	} else {
		// Not a togglable value, show the input form
?>
  <script type="text/javascript" language="javascript"><!--
    function errorWindow() {
      myWindow = window.open("", "ProblemWindow", 'toolbar,width=850,height=400');
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

  <span class="title1">Change configuration value</span>

  <br><br>

  <form action="<?php echo $PHP_SELF ?>" method="post" name="phpqladmin_modify">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Set/Modify <?=$attrib."\n"?>
        <tr class="subtitle">
          <table>
            <td colspan="2"><img src="images/info.png" width="16" height="16" border="0" align="left"></td>
            <td>
              These are the objectclasses that exists in the LDAP server,<br>
              exept those that's already included for use.
            </td>
          </table>
        </tr>
      </th>
    </table>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Add to <?=$attrib?>
<?php   if(($test == 'userobjectclass') or ($test == 'branchobjectclass')) {
            if($test == 'userobjectclass')
                $existing = $config["PQL_CONF_OBJECTCLASS_USER"][$dn];
            else
                $existing = $config["PQL_CONF_OBJECTCLASS_DOMAIN"][$dn];
            $existing = pql_split_oldvalues($existing); // Convert to array

            $objectclasses = pql_get_subschema($_pql->ldap_linkid, "objectclasses");
			if(is_array($objectclasses)) {
				$i = 0;
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
<?php                   }
				        $new_tr = 1;

					    // TODO: We should use type="checkbox", indexed with $i here,
					    //       but I'm not quite sure how to pass the value to attribute_save()...
?>
          <td class="<?=$class?>">
            <input type="radio" name="<?=$attrib?>" value="<?=$key?>" size="50"><?=$objectclasses[$key]["NAME"]?>
          </td>

<?php                   $i++;
					}
                }
			} else {
				// No objectclasses in LDAP!
?>
          <td class="<?=$class?>"><i><a href="javascript:errorWindow()">No objectclasses!</i></a></td
<?php
			}
        } else {
?>
          <td>
            <?php echo format_error($error[$attrib]); ?><input type="text" name="<?=$attrib?>" value="<?php if(($test != 'passwordscheme') and ($test != 'whoarewe')) { echo $oldvalue; } ?>" size="40">
          </td>
<?php   } ?>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
<?php if($toggle) { ?>
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
<?php } ?>
    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>

  <script language="JavaScript">
  <!--
    document.phpqladmin_modify.<?=$attrib?>.focus();
	// -->
  </script>
<?php
	}

	// Do the actual modification
	if($toggle) {
		if(! $rootdn) {
			// We don't have a root DN. This means it's a global
			// variable, and should be set in every namingContexts
			foreach($_pql->ldap_basedn as $dn) {
				if(is_array($entry)) {
					if(! pql_set_domain_value($_pql->ldap_linkid, $dn, '', $entry)) {
						$msg = "Failed to add/replace $attrib: " . ldap_error($_pql->ldap_linkid);
					} else {
						$msg = "Successfully replaced $attrib."; $rlnb = 1;
					}
				} else {
					if(! pql_set_domain_value($_pql->ldap_linkid, $dn, $entry, '')) {
						$msg = "Failed to add/replace $attrib: " . ldap_error($_pql->ldap_linkid);
					} else {
						$msg = "Successfully replaced $attrib."; $rlnb = 1;
					}
				}
			}
		} else {
			if(! pql_set_domain_value($_pql->ldap_linkid, $rootdn, '', $entry)) {
				$msg = "Failed to add/replace $attrib: " . ldap_error($_pql->ldap_linkid);
			} else {
				$msg = "Successfully replaced $attrib."; $rlnb = 1;
			}
		}
	
		attribute_forward($msg, $rlnb);
	}
}

function attribute_save() {
    global $domain, $attrib, $$attrib, $oldvalue, $administrator, $_pql, $rootdn;
	global $delval;

	if($$attrib) {
		if(strtolower($attrib) == 'passwordscheme') {
			$value[$attrib][] = strtoupper($$attrib);
		} else {
			$value[$attrib][] = $$attrib;
		}
    }

	// If we HAVE an old value, add it...
	if($oldvalue and $toggle) {
		$values = pql_split_oldvalues($oldvalue);
		for($i=0; $values[$i]; $i++)
		  $value[$attrib][] = $values[$i];
	} else {
		// No old value, and it's not a toggle...
		// Fetch old values from DB
		$oldvalues = pql_get_domain_value($_pql, $rootdn, $attrib);
		foreach($oldvalues as $val) {
			if(!@eregi($delval, $val)) {
			   // Remove the one we don't want
			   $value[$attrib][] = $val;
            }
        }
	}

	if(! $rootdn) {
		// We don't have a root DN. This means it's a global
		// variable, and should be set in every namingContexts
		foreach($_pql->ldap_basedn as $dn) {
			if(! pql_set_domain_value($_pql->ldap_linkid, $dn, '', $value)) {
				$msg = "Failed to add/replace $attrib: " . ldap_error($_pql->ldap_linkid);
			} else {
				$msg = "Successfully replaced $attrib."; $rlnb = 1;
			}
		}
	} else {
		if(! pql_set_domain_value($_pql->ldap_linkid, $rootdn, '', $value)) {
			$msg = "Failed to add/replace $attrib: " . ldap_error($_pql->ldap_linkid);
		} else {
			$msg = "Successfully replaced $attrib."; $rlnb = 1;
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
	for($j=0; $j < $ocs[0]["objectclass"]["count"]; $j++) {
		if(eregi('phpQLAdminConfig', $ocs[0]["objectclass"][$j]))
		  return 1;
	}

	return 0;
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
