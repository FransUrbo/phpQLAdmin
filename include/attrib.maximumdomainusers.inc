<?php
// Attribute plugin for
// maximumDomainUsers
// $Id: attrib.maximumdomainusers.inc,v 2.1 2003-04-30 08:39:16 turbo Exp $

function attribute_check($type = '') {
	global $error, $attrib, $$attrib;

    // is typed in ?
    if($attrib == "") {
		$error[$attrib] = PQL_LANG_MISSING;
		return false;
    }
    
	return true;
}

function attribute_print_form($type = "modify") {
	global $error, $attrib, $$attrib, $domain, $rootdn;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Maximum allowed users in domain <?=$domain?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Maximum allowed users</td>
          <td>
            <?php echo format_error($error[$attrib]); ?>
            <input type="text" name="<?=$attrib?>" value="<?$$attrib?>" size="50">
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="2">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="domain" value="<?=$domain?>">

    <br><br>

    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>
<?php
}

function attribute_save($type) {
	global $error, $attrib, $$attrib, $domain, $rootdn, $type, $_pql;

	echo "Type: $type<br>";
	echo "Domain: $domain<br>";
	echo "Attrib: $attrib (".$$attrib.")<br>";
	echo "Root DN: $rootdn<br>";

    switch($type) {
      case "add":
      case "modify":
		if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $$attrib))
		  $msg = 'Successfully changed maximum allowed users for domain';
		else
		  $msg = 'Failed to set/change maximum allowed users for domain: &nbsp;".ldap_error($_pql->ldap_linkid);
	
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
