<?php
// Attribute plugin for
// defaultPasswordScheme
// $Id: attrib.defaultpasswordscheme.inc,v 2.3 2003-06-25 07:07:02 turbo Exp $

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
      <th colspan="3" align="left">Password scheme to use if not specifically specified</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Default password scheme</td>
          <td>
            <?php echo format_error($error[$attrib]); ?>
            <select name="<?=$attrib?>">
<?php // Convert the array to a index
	  $schemes = pql_split_oldvalues(pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $rootdn));
	  foreach($schemes as $scheme) {
?>
              <option value="<?=$scheme?>"><?=$scheme?></option>
<?php } ?>
            </select>
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
		  $msg = 'Successfully changed default password scheme for domain';
		else
		  $msg = 'Failed to set/change default password scheme for domain: &nbsp;'.ldap_error($_pql->ldap_linkid);
	
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
