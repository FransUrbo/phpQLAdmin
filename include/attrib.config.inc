<?php
// attribute plugin for phpQLAdmin configuration
// $Id: attrib.config.inc,v 1.11 2003-04-08 10:35:19 turbo Exp $

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
		else
		  $oldvalue = 'FALSE';
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
  <form action="<?php echo $PHP_SELF ?>" method="post" name="phpqladmin_modify">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Set/Modify <?=$attrib?></th>
        <tr class="<?php table_bgcolor(); ?>">
<?php   if(($test == 'userobjectclass') or ($test == 'branchobjectclass')) { ?>
          <td class="title">Add to <?=$attrib?>
            <select name="<?=$attrib?>">
<?php       $objectclasses = pql_get_subschema($_pql->ldap_linkid, "objectclasses");
            foreach($objectclasses as $key => $oc) {
?>
            <option value="<?=$key?>"><?=$objectclasses[$key]["NAME"]?></option>
<?php       } ?>
            </select>
          </td>
<?php   } else { ?>
          <td class="title"><?=$attrib?></td>
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
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
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
	if(strtolower($attrib) == 'passwordscheme') 
	  $value[$attrib][] = strtoupper($$attrib);
	else
	  $value[$attrib][] = $$attrib;

	if($value[$attrib] == $oldvalue){
		// don't change if new value equal to old one
		$msg = PQL_LANG_CN_UNCHANGED;
		attribute_forward($msg);
		break;
	}

	// If we HAVE an old value, add it...
	if($oldvalue and $toggle) {
		$values = pql_split_oldvalues($oldvalue);
		for($i=0; $values[$i]; $i++)
		  $value[$attrib][] = $values[$i];
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
