<?php
// Attribute plugin for
// defaultPasswordScheme
// $Id: attrib.defaultpasswordscheme.inc,v 2.8 2003-11-12 09:33:32 turbo Exp $

function attribute_check($type = '') {
	global $error, $attrib, $$attrib, $LANG;

    // is typed in ?
    if($attrib == "") {
		$error[$attrib] = $LANG->_('Missing');
		return false;
    }
    
	return true;
}

function attribute_print_form($type = "modify") {
	global $error, $attrib, $$attrib, $domain, $rootdn, $view, $orgname, $LANG;
?>
  <form action="<?$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Password scheme to use if not specifically specified')?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Default password scheme')?></td>
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
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
	global $error, $attrib, $$attrib, $domain, $rootdn, $type, $_pql, $view, $LANG;
	
    switch($type) {
      case "add":
      case "modify":
	  case "delete":
		if($type == 'delete')
		  $$attrib = '';

		if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $$attrib))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
									   array('what'   => $LANG->_('default password scheme'),
											 'domain' => pql_maybe_decode(urldecode($domain))));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what% for domain %domain%'),
									   array('what'   => $LANG->_('default password scheme'),
											 'domain' => pql_maybe_decode(urldecode($domain)))) . ":&nbsp;".ldap_error($_pql->ldap_linkid);
	
		attribute_forward($msg);
		break;
		
      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
