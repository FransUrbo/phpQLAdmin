<?php
// Attribute plugin for
// usernamePrefix
// $Id: attrib.usernameprefix.inc,v 2.4 2003-11-20 08:01:37 turbo Exp $

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
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Username prefix to use when automaticly creating username in domain %domain%'), array('domain' => $orgname)); ?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Username prefix')?></td>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
            <input type="text" name="<?=$attrib?>" value="<?=$$attrib?>" size="50">
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td>
            <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
            <table>
              <?=$LANG->_('The username will be created with this as prefix, and a incrementing number after it when automaticly creating a username (uid)')?>
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
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('username prefix'),
											 'new'  => $$attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('username prefix'))) . ": ".ldap_error($_pql->ldap_linkid);
	
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
