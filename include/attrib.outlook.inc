<?php
// attribute plugin for pilotPerson objects etc
// $Id: attrib.outlook.inc,v 1.3 2003-01-22 09:06:36 turbo Exp $

/*
	Attributes managed here:
	------------------------
	c
	facsimiletelephonenumber
	homePhone
	info
	l
	mobile
	o
	pager
	physicalDeliveryOfficeName
	postalAddress
	postalCode
	postOfficebox
	seealso
	st
	telephoneNumber
	title
	street

	Not implemented (see no point!)
	-------------------------------
	registeredAddress
	preferredDeliveryMethod
	physicalDeliveryOfficeName
*/

function attribute_check() {
    global $user, $domain, $oldvalue, $_pql, $error, $attrib, $$attrib;

    // is typed in ?
    if($$attrib == ""){
		$error[$attrib] = PQL_MISSING;
		return false;
    }
    
    return true;
}

function attribute_init() {
    global $$attrib, $oldvalue;
    $$attrib = $oldvalue;
}

function attribute_print_form() {
    global $user, $domain, $PHP_SELF, $attrib, $$attrib, $oldvalue, $error, $_pql;
?>

  <form action="<?php echo $PHP_SELF ?>" method="post" name="outlook">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Set/Modify <?=$attrib?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$attrib?></td>
          <td><?php echo format_error($error[$attrib]); ?><input type="text" name="<?=$attrib?>" value="<?=$$attrib?>"></td>
        </tr>
      </th>
    </table>

	<input type="hidden" name="submit" value="2">
	<input type="hidden" name="attrib" value="<?=$attrib?>">
	<input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
	<input type="hidden" name="domain" value="<?=$domain?>">
	<input type="hidden" name="user" value="<?=$user?>">
	<input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>

  <script language="JavaScript">
    <!--
    document.outlook.<?=$attrib?>.focus();
    // -->
  </script>
<?php
}

function attribute_save($type) {
    global $user, $domain, $attrib, $$attrib, $oldvalue, $_pql;

// TODO: Only do this if .. ?
//	$value = urlencode($$attrib);
	$value = $$attrib;

    switch($type){
      case "add":
      case "modify":
		if($value == $oldvalue){
			// don't change if new value equal to old one
			$msg = "$attrib unchanged ($value == $oldvalue)";
			attribute_forward($msg);
			break;
		}
		
		if($domain and !$user) {
			// Save a domain attribute
			if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $value))
			  $success = 1;
		} elseif($user and !$domain) {
			// Save a user attribute
			if(pql_replace_userattribute($_pql->ldap_linkid, $user, $attrib, $value))
			  $success = 1;
		}

		if($success)
		  $msg = "Successfully replaced $attrib.";
		else
		  $msg = "Failed to add/replace $attrib: " . ldap_error($_pql->ldap_linkid);

		attribute_forward($msg);
		break;

      case "delete";
		if($domain and !$user) {
			// Save a domain attribute
			if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, ''))
			  $success = 1;
//		} elseif($user and !$domain) {
// Currently no DELETE option
		}

		if($success)
		  $msg = "Successfully deleted $attrib.";
		else
		  $msg = "Failed to add/replace $attrib: " . ldap_error($_pql->ldap_linkid);

		attribute_forward($msg);
		break;

      default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
    }
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
