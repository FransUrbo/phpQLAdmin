<?php
// attribute plugin for phpQLAdmin configuration
// $Id: attrib.config.inc,v 1.1 2003-01-18 11:30:00 turbo Exp $

function attribute_check() {
    global $administrator, $domain, $oldvalue, $_pql, $error, $attrib, $$attrib;

    // is typed in ?
    if($$attrib == ""){
		$error[$attrib] = PQL_MISSING;
		return false;
    }
    
    return true;
}

function attribute_print_form($action) {
    global $administrator, $domain, $PHP_SELF, $attrib, $$attrib, $oldvalue, $error, $_pql;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
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

<?php
}

function attribute_save($type) {
    global $domain, $attrib, $$attrib, $oldvalue, $administrator, $_pql;
	$value = urlencode($$attrib);
	
    switch($type){
      case "add":
      case "modify";
		if($value == $oldvalue){
			// don't change if new value equal to old one
			$msg = PQL_LDAP_CN_UNCHANGED;
			attribute_forward($msg);
			break;
		}
		
		if(! pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $value)) {
			$msg = "Failed to add/replace $attrib: " . ldap_error($_pql->ldap_linkid);
		}
		$msg = "Successfully replaced $attrib.";

		attribute_forward($msg);
		break;

      case "delete";
		if(! pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, '')) {
			$msg = "Failed to add/replace $attrib: " . ldap_error($_pql->ldap_linkid);
		}
		$msg = "Successfully deleted $attrib.";

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
