<?php
// Attribute plugin for
// maximumDomainUsers
// $Id: attrib.maximummailinglists.inc,v 2.2 2003-06-25 07:07:02 turbo Exp $

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
	global $error, $attrib, $$attrib, $domain, $rootdn, $view, $orgname;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Maximum allowed mailinglists in domain <?=$orgname?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Maximum allowed lists</td>
          <td>
            <?php echo format_error($error[$attrib]); ?>
            <input type="text" name="<?=$attrib?>" value="<?=$$attrib?>" size="50">
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"></td>
          <td>
            <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
            <table>
              A limit of 0 (zero) means unlimited accounts and a negative value means that no mailinglists are allowed.
            </table>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="2">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
    <input type="hidden" name="view"   value="<?=$view?>">
    <br>
    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>
<?php
}

function attribute_save($type) {
	global $error, $attrib, $$attrib, $domain, $rootdn, $type, $_pql, $view;
	
    switch($type) {
      case "add":
      case "modify":
	  case "delete":
		if($type == 'delete') {
			$$attrib = '';
		}

		if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $$attrib))
		  $msg = 'Successfully changed maximum allowed mailinglists for domain';
		else
		  $msg = 'Failed to set/change maximum allowed mailinglists for domain: &nbsp;'.ldap_error($_pql->ldap_linkid);
	
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
