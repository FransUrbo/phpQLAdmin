<?php
// attribute plugin for phpQLAdmin configuration
// $Id: attrib.config.inc,v 1.3 2003-01-20 08:55:36 turbo Exp $

function attribute_check() {
    global $administrator, $domain, $oldvalue, $_pql, $error, $attrib, $$attrib;

    // is typed in ?
    if($$attrib == ""){
		$error[$attrib] = PQL_MISSING;
		return false;
    }
    
    return true;
}

function attribute_print_form() {
    global $administrator, $domain, $PHP_SELF, $attrib;
	global $$attrib, $oldvalue, $error, $_pql, $toggle;
	global $PQL_ATTRIBUTE;

	$test = strtolower($attrib);

	// Get the first defined value from the defines
	foreach($PQL_ATTRIBUTE as $key => $value) {
		$value = strtolower($value);
		if($value == $test) {
			$def = $key;
			$oldvalue = constant($key);
			break;
		}
	}

	// Now, get the second value from the LDAP database,
	// overwriting the defines if existing in db.
	$value = pql_get_domain_value($_pql, $_pql->ldap_basedn[0], $test);
	if($value != '') $oldvalue = $value;

	if($toggle) {
		// A togglable value, reverse state
		$opt = ($oldvalue ? '0' : '1');
		$entry[$attrib] = $opt;

		// Get all object classes of the DN, making sure it
		// contains the phpQLAdminBranch
		$sr  = @ldap_read($_pql->ldap_linkid,
						  $_pql->ldap_basedn[0],
						  $_pql->ldap_basedn[0],
						  array('objectclass'));
		$ocs = @ldap_get_entries($_pql->ldap_linkid, $sr) or pql_errormsg($linkid);
		for($j=0; $j < $ocs[0]["objectclass"]["count"]; $j++) {
			if(eregi('phpQLAdminConfig', $ocs[0]["objectclass"][$j]))
			  $got_phpqladmin_objectclass_config = 1;
		}

		// We don't have the 'phpQLAdminConfig' objectclass
		if(! $got_phpqladmin_objectclass_config)
		  $entry["objectClass"][] = 'phpQLAdminConfig';
		
		if(! pql_set_domain_value($_pql->ldap_linkid, $_pql->ldap_basedn[0], '', $entry))
		  $msg = "Failed to add/replace $attrib: " . ldap_error($_pql->ldap_linkid);
		else
		  $msg = "Successfully replaced $attrib.";
		
		attribute_forward($msg);
	} else {
		// Not a togglable value, show the input form
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Set/Modify <?=$attrib?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$attrib?></td>
          <td><?php echo format_error($error[$attrib]); ?><input type="text" name="<?=$attrib?>" value="<?=$oldvalue?>"></td>
        </tr>
      </th>
    </table>

	<input type="hidden" name="attrib" value="<?=$attrib?>">
	<input type="hidden" name="submit" value="1">
	<input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>
<?php
	}
}

function attribute_save() {
    global $domain, $attrib, $$attrib, $oldvalue, $administrator, $_pql;
	$value = $$attrib;
	
	if($value == $oldvalue){
		// don't change if new value equal to old one
		$msg = PQL_LDAP_CN_UNCHANGED;
		attribute_forward($msg);
		break;
	}
	
	if(! pql_set_domain_value($_pql->ldap_linkid, $_pql->ldap_basedn[0], $attrib, $value))
	  $msg = "Failed to add/replace $attrib: " . ldap_error($_pql->ldap_linkid);
	else
	  $msg = "Successfully replaced $attrib.";
	
	attribute_forward($msg);
	break;
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
